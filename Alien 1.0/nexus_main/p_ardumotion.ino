////////////////////////////////////////
// Archivo: p_ardumotion
//
////////////////////////////////////////


void prueba_ardumotion(void);
void seleciona_movimiento_ardumotion(byte reset);

void prueba_ardumotion(void)
{
  byte estado_ardumotion = 0;
  byte menu_ardumotion = 1;
  byte primera_vez=1;

  movimiento = 0;

  while(menu_ardumotion)
  {  
    switch(estado_ardumotion)
    {
    case 0:
      seleciona_movimiento_ardumotion(0);
      if(digitalRead(GO) == 0)
      {
        seleciona_movimiento_ardumotion(1);
        estado_ardumotion = 1;
        menu_ardumotion = espera_go_timeout();
      }
      break;

    case 1:     
      if(primera_vez)
      {
        oled_borra_pantalla();
        //Mueve el robot con el movimiento y velocidad selecionados
        Serial.print('V');
        Serial.print((byte)velocidad_robot);
        if(espera_serie(0,1))
          respuesta = Serial.read();

        Serial.print('M');
        Serial.write((byte)movimiento);
        if(espera_serie(0,1))
          respuesta = Serial.read();
        primera_vez=0;
      }

      if(digitalRead(GO) == 0)
      {
        beep();
        Serial.print('P');
        if(espera_serie(0,1))
        {
          respuesta = Serial.read();
          estado_ardumotion = 0;
          menu_ardumotion = espera_go_timeout();
          primera_vez=1;
        }
      }
      break;
    }
  }
  espera_go();
}

void seleciona_movimiento_ardumotion(byte reset)
{
  static byte primera_vez=1;

  if(reset)
    primera_vez=1;
  else
  {
    if(primera_vez)
    {
      oled_texto("Prueba ArduMotion",1,0,GRANDE,BLANCO);
      oled_texto("GO ->",14,15,GRANDE,ROJO);
      selector_anterior = lee_selector();   
      oled_visualiza_entero(movimiento,30,117,MEDIANO);
      oled_texto_sin_formato("Mov:",0,117,MEDIANO,BLANCO);  
      oled_indicador_lineal(0, 10, 108, 0);
      velocidad_robot = -1;
      selector_anterior=255;
      for (conta=0; conta<48;conta++)
        oled_led(OFF,80+c48_x[conta],58-c48_y[conta],LED_MUY_PEQUE,AZUL);

      oled_linea((80+c48_x[0]-10),(58-c48_y[0]),(80+c48_x[0]-3),(58-c48_y[0]),AZUL);
      oled_linea((80+c48_x[12]),(58-c48_y[12]+10),(80+c48_x[12]),(58-c48_y[12]+3),AZUL);
      oled_linea((80+c48_x[24]+10),(58-c48_y[24]),(80+c48_x[24]+3),(58-c48_y[24]),AZUL);
      oled_linea((80+c48_x[36]),(58-c48_y[36]-10),(80+c48_x[36]),(58-c48_y[36]-3),AZUL);

      oled_texto_sin_formato("0",(80+c48_x[0]-15),(58-c48_y[0]-3),PEQUE,AZUL);
      oled_texto_sin_formato("12",(80+c48_x[12]-5),(58-c48_y[12]+11),PEQUE,AZUL);
      oled_texto_sin_formato("24",(80+c48_x[24]+10),(58-c48_y[24]-3),PEQUE,AZUL);
      oled_texto_sin_formato("36",(80+c48_x[36]-5),(58-c48_y[36]-15),PEQUE,AZUL);

      //Reseteamos el contador del potenciometro digital
      lee_pot_digital(0,0);
      movimiento_anterior = -1;

      primera_vez=0;
    }

    movimiento = lee_pot_digital(1,0);
    if (movimiento <0)
    {
      lee_pot_digital(2,47);
      movimiento=0;
    }
    
    if(movimiento >47)
    {
      lee_pot_digital(0,0);
      movimiento=0;
    }

    if (movimiento != movimiento_anterior)
    {
      oled_visualiza_entero(movimiento,30,117,MEDIANO);
      oled_texto_sin_formato("Mov:",0,117,MEDIANO,BLANCO);
      oled_led(OFF,80+c48_x[movimiento_anterior],58-c48_y[movimiento_anterior],LED_MUY_PEQUE,AZUL);
      oled_led(ON,80+c48_x[movimiento],58-c48_y[movimiento],LED_MUY_PEQUE,AZUL);
      movimiento_anterior=movimiento;
    }

    //Hay que promediarla
    velocidad= map(promedia_ai(POT_VEL,8), 0, 1023, 1, 100);
    if (velocidad != velocidad_robot)
    {
      oled_indicador_lineal(velocidad, 10, 108, 1);
      velocidad_robot = velocidad;
      //delay(100);  
    }
  }
}






