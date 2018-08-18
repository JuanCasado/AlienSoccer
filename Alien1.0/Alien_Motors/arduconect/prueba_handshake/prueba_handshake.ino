#include <Wire.h>

//I2C
#define INTERRUPTOR_MENOR 6
#define INTERRUPTOR_MAYOR 7
#define ADD_BASE 0x40
#define HANDSHAKE 13

#define LED 11  //Indicador de estado

boolean handshake = false; //Cambia entre los modos recepción de velocidad_i2c y aplicación de la misma

int dato1, dato2, address, porcentaje_i2c;

void setup() {
  pinMode(LED, OUTPUT);

  pinMode(HANDSHAKE, INPUT);


  //I2C
  address = ADD_BASE + digitalRead(INTERRUPTOR_MENOR) + (2 * digitalRead(INTERRUPTOR_MAYOR));

  Wire.begin(address);              // Ponemos la direccion de la placa
  Wire.onReceive(receiveEvent);     //Activamos evento de lectura.
    digitalWrite(LED, HIGH);
}

void loop() {
  handshake = digitalRead(HANDSHAKE);
  if (handshake)
  {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
    handshake = false;

  }

}

//I2c
void receiveEvent(int howMany)  //Evento de recpcion
{
  while ( Wire.available())     //Leemos la velocidad
  {
    porcentaje_i2c = Wire.read() - 100;
  }
}
