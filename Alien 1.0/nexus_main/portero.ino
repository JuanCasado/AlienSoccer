//
///////////////////////////////////////
// Archivo: portero
//
// Funciones para el roll de portero en campo B
//
// junio 2011
//
////////////////////////////////////////

void juega_portero(void);
void centra_portero(void);
void portero_preventivo(void);
void portero_despeja(void);
void recibe_xbee(void);


void juega_portero(void)
{ 
  
rol_robot = (digitalRead(INT_JUGADOR));

  if(robot == ROBOT_NEXUS_9_2)
  {
#define CONSUMO_MAXIMO_ROLLER 20
  } 
  else
  {
#define CONSUMO_MAXIMO_ROLLER 30
  }
  

  // Comienza el roll de portero
  while(true)
  {
    
    
    //Actualizamos periodicamente todos los sensores
    actualiza_sensores();

    recibe_xbee();

    //Nos centramos con la brujula
    centra_brujula(0);  

    //vemos la bola?
    if((sensor_maximo>47) || (del_pelota))
    {
      //Si no vemos bola nos centramos en la porteria
      activa_roller(false);
      centra_portero();
    }     
    //Si vemos bola    
    else
    {
      activa_roller(true);

      if((distancia_bola <= 3) && (sensor_maximo > 4) && (sensor_maximo < 20))
      {
        portero_preventivo();
        //centra_portero();
        //portero_despeja();
      }
      else
      {
        portero_despeja();
        if((distancia_bola > 4) || (direccion < 10) && (direccion > 14) && (consumo_roller < CONSUMO_MAXIMO_ROLLER))
        {
          velocidad_robot=80;
        }
        else
          velocidad_robot=100;
      }
    }
      
    actualiza_motores();
  }
}

void centra_portero(void)
{  
  #define MAXIMO_LATERAL 100
  #define DISTANCIA_TRASERA 20
  #define DISTANCIA_TRASERA_ESQUINA 28
  #define ERROR_US_LATERAL 5
  #define ERROR_US_TRASERO 2
  #define KP_US 40

  static unsigned long espera_us=0;
  int us_error_trasero, us_error_lateral;

  // Se centra en horizontal y luego en trasero con la porteria
  ////////////////////////////////////////////////////////////////////////////
  if(us_error_lateral > 30)
    us_error_trasero = abs(us_trasero - DISTANCIA_TRASERA_ESQUINA);
  else
    us_error_trasero = abs(us_trasero - DISTANCIA_TRASERA_ESQUINA);
    
    
  if((us_izquierdo+us_derecho)>MAXIMO_LATERAL)
  {
    us_error_lateral = abs(us_izquierdo - us_derecho);

    // Primera parte : Centrado trasero
    if (us_error_trasero > ERROR_US_TRASERO)
    {
      us_error_trasero = (us_error_trasero*KP_US)/10;
      us_error_trasero = constrain(us_error_trasero,0,100);
      velocidad_robot = us_error_trasero;
      if (us_trasero > DISTANCIA_TRASERA)
      {
        estoy_centrado=0;
        if (us_error_lateral > ERROR_US_LATERAL)
        {
          if (us_derecho > us_izquierdo)
          {
            direccion= 40;
            
            if(us_trasero < 60)
              direccion= 44;  
              
            if(us_trasero < 40)
              direccion= 2;  
            
            if(us_error_lateral < 10)
              direccion= 39;
            
          }
          else
          {
            direccion= 32;
            
            if(us_trasero < 60)
              direccion= 28;  
              
            if(us_trasero < 40)
              direccion= 22; 
              
            if(us_error_lateral < 10)
              direccion= 33; 
          }
        }
        else
          direccion=36;
      }
      else
      {
        estoy_centrado=0;
        direccion=12;
      }
    }

    // Segunda parte : Centrado lateral
    else if (us_error_lateral > ERROR_US_LATERAL)
    {
      us_error_lateral = (us_error_lateral*KP_US)/10;
      us_error_lateral = constrain(us_error_lateral,0,100);
      velocidad_robot = us_error_lateral;

      if (us_derecho > us_izquierdo)
        direccion=0;
      else
        direccion=24;
    }
    else
    {
      //Para robot
        direccion=255;    
    }
  }    
  else
  {
    //La distancia lateral es menor de lo esperado (esquina, robot contrario...)
    if(((us_izquierdo+us_derecho) < 45) && (us_trasero < 25))
    {

      espera_us=millis();
      while(((millis()-espera_us) < 500) && (distancia_bola < 5))
      {
        actualiza_sensores();
        centra_brujula(0); 
        direccion=12;
        velocidad_robot=100;
        actualiza_motores();
      }
      espera_us=millis();
      while(((millis()-espera_us) < 400) && (distancia_bola < 5))
      {
        actualiza_sensores();
        centra_brujula(0); 
        velocidad_robot=100;
        if(us_derecho>us_izquierdo)
          direccion=0;
        else
          direccion=24;
        actualiza_motores();
      }
    }
    else if(((us_izquierdo+us_derecho) < 45) && (us_trasero > 40))
    {
      us_error_lateral=0;
      direccion=36;
    }
  }
}

void portero_preventivo(void)
{
  centra_portero_ir();
}

void portero_despeja(void)
{
  //velocidad_robot=100;

  //Intentamos ponernos detras de la bola y ganarla la posicion
  voy_detras_de_la_bola();
  
  //Salir de la esquina
  if(((us_izquierdo+us_derecho) < 45) && (us_trasero < 25))
  {
    direccion = 12;
  }
  if(((us_izquierdo+us_derecho) > 100) && (us_trasero < 25))
  {
    if((sensor_maximo > 24) && (sensor_maximo < 36))
      direccion=24;
    else if(sensor_maximo > 36)
      direccion=0;
  }
  tira_porteria();
  //despejar();
}


void centra_portero_ir(void)
{  
#define DISTANCIA_TRASERA 16
#define ERROR_US_TRASERO 2
#define LIMITE_MOV 65

   //                                    6   7   8   9  10  11  12  13  14  15  16  17  18
  static int tabla_vel_preventiva[13]={100, 95, 90, 85, 80, 75,  0, 75, 80, 85, 90, 95,100};
  
  int us_error_trasero;

  us_error_trasero = abs(us_trasero - DISTANCIA_TRASERA);

  // Primera parte : Centrado trasero
  if (us_error_trasero > ERROR_US_TRASERO)
  {
    velocidad_robot = tabla_vel_preventiva[sensor_maximo-6];
    if (us_trasero > DISTANCIA_TRASERA)
    {
      if(tabla_vel_preventiva[sensor_maximo-6] > 0)
      {  
        if (sensor_maximo < 12)
        {
          if(us_derecho > LIMITE_MOV)
          {
            if(us_trasero > 10)
              direccion=40;
            else
              direccion=46;
          }
          else
          {
            direccion=24;
            velocidad_robot=10;
          }
        }
        else
        {
          if(us_izquierdo > LIMITE_MOV)
          {
            if(us_trasero > 50)
              direccion=32;
            else
              direccion=26;
          }
          else
          {
            direccion=0;
            velocidad_robot=50;
          }
        }
      }
      else
        direccion=36;
    }
    else
      direccion=12;  
  }
  // Segunda parte : Centrado lateral
  else if(tabla_vel_preventiva[sensor_maximo] > 0)
  {
    velocidad_robot = tabla_vel_preventiva[sensor_maximo];
    
    if (sensor_maximo < 12)
    {
      if(us_derecho > LIMITE_MOV)
        direccion=0;
      else
      {
        direccion=24;
        velocidad_robot=50;
      }
    }
    else
    {
      if(us_izquierdo > LIMITE_MOV)
        direccion=24;
      else
      {
        direccion=0;
        velocidad_robot=50;
      }
    }
  }
  else
  {
    //Para robot
    direccion=255;      
  }
}
void recibe_xbee(void)
{
#define TIME_OUT_XBEE 800

  static unsigned long time_out_xbee=0;
  static char lee_xbee, primera_vez=1;
  
  if(primera_vez)
  {
    time_out_xbee=millis();
    primera_vez=0;
  }
  
  if((millis()-time_out_xbee) > TIME_OUT_XBEE)
    xbee=false;
  
  if(xbee)
  {
    if(Serial3.available() > 0)
    {
      lee_xbee = Serial3.read();
        
      switch(lee_xbee)
      {
        case 'C':
          del_pelota=true;
          break;
        case 'D':
          del_pelota=false;
          break;
      }   
      time_out_xbee=millis();
    }
  }
}
