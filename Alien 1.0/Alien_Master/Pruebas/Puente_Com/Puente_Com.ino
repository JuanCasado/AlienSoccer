// Establece un puente entre el puerto serie 0 y cualquiera de los otros tres (1, 2 ó 3
// de una Arduino Mega

#define VELOCIDAD_SERIE 115200

int puerto, caracteres;

void setup()
{
  Serial.begin(VELOCIDAD_SERIE);
  Serial1.begin(VELOCIDAD_SERIE);
  Serial2.begin(VELOCIDAD_SERIE);
  Serial3.begin(VELOCIDAD_SERIE);

  Serial.println("Seleccione el puerto serie");
  Serial.println("1, 2 o 3 que deseas puentear");
  Serial.println("con el puerto 0");

  while (Serial.available() == 0);
  puerto = Serial.read();

  Serial.print("Puerto 0 puenteado con el puerto ");
  Serial.write(puerto);
  Serial.println(" ");
}

void loop()
{
  byte dato;
  switch (puerto)
  {
    case '1':
        if(Serial.available() > 0)
        {
          dato = Serial.read();
          Serial1.write(dato);
        }
        if(Serial1.available() > 0)
        {
          dato = Serial1.read();
          Serial.write(dato);
        }
      break;

    case '2':
        if(Serial.available() > 0)
        {
          dato = Serial.read();
          Serial2.write(dato);
        }
        if(Serial2.available() > 0)
        {
          dato = Serial2.read();
          Serial.write(dato);
        }
      break;

    case '3':
        if(Serial.available() > 0)
        {
          dato = Serial.read();
          Serial3.write(dato);
        }
        if(Serial3.available() > 0)
        {
          dato = Serial3.read();
          Serial.write(dato);
        }
      break;
      
    default:
      break;
  }
}
