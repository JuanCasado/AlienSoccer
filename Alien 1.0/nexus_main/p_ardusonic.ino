////////////////////////////////////////
// Archivo: p_ardusonic
//
////////////////////////////////////////


void prueba_ardusonic(void);

void prueba_ardusonic(void)
{
  oled_texto("Prueba ArduSonic",1,0,GRANDE,BLANCO);
  oled_texto("x:",5,4,MEDIANO,BLANCO);
  oled_texto("y:",5,6,MEDIANO,BLANCO);
  while(1)
  {
    actualiza_lecturas_ardusonic();
    oled_visualiza_entero(us_izquierdo,6,83,MEDIANO);
    oled_visualiza_entero(us_trasero,50,90,MEDIANO);
    oled_visualiza_entero(us_frontal,50,70,MEDIANO);
    oled_visualiza_entero(us_derecho,101,83,MEDIANO);
    
    oled_visualiza_entero(posicion_x,50,30,MEDIANO);
    oled_visualiza_entero(posicion_y,50,50,MEDIANO);
    
    delay(50);
  }
}
