////////////////////////////////////////
// Archivo: p_arduball
//
////////////////////////////////////////


void prueba_arduball(void);
void visualiza_pot_arduball(byte reset);
void visualiza_grafica_barras_arduball(byte reset);
void visualiza_grafica_circular_arduball(byte reset);

void prueba_arduball(void)
{
#define POS_X 80
#define POS_Y 72

  byte estado_arduball = 0;
  byte menu_arduball=1;

  estado_arduball = 0;
  while(digitalRead(GO)!=0)
  {
    visualiza_pot_arduball(0);
  }
  visualiza_pot_arduball(1);
  espera_go();

  while(menu_arduball)
  {
    switch(estado_arduball)
    {
    case 0:
      visualiza_grafica_barras_arduball(0);
      if(digitalRead(GO) == 0)
      {
        visualiza_grafica_barras_arduball(1);
        estado_arduball = 1;
        menu_arduball = espera_go_timeout();
      }
      break;

    case 1:
      visualiza_grafica_circular_arduball(0);
      if(digitalRead(GO) == 0)
      {
        visualiza_grafica_circular_arduball(1);
        estado_arduball = 0;
        menu_arduball = espera_go_timeout();   
      }
      break;
    }
  }
  espera_go();  
}

void visualiza_pot_arduball(byte reset)
{
  static byte primera_vez=1;
  if(reset)
    primera_vez=1;

  else
  {
    if(primera_vez)
    {
      oled_borra_pantalla();
      oled_texto("Prueba ArduBall",2,0,GRANDE,BLANCO);
      oled_texto("GO ->",14,15,GRANDE,ROJO);

      oled_texto("Ball",2,16,PEQUE,BLANCO);
      oled_indicador_lineal(0, 10, 44, 0);
      oled_texto("Sensor",2,4,PEQUE,BLANCO);
      oled_indicador_lineal(0, 10, 77, 0);
      primera_vez=0;
    }

    Serial1.print('p');

    if(espera_serie(1,1))
    { 
      respuesta = Serial1.read();
      respuesta = constrain(respuesta,0,100);
    }

    oled_indicador_lineal(respuesta, 10, 44, 1);

    if(espera_serie(1,1))
    {

      respuesta = Serial1.read();
      respuesta = constrain(respuesta,0,100);
    }

    oled_indicador_lineal(respuesta, 10, 77, 1);
    
    if(sensor_maximo == 255)
      beep();

    delay(50);
  }
}

void visualiza_grafica_barras_arduball(byte reset)
{
  static byte primera_vez=1;
  static byte num_sensores=0;
  static byte sum_sensores=0;

  if(reset)
    primera_vez=1;

  else
  {
    if(primera_vez)
    {
      oled_borra_pantalla();
      oled_texto("Sensor:",0,0,PEQUE,BLANCO);
      oled_texto("Distancia:",12,0,PEQUE,BLANCO);
      
      /*for (conta=0; conta<24; conta++)
        oled_led(OFF, 5+(6*conta),12,LED_PEQUE,ROJO);*/

      sensor_maximo = 255;
      ultimo_sensor_maximo = 255;
      ultima_distancia_bola = 255;
      primera_vez=0;
    }

    //Leemos los sensores IR en analogico
    Serial1.print('a');
    for(conta=0; conta<24; conta++)
    {
      if(espera_serie(1,1))
      {
        sensores[conta] = Serial1.read();
      }
    }
    for (conta=0; conta<24; conta++)
      oled_indicador_lineal_vertical(sensores[conta], 5+(6*conta), 122);

    //Leemos el sensor que este dando el nivel mas alto, la distancia  a la que se encuentra la bola y si ve la bola en el chutador
    actualiza_lecturas_arduball();

    //Aqui modificamos el sensor máximo procesando los valores analogicos de sensores[..]
    //y aplicando un nivel de referencia con el potenciómetro de velocidad.
    //Leemos el potenciómetro de velocidad

    /*if((sensor_maximo > 3) && (sensor_maximo<9))
    {
      num_sensores=0;
      sum_sensores=0;
      for(conta=4; conta<9; conta++)
      {
        velocidad= map(analogRead(POT_VEL), 0, 1023, 1, 100);
        if(sensores[conta] > velocidad)
        {
          num_sensores++;
          sum_sensores= sum_sensores+conta; 
        }
      }
      if(num_sensores)
        sensor_maximo=(sum_sensores/num_sensores);
    }*/

    if ((sensor_maximo != ultimo_sensor_maximo) || (distancia_bola != ultima_distancia_bola))
    {
      if (sensor_maximo < 255)
      {     
        if (ultimo_sensor_maximo < 255)
          oled_led(ON, 5+(3*ultimo_sensor_maximo),12,LED_PEQUE,NEGRO);
        oled_led(ON, 5+(3*sensor_maximo),12,LED_PEQUE,ROJO);
      }
      else
        oled_led(ON, 5+(3*ultimo_sensor_maximo),12,LED_PEQUE,NEGRO);

      if (sensor_maximo < 255)
      {
        oled_visualiza_entero(sensor_maximo,20,0,MEDIANO);
        oled_visualiza_entero(distancia_bola,105,0,MEDIANO);
        digitalWrite(LED_1,HIGH);
      }
      else
      {
        oled_texto("     ",14,0,PEQUE,BLANCO);
        oled_texto("     ",30,0,PEQUE,BLANCO);
        digitalWrite(LED_1,LOW);
      } 

      //La funcion de visualizar entero imprime 5 caracteres. Si queremos poner el valor pegado a la
      //etiqueta tenemos que rescribir esta
      oled_texto("Sensor:",0,0,PEQUE,BLANCO);
      oled_texto("Distancia:",12,0,PEQUE,BLANCO); 
      ultimo_sensor_maximo = sensor_maximo;
      ultima_distancia_bola = distancia_bola;
    }
  }
}

void visualiza_grafica_circular_arduball(byte reset)
{
  static byte primera_vez=1;
  if(reset)
    primera_vez=1;

  else
  {
    if(primera_vez)
    {
      oled_borra_pantalla();
      oled_texto("Prueba ArduBall",2,0,GRANDE,BLANCO);

      oled_texto("Sensor:",0,2,PEQUE,BLANCO);
      oled_texto("Distancia:",12,2,PEQUE,BLANCO);

      //Mostramos un circulo de 24 elementos
      for (conta=0; conta<48;conta++)
        oled_led(OFF,POS_X+c48_x[conta],POS_Y-c48_y[conta],LED_PEQUE,ROJO);

      //Ponemos una linea en cada uno de los puntos pricipales    
      oled_linea((POS_X+c48_x[0]-13),(POS_Y-c48_y[0]),(POS_X+c48_x[0]-3),(POS_Y-c48_y[0]),ROJO);
      oled_linea((POS_X+c48_x[12]),(POS_Y-c48_y[12]+13),(POS_X+c48_x[12]),(POS_Y-c48_y[12]+3),ROJO);
      oled_linea((POS_X+c48_x[24]+13),(POS_Y-c48_y[24]),(POS_X+c48_x[24]+3),(POS_Y-c48_y[24]),ROJO);
      oled_linea((POS_X+c48_x[36]),(POS_Y-c48_y[36]-13),(POS_X+c48_x[36]),(POS_Y-c48_y[36]-3),ROJO);

      //Indicamos que numero de sensor hay en cada una de las lineas principales
      oled_texto_sin_formato("0",(POS_X+c48_x[0]-17),(POS_Y-c48_y[0]-3),PEQUE,BLANCO);
      oled_texto_sin_formato("12",(POS_X+c48_x[12]-6),(POS_Y-c48_y[12]+15),PEQUE,BLANCO);
      oled_texto_sin_formato("24",(POS_X+c48_x[24]+15),(POS_Y-c48_y[24]-3),PEQUE,BLANCO);
      oled_texto_sin_formato("36",(POS_X+c48_x[36]-4),(POS_Y-c48_y[36]-18),PEQUE,BLANCO);

      oled_texto("GO ->",14,15,GRANDE,ROJO);

      sensor_maximo = 255;
      ultimo_sensor_maximo = 255;
      primera_vez=0;
    }

    //Leemos el sensor que este dando el nivel mas alto, la distancia  a la que se encuentra la bola y si ve la bola en el chutador
    actualiza_lecturas_arduball();

    //Actualizamos los valores en el circulo de 24 sensores
    if (sensor_maximo != ultimo_sensor_maximo)
    {
      if(sensor_maximo < 255)
      {
        if(ultimo_sensor_maximo < 255)
          oled_led(OFF,POS_X+c48_x[ultimo_sensor_maximo],POS_Y-c48_y[ultimo_sensor_maximo],LED_PEQUE,ROJO);

        oled_led(ON,POS_X+c48_x[sensor_maximo],POS_Y-c48_y[sensor_maximo],LED_PEQUE,ROJO);
      }
      else
        oled_led(OFF,POS_X+c48_x[ultimo_sensor_maximo],POS_Y-c48_y[ultimo_sensor_maximo],LED_PEQUE,ROJO);

      if (sensor_maximo < 255)
      {
        oled_visualiza_entero(sensor_maximo,20,16,MEDIANO);
        digitalWrite(LED_1,HIGH);
      }
      else
      {
        oled_texto("            ",0,2,PEQUE,BLANCO);
        digitalWrite(LED_1,LOW);
      }

      oled_texto("Sensor:",0,2,PEQUE,BLANCO);

      ultimo_sensor_maximo = sensor_maximo;
    }
    delay(50);
  }
}






