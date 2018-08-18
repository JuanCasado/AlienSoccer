////////////////////////////////////////
// Archivo: cmps03.cpp
//
////////////////////////////////////////

#include <Wire.h>
#include "WProgram.h"
#include "brujula.h"
#include "nexus.h"

void inicializa_brujula(void)
{
  Wire.begin();
}

int lee_brujula(void)
{
  int medida=0;
  
  #define I2C_TIME 200
  static long t_i2c_out=0;
  t_i2c_out=millis();
  /*if(digitalRead(TIPO_ROBOT_A)+digitalRead(TIPO_ROBOT_B) == ROBOT_NEXUS_8_2)
  {
    Wire.beginTransmission(0X60);
    Wire.send(1);
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(0X60, 1);
    while(Wire.available() < 1);           
    medida = Wire.receive();
  }
  else
  {*/
    Wire.beginTransmission(0X21);
    Wire.send("A");
    Wire.endTransmission();
    delay(10);
  
    Wire.requestFrom(0X21, 2);
    while((Wire.available() < 2) && ((millis()-t_i2c_out) < I2C_TIME));
    medida = Wire.receive()<<8;
    medida = medida + Wire.receive();
    medida = map(medida, 0, 3599, 0, 255);
  //}
  
  if(medida>127)
    medida-=256;   //Error detectado el 20 de septiempre de 2009, antes ponia 255

  return medida;
}

int varia_posicion_brujula(int pos_inicial)
{
  int posicion, brujula;

  brujula=lee_brujula();
  if(abs(brujula-pos_inicial)<=127)
    posicion= brujula-pos_inicial;
  else
  {
    if(brujula>pos_inicial)
      posicion=brujula-255-pos_inicial;  //Revisar esta parte del codigo 
    else
      posicion=brujula+255-pos_inicial;
  }

  return posicion;
}
