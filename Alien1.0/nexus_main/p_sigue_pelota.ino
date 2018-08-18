////////////////////////////////////////
// Archivo: p_sigue_pelota
//
////////////////////////////////////////


void sigue_pelota(void);

void sigue_pelota(void)
{
  delay(200);
  while(digitalRead(GO) == 1)
  {
    velocidad= map(analogRead(POT_VEL), 0, 1023, 1, 100);
    if (velocidad != velocidad_robot)
    {
      Serial.print('V');
      Serial.write((byte)velocidad);
      while(Serial.available() == 0);
      respuesta = Serial.read();      
      oled_indicador_lineal(velocidad, 10, 95, 0);
      velocidad_robot = velocidad;
      delay(100);
    }
  }
  espera_go();
  MsTimer2::start();
  while(1)
  {
    velocidad_robot = 100;
    actualiza_sensores();
    
    
    centra_brujula(0);
    
    if(sensor_maximo > 47)
    {
      direccion=48;
    }    
    else
    {
      direccion=sensor_maximo;
      
    }
    actualiza_motores();
  }
}
