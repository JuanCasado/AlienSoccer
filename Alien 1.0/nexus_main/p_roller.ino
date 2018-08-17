////////////////////////////////////////
// Archivo: p_roller
//
////////////////////////////////////////


void prueba_roller(void)
{
  #define CONSUMO_MAXIMO_ROLLER 20

  byte estado_anterior = 0;
  oled_texto("Prueba Roller",3,1,GRANDE,BLANCO);
  oled_texto("LED on = Pelota",2,3,GRANDE,ROJO);
  analogWrite(ROLLER,191);
  
  while(1)
  {
    actualiza_sensores();
    if(estado_anterior!=consumo_roller)
    {
      estado_anterior=consumo_roller;
      oled_visualiza_entero(estado_anterior,34,105,MEDIANO);
      if(estado_anterior > CONSUMO_MAXIMO_ROLLER)
        digitalWrite(LED_1,HIGH);
      else
        digitalWrite(LED_1,LOW);
      delay(100);
    }
    delay(10);
  }
}
