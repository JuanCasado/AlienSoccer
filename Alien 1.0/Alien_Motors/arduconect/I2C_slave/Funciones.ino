void requestEvent()             //Evento de peticion se activa cuando un master nos pide que le enviemos algun dato
{
  Wire.write(estado);           // Enviamos el estado de la placa
}

void receiveEvent(int howMany)  //Evento de recpcion
{
  while ( Wire.available())     //Leemos la velocidad
  {
    velocidad = Wire.read();
  }
}
