void actualiza_motores(void);

void actualiza_motores(void)
{
  if(flag_motores)
  {
    flag_motores = false;
    //if(direccion != direccion_anterior)
    //{
      digitalWrite(LED_4,HIGH);
      direccion_anterior=direccion;
      MsTimer2::stop();
      mueve_robot(direccion);
      MsTimer2::start();
      digitalWrite(LED_4,LOW);
    //}
  
    if(velocidad_robot != velocidad_anterior)
    {
      velocidad_anterior=velocidad_robot;
      MsTimer2::stop();
      vel_robot(velocidad_robot);
      MsTimer2::start();
    }
    
    if(giro_robot != giro_robot_anterior)
    {
      giro_robot_anterior=giro_robot;
      MsTimer2::stop();
      gira_robot(giro_robot);
      MsTimer2::start();
    }
  }
}
