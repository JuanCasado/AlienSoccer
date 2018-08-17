void mueve_robot(byte movimiento);
void gira_robot(char giro_robot);
void vel_robot(byte vel);
void vel_max_robot(byte vel);
void para_robot(void);
void chuta(void);
void voy_detras_de_la_bola(void);
void tira_porteria(void);
void pivota(char pivotar);
void activa_roller(byte control_roller);
void centra_tiro(void);
void corrige_mov_us(void);

byte zona[48]={8,8,8,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,6,6,7,7,7,7,7,7,8,8,8};
/*
     //           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
byte mov_d1[48]={45,46, 0, 1, 2, 4, 5, 6, 7, 8, 9,10,12,13,14,15,16,17,18,19,21,22,23,25,26,27,28,28,29,30,31,32,33,34,35,36,36,35,36,37,38,39,40,41,42,43,43,44};
byte mov_d2[48]={45,46, 0, 1, 2, 3, 4, 6, 7, 8, 9,10,12,13,14,15,16,17,19,20,21,22,23,25,26,27,28,29,30,30,31,32,33,34,35,36,36,35,36,37,38,39,40,41,41,42,43,44};
byte mov_d3[48]={45,46, 0, 1, 2, 3, 4, 5, 7, 8, 9,10,12,13,14,15,16,18,19,20,21,22,23,25,26,27,28,29,30,36,36,38,38,39,39,40,40,33,34,34,35,35,36,36,41,42,43,44};
byte mov_d4[48]={45,46, 0, 1, 2, 3, 4, 5, 6, 8, 9,10,12,13,14,15,17,18,19,20,21,22,23,25,26,27,28,29,30,36,37,38,39,39,40,40,41,33,33,34,34,35,35,36,41,42,43,44};
byte mov_d5[48]={44,45,46, 0, 1, 2, 3, 5, 6, 7, 9,10,12,13,14,16,17,18,20,21,22,23,25,26,27,28,29,29,30,37,37,38,39,40,41,41,42,32,32,33,33,34,35,35,41,42,42,43};
byte mov_d6[48]={44,45,46, 0, 1, 2, 3, 4, 5, 7, 8,10,12,13,15,16,18,19,20,21,22,23,25,26,27,28,29,30,31,37,38,39,40,40,41,41,43,31,31,32,32,33,34,35,40,41,42,43};
     //           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
byte mov_d7[48]={38,40,42,44,46, 1, 3, 4, 5, 8,10,10,12,14,16,20,21,22,23,25,27,28,30,32,34,35,36,37,40,41,42,43,43,44,45,47, 0,25,27,28,29,30,31,32,34,35,36,37};
byte mov_d8[48]={35,37,39,41,43, 0, 2, 3, 4, 7, 9,10,12,14,17,21,22,23,24,26,28,29,31,33,36,37,38,39,41,41,42,43,44,45,46,47, 0,25,26,27,28,29,30,30,32,34,35,34};
byte mov_d9[48]={34,36,38,40,42,47, 1, 2, 3, 6, 8,10,12,14,17,22,23,24,25,27,29,30,32,34,37,38,40,41,43,43,44,44,45,45,46, 0, 0,24,26,27,27,28,28,29,29,31,32,33};
 */

//Nexus 9.1

          //           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
byte mov_d123_91[48]={46,47, 0, 1, 2, 3, 4, 6, 7, 8, 9,10,12,14,15,16,17,18,20,21,22,23,24,25,26,27,28,28,29,30,31,32,33,34,34,36,38,36,38,38,39,40,41,42,43,44,44,45};
byte mov_d456_91[48]={46,47, 0, 0, 2, 2, 3, 4, 6, 7, 8,10,12,14,15,15,16,17,19,20,22,23,24,26,27,28,29,31,32,33,34,35,36,37,38,39,40,33,34,35,36,37,38,39,40,41,42,43};
byte mov_d789_91[48]={43,44,45,46,47, 0, 1, 2, 3, 4, 5,11,12,13,14,17,19,20,21,23,26,29,31,32,33,34,35,36,37,39,39,40,41,42,43,44,45,28,29,30,31,32,33,33,35,36,37,38};


//Nexus 9.2

          //           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
byte mov_d123_92[48]={46,47, 0, 1, 2, 3, 4, 6, 7, 8, 9,10,12,14,15,16,17,18,20,21,22,23,24,25,26,27,28,28,29,30,31,32,33,34,34,36,38,36,38,38,39,40,41,42,43,44,44,45};
byte mov_d456_92[48]={46,47, 0, 0, 2, 2, 3, 4, 6, 7, 8,10,12,14,15,15,16,17,19,20,22,23,24,26,27,28,29,31,32,33,34,35,36,37,38,39,40,33,34,35,36,37,38,39,40,41,42,43};
byte mov_d789_92[48]={42,44,45,47, 0, 1, 2, 3, 4, 6, 7,11,12,13,15,16,19,21,24,26,29,31,32,32,33,34,35,36,37,39,39,40,41,42,43,44,45,28,29,30,31,32,33,33,35,36,37,38};

          //           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
byte mov_portero[48]={ 0, 0, 0, 0, 7, 8, 9,10,11,11,12,12,12,12,12,13,13,14,15,16,17,24,24,24,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47};

void mueve_robot(byte movimiento)
{
  static int ultimo_movimiento = -1;

  if(movimiento != ultimo_movimiento)
  {
    ultimo_movimiento = movimiento;
    if (movimiento < 48)
    {
      Serial.print('M');
      Serial.write((byte)movimiento);
      if(espera_serie(0,1))
      {
        respuesta = Serial.read();
      } 
    }
    else
    {
      Serial.print('P');
      if(espera_serie(0,1))
      {
        respuesta = Serial.read();
      }
    }
  }  
}

void gira_robot(char giro_robot)
{
  Serial.print('G');
  Serial.write((byte)giro_robot);
  if(espera_serie(0,1))
  {
    respuesta = Serial.read();
  }
}

void pivota(char pivotar)
{
  if(pivotar > 100)
    pivotar=100;
  if(pivotar < -100)
    pivotar=-100;
    
  pivotar= pivotar+100;
  Serial.print('p');
  Serial.write((byte)pivotar);
  if(espera_serie(0,1))
  {
    respuesta = Serial.read();
  }
}

//Fija la velocidad maxima de un movimiento
void vel_robot(byte vel)
{
  static int ultima_vel = -1;

  if(vel != ultima_vel)
  {
    ultima_vel = vel;
    Serial.print('v');
    Serial.write((byte)vel);
    if(espera_serie(0,1))
    {
      respuesta = Serial.read();
    }
  }
}

//Fija la velocidad maxima del robot
//se fija con el potenciómetro (amarillo) del robot
void vel_max_robot(byte vel)
{
  Serial.print('V');
  Serial.write((byte)vel);
  if(espera_serie(0,1))
  {
    respuesta = Serial.read();
  }
}

void para_robot(void)
{
  Serial.print('P');
  if(espera_serie(0,1))
  {
    respuesta = Serial.read();
  }
}

void chuta(void)
{
  static long timer_chutador = 0;
  static byte primera_vez = 1;

  if(primera_vez)
  {
    primera_vez = 0; 
    timer_chutador = millis();
  }

  if((millis()-timer_chutador) > 600)
  {
    timer_chutador = millis();
    digitalWrite(CHUTADOR,LOW);
    delay(1);
    digitalWrite(CHUTADOR,HIGH);
    espera_brujula_chutador=millis();
  }
}

/*void voy_detras_de_la_bola(void)
{
  
  switch(distancia_bola)
  {
    case 1:
      direccion=mov_d1[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 2:
      direccion=mov_d2[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
    
    case 3:
      direccion=mov_d3[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 4:
      direccion=mov_d4[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 5:
      direccion=mov_d5[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 6:
      direccion=mov_d6[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 7:
      direccion=mov_d7[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 8:
      direccion=mov_d8[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    case 9:
      direccion=mov_d9[sensor_maximo];
      if ((rol_robot == 0) && (us_trasero < 25))
      {
        if((sensor_maximo >= 20) && (sensor_maximo <= 36))
          direccion=24;
        if((sensor_maximo >= 37) || (sensor_maximo <= 4))
          direccion=0;
      }
      break;
      
    default:
      break;
  }
}*/

void voy_detras_de_la_bola(void)
{
  if(robot == ROBOT_NEXUS_9_1)
  {
    switch(distancia_bola)
    {
      case 1:
      case 2:
      case 3:
        direccion=mov_d123_91[sensor_maximo];
        break;
        
      case 4:
      case 5:
      case 6:
        direccion=mov_d456_91[sensor_maximo];
        break;
      
      case 7:
      case 8:
      case 9:
        direccion=mov_d789_91[sensor_maximo];
        break;   
      
      default:
        break;
    }
  }
  else if(robot == ROBOT_NEXUS_9_2)
  {
    switch(distancia_bola)
    {
      case 1:
      case 2:
      case 3:
        direccion=mov_d123_92[sensor_maximo];
        break;
      
      case 4:
      case 5:
      case 6:
        direccion=mov_d456_92[sensor_maximo];
        break;
      
      case 7:
      case 8:
      case 9:
        direccion=mov_d789_92[sensor_maximo];
        break;   
      
      default:
        break;
    }
  }
  if(((us_izquierdo+us_derecho) > 100) && (us_trasero < 25) && (rol_robot == 0))
  {
    mov_portero[sensor_maximo];
  }
    
  /*if (us_trasero < 25)
  {
    if((sensor_maximo >= 20) && (sensor_maximo <= 36))
      direccion=24;
    if((sensor_maximo >= 37) || (sensor_maximo <= 4))
      direccion=0;
  }*/
  
}


void tira_porteria(void)
{
  #define MARGEN_GIRO 10
  #define T_ROLLER 50
  #define GIRA_P 5
  #define T_OUT_GIRO 400
  #define D_TRASERA 110
  static int gira_p=0,brujula_pre=0, abs_us=0;
  static unsigned long t_roller=0, t_out_giro=0;
  static byte primera_vez=1;
  
  //Tengo la bola
  if((sensor_chutador) && (consumo_roller > CONSUMO_MAXIMO_ROLLER))
  {
    actualiza_sensores();
    
    abs_us=abs(us_derecho-us_izquierdo);
    
    if(abs_us > MARGEN_GIRO)
    {
      if(us_derecho > us_izquierdo)
      { 
        if(us_trasero > D_TRASERA)
        {
          while((posicion_robot_actual < 20) && (sensor_chutador) && (consumo_roller > CONSUMO_MAXIMO_ROLLER))
          {
            manda_xbee();
            actualiza_sensores();
            pivota(30);
          }
          t_out_giro=millis();
          
          while((millis()-t_out_giro) < 50)
            manda_xbee();
          
          chuta();
        }
        else
        {
          while((abs_us > MARGEN_GIRO) && (sensor_chutador) && (consumo_roller > CONSUMO_MAXIMO_ROLLER))
          {
            manda_xbee();
            actualiza_sensores();
            abs_us=abs(us_derecho-us_izquierdo);
            direccion=6;
            centra_brujula(0);
            actualiza_motores();
          }
          if((posicion_robot_actual > -10) && (posicion_robot_actual < 10))
            chuta();
        }
      }
      else
      {
        if(us_trasero > D_TRASERA)
        {
          while((posicion_robot_actual > -20) && (sensor_chutador) && (consumo_roller > CONSUMO_MAXIMO_ROLLER))
          {
            manda_xbee();
            actualiza_sensores();
            pivota(-30);
          }
          t_out_giro=millis();
          
          while((millis()-t_out_giro) < 50)
            manda_xbee();
          
          chuta();
        }
        else
        {
          while((abs_us > MARGEN_GIRO) && (sensor_chutador) && (consumo_roller > CONSUMO_MAXIMO_ROLLER))
          {
            manda_xbee();
            actualiza_sensores();
            abs_us=abs(us_derecho-us_izquierdo);
            direccion=18;
            centra_brujula(0);
            actualiza_motores();

          }
          if((posicion_robot_actual > -10) && (posicion_robot_actual < 10))
            chuta();
        }
      }
    }
    else
    {
      if((posicion_robot_actual > -10) && (posicion_robot_actual < 10))
        chuta();  
    }
  }
}

void activa_roller(byte control_roller)
{
  if (control_roller)
  {
  //Activamos el roller cuando tenemos la pelota por la zona delantera
  if((sensor_maximo>6)&&(sensor_maximo<18))
    analogWrite(ROLLER,255);
  else
    analogWrite(ROLLER,0);
  }
  else
    analogWrite(ROLLER,0);
}
void centra_tiro(void)
{
  if(us_derecho> us_izquierdo)
  {
    if(abs(us_derecho-us_izquierdo > 40))
      direccion=8;
      
    if(abs(us_derecho-us_izquierdo > 60))
      direccion=6;
      
  }
  else
  {
    if(abs(us_derecho-us_izquierdo > 40))
      direccion=15;
      
    if(abs(us_derecho-us_izquierdo > 60))
      direccion=18;
  }
}
void corrige_mov_us(void)
{
  if((direccion > 16) && (direccion < 32) && (us_izquierdo == 1) && (us_trasero < 20))
    direccion=12;
  if((direccion < 8) || (direccion > 40) && (us_derecho == 1) && (us_trasero < 20))
    direccion=12;  
  if((direccion >= 32) && (direccion <= 32) && (us_trasero == 1))
  {
    if((sensor_maximo > 12) && (sensor_maximo < 36))
      direccion=24;     
    else
      direccion=0;
  }
}
void despejar(void)
{
  if(consumo_roller > CONSUMO_MAXIMO_ROLLER)
  {
    conta_tengo_roller++;
    if((conta_tengo_roller > 20) && (posicion_robot_actual > -10) && (posicion_robot_actual < 10))
    {
      chuta();
      conta_tengo_roller=0;
    }
  }
  else
  {
    conta_tengo_roller=0;
  }
}
