void actualizarMotor(char velocidad, byte motor)
{
  velocidad = ((((float)velocidad) * ((float)(ajuste_velocidad / 100.0))) + 100);

  Wire.beginTransmission(ADD_BASE + motor);     // Enviamos a la dirección
  Wire.write(velocidad);                        //Enviamos velocidad+100
  Wire.endTransmission();                       //Terminamos la trama
}

void parar(void)
{
  for (int motor = 0; motor < 4; motor++)
  {
    Wire.beginTransmission(ADD_BASE + motor);     // Enviamos a la dirección
    Wire.write(100);                              //Enviamos velocidad=100 para parar
    Wire.endTransmission();                       //Terminamos la trama
  }
  lanzarI2C ();
}

void lanzarI2C (void)
{
  digitalWrite(HANDSHAKE, HIGH);
  delayMicroseconds(tiempo);
  digitalWrite(HANDSHAKE, LOW);
}

void mueve_robot(byte mov)
{
  boolean signo;
  int velocidad;
  int termino_error;

  for (motor = 0; motor < 4; motor++)
  {
    velocidad = ((int) velocidades[movimiento][motor]) - componente_de_giro;
    if (brujula_activada && (error_abs > 0))
    {
      termino_error = constrain((((error * KP) / 100.0)), -MARGEN_P, MARGEN_P);
      integral += error;
      integral = constrain(integral, -MARGEN_I, MARGEN_I);
      if (error_abs < (ERROR_BRUJULA + 1))
        integral = 0;

      velocidad -= ((( termino_error + ((integral * KI) / 1000.0)) * KT) / 100.0);

      //Normaliza las medidas
      velocidad = constrain(velocidad, -100, 100);
    }

    actualizarMotor(((char)velocidad), motor);
  }
  lanzarI2C ();
}

void guardar_tiempo()
{
  if (digitalRead(PIN_BRUJULA))
    crono = micros();
  else
  {
    medida[x] = (micros() - crono);
    x++;
    if (x == PROMEDIOS)
      x = 0;
  }
}

