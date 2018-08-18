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


boolean handshake = false; //Cambia entre los modos recepción de velocidad_i2c y aplicación de la misma

int dato1, dato2, address, velocidad, porcentaje_i2c, tipo;


long vel_consigna;      //Velocidad a la que queremos ir (potenciometro)

long vel_motor;         //Velocidad de error ajustada pwm

void setup()
{
  pinMode(LED, OUTPUT);

  pinMode(HANDSHAKE, INPUT);


  //I2C
  address = ADD_BASE + digitalRead(INTERRUPTOR_MENOR) + (2 * digitalRead(INTERRUPTOR_MAYOR));

  Wire.begin(address);              // Ponemos la direccion de la placa
  Wire.onReceive(receiveEvent);     //Activamos evento de lectura.

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
      vel_consigna = map(vel_consigna, 0, 100, 0, 255);   //Velocidad en porcentaje a rpm
      tipo = 0;
    }
    else if (vel_consigna < 0)
    {
      vel_consigna = map(abs(vel_consigna), 0, 100, 0, 255);   //Velocidad en porcentaje a rpm
      tipo = 1;
    }
    else
      tipo = 2;

    avanzar(vel_consigna, tipo);

    handshake = false;

  }
}
