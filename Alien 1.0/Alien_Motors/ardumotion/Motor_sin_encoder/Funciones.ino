//Funciones para controlar el movimiento de los motores
void avanzar (int v, int tipo)
{
  if (tipo == 0) {
    digitalWrite(PHASE, LOW);
    analogWrite(ENABLE, v);
  }
  else if (tipo == 1) {
    digitalWrite(PHASE, HIGH);
    analogWrite(ENABLE, v);
  }
  else if (tipo == 2)
    digitalWrite(ENABLE, LOW);
}


//I2c
void receiveEvent(int howMany)  //Evento de recpcion
{
  while ( Wire.available())     //Leemos la velocidad
  {
    porcentaje_i2c = Wire.read() - 100;
  }
}
