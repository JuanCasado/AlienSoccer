////////////////////////////////////////
// Archivo: nexus.cpp
//
////////////////////////////////////////


#include "WProgram.h"
#include "nexus.h"
#include "oled_160.h"
#include "brujula.h"

void beep(void)
{
  static byte primera_vez=0;

  if (primera_vez == 0)
  {
    pinMode(ZUMBADOR,OUTPUT);
    primera_vez=1;
  }

  digitalWrite(ZUMBADOR,HIGH);
  delay(50);
  digitalWrite(ZUMBADOR,LOW);
}

void doble_beep(void)
{
  static byte primera_vez=0;

  if (primera_vez == 0)
  {
    pinMode(ZUMBADOR,OUTPUT);
    primera_vez=1;
  }

  digitalWrite(ZUMBADOR,HIGH);
  delay(50);
  digitalWrite(ZUMBADOR,LOW);
  delay(50);
  digitalWrite(ZUMBADOR,HIGH);
  delay(50);
  digitalWrite(ZUMBADOR,LOW);
}

void resetea_perifericos(void)
{
  static byte primera_vez=0;

  if (primera_vez == 0)
  {
    pinMode(RESET,OUTPUT);
    primera_vez=1;
  }
  //Reseteamos perifericos
  digitalWrite(RESET,HIGH);
  delay(5);
  digitalWrite(RESET,LOW);  
}

void espera_go(void)
{
  static byte primera_vez=0;

  if (primera_vez == 0)
  {
    pinMode(GO,INPUT);
    primera_vez=1;
  }   
  while(digitalRead(GO) == 1);
  delay(50);
  while(digitalRead(GO) == 0);
  delay(50);
}  

byte espera_go_timeout(void)
{
  while(digitalRead(GO) == 1);

  //Elimina rebotes
  delay(50);

  while(digitalRead(GO) == 0)
  {
    if(espera_cambio_menu(digitalRead(GO)) == 0)
      return 0;
  }

  //Elimina rebotes
  delay(50);

  //Reseteamos la funcion para inicializar el contador
  espera_cambio_menu(1);
  return 1;
}

void configura_interruptores(void)
{
  pinMode(INT_XBEE,INPUT);
  pinMode(INT_JUGADOR,INPUT);
  pinMode(INT_PORTERIA,INPUT);
  pinMode(INT_MODO,INPUT);
}

void configura_selector(void)
{
  pinMode(SELECTOR_1,INPUT);
  pinMode(SELECTOR_2,INPUT);
  pinMode(SELECTOR_4,INPUT);
  pinMode(SELECTOR_8,INPUT);
}

byte lee_selector(void)
{
  return (!digitalRead(SELECTOR_1) + (!digitalRead(SELECTOR_2) <<1) + (!digitalRead(SELECTOR_4) <<2) + (!digitalRead(SELECTOR_8) <<3));
}

void comprueba_led(byte maneja_led)
{
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
  pinMode(LED_3,OUTPUT);
  pinMode(LED_4,OUTPUT);

  if(maneja_led == 1)
  {
    digitalWrite(LED_1,HIGH);
    digitalWrite(LED_2,HIGH);
    digitalWrite(LED_3,HIGH);
    digitalWrite(LED_4,HIGH);
  }

  else
  {
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,LOW);
    digitalWrite(LED_3,LOW);
    digitalWrite(LED_4,LOW); 
  } 
}

int promedia_ai(int ai, int promedios)
{
  int conta;
  unsigned int acumula=0;

  if (promedios > 64)
    promedios =64;

  for (conta = 0; conta <promedios; conta ++)
    acumula += analogRead(ai);

  acumula/=promedios;

  return (int)acumula;
}

void pantalla_presentacion(void)
{
  byte respuesta=0;
  byte cont=0;
  
  oled_opaco_transparente(OPACO);
  oled_pluma(SOLIDO);
  //////////////////
  oled_texto("ArduBall Mod 1.1",0,3,PEQUE,BLANCO); 
  Serial1.print('i');
  delay(50);
  if(Serial1.available() == 0)
    oled_texto("[FAIL]",20,3,PEQUE,ROJO);
  else
  {
    oled_texto("[OK]",20,3,PEQUE,VERDE);
    do
    {
      respuesta = Serial1.read();
    }while(respuesta != 0x0A);
  }  
  
  delay(50);
    
  oled_texto("ArduSonic 1.1",0,4,PEQUE,BLANCO); 
  Serial1.print('W');
  delay(50);
  if(Serial1.available() == 0)
    oled_texto("[FAIL]",20,4,PEQUE,ROJO);
  else
  {
    oled_texto("[OK]",20,4,PEQUE,VERDE); 
    do
    {
      respuesta = Serial1.read();
    }while(respuesta != 0x0A);
  }
  
  delay(50);
 
  oled_texto("ArduMotion 1.2",0,5,PEQUE,BLANCO); 
  Serial.print('i');
  delay(50);
  if(Serial.available() == 0)
    oled_texto("[FAIL]",20,5,PEQUE,ROJO);
  else
  {
    oled_texto("[OK]",20,5,PEQUE,VERDE); 
    do
    {
      respuesta = Serial.read();
    }while(respuesta != 0x0A);
  }  
   
  delay(500); 
  oled_borra_pantalla();

  if(digitalRead(TIPO_ROBOT_A)+digitalRead(TIPO_ROBOT_B) == ROBOT_NEXUS_9_1)
    oled_texto("NEXUS 9.1 - 48",3,2,GRANDE,ROSA);
  else
    oled_texto("NEXUS 9.2 - 48",3,2,GRANDE,ROSA);
    
  oled_texto("COMPLUBOT SOCCER",2,4,GRANDE,ROJO);
  oled_texto("RCJ 2011",6,6,GRANDE,ROJO);
  oled_texto("www.complubot.org",2,9,GRANDE,AZUL);

  delay(800);
  oled_borra_pantalla();
}

void visualiza_menu(void)
{
  oled_borra_pantalla();

  oled_texto("MENU",8,0,MEDIANO,ROJO);
  oled_texto("0-> Baterias",0,2,MEDIANO,BLANCO);
  oled_texto("1-> ArduBall",0,3,MEDIANO,BLANCO);
  oled_texto("2-> ArduMotion",0,4,MEDIANO,BLANCO);
  oled_texto("3-> ArduSonic",0,5,MEDIANO,BLANCO);
  oled_texto("4-> Brujula",0,6,MEDIANO,BLANCO);
  oled_texto("5-> Chutador",0,7,MEDIANO,BLANCO); 
  oled_texto("6-> Sigue bola",0,8,MEDIANO,BLANCO);
  oled_texto("7-> Calibra Brujula",0,9,MEDIANO,BLANCO);
  oled_texto("8-> Comunica XBee",0,10,MEDIANO,BLANCO);
  oled_texto("9-> Roller + S cerca",0,11,MEDIANO,BLANCO);
  oled_texto("GO ->",14,15,GRANDE,ROJO);
}

byte espera_cambio_menu(byte p_go)
{
#define ESPERA_GO_CONTINUO 1200

  static long contador_menu = 0;
  static byte primera_vez = 1;

  if(p_go == 0)
  {
    if(primera_vez)
    {
      contador_menu = millis();
      primera_vez=0;
    }
    else
    {
      if((millis()-contador_menu) > ESPERA_GO_CONTINUO)
      {
        beep();
        primera_vez=1;
        return 0;
      }   
    }
  }
  else
    primera_vez=1;  

  return 1;
}

int lee_pot_digital(byte modo, int value)
{ 
  //Modos: 0=> Reset; 1=> Actualiza medida; 2=> Carga contador con valor

#define encoder_PinA  22
#define encoder_PinB  23

  static int encoder_Pos = 0;
  static int encoder_PinALast = LOW;
  int n = LOW;

  switch(modo)
  {
  case 0: //Reset
    pinMode (encoder_PinA,INPUT);
    pinMode (encoder_PinB,INPUT);
    encoder_Pos = 0;
    encoder_PinALast = LOW;
    break;

  case 1: //Actualiza medida
    n = digitalRead(encoder_PinA);
    if ((encoder_PinALast == LOW) && (n == HIGH))
    {
      if (digitalRead(encoder_PinB) == LOW)
        encoder_Pos--;
      else 
        encoder_Pos++;
    } 
    encoder_PinALast = n;
    break;

  case 2: //Carga contador con valor
    encoder_Pos = value;
    break;
  }
  return encoder_Pos;

} 


