  #include <Wire.h>

#define ADD_BASE 0x40

#define LED 11              //Indicador de estado

#define HANDSHAKE 13

#define TIPO 16             //16 o 32

#define PIN_BRUJULA 3

#define PERIODO_MIN 5       //Cada cuanto comprobamos si hay que actualizar el movimiento
#define PERIODO_MAX 500     //Cada cuanto tiempo lo actualizamos de forma forzada

#define PROMEDIOS 8         //Precisión de la medidas de la brújula

#define ERROR_BRUJULA 3    //Precisión del centrado
#define CENTRADO 150        //Punto en el que estamos centrados

#define MARGEN_I 600
#define MARGEN_P 45

#define SUELO1 4
#define SUELO2 5
#define SUELO3 6
#define SUELO4 7
boolean hay_suelo = false;
byte caso_suelo = 0;

volatile boolean estado = false;
volatile unsigned long crono;
volatile unsigned long medida[PROMEDIOS] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile unsigned long control1 = 0;
volatile unsigned long control2 = 0;
volatile int x = 0;
byte ultimo_movimiento = 255;
unsigned long tiempo_ultimo_movimiento = 0;

boolean handshake;          //Cambia entre los modos recepción de velocidad_i2c y aplicación de la misma


byte motor, tiempo = 100;   //La velocidad minima del hadshake son 10 micos;
byte movimiento = TIPO;     // Para robot

byte ajuste_velocidad = 100;//velocidad a la que nos movemos con respoecto al máximo

int error = 0;
int error_abs = 0;
byte KP = 28;
long integral = 0;
int KI = 4;
int KT = 200;


boolean brujula_activada = false;

char componente_de_giro = 0;

byte caso;

const char velocidades[TIPO + 1][4] = {
  {95, 95, -95, -95},       //0       A
  {58, 95, -58, -95},       //1       B
  {0, 100, 0, -100},        //2       C
  { -58, 95, 58, -95},      //3       D
  { -95, 95, 95, -95},      //4       E
  { -95, 58, 95, -58},      //5       F
  { -100, 0, 100, 0},       //6       G
  { -95, -58, 95, 58},      //7       H
  { -95, -95, 95, 95},      //8       I
  { -58, -95, 58, 95},      //9       J
  {0, -100, 0, 100},        //10      K
  {58, -95, -58, 95},       //11      L
  {95, -95, -95, 95},       //12      M
  { 95, 58, -95, -58},      //13      N
  {100, 0, -100, 0},        //14      O
  {95, 58, -95, -58},       //15      P
  {0, 0, 0, 0}              //Parar   X
};
const byte turbos[4][4] = {
  {  100,  100, -100, -100},   //0
  { -100,  100,  100, -100},   //1
  { -100, -100,  100,  100},   //2
  {  100, -100, -100,  100}    //3
};
void setup()
{
  pinMode(SUELO1, INPUT);
  pinMode(SUELO2, INPUT);
  pinMode(SUELO3, INPUT);
  pinMode(SUELO4, INPUT);
  pinMode(PIN_BRUJULA, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(HANDSHAKE, OUTPUT);
  Serial.begin(115200);
  Wire.begin();        //Iniciamos el bus I2C

  attachInterrupt(1, guardar_tiempo , CHANGE);

  for (int x = 0; x < 5; x++)
  {
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
    digitalWrite(LED, HIGH);
  }
}


void loop()
{
  unsigned long tiempo_anterior = 0, tiempo_actual;

  hay_suelo = !(digitalRead(SUELO1) && digitalRead(SUELO2) && digitalRead(SUELO3) && digitalRead(SUELO4))  
  

  if (Serial.available() > 0)
  {
    caso = Serial.read();

    switch (caso)
    {
      case 'A':  // Movimiento 0
      case 'B':  // Movimiento 1
      case 'C':  // Movimiento 2
      case 'D':  // Movimiento 3
      case 'E':  // Movimiento 4
      case 'F':  // Movimiento 5
      case 'G':  // Movimiento 6
      case 'H':  // Movimiento 7
      case 'I':  // Movimiento 8
      case 'J':  // Movimiento 9
      case 'K':  // Movimiento 10
      case 'L':  // Movimiento 11
      case 'M':  // Movimiento 12
      case 'N':  // Movimiento 13
      case 'O':  // Movimiento 14
      case 'P':  // Movimiento 15
        movimiento = caso - 65;
        break;

      case 'v':
        while (Serial.available() < 1);
        ajuste_velocidad = Serial.read();
        ajuste_velocidad = constrain(ajuste_velocidad, 0, 100);
        break;

      case 'x':
        //parar();
        movimiento = TIPO; // el ultimo movimiento de la tabla es parar
        break;

      case 'i':
        Serial.write('k');
        break;

      case 't':
        brujula_activada = ! brujula_activada;
        break;

      case 'g':
        while (Serial.available() < 1);
        componente_de_giro = Serial.read();
        componente_de_giro = constrain (componente_de_giro, -100, 100);
        break;
    }
  }

  if (hay_suelo)
  {
     movimiento = TIPO;
  }



  tiempo_actual = millis();

  if ((tiempo_actual - tiempo_anterior) > PERIODO_MIN)
  {
    tiempo_anterior = tiempo_actual;
    periodica();
  }
}

void periodica(void)
{

  error = CENTRADO - medir();
  error_abs = abs(error);

  if ((movimiento != ultimo_movimiento) || ((millis() - tiempo_ultimo_movimiento) > PERIODO_MAX) || ((error_abs > ERROR_BRUJULA) && brujula_activada) || hay_suelo)
  {
    mueve_robot(movimiento);
    ultimo_movimiento = movimiento;
    tiempo_ultimo_movimiento = millis();
  }
}

