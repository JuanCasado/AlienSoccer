#include <Wire.h>
#define DATO1 2
#define DATO2 3
int dato1, dato2, adress, velocidad;

bool estado = true;
void setup()
{
  dato1 = digitalRead(DATO1);
  dato2 = digitalRead(DATO2);
  if (dato1)
  {
    if (dato2)
    {
      adress = B00000011;
    }
    else
    {
      adress = 00000001;
    }
  }
  else
  {
    if (dato2)
    {
      adress = 00000010;
    }
    else
    {
      adress = 00000000;
    }

    Wire.begin(adress);           // Ponemos la direccion de la placa
    Wire.onRequest(requestEvent); // Activamos evento de peticion
    Wire.onReceive(receiveEvent); //Activamos evento de lectura.
  }
}
void loop()
{

}
