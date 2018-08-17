#include "Arduino.h"
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

//Posiciones X e Y para trazar un círculo con 48 LEDs
//Se usan para indicar los movimientos y los sensores
//Las cordenadas indican el centro del circulo

int c48_x[48] = {
  43, 43, 42, 40, 37, 34, 30, 26, 22, 16, 11, 6, 0, -6, -11, -16, -22, -26, -30, -34, -37, -40, -42, -43,
  -43, -43, -42, -40, -37, -34, -30, -26, -22, -16, -11, -6, 0, 6, 11, 16, 22, 26, 30, 34, 37, 40, 42, 43
};
int c48_y[48] = {
  0, 6, 11, 16, 22, 26, 30, 34, 37, 40, 42, 43, 43, 43, 42, 40, 37, 34, 30, 26, 22, 16, 11, 6,
  0, -6, -11, -16, -22, -26, -30, -34, -37, -40, -42, -43, -43, -43, -42, -40, -37, -34, -30, -26, -22, -16, -11, -6,
};

//Variable comun donde guardamos lo que recibimos por el puerto serie
byte respuesta = 0, rellena_medidas = 0;

int respuesta_int = 0;

int consumo_roller = 0;
int conta = 0;
byte maquina_centrado_b = 0, estoy_centrado = 0;
char centradito_porteria = 0;

byte marca_gol_amarillo = 0, si_xbee = 0, rol_robot = 0;

//Variable modificada por varias funciones que se usa para controlar el movimiento del robot
int direccion = 255, direccion_anterior = 0;

//Variables del selector
byte selector = 0;
byte selector_anterior = 255;

//Variable para la identificacion del robot
byte robot;

//Variables para el movimiento
int velocidad_robot = -1;
int velocidad = 10;
int movimiento = 255;
int movimiento_anterior = 0;
int velocidad_anterior = 0;
int giro_robot = 0, giro_robot_anterior = 0;
int conta_tengo_roller = 0;

//Variables de arduball
byte sensor_chutador = 0;
byte sensor_maximo = 49, ultimo_sensor_maximo;
byte distancia_bola, ultima_distancia_bola;
byte sensores[24];

//Variables de la brujula
int posicion_robot_actual = 0;
int brujula_inicial;
long espera_brujula_chutador = 0;


//Variables de ardusonic
int us_izquierdo = 0, us_derecho = 0, us_trasero = 0, us_frontal = 0;
int us_c_izquierdo = 0, us_c_derecho = 0, corrige_us_d = 0, corrige_us_i = 0;

//Varibles de el XBee
int xbee = false;
char del_pelota = false;

//Flags para la actualizacion de sensores
byte flag_brujula = false, flag_us = false, flag_ir = false, flag_motores = false, flag_roller = false;

byte posicion_x = 0, posicion_y = 0;


//Sensores
struct SENSORES {
  int suelo_id, suelo_dd, suelo_dt, suelo_it, medida_dd, medida_id, medida_dt, medida_it;
  int us_izq, us_der, us_tra, us_fron;
  int brujula;
  int b_pot, b_ele;
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
  digitalWrite(CHUTADOR, HIGH);
  pinMode(CHUTADOR, OUTPUT);


  //Hace un parpadeo con los 4 LED's de señalizacion y pita dos veces el zumbador
  comprueba_led(1);
  doble_beep();
  comprueba_led(0);

  //comprobamos tipo de robot
  pinMode(TIPO_ROBOT_A, INPUT);
  pinMode(TIPO_ROBOT_B, INPUT);
  robot = (digitalRead(TIPO_ROBOT_A) + digitalRead(TIPO_ROBOT_B));

  //Muestra la pnatalla de presentacion
  pantalla_presentacion();
}

void loop()
{
  //Interruptor en modo JUGAR
  if (digitalRead(INT_MODO) == 1)
  {
    jugar();
  }
  MsTimer2::stop();
  //Interrutor en modo AJUSTAR
  //Menú de módulos de prueba
  visualiza_menu();
  while (digitalRead(GO) == 1)
  {
    selector = lee_selector();
    if (selector != selector_anterior)
    {
      if (selector_anterior != 255)
      {
        oled_caracter(selector_anterior + 0x30, 0, selector_anterior + 2, MEDIANO, BLANCO);
        oled_texto("->", 1, selector_anterior + 2, MEDIANO, BLANCO);
      }
      oled_caracter(selector + 0x30, 0, selector + 2, MEDIANO, AZUL_CLARO);
      oled_texto("->", 1, selector + 2, MEDIANO, AZUL_CLARO);
      oled_visualiza_entero(selector, 42, 117, MEDIANO);
      oled_texto_sin_formato("Selector:", 0, 117, MEDIANO, BLANCO);
      selector_anterior = selector;
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
  selector_anterior = 255;
}

