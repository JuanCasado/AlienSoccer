////////////////////////////////////////
// Archivo: jugar
//
////////////////////////////////////////

void jugar (void);

void jugar (void)
{
  int x=60,y=1;                             //para el rectangulo donde pone gol
  unsigned long bat;
  sensor.brujula=255;
  
  oled_borra_pantalla();

  oled_texto("SSI",15,4,MEDIANO,AZUL);
  oled_texto("SSD",3,4,MEDIANO,AZUL);
  oled_texto("BRU",9,4,MEDIANO,AZUL);
  oled_texto("USD",17,9,MEDIANO,ROJO);
  oled_texto("USI",1,9,MEDIANO,ROJO);
  oled_texto("ELE",5,190,MEDIANO,AZUL);     //Baterias
  oled_texto("POT",15,190,MEDIANO,AZUL);
  //tabla(s_suelo y brujula)      
  oled_linea(10,25,150,25,BLANCO);          //lineas horizontales 
  oled_linea(10,45,150,45,BLANCO); 
  oled_linea(10,65,150,65,BLANCO);
  oled_linea(10,25,10,65,BLANCO);           //lineas verticales
  oled_linea(150,25,150,65,BLANCO);    
  oled_linea(56,25,56,65,BLANCO);    
  oled_linea(102,25,102,65,BLANCO); 

  marca_gol_amarillo = 255;                 //valores que nunca se van a dar para que la primera vez sea distinto
  si_xbee = 225;
  rol_robot=225;

  //Actualiza pantalla hasta que se pulsa GO
  while(digitalRead(GO) == 1)
  {
    //Actuliza la medida de todos los sensores
    actualiza_sensores();
    
    ///////////////////////////////////////////////////////////////
    //Interruptores
    
    respuesta = (digitalRead(INT_PORTERIA));             //Seleccion de la porteria en la que metemos gol
    if (marca_gol_amarillo != respuesta)                 //la primera vez siempre sera distinto es decir, actualiza
    {
      marca_gol_amarillo = respuesta;                    //marca gol amarillo vale ahora 0 o 1
      if(marca_gol_amarillo == 1)          
      {
        oled_rectangulo(x,y,40+x,14+y,AMARILLO);                
        oled_texto_sin_formato("-GOL-",x+6,y+4,PEQUE,BLANCO);
      }  
      else
      {
        oled_rectangulo(x,y,40+x,14+y,AZUL);                
        oled_texto_sin_formato("-GOL-",x+6,y+4,PEQUE,BLANCO);
      }
    }  

    respuesta = (digitalRead(INT_XBEE));                  //decidimos si usamos o no la cc3
    if (si_xbee != respuesta)
    {
      si_xbee = respuesta;
      if(si_xbee == 1)
      {      
        oled_texto("XBee",14,1,MEDIANO,VERDE);
        xbee=true;
      }
      else
      {
        oled_texto("XBee",14,1,MEDIANO,ROJO);
        xbee=false;
      }   
    }

    respuesta = (digitalRead(INT_JUGADOR));             //decidimos si juega de portero o de delantero
    if (rol_robot != respuesta)
    {
      rol_robot = respuesta;
      if(rol_robot == 1)
        oled_texto("DEL   ",1,1,MEDIANO,VERDE);
      else
      {
        oled_texto("POR  ",1,1,MEDIANO,ROSA); 
      }
    }   
    
    ///////////////////////////////////////////////////////////////
    //Suelo
    //Dibuja un circulo en blanco o en verde segun el color que este viendo
    oled_pluma(SOLIDO);
    if(sensor.suelo_dd)
    {
      sensor.suelo_dd=0;
      oled_circulo(115,50,GRANDE,BLANCO);
    }
    else
      oled_circulo(115,50,GRANDE,VERDE);

    //**************************************
    //Dibuja un circulo en blanco o en verde segun el color que este viendo
    if(sensor.suelo_id)
    {
      sensor.suelo_id=0;
      oled_circulo(50,50,GRANDE,BLANCO);
    }
    else
       oled_circulo(50,50,GRANDE,VERDE);

    //**************************************
    //Dibuja un circulo en blanco o en verde segun el color que este viendo
    if(sensor.suelo_dt)
    {
      sensor.suelo_it=0;
      oled_circulo(145,60,GRANDE,BLANCO);
    }
    else
      oled_circulo(145,60,GRANDE,VERDE);

    //**************************************
    if(sensor.suelo_it)
    {
      sensor.suelo_it=0;
      oled_circulo(20,60,GRANDE,BLANCO);
    }
    else
      oled_circulo(20,60,GRANDE,VERDE);
    
    ///////////////////////////////////////////////////////////////
    //Velocidad
    velocidad = map(promedia_ai(POT_VEL,16), 1, 950, 1, 100);            //Indicador de velocidad de juego
    if(velocidad != velocidad_robot)
    {
      velocidad_robot = velocidad;  
      oled_indicador_lineal(velocidad, 10, 100, 1);
    }
    
    ///////////////////////////////////////////////////////////////
    //Brujula
    //Solo se pinta en la patanlla si la posicion de la brujula varia
    
    if(posicion_robot_actual != sensor.brujula)
    {
      sensor.brujula = posicion_robot_actual;
      oled_visualiza_entero(posicion_robot_actual,60,53,MEDIANO);
    }

    ///////////////////////////////////////////////////////////////
    //Ultrasonidos
    //Solo se pinta en el display si los valores de los sensores cambian
    
    respuesta_int = us_izquierdo;
    if(respuesta_int != sensor.us_izq)
    {
      sensor.us_izq = respuesta_int; 
      oled_visualiza_entero(respuesta_int,6,83,MEDIANO);
    }
    
    respuesta_int = us_frontal;
    if(respuesta_int != sensor.us_fron)
    {
      sensor.us_fron = respuesta_int;   
      oled_visualiza_entero(respuesta_int,50,70,MEDIANO);
    }
      
    respuesta_int = us_trasero;
    if(respuesta_int != sensor.us_tra)
    {
      sensor.us_tra = respuesta_int;   
      oled_visualiza_entero(respuesta_int,50,90,MEDIANO);
    }
      
    respuesta_int = us_derecho;
    if(respuesta_int != sensor.us_der)
    {
      sensor.us_der = respuesta_int;    
      oled_visualiza_entero(us_derecho,101,83,MEDIANO);
    }

    ///////////////////////////////////////////////////////////////
    //Baterías
    //Solo se pinta en el display si los valores de los sensores cambian
    bat = (promedia_ai(BAT_POT,16)* 59.0)/25.0;
    if(bat != sensor.b_pot)
    {
      sensor.b_pot = bat;
      oled_visualiza_entero(bat, 80, 175,MEDIANO);
    }

    bat = (promedia_ai(BAT_ELECTRO,16)* 59.0)/25.0;
    if(bat != sensor.b_ele)
    {
      sensor.b_ele = bat;
      oled_visualiza_entero(bat, 1, 175,MEDIANO);
    }
  }

  //Se ha pulsado el GO
  //Fija la velocidad maxima del robot
  MsTimer2::stop();
  vel_max_robot(velocidad_robot);

  //Pone el ArduBall en modo juego
  Serial1.print('j');
  if(espera_serie(1,1))
  {
    respuesta = Serial1.read();
    MsTimer2::start();

    oled_borra_pantalla();
    oled_texto("SUELTE PARA EMPEZAR",1,10,MEDIANO,ROSA);
    while(digitalRead(GO) == 0);
    oled_borra_pantalla();

    if (rol_robot)
      juega_delantero();
    else
    {
      juega_portero();
    }
  }
}
