void lanzar ()
{
  byte avance[4]={71,71,-71,-71};
  while (/*Variable MegaMaster==avance*/true)
  {
    for (motor = 0; motor<4; x++)
    {
      actualizarMotor(avance[motor]++, motor);
    }
    
    lanzarI2C ();
    
    //Actualizar variable MegaMaster
  }
  /**
   * 
   * La MegaMaster envía constantemente un comado de lanzar mientrs lee los sensores, 
   * si la bola se sale de la parte delantera del robot dejará de enviar ese comando 
   * y se saldrá de la función
   **/
}

void ajusteDerecha(byte movimiento[4])
{
    actualizarMotor(movimiento[1]+2, 1);

    actualizarMotor(movimiento[3]+2, 3);
    
    lanzarI2C (); 
}

void ajusteIzquierda(byte movimiento[4])
{
    actualizarMotor(movimiento[0]-2, 0);
    actualizarMotor(movimiento[1]+2, 0);
    actualizarMotor(movimiento[2]-2, 0);
    actualizarMotor(movimiento[3]+2, 2);
    
    lanzarI2C ();   
}

void actualizarMotor(byte velocidad, byte motor)
{
    velocidad=(velocidad+100)*(escala/100);
    Wire.beginTransmission(ADD_BASE+motor);       // Enviamos a la dirección
    Wire.write(velocidad);                        //Enviamos velocidad+100
    Wire.endTransmission();                       //Terminamos la trama
}

void parar(void)
{
    for (motor=0;motor<4;motor++)
    {
      if (motor=!2)
      {
      Wire.beginTransmission(ADD_BASE+motor);       // Enviamos a la dirección
      Wire.write(100);                              //Enviamos velocidad=100 para parar
      Wire.endTransmission();                       //Terminamos la trama
      }
      
    }
    lanzarI2C ();
}

void lanzarI2C (void)
{
    digitalWrite(HANDSHAKE, HIGH);
    delayMicroseconds(tiempo);
    digitalWrite(HANDSHAKE, LOW);
}

void medir (void)
{
  if (brujula)
  {
    acumula_angulo+=pulseIn(BRUJULA,HIGH);
    contador++;
    
    if (contador == PROMEDIOS)
    {
      angulo = acumula_angulo / PROMEDIOS;
      acumula_angulo = 0;
    }
  }
}


