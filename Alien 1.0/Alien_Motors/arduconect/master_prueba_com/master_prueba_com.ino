#include <Wire.h>

#define ADD_BASE 0x40

#define LED 11    //Indicador de estado

#define HANDSHAKE 13

#define TIPO 16 //16 o 32

#define PROMEDIOS 15
#define BRUJULA 7

#define ESCALA_MAX 100
#define ESCALA_MIN 50
boolean handshake; //Cambia entre los modos recepción de velocidad_i2c y aplicación de la misma

boolean brujula = false;
unsigned long angulo = 0;
unsigned long acumula_angulo = 0;
int contador = 0;

int escala = 100;

byte movimiento, motor, tiempo = 100; //La velocidad minima del hadshake son 10 micos;

void actualizarMotor(byte, byte);
void lanzarI2C (void);
void medir (void);
void parar(void);

byte caso;

const byte velocidades[TIPO][4] = { {71, 71, -71, -71}, //0       a
  //{-98,98,-20,20},    //1
  { -49, -87, 49, 87},  //2       b
  //{-83,83,-56,56},    //3
  {0, 100, 0, -100},    //4       c
  //{-56,56,-83,83},    //5
  {87, -49, -87, 49},   //6       d
  //{-20,20,-98,98},    //7
  { -71, 71, 71, -71},  //8       e
  //{20,-20,-98,98},    //9
  {53, -87, -53, 87},   //10      f
  //{56,-56,-83,83},    //11
  { -100, 0, 100, 0},   //12      g
  //{83,-83,-56,56},    //13
  {87, 49, -87, -49},   //14      h
  //{98,-98,-20,20},    //15
  { -71, -71, 71, 71},  //16      i
  //{98,-98,20,-20},    //17
  {53, 87, -53, -87},   //18      j
  //{83,-83,56,-56},    //19
  {0, -100, 0, 100},    //20      k
  //{56,-56,83,-83},    //21
  { -49, 80, 49, -81},  //22      l
  //{20,-20,98,-98},    //23
  {71, -71, -71, 71},   //24      m
  //{-20,20,98,-98},    //25
  { -87, 49, 87, -49},  //26      n
  //{-56,56,83,-83},    //27
  {100, 0, -100, 0},    //28      o
  //{-83,83,56,-56},    //29
  { -87, -49, 87, 49},  //30      p
  //{-98,98,20,-20}     //31
};
const byte turbos[4][4] = { {100, 100, -100, -100},   //0
  { -100, 100,  100, -100},   //1
  { -100, -100, 100,  100},   //2
  {  100, -100, -100, 100}
};  //3
void setup()
{
  pinMode(BRUJULA, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(HANDSHAKE, OUTPUT);
  Serial.begin(115200);
  Wire.begin();        //Iniciamos el bus I2C

  digitalWrite(LED, HIGH);

  delay(500);

}
int x = 0;

void loop()
{
  byte caso;

  if (Serial.available() > 0)
  {

    //caso = Serial.read();
    caso = 'A';
    switch (caso)
    {

      case 'i':
        Serial.write('k');
        break;

      case 'A':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[0][motor], motor);

        lanzarI2C ();
        break;

      case 'B':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[1][motor], motor);

        lanzarI2C ();
        break;

      case 'C':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[2][motor], motor);

        lanzarI2C ();
        break;

      case 'D':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[3][motor], motor);

        lanzarI2C ();
        break;

      case 'E':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[4][motor], motor);

        lanzarI2C ();
        break;

      case 'F':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[5][motor], motor);

        lanzarI2C ();
        break;

      case 'G':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[6][motor], motor);

        lanzarI2C ();
        break;

      case 'H':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[7][motor], motor);

        lanzarI2C ();
        break;
      case 'I':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[8][motor], motor);

        lanzarI2C ();
        break;
      case 'J':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[9][motor], motor);

        lanzarI2C ();
        break;
      case 'K':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[10][motor], motor);

        lanzarI2C ();
        break;

      case 'L':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[11][motor], motor);

        lanzarI2C ();
        break;

      case 'M':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[12][motor], motor);

        lanzarI2C ();
        break;

      case 'N':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[13][motor], motor);

        lanzarI2C ();
        break;

      case 'O':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[14][motor], motor);

        lanzarI2C ();
        break;

      case 'P':
        Serial.print("dentro");
        for (motor = 0; motor < 4; motor++)
          actualizarMotor(velocidades[15][motor], motor);

        lanzarI2C ();
        break;



    }
  }
}

