#include <Wire.h>

//I2C
#define INTERRUPTOR_MENOR 6
#define INTERRUPTOR_MAYOR 7
#define ADD_BASE 0x40
#define HANDSHAKE 13

#define LED 11  //Indicador de estado

//Pines del motor
#define PHASE 8      //rojo
#define ENABLE 9     //negro

//Pines del encoder
#define ENCODER1 2  //amarillo
#define ENCODER2 3  //blanco

//I2c
boolean handshake = false;  //Cambia entre los modos recepción de velocidad_i2c y aplicación de la misma
int dato1, dato2, address;  //Nombre de la placa en el i2c
int porcentaje_i2c;         //Porcentaje de velocidad recibido por i2c
int tipo;                   //Avanzar=0;Retroceder=1;Parar=2       

//variables para calcular la vel_real
long pos, tim;
volatile long encoderPos = 0;
long newposition = 0;
long oldposition = 0;
long tiempo = 0;          //Diferencia newtime-oldtime

unsigned long oldtime, newtime; //tiempos en poitivo

long vel_consigna = 0;    //Velocidad a la que queremos ir (potenciometro)
long vel_real;            //Encoder
long error;               //V_consigna-V_real
long error_anterior = 0;  //Error en t-1
long integral = 0;        //Varable acumulativa para calcular la integral del error
long vel_proporcional;
long vel_integral;
long vel_derivativa;
long vel_error;           //vel_proporcional+vel_integral+vel_derivativa
long vel_motor;           //Velocidad de error ajustada pwm
long posicion;

//Constantes de control
int ConstanteProporcional = 4;
int ConstanteIntegral = 10;
int ConstanteDerivativa = 100;

int rpm_max=400;          //Velocidad máxima de los motores

void contar();
void avanzar (int v,int tipo);

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(PHASE, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(ENCODER1, INPUT_PULLUP);  //Sustituye a una resistencia(PULLUP)
  pinMode(ENCODER2, INPUT_PULLUP);  //Sustituye a una resistencia(PULLUP)
  pinMode(HANDSHAKE, INPUT);

  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(ENCODER1), contar, RISING);

  //I2C
  address = ADD_BASE + digitalRead(INTERRUPTOR_MENOR) + (2 * digitalRead(INTERRUPTOR_MAYOR));
  Wire.begin(address);              // Ponemos la direccion de la placa
  Wire.onReceive(receiveEvent);     //Activamos evento de lectura.

  //Iniciamos cronómetro
  oldtime = millis();

  digitalWrite(LED, HIGH);

}
void loop()
{
  handshake = digitalRead(HANDSHAKE);
  
  if (handshake)
  {

    vel_consigna = porcentaje_i2c;  //Aplica la velocidad a los motores

    if (vel_consigna > 0)
    {
      vel_consigna = map(vel_consigna, 0, 100, 0, rpm_max);   //Velocidad en porcentaje a rpm
      tipo = 0;
    }
    else if (vel_consigna < 0)
    {
      vel_consigna = abs(vel_consigna);
      vel_consigna = map(vel_consigna, 0, 100, 0, rpm_max);   //Velocidad en porcentaje a rpm
      tipo = 1;
    }
    else
      tipo = 2;

    handshake = false;
  }
  
  //Constantemente medimos el tiempo
  newtime = millis();
  
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
    integral += error * 0.001;
    vel_integral = integral * ConstanteIntegral;

    //Control de velocidad derivativo
    vel_derivativa = ((error - error_anterior) / 0.001) * ConstanteDerivativa;
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
}
