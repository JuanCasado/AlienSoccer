#include "WProgram.h"
#include "nexus.h"
#include "oled_160.h"
#include "brujula.h"
#include <Wire.h>
#include <MsTimer2.h>

#define TRES_US 0
#define CUATRO_US 1

//definimos el tipo de brujula que usamos
//#define CMPS03
//#define HMC5362

//Posiciones X e Y para trazar un c\u00edrculo con 48 LEDs
//Se usan para indicar los movimientos y los sensores
//Las cordenadas indican el centro del circulo
  
#include "WProgram.h"
void periodica(void);
void setup();
void loop();
void comunica_xbee(void);
void actualiza_motores(void);
void actualiza_sensores(void);
void actualiza_lecturas_ardusonic(void);
void actualiza_lecturas_arduball(void);
void actualiza_sensores_suelo(void);
void actualiza_consumo_roller(void);
byte espera_serie(byte p_serie,byte n_datos);
void pruebas(void);
void juega_delantero(void);
void centra_delantero(void);
void manda_xbee(void);
void jugar (void);
void mueve_robot(byte movimiento);
void gira_robot(char giro_robot);
void pivota(char pivotar);
void vel_robot(byte vel);
void vel_max_robot(byte vel);
void para_robot(void);
void chuta(void);
void voy_detras_de_la_bola(void);
void tira_porteria(void);
void activa_roller(byte control_roller);
void centra_tiro(void);
void corrige_mov_us(void);
void despejar(void);
void prueba_arduball(void);
void visualiza_pot_arduball(byte reset);
void visualiza_grafica_barras_arduball(byte reset);
void visualiza_grafica_circular_arduball(byte reset);
void prueba_ardumotion(void);
void seleciona_movimiento_ardumotion(byte reset);
void prueba_ardusonic(void);
void monitor_baterias(void);
void prueba_brujula(void);
byte centra_brujula (int parar);
void prueba_chutador(void);
void prueba_roller(void);
void sigue_pelota(void);
void juega_portero(void);
void centra_portero(void);
void portero_preventivo(void);
void portero_despeja(void);
void centra_portero_ir(void);
void recibe_xbee(void);
int c48_x[48]={
   43, 43, 42, 40, 37, 34, 30, 26, 22, 16, 11, 6,0,-6,-11,-16,-22,-26,-30,-34,-37,-40,-42,-43,
  -43,-43,-42,-40,-37,-34,-30,-26,-22,-16,-11,-6,0, 6, 11, 16, 22, 26, 30, 34, 37, 40, 42, 43};
int c48_y[48]={
  0, 6, 11, 16, 22, 26, 30, 34, 37, 40, 42, 43, 43, 43, 42, 40, 37, 34, 30, 26, 22, 16, 11, 6,
  0,-6,-11,-16,-22,-26,-30,-34,-37,-40,-42,-43,-43,-43,-42,-40,-37,-34,-30,-26,-22,-16,-11,-6,};  

//Variable comun donde guardamos lo que recibimos por el puerto serie
byte respuesta=0, rellena_medidas=0;

int respuesta_int=0;

int consumo_roller=0;
int conta=0;
byte maquina_centrado_b=0, estoy_centrado=0;
char centradito_porteria=0;

byte marca_gol_amarillo=0, si_xbee=0, rol_robot=0;

//Variable modificada por varias funciones que se usa para controlar el movimiento del robot
int direccion=255, direccion_anterior=0;

//Variables del selector
byte selector=0;
byte selector_anterior=255;

//Variable para la identificacion del robot
byte robot;

//Variables para el movimiento
int velocidad_robot=-1;
int velocidad=10;
int movimiento=255;
int movimiento_anterior=0;
int velocidad_anterior=0;
int giro_robot=0, giro_robot_anterior=0;
int conta_tengo_roller=0;

//Variables de arduball
byte sensor_chutador = 0;
byte sensor_maximo=49, ultimo_sensor_maximo;
byte distancia_bola, ultima_distancia_bola;
byte sensores[24];

//Variables de la brujula
int posicion_robot_actual=0;
int brujula_inicial;
long espera_brujula_chutador=0;


//Variables de ardusonic
int us_izquierdo=0, us_derecho=0, us_trasero=0, us_frontal=0;
int us_c_izquierdo=0, us_c_derecho=0, corrige_us_d=0, corrige_us_i=0;

//Varibles de el XBee
int xbee=false;
char del_pelota=false;

//Flags para la actualizacion de sensores
byte flag_brujula = false, flag_us = false, flag_ir = false, flag_motores = false, flag_roller = false;

byte posicion_x=0, posicion_y=0;


//Sensores
struct SENSORES{
  int suelo_id,suelo_dd,suelo_dt,suelo_it, medida_dd, medida_id, medida_dt, medida_it;
  int us_izq,us_der,us_tra,us_fron;
  int brujula;
  int b_pot,b_ele;
};
SENSORES sensor;

byte n_us = TRES_US;

void periodica(void)
{
#define VECES_BRUJULA 4
#define VECES_US 2
#define VECES_IR 2
#define VECES_ROLLER 4 
#define VECES_MOTORES 4 

  static byte flag_conta_brujula = VECES_BRUJULA;
  static byte flag_conta_us = VECES_US;
  static byte flag_conta_ir = VECES_IR;
  static byte flag_conta_roller = VECES_ROLLER;
  static byte flag_conta_motores = VECES_MOTORES;

  //Actualiza brujula
  flag_conta_brujula--;
  if (flag_conta_brujula == 0);
  {
    flag_brujula = true;
    flag_conta_brujula = VECES_BRUJULA;
  }

  //Actualiza ultrasonidos
  flag_conta_us--;
  if (flag_conta_us == 0);
  {
    flag_us = true;
    flag_conta_us = VECES_US;
  }

  //Actualiza los ir's
  flag_conta_ir--;
  if (flag_conta_ir == 0);
  {
    flag_ir = true;
    flag_conta_ir = VECES_IR;
  }

  //Actualizamos sensores suelo
  actualiza_sensores_suelo();

  //Actualiza consumo roller
  flag_conta_roller--;
  if (flag_conta_roller == 0);
  {
    flag_roller = true;
    flag_conta_roller = VECES_ROLLER;
  }

  //Actualiza motores
  flag_conta_motores--;
  if (flag_conta_motores == 0);
  {
    flag_motores = true;
    flag_conta_motores = VECES_MOTORES;
  }
}

void setup()
{
  //Resetea los perifericos
  resetea_perifericos();

  //Inicializa el display
  oled_ini();
  oled_opaco_transparente(OPACO);
  oled_pluma(SOLIDO);

  configura_selector();

  //Inicializa el puerto serie de Ardumotion
  Serial.begin(250000);

  //Inicializa el puerto serie de Arduball y Ardusonic
  Serial1.begin(250000);
  
  //Inicializa puerto serie XBee
  Serial3.begin(38400);
  
  //Inicializa la brujula y toma la posicion inicial 
  inicializa_brujula();
  delay(10);
  brujula_inicial = lee_brujula();

  //Configura periodica
  MsTimer2::set(10, periodica); // se ejecuta cada 10ms
  MsTimer2::start();

  //Configuramos el chutador
  digitalWrite(CHUTADOR,HIGH);
  pinMode(CHUTADOR,OUTPUT);


  //Hace un parpadeo con los 4 LED's de se\u00f1alizacion y pita dos veces el zumbador
  comprueba_led(1);
  doble_beep();
  comprueba_led(0);

  //comprobamos tipo de robot
  pinMode(TIPO_ROBOT_A, INPUT);
  pinMode(TIPO_ROBOT_B, INPUT);
  robot=(digitalRead(TIPO_ROBOT_A)+digitalRead(TIPO_ROBOT_B));

  //Muestra la pnatalla de presentacion
  pantalla_presentacion();
}

void loop() 
{
  //Interruptor en modo JUGAR
  if(digitalRead(INT_MODO) == 1)
  {
    jugar();
  }
  MsTimer2::stop();
  //Interrutor en modo AJUSTAR
  //Men\u00fa de m\u00f3dulos de prueba
  visualiza_menu();  
  while(digitalRead(GO) == 1)
  {
    selector = lee_selector();
    if(selector != selector_anterior)
    {
      if (selector_anterior != 255)
      {
        oled_caracter(selector_anterior+0x30,0,selector_anterior+2,MEDIANO,BLANCO);
        oled_texto("->",1,selector_anterior+2,MEDIANO,BLANCO);
      }
      oled_caracter(selector+0x30,0,selector+2,MEDIANO,AZUL_CLARO);
      oled_texto("->",1,selector+2,MEDIANO,AZUL_CLARO);
      oled_visualiza_entero(selector,42,117,MEDIANO);
      oled_texto_sin_formato("Selector:",0,117,MEDIANO,BLANCO);
      selector_anterior=selector;
      delay(50);
    }
  }

  //Se ha seleccionado alguno de los modulos de prueba, y entra
  espera_go();
  oled_borra_pantalla();

  switch (selector)
  {
  case 0:      //muestra el nivel de baterias
    monitor_baterias();
    break;

  case 1:      //lee el valor de los 24 sensores
    prueba_arduball();
    break;  

  case 2:     //muestra el valor del pot de ajustar la velocidad
    prueba_ardumotion();
    break;

  case 3:
    prueba_ardusonic();
    break;

  case 4:
    prueba_brujula();
    break;

  case 5:
    prueba_chutador();
    break;

  case 6:
    sigue_pelota();
    break;

  case 7:
    //Calibracion de la brujula HMC5362
    pruebas();
    break;

  case 8:
    //prueba_cmucam3();
    comunica_xbee();
    break;

  case 9:
    prueba_roller();
    break;

  default:
    break;  
  } 

  //Permite volver a mostrar el mismo elemento del menu
  selector_anterior=255;
}

void comunica_xbee(void);

int mv=0;

void comunica_xbee(void)
{
  //Borra terminal VT100
  Serial3.print(0x1B,BYTE);
  Serial3.print("[2J");
  
  if(robot == ROBOT_NEXUS_9_2)
    Serial3.println("Nexus 9.2 - Command Mode");

  else
    Serial3.println("Nexus 9.1 - Command Mode");

  Serial3.print("> ");

  while(true)
  {    
    if(Serial3.available() > 0)
    {
      switch(Serial3.read())
      {
      case 'B':
        Serial3.println();
        Serial3.print("> ");
        Serial3.print("Bat Electro: ");
        Serial3.print((promedia_ai(BAT_ELECTRO,16) * 59.0)/2500.0);
        Serial3.print("V");
        Serial3.print(" Bat Pot: ");
        Serial3.print((promedia_ai(BAT_POT,16) * 59.0)/2500.0);
        Serial3.println("V");
        Serial3.print("> ");
        break;

      case 'c':
        Serial3.println();
        Serial3.print("> ");
        Serial3.print("Posicion Brujula: ");
        Serial3.print(varia_posicion_brujula(brujula_inicial));
        Serial3.println();
        Serial3.print("> ");
        break;  

      case 'C':
        chuta();
        Serial3.println();
        Serial3.print("> ");
        break;


      case 'h':
      case 'H':
        Serial3.println();
        Serial3.println();
        Serial3.println("Ayuda");
        Serial3.println("-----");
        Serial3.println();
        Serial3.println("B   -> Tension Baterias");
        Serial3.println("c   -> Brujula");
        Serial3.println("C   -> Chuta");
        Serial3.println("I   -> Identificacion");
        Serial3.println("M   -> Mueve Robot");
        Serial3.println("P,p -> Para Robot");
        Serial3.println("S   -> Sensores Bola");
        Serial3.println("U   -> Sensores Ultrasonidos");
        Serial3.println("V   -> Ajusta Velocidad");
        Serial3.println();
        Serial3.print("> ");
        break;

      case 'I':
        beep();
        if(robot == ROBOT_NEXUS_9_2)
          Serial3.println(" Nexus 9.2");

        else
          Serial3.println(" Nexus 9.1");
        Serial3.print("> ");
        break;

      case 'M':
        while(Serial3.available() < 2);
        mv=((Serial3.read()-0x30)*10)+(Serial3.read()-0x30);
        if((mv>=0) && (mv<48))
        {
          mueve_robot(mv);
        }
        else
        {
          Serial3.println("Error");
        }
        Serial3.println();
        Serial3.print("> ");
        break;

      case 'p':
      case 'P':
        para_robot();
        Serial3.println();
        Serial3.print("> ");
        break;

      case 'S':
        actualiza_lecturas_arduball();

        Serial3.println();
        Serial3.print("> ");
        Serial3.print("Sensor Max: ");
        Serial3.print(sensor_maximo);
        Serial3.print("Distancia: ");
        Serial3.println(distancia_bola);
        Serial3.print("> ");
        break;

      case 'U':
        actualiza_lecturas_ardusonic();

        Serial3.println();
        Serial3.print("> ");
        Serial3.print("US Derecho: ");
        Serial3.print(us_derecho);
        Serial3.print(" US Izquierdo: ");
        Serial3.print(us_izquierdo);
        Serial3.print(" US Trasero: ");
        Serial3.println(us_trasero);
        Serial3.print("> ");
        break;

      case 'V':
        while(Serial3.available() < 3);
        mv=((Serial3.read()-0x30)*100)+((Serial3.read()-0x30)*10)+(Serial3.read()-0x30);
        if((mv>=0) && (mv<101))
        {
          vel_max_robot(mv);
        }
        else
        {
          Serial3.println("Error");
        }
        Serial3.println();
        Serial3.print("> ");
        break;

      default:
        Serial3.println(" what?");
        Serial3.print("> ");
        break;
      }
    }
  }
}


void actualiza_motores(void);

void actualiza_motores(void)
{
  if(flag_motores)
  {
    flag_motores = false;
    //if(direccion != direccion_anterior)
    //{
      digitalWrite(LED_4,HIGH);
      direccion_anterior=direccion;
      MsTimer2::stop();
      mueve_robot(direccion);
      MsTimer2::start();
      digitalWrite(LED_4,LOW);
    //}
  
    if(velocidad_robot != velocidad_anterior)
    {
      velocidad_anterior=velocidad_robot;
      MsTimer2::stop();
      vel_robot(velocidad_robot);
      MsTimer2::start();
    }
    
    if(giro_robot != giro_robot_anterior)
    {
      giro_robot_anterior=giro_robot;
      MsTimer2::stop();
      gira_robot(giro_robot);
      MsTimer2::start();
    }
  }
}
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

void pruebas(void);

void pruebas(void)
{
  while(digitalRead(GO)!=0)
  {
    oled_texto("Calibra brujula",1,0,GRANDE,BLANCO);
    velocidad_robot = 15;  
  }
  espera_go();
  oled_borra_pantalla();
  
  oled_texto("Calibrando brujula...",1,0,GRANDE,BLANCO);
  Wire.beginTransmission(0X21);
  Wire.send("C");
  Wire.endTransmission();  
  gira_robot(velocidad_robot);
  delay(30000);
  para_robot();
  Wire.beginTransmission(0X21);
  Wire.send("E");
  Wire.endTransmission();
  oled_borra_pantalla();
  oled_texto("Calibracion terminada",1,0,GRANDE,BLANCO);
  delay(2000);
}
////////////////////////////////////////
// Archivo: delantero
//
////////////////////////////////////////

void juega_delantero(void);
void centra_delantero(void);
void manda_xbee(void);

void juega_delantero(void)
{
  rol_robot = (digitalRead(INT_JUGADOR));

  if(robot == ROBOT_NEXUS_9_2)
  {
#define CONSUMO_MAXIMO_ROLLER 30
  } 
  else
  {
#define CONSUMO_MAXIMO_ROLLER 30
  }   

  while(1)
  {
    //Actualizamos periodicamente todos los sensores y los motores
    actualiza_sensores();
    manda_xbee();

    centra_brujula(0);

    if(sensor_maximo>47)
    {
      centra_delantero();
      //direccion=48;
      activa_roller(0);
    }
    else
    {
      activa_roller(1);
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
      //Intentamos tirar a la porteria
      tira_porteria();

      if((distancia_bola > 6) || (direccion < 10) && (direccion > 14) && (consumo_roller < CONSUMO_MAXIMO_ROLLER))
      {
        velocidad_robot=80;
      }
      else
        velocidad_robot=100;
    }   

    actualiza_motores();
  }
}

void centra_delantero(void)
{  
#define MAXIMO_LATERAL_A 105
#define DISTANCIA_TRASERA_A 50
#define ERROR_US_LATERAL 10
#define ERROR_US_TRASERO 5
#define KP_US 30

  int us_error_trasero, us_error_lateral;
  static unsigned long espera_us=0;

  us_c_izquierdo = us_izquierdo;
  us_c_derecho = us_derecho;

  // Se centra en horizontal y luego en trasero con la porteria
  ////////////////////////////////////////////////////////////////////////////
  us_error_trasero = abs(us_trasero - DISTANCIA_TRASERA_A);

  if((us_izquierdo+us_derecho)>MAXIMO_LATERAL_A)
    us_error_lateral = abs(us_izquierdo - us_derecho);    
  else
  {
    if(((us_izquierdo+us_derecho) < 45) && (us_trasero < 25))
    {
      espera_us=millis();
      while(((millis()-espera_us) < 400) && (sensor_maximo>23))
      {
        centra_brujula(0); 
        actualiza_motores();
        actualiza_sensores();
        direccion=12;
        velocidad_robot=velocidad;
      }
      espera_us=millis();
      while(((millis()-espera_us) < 600) && (sensor_maximo>23))
      {
        centra_brujula(0); 
        actualiza_motores();
        actualiza_sensores();
        velocidad_robot=velocidad;
        if(us_derecho>us_izquierdo)
          direccion=0;
        else
          direccion=24;
      }
    }

    us_error_lateral=0;
  }
  ////////////////////////////////////////////////////////////////////////////

  // Primera parte : Centrado trasero
  if (us_error_trasero > ERROR_US_TRASERO)
  {
    us_error_trasero = (us_error_trasero*KP_US)/10;
    us_error_trasero = constrain(us_error_trasero,0,100);
    velocidad_robot = us_error_trasero;
    if (us_trasero > DISTANCIA_TRASERA_A)
    {
      estoy_centrado=0;
      if (us_error_lateral > ERROR_US_LATERAL)
      {
        if (us_derecho > us_izquierdo)
          direccion=40;
        else
          direccion=32;
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
    velocidad_robot=us_error_lateral;
    if (us_derecho > us_izquierdo)
      direccion=0;
    else
      direccion=24;
  }
  else
  {
    direccion=255;      
  }
}

void manda_xbee(void)
{
#define TIME_OUT_XBEE 250

  static unsigned long time_out_xbee=0;
  static char tengo_bola_roller=0, tengo_bola_roller_anterior=1, cambio=0, primera_vez=1;
  
  if(primera_vez)
  {
    time_out_xbee=millis();
    primera_vez=0;
  }
  
  if(xbee)
  {
    if((consumo_roller > CONSUMO_MAXIMO_ROLLER) && (tengo_bola_roller=1))
      tengo_bola_roller=1;
    else if(consumo_roller < CONSUMO_MAXIMO_ROLLER)
      tengo_bola_roller=0;


    if(tengo_bola_roller_anterior != tengo_bola_roller)
    {
      cambio=true;
      tengo_bola_roller_anterior = tengo_bola_roller;
    }  
    else
      cambio=false;

    if((cambio) || ((millis()-time_out_xbee) > TIME_OUT_XBEE))
    {
      if(tengo_bola_roller)
        Serial3.print('C');
      else
        Serial3.print('D');

      time_out_xbee=millis();
    }
  }
}





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
    //Bater\u00edas
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
      Serial.print(movimiento, BYTE);
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
  Serial.print(giro_robot,BYTE);
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
  Serial.print(pivotar, BYTE);
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
    Serial.print(vel,BYTE);
    if(espera_serie(0,1))
    {
      respuesta = Serial.read();
    }
  }
}

//Fija la velocidad maxima del robot
//se fija con el potenci\u00f3metro (amarillo) del robot
void vel_max_robot(byte vel)
{
  Serial.print('V');
  Serial.print(vel,BYTE);
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

    //Aqui modificamos el sensor m\u00e1ximo procesando los valores analogicos de sensores[..]
    //y aplicando un nivel de referencia con el potenci\u00f3metro de velocidad.
    //Leemos el potenci\u00f3metro de velocidad

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
        Serial.print(velocidad_robot,BYTE);
        if(espera_serie(0,1))
          respuesta = Serial.read();

        Serial.print('M');
        Serial.print(movimiento, BYTE);
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
////////////////////////////////////////
// Archivo: p_baterias
//
////////////////////////////////////////


void monitor_baterias(void);

void monitor_baterias(void)
{
  unsigned long bat;

  oled_texto("MONITOR DE BATERIAS",0,1,GRANDE,BLANCO);

  while(espera_cambio_menu(digitalRead(GO)))
  {
    bat = (promedia_ai(BAT_POT,16) * 59.0)/25.0;
    oled_bat("Pot", bat, 30, 45, VERDE, 0);
    bat = (promedia_ai(BAT_ELECTRO,16) * 59.0)/25.0;
    oled_bat("Elec",bat, 80, 45, VERDE, 0);
    delay(200);
  }
  
  espera_go();
}  
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
  Serial.print(velocidad_robot,BYTE);
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
  // A partir de ellos siempre se calcular\u00e1 el kp_brujula 
  /*kp_brujula = analogRead(POT_VEL);
  kp_brujula=map(kp_brujula,0,1023,0,50);
  oled_visualiza_entero(kp_brujula,40,40,GRANDE);*/
  /////////////////////////////////////////////////////////////////////

  //Esa espera es un timeout para no centrar inmediatamente despu\u00e9s del chutado porque se torcer\u00eda
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
    // Como el de "sensor" (lejos), =primer dato, no nos hace falta aqu\u00ed, no se hace nada con ello
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
////////////////////////////////////////
// Archivo: p_sigue_pelota
//
////////////////////////////////////////


void sigue_pelota(void);

void sigue_pelota(void)
{
  delay(200);
  while(digitalRead(GO) == 1)
  {
    velocidad= map(analogRead(POT_VEL), 0, 1023, 1, 100);
    if (velocidad != velocidad_robot)
    {
      Serial.print('V');
      Serial.print(velocidad,BYTE);
      while(Serial.available() == 0);
      respuesta = Serial.read();      
      oled_indicador_lineal(velocidad, 10, 95, 0);
      velocidad_robot = velocidad;
      delay(100);
    }
  }
  espera_go();
  MsTimer2::start();
  while(1)
  {
    velocidad_robot = 100;
    actualiza_sensores();
    
    
    centra_brujula(0);
    
    if(sensor_maximo > 47)
    {
      direccion=48;
    }    
    else
    {
      direccion=sensor_maximo;
      
    }
    actualiza_motores();
  }
}
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
  #define MAXIMO_LATERAL 105
  #define DISTANCIA_TRASERA 16
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

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

