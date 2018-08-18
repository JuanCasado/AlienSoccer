#include <Wire.h>

#define LED 11  //Indicador de estado

//Pines potenciómetro
#define POT A4

//Pines del motor
#define PHASE 8      //rojo
#define ENABLE 9     //negro

//Pines del encoder
#define ENCODER1 2  //amarillo
#define ENCODER2 3  //blanco     

//variables para calcular la vel_real
long pos, tim;
volatile long encoderPos = 0;
long newposition = 0;
long oldposition = 0;
long tiempo = 0;          //Diferencia newtime-oldtime

unsigned long oldtime, newtime; //tiempos en poitivo

int vel_real, vel_real_pwm;            //Encoder
int error;               //V_consigna-V_real
int error_anterior = 0;  //Error en t-1
long integral = 0;        //Varable acumulativa para calcular la integral del error
int vel_proporcional;
int vel_integral;
int vel_derivativa;
int vel_error;           //vel_proporcional+vel_integral+vel_derivativa
int vel_motor;           //Velocidad de error ajustada pwm
long posicion;

//Constantes de control
int ConstanteProporcional = 1;
float ConstanteIntegral = 0.5;
float ConstanteDerivativa = 0.0005;

int rpm_max = 400;        //Velocidad máxima de los motores


int tipo = 0;             //0-->alante;1-->atras;2-->parar
long vel_consigna;        //Velocidad a la que queremos ir

void contar();
void avanzar (int v, int tipo);

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(PHASE, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(ENCODER1, INPUT_PULLUP);  //Sustituye a una resistencia(PULLUP)
  pinMode(ENCODER2, INPUT_PULLUP);  //Sustituye a una resistencia(PULLUP)

  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(ENCODER1), contar, RISING);

  //Iniciamos cronómetro
  oldtime = millis();

  digitalWrite(LED, HIGH);

}
void loop()
{
  //Constantemente medimos el tiempo
  newtime = millis();

  vel_consigna = map(analogRead(POT), 0, 1023, 0, rpm_max); //vel_consigna en rpm

  if (newtime - oldtime >= 100)
  {
    //Mide la velociad real
    tiempo = newtime - oldtime;
    oldtime = millis();         //Actualizamos el tiempo para hacer la diferecia del if
    newposition = encoderPos;   //Actualizamos la posicion
    posicion = newposition - oldposition;

    pos = (posicion * 60000);
    tim = long(tiempo * 245);   //12 pulsos=1rpm 20,4:1
    vel_real = pos / tim;       //Velocidad del encoder a rpm


    //Calculo del error
    error = vel_consigna - vel_real;

    //Control de velocidad proporcional
    vel_proporcional = ConstanteProporcional * error;

    //Control de velocidad integral
    integral += error * 0.1; //t(minutos)=0.0017
    vel_integral = integral * ConstanteIntegral;

    //Control de velocidad derivativo
    vel_derivativa = ((error - error_anterior) / 0.0017) * ConstanteDerivativa; //t(minutos)=0.0017
    error_anterior = error;

    //Control de velocidad PID
    vel_error = vel_proporcional + vel_integral + vel_derivativa;



    //Paso de rpm a pwm
    vel_motor = map(vel_error, 0, rpm_max, 0, 255); //Pasamos de rpm a pwm

    //Control de saturación
    if (vel_motor > 255)
      vel_motor = 255;
    if (vel_motor < 0)
      vel_motor = 0;

    avanzar(vel_motor, tipo);

    oldposition = newposition; // Actualizar la posicion para la siguiente entrada en el bucle
  }

  Serial.print("POT: ");
  Serial.print(vel_consigna);
  Serial.print("  \tREAL: ");
  Serial.print(vel_real);
  Serial.print("  \tPWM: ");
  Serial.print(vel_real_pwm);
  Serial.print("  \tMOTOR: ");
  Serial.print(vel_motor);
  Serial.print("\tERROR: ");
  Serial.println(error);

}
