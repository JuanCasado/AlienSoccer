//////////////////////////////////
//Prueba brujula
//////////////////////////////////
void prueba_brujula(void);
byte centra_brujula (int parar);

void prueba_brujula(void)
{
  oled_texto("Prueba Brujula",3,0,GRANDE,BLANCO);
  oled_texto("GO ->",14,15,GRANDE,ROJO);


  while(digitalRead(GO) == 1)
  {
    actualiza_sensores();
    oled_visualiza_entero(posicion_robot_actual,50,50,MEDIANO);

    velocidad= map(analogRead(POT_VEL), 0, 1023, 1, 100);
    if (velocidad != velocidad_robot)
    {
      oled_indicador_lineal(velocidad, 10, 108, 1);
      velocidad_robot = velocidad;
    }
    delay(50);
  }
  
  espera_go();
  oled_borra_pantalla();

  // Vel_max_robot
  Serial.print('V');
  Serial.write((byte)velocidad_robot);
  while(Serial.available() <=  0);
  respuesta = Serial.read();
  
  while(1)
  {

    centra_brujula(1);
    posicion_robot_actual = varia_posicion_brujula(brujula_inicial);
    gira_robot(giro_robot);
    mueve_robot(direccion);
  }
}

byte centra_brujula (int parar)
{
#define ERROR_POSICION 2  

  //Kp brujula min es el de la velocidad 100
  int kp_brujula_min, ks_brujula;
  int giro_centra_brujula=0;
  int error_centra_brujula=0;
  static byte centrado=0;
  static byte primera_vez=1;
  static int kp_brujula=0;

  if (primera_vez == 1)
  {
    primera_vez=0;
    switch(robot)
    {
      
    case ROBOT_NEXUS_9_2:
      kp_brujula_min = 7;     //Kp para 100%
      ks_brujula = 18;
      break;

    case ROBOT_NEXUS_9_1:   
      kp_brujula_min = 7;   //Kp para 100%
      ks_brujula = 18;
      break;
    }
    kp_brujula= (100-velocidad_robot)*ks_brujula;
    kp_brujula= kp_brujula/100;
    kp_brujula += kp_brujula_min; 
  }

  /////////////////////////////////////////////////////////////////////
  // Descomentar para calibrar la brujula /////////////////////////////
  // KS = ([KP(40) - KP(100)] / 100-40) *100
  // Para ajustar el Kp, usando el modulo de prueba de la brujula, ponemos la velocidad a 100
  // Despues de pulsar el GO, movemos el potenciometro hasta que ajustemos el kp para 100
  // Ese numero sera kp_brujula_min 
  // Repetimos el proceso con velocidad 40 y usando esa formula sacamos el valor de KS
  // Dentro del if(primera_vez) escribimos los valores del kp_brujula_min y ks_brujula
  // A partir de ellos siempre se calculará el kp_brujula 
  /*kp_brujula = analogRead(POT_VEL);
  kp_brujula=map(kp_brujula,0,1023,0,50);
  oled_visualiza_entero(kp_brujula,40,40,GRANDE);*/
  /////////////////////////////////////////////////////////////////////

  //Esa espera es un timeout para no centrar inmediatamente después del chutado porque se torcería
  if((millis()-espera_brujula_chutador)>0)
  {
    if (abs(posicion_robot_actual) > ERROR_POSICION)
    {
      digitalWrite(LED_2,LOW);
      error_centra_brujula = - posicion_robot_actual;
      giro_centra_brujula = (error_centra_brujula*kp_brujula)/10;
      giro_centra_brujula = constrain(giro_centra_brujula,-100,100);
      giro_robot=giro_centra_brujula;
      centrado=0;
    }  
    else
    {
      digitalWrite(LED_2,HIGH);
      centrado=1;
      if(parar)
        direccion=255;
    }
  }
  return centrado;
}
