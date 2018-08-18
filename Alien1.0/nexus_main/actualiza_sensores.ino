////////////////////////////////////////
// Archivo: actualiza_sensores
//
////////////////////////////////////////
char lee_datos_b=0;


//Funciones de los distintos sensores
void actualiza_sensores(void);
void actualiza_lecturas_ardusonic(void);
void actualiza_lecturas_arduball(void);
void actualiza_sensores_suelo(void);
void actualiza_consumo_roller(void);
byte espera_bus(byte p_serie,byte n_datos);

////////////////////////////////////////
//Funciones para actualizar los sensores
////////////////////////////////////////
//
//Esta funcion llama a los distintos sensores que han de ser actualizados.
//Estos se ejecutan con una determinada frecuencia independiente para cada uno de ellos.
void actualiza_sensores(void)
{   
  if(flag_ir)
  {
    flag_ir=false;
    MsTimer2::stop();
    actualiza_lecturas_arduball();
    MsTimer2::start();
  }
  
  if(flag_us)
  {
    flag_us=false;
    MsTimer2::stop();
    actualiza_lecturas_ardusonic();
    MsTimer2::start();
  }
    
  if (flag_brujula)
  {
    flag_brujula=false;
    MsTimer2::stop();
    posicion_robot_actual = varia_posicion_brujula(brujula_inicial);
    MsTimer2::start();
  }
  
  if(flag_roller)
  {
    flag_roller=false;
    MsTimer2::stop();
    actualiza_consumo_roller();
    MsTimer2::start();
  }
}
void actualiza_lecturas_ardusonic(void)
{
  static byte primera_vez_ardusonic = 0;
  static byte dato_us=0;

  if (primera_vez_ardusonic == 0)
  {
    if(n_us)
    {
      while(Serial1.available() > 0)
        lee_datos_b=Serial1.read();
      
      //Activamos las medidas por ultrasonidos en el ArduSonic
      Serial1.print('C');
      if(espera_serie(1,1))
      {
        dato_us = Serial1.read();
        primera_vez_ardusonic = 1;
      }
    } 
    else
    {
      while(Serial1.available() > 0)
        lee_datos_b=Serial1.read();
      
      Serial1.print('Q');
      if(espera_serie(1,1))
      {
        dato_us = Serial1.read();
        primera_vez_ardusonic = 1;
      }
    }
  }

  //Este comando solicita las medidas de todos los sensores de ultrasonidos, los recibe y los guarda
  if(n_us)
  {
    while(Serial1.available() > 0)
      lee_datos_b=Serial1.read();
    
    Serial1.print('A');
    if(espera_serie(1,4))
    {
      dato_us = Serial1.read();
      us_derecho = dato_us;
      dato_us = Serial1.read();
      us_trasero = dato_us;
      dato_us = Serial1.read();
      us_izquierdo = dato_us;
      dato_us = Serial1.read();
      us_frontal = dato_us;
    }
  }
  else
  {
    while(Serial1.available() > 0)
      lee_datos_b=Serial1.read();
    
    Serial1.print('A');
    if(espera_serie(1,3))
    {
      dato_us = Serial1.read();
      if(dato_us != 0)
        us_derecho = dato_us;

      dato_us = Serial1.read();
      if(dato_us != 0)
        us_trasero = dato_us;
      
      dato_us = Serial1.read();
      if(dato_us != 0)
        us_izquierdo = dato_us;
    }
  }
}

void actualiza_lecturas_arduball(void)
{
  #define ACUMULA_SENSOR_CHUTADOR 4
  #define N_PROMEDIOS_DB 3
  static int promedia_d_bola[N_PROMEDIOS_DB]= {0, 0, 0};
  static int conta_promedios_db=0;
  int promediado_de_b=0, db=0;
  //Los sensores de proximidad de bola se leen tantas veces como indica la constante
  //ACUMULA_SENSOR_CHUTADOR. La variable sensor_chutador se activa si aparece un nivel alto
  //en alguna de las medidas acumuladas

  static byte x=0, medidas[ACUMULA_SENSOR_CHUTADOR],primera_vez=1;
  byte contador, acumula;

  //Inicializa el buffer circular
  if(primera_vez)
  {
    for (contador =0; contador<ACUMULA_SENSOR_CHUTADOR; contador++)
      medidas[contador]=0;
    primera_vez=0;
  }
  
  while(Serial1.available() > 0)
    lee_datos_b=Serial1.read();
  
  Serial1.print('d');
  if(espera_serie(1,3))
  {
    sensor_maximo = Serial1.read();
    distancia_bola = Serial1.read();
    
    /*promedia_d_bola[conta_promedios_db]= Serial1.read();
    
    conta_promedios_db++;
    
    if(conta_promedios_db >= N_PROMEDIOS_DB)
      conta_promedios_db=0;
    
    promediado_de_b=0;
    for(db=0; db<N_PROMEDIOS_DB; db++)
      promediado_de_b= promediado_de_b + promedia_d_bola[db];
    
    distancia_bola =(promediado_de_b/N_PROMEDIOS_DB);*/

    //Siempre lee la medida de estos sensores de cerca pero los va metiendo en un buffer circular
    //Para que se tome como "bola no vista" tiene que no-verla varias veces (las del buffer) pero para "vista" solo una
    medidas[x]=Serial1.read();
  
    x++;
    if (x >= ACUMULA_SENSOR_CHUTADOR)
      x = 0;

    acumula = 0;
    for (contador =0; contador<ACUMULA_SENSOR_CHUTADOR; contador++)
      acumula = acumula +medidas[contador];

    if (acumula == 0)
      sensor_chutador = false;
    else
      sensor_chutador = true;
   
    if((sensor_maximo > 47) && (sensor_maximo < 255))
      sensor_maximo=255;
   
  }    
}

void actualiza_sensores_suelo(void)
{
  sensor.medida_dd = analogRead(SUELO_DERECHO_DEL);
  sensor.medida_id = analogRead(SUELO_IZQUIERDO_DEL);
  sensor.medida_dt = analogRead(SUELO_DERECHO_TRA);
  sensor.medida_it = analogRead(SUELO_IZQUIERDO_TRA);
    
  if((sensor.medida_dd>UMBRAL_SUELO_BLANCO) && (sensor.suelo_dd == 0))
    sensor.suelo_dd=1;
      
  if((sensor.medida_id>UMBRAL_SUELO_BLANCO) && (sensor.suelo_id == 0))
    sensor.suelo_id=1;
      
  if((sensor.medida_dt>UMBRAL_SUELO_BLANCO) && (sensor.suelo_dt == 0))
    sensor.suelo_dt=1;
      
  if((sensor.medida_it>UMBRAL_SUELO_BLANCO) && (sensor.suelo_it == 0))
    sensor.suelo_it=1;
}

void actualiza_consumo_roller(void)
{
  #define PROMEDIOS 8

  static int conta=0, acumula=0;
                
  if(conta<PROMEDIOS)
  {
    conta++;
    acumula += analogRead(CONSUMO_ROLLER);
  }
  else
  {
    acumula=acumula>>3;
    consumo_roller= acumula;
    acumula=0;
    conta=0;
  }
}

byte espera_serie(byte p_serie,byte n_datos)
{
#define SERIAL_TIME 200
  static long t_serial_out=0;
  t_serial_out=millis();
  switch(p_serie)
  {
    case 0:
      while((Serial.available() < n_datos) && ((millis()-t_serial_out) < SERIAL_TIME));
      break;
    case 1:
      while((Serial1.available() < n_datos) && ((millis()-t_serial_out) < SERIAL_TIME));
      break;
    case 2:
      while((Serial2.available() < n_datos) && ((millis()-t_serial_out) < SERIAL_TIME));
      break;
    case 3:
      while((Serial3.available() < n_datos) && ((millis()-t_serial_out) < SERIAL_TIME));
      break;   
  }
  if((millis()-t_serial_out) < SERIAL_TIME)
    return 1;
  else
    return 0;  
}

