////////////////////////////////////////
// Archivo: p_chutador
//
////////////////////////////////////////


void prueba_chutador(void);

void prueba_chutador(void)
{
  byte estado_anterior = 0;
  byte cambia_pot = 0;

  oled_texto("Prueba Chutador",2,0,GRANDE,BLANCO);
  oled_texto("Bola en chutador",2,3,GRANDE,BLANCO);
  oled_texto("Pulse GO para chutar",0,9,GRANDE,BLANCO);
  oled_led(OFF,83,65,LED_PEQUE,AMARILLO);
  oled_indicador_lineal(respuesta, 10, 90, 1);

  while(1)
  { 
    if(digitalRead(GO) == 0)
    {  
      espera_go();  
      beep();
      chuta();
    }    
    else
    {
      Serial1.flush();
      Serial1.print('b');
      while(Serial1.available() == 0);
      sensor_chutador = Serial1.read();

      if(sensor_chutador != estado_anterior)
      {
        if(sensor_chutador == 1)
          oled_led(ON,83,65,LED_PEQUE,AMARILLO);
        else
          oled_led(OFF,83,65,LED_PEQUE,AMARILLO);

        estado_anterior = sensor_chutador;
      }
    }
    
    // Este comando devuelve la lectura de los dos potenciometros.
    // Como el de "sensor" (lejos), =primer dato, no nos hace falta aquí, no se hace nada con ello
    Serial1.flush();
    Serial1.print('p');
    while (Serial1.available() == 0); 
    respuesta = Serial1.read();
    while (Serial1.available() == 0); 
    respuesta = Serial1.read();
    respuesta = constrain(respuesta,0,100);

    // Indicador lineal del potenciometro de "ball"
    if(cambia_pot != respuesta)
    {
      oled_indicador_lineal(respuesta, 10, 90, 1);
      cambia_pot = respuesta;
    }
  }
}
