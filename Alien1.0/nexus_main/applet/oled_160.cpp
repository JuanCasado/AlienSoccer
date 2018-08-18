////////////////////////////////////////
// Archivo: oled_160.cpp
//
////////////////////////////////////////


#include "WProgram.h"
#include "oled_160.h"

void oled_ini()
{
  Serial2.begin(OLED_BAUDRATE);

  //Espera a que termine la inicialización del display
  delay(1000);  //Tiempo de espera para la inicialización

  //Auto Baudrate
  Serial2.print(0x55,BYTE);
  oled_espera_respuesta();
  //delay(1000);
}

byte oled_espera_respuesta()
{
  byte respuesta;
  
  do
  {
  // Wait for data avaliable
  while (!Serial2.available()) { delay(1); }
  
  // Read incoming byte
  respuesta = Serial2.read();
  }while (respuesta !=0x06 && respuesta!=0x15 );
  
  return respuesta;
}

void oled_demo()
{
  int x,y;
  
  oled_borra_pantalla();
  
  oled_opaco_transparente(OPACO);
  oled_pluma(SOLIDO);
  oled_texto("NEXUS 6.5",6,2,GRANDE,ROSA);
  oled_texto("COMPLUBOT SOCCER",2,4,GRANDE,ROJO);
  oled_texto("RCJ 2009",6,6,GRANDE,ROJO);
  oled_texto("www.complubot.org",2,9,GRANDE,AZUL);

  delay(2000);

  oled_borra_pantalla();
  
  for(x=0; x<12; x++)
    oled_led(OFF, 15+(x*12), 10, LED_PEQUE, ROJO);
  
  for(x=0; x<12; x++)
    oled_led(OFF, 15+(x*12), 30, LED_PEQUE, VERDE);
    
  oled_bat("Pot", 1234, 30, 45, VERDE, 0);
  oled_bat("Elec.",785, 80, 45, VERDE, 0);
  
  oled_indicador_lineal(73, 10, 95, 0);
  oled_indicador_lineal(34, 10, 110, 0);
  
  delay(1000);
  
  oled_indicador_lineal(73, 10, 95, 0);
  oled_indicador_lineal(34, 10, 110, 0);
  
  delay(500);
  
  oled_indicador_lineal(90, 10, 95, 0);
  oled_indicador_lineal(4, 10, 110, 0);
  
  delay(500);
  
  oled_indicador_lineal(56, 10, 95, 0);
  oled_indicador_lineal(98, 10, 110, 0);
  
  delay(500);
  
  for(x=0; x<12; x++)
  {
    oled_led(ON, 15+(x*12), 10, LED_PEQUE, ROJO);
    oled_led(ON, 15+(x*12), 30, LED_PEQUE, VERDE);
    delay(100);
    oled_led(OFF, 15+(x*12), 10, LED_PEQUE, ROJO);
    oled_led(OFF, 15+(x*12), 30, LED_PEQUE, VERDE);
  }
  
  delay(1000);
  
  for(y=0; y<5;y++)
  {
    for(x=0; x<12; x++)
    {
      oled_led(ON, 15+(x*12), 10, LED_PEQUE, ROJO);
      oled_led(ON, 15+(x*12), 30, LED_PEQUE, VERDE);
    }
    delay(100);
    for(x=0; x<12; x++)
    {
      oled_led(OFF, 15+(x*12), 10, LED_PEQUE, ROJO);
      oled_led(OFF, 15+(x*12), 30, LED_PEQUE, VERDE);
    }
  }
    
  delay(2000);

  oled_borra_pantalla();
  int dato[30]={0,0,0,0,0,0,0,0,0,0,10,23,45,78,98,60,30,10,0,0,0,0,0,0,0,0,0,0,0,0};
  int pos[8]={3,2,1,0,1,2,3,4};
  for (y=0; y<8; y++)
  {
    for (x=0; x<24; x++)
      oled_indicador_lineal_vertical(dato[x+pos[y]], 5+(5*x), 120);
    delay(50);
  }
    
  delay(2000);
  oled_borra_pantalla();
}

//oled_texto
//Visualiza una cadena de caracteres dentro de la rejilla de filas y columnas
void oled_texto(char *mensaje, unsigned char columna, unsigned char fila, unsigned char fuente, int color)
{
  int i;

  Serial2.print('s',BYTE);	
  Serial2.print(columna,BYTE);
  Serial2.print(fila,BYTE);
  //fuente (0/1/2)
  Serial2.print(fuente,BYTE);
  oled_color(color);

  for (int i=0 ; i<strlen(mensaje) ; i++)
    Serial2.print(mensaje[i],BYTE); // character to write

  Serial2.print(0x00,BYTE);

  //delay(15);
  oled_espera_respuesta();
}

//oled_boton
//
void oled_boton(char *mensaje, unsigned char estado, unsigned char x, unsigned char y)
{
  Serial2.print('b');
  Serial2.print(estado,BYTE);
  Serial2.print(x,BYTE);
  Serial2.print(y,BYTE);
  //color del boton
  Serial2.print(0x00,BYTE);
  Serial2.print(0x1F,BYTE);
  //fuente grande
  Serial2.print(0x02,BYTE);
  //color del texto
  Serial2.print(0xFF,BYTE);
  Serial2.print(0xFF,BYTE);
  //tamaño
  Serial2.print(1,BYTE);
  Serial2.print(1,BYTE);

  for (int i=0 ; i<strlen(mensaje) ; i++)
    Serial2.print(mensaje[i],BYTE); // character to write

  Serial2.print(0x00,BYTE);

  //delay(15);
  oled_espera_respuesta();
}

//oled_rectangulo
//
void oled_rectangulo(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, int color)
{
  Serial2.print('r');
  Serial2.print(x1,BYTE);
  Serial2.print(y1,BYTE);
  Serial2.print(x2,BYTE);
  Serial2.print(y2,BYTE);

  //color 
  oled_color(color);

  //delay(4);
  oled_espera_respuesta();
}

//oled_circulo
//
void oled_circulo(unsigned char x, unsigned char y, unsigned char rad, int color)
{
  Serial2.print('C');

  //Coordenadas del centro y radio del circulo
  Serial2.print(x,BYTE);
  Serial2.print(y,BYTE);
  Serial2.print(rad,BYTE);

  //color 
  oled_color(color);

  //delay(4);
  oled_espera_respuesta();
}

//oled_linea
//
void oled_linea(unsigned char x1, unsigned char y1, unsigned char x2, unsigned int y2, int color)
{
  Serial2.print('L');

  //Coordenadas del principio y el fin de la linea
  Serial2.print(x1,BYTE);
  Serial2.print(y1,BYTE);
  Serial2.print(x2,BYTE);
  Serial2.print(y2,BYTE);

  //color 
  oled_color(color);

  //delay(5);
  oled_espera_respuesta();
}

//oled_opaco_transparente
//
void oled_opaco_transparente(char opcion)
{
  Serial2.print('O',BYTE);
  Serial2.print(opcion,BYTE);

  //delay(1);
  oled_espera_respuesta();
}

//
//
void oled_pluma(char opcion)
{
  Serial2.print('p',BYTE);
  Serial2.print(opcion,BYTE);

  //delay(1);
  oled_espera_respuesta();

}

//
//
void oled_texto_sin_formato(char *mensaje, unsigned char x, unsigned char y, char fuente, int color)
{
  Serial2.print('S');	
  Serial2.print(x,BYTE);
  Serial2.print(y,BYTE);
  //fuente y color
  Serial2.print(fuente,BYTE);
  oled_color(color);
  //tamaño
  Serial2.print(1,BYTE);
  Serial2.print(1,BYTE);

  for (int i=0 ; i<strlen(mensaje) ; i++)
    Serial2.print(mensaje[i],BYTE); // character to write

  Serial2.print(0x00,BYTE);
  //delay(15);
  oled_espera_respuesta();
}

//
//
void oled_caracter(char caracter, unsigned char columna, unsigned char fila, char tamano,int color)
{
  oled_fuente(tamano);  
  //Visualiza un unico caracter en la (columna,fila) indicadas
  Serial2.print('T');	
  Serial2.print(caracter,BYTE);
  Serial2.print(columna,BYTE);
  Serial2.print(fila,BYTE);
  oled_color(color);

  //delay(5);
  oled_espera_respuesta();
}

//
//
void oled_caracter_sin_formato(char caracter, unsigned char x, unsigned char y, char tamano, int color)
{
  oled_fuente(tamano);
  //Visualiza un único caracter en la las posiciones de pantalla (x,y) indicadas
  Serial2.print('t');	
  Serial2.print(caracter,BYTE);
  Serial2.print(x,BYTE);
  Serial2.print(y,BYTE);
  oled_color(color);
  //Multiplicador del tamano
  Serial2.print(1,BYTE);
  Serial2.print(1,BYTE);

  //delay(15);
  oled_espera_respuesta();
}

//oled_led
//
void oled_led(unsigned char estado, unsigned char x, unsigned char y, int tamano, int color)
{
  oled_pluma(SOLIDO);

  if (estado == OFF)
  {
    //Borra el led antes de pintarle Off
    oled_circulo(x,y,tamano,NEGRO);
    oled_pluma(HUECO);
  }

  oled_circulo(x,y,tamano,color);
}

//oled_bat
//
void oled_bat(char *nombre_bat, int valor_bat, unsigned char x, unsigned char y, int color, unsigned char actualizar)
{
  char buffer[6]={' ',' ','.',' ',' ',0};
  int i = 4;
  
  if (actualizar == 0)			//Eso quiere decir que es la primera vez que se le llama y por tanto, hay que escribirlo todo
  {
    oled_pluma(SOLIDO);
    oled_rectangulo(4+x,0+y,8+x,4+y,color);
    oled_rectangulo(32+x,0+y,36+x,4+y,color);
    oled_rectangulo(0+x,5+y,40+x,8+y,color);
    oled_rectangulo(2+x,9+y,38+x,24+y,color);
    oled_rectangulo(0+x,25+y,40+x,28+y,color);
    oled_texto_sin_formato(nombre_bat,x+6,y+13,PEQUE,BLANCO);
  }
  
  if (valor_bat == 0)
  {
    buffer[1]='0';
    buffer[3]='0';
    buffer[4]='0';
  }
  
  else
  {
    while ((valor_bat > 0) && i>=0)
    {
      if(i!=2)
      {
        buffer[i] = (valor_bat % 10) + '0';
        valor_bat /= 10;
      }
      i--;
    }
  }
  
  oled_texto_sin_formato(buffer,x,y+33,PEQUE,BLANCO);
  
}

//oled_indicador_lineal
//
void oled_indicador_lineal(unsigned char valor, unsigned char x, unsigned char y, unsigned char actualizar)
{
  char buffer[5];

  //Ajusta el valor
  if (valor >100)
    valor = 100;

  //Convierte el valor a cadena de caracteres, ajustando blancos por la izquierda
  if (valor == 100)
    buffer[0]='1';
  else
    buffer[0]=' ';

  if (valor <10)
    buffer[1]=' ';
  else
    buffer[1]=((valor/10)%10)+'0';

  buffer[2]=(valor%10)+'0';
  buffer[3]='%';
  buffer[4]=0;

  if (actualizar == 0)			//Eso quiere decir que es la primera vez que se le llama y por tanto, hay que escribirlo todo
  {
    oled_pluma(HUECO);
    oled_rectangulo(3+x,0+y,102+3+x,4+y,BLANCO);
  }

  oled_pluma(SOLIDO);
  oled_rectangulo(4+x,1+y,100+4+x,3+y,NEGRO);
  oled_rectangulo(4+x,1+y,valor+4+x,3+y,BLANCO);

  oled_texto_sin_formato(buffer,x+110,y,MEDIANO,AZUL_CLARO);
}

//oled_indicador_lineal_vertical
//
void oled_indicador_lineal_vertical(unsigned char valor, unsigned char x, unsigned char y)
{
  oled_pluma(SOLIDO);
  //Borra la anterior indicación
  oled_rectangulo(x,y-100,x+3,y,NEGRO);
  //Escribe el nuevo valor
  oled_rectangulo(x,y-valor,x+3,y,BLANCO);
}

//oled_indicador
//
void oled_indicador(unsigned char valor, unsigned char x, unsigned char y,int tamano)
{
  char buffer[5];

  //Ajusta el valor
  if (valor >100)
    valor = 100;

  //Convierte el valor a cadena de caracteres, ajustando blancos por la izquierda
  if (valor == 100)
    buffer[0]='1';
  else
    buffer[0]=' ';

  if (valor <10)
    buffer[1]=' ';
  else
    buffer[1]=((valor/10)%10)+'0';

  buffer[2]=(valor%10)+'0';
  buffer[3]='%';
  buffer[4]=0;

  oled_texto_sin_formato(buffer,x,y,tamano,AZUL_CLARO);
}

void oled_visualiza_entero(int valor, unsigned char x, unsigned char y, int tamano)
{
  char buffer[6]={' ',' ',' ',' ','0',0};
  int i=0;
  
  if (valor < 0)
  {
    valor = -valor;
     buffer[0]='-';
  }
  else
    buffer[0]=' ';
    
  i=4;
  while (valor > 0) {
    buffer[i] = '0'+valor % 10;
    valor /= 10;
    i--;
  }
 
  oled_texto_sin_formato(buffer,x,y,tamano,AZUL_CLARO);
  
}

//oled_borra_pantalla
//
void oled_borra_pantalla(void)
{
  Serial2.print('E',BYTE);
  oled_espera_respuesta();
  //delay(40);
}

//oled_color
//
void oled_color(int color)
{
  unsigned char color_bajo, color_alto;
  //calculamos las componentes de los colores
  color_bajo = color & 0x00FF;
  color_alto = color >> 8;

  Serial2.print(color_alto,BYTE);
  Serial2.print(color_bajo,BYTE);
}

void oled_fuente(char tamano)
{
  Serial2.print('F',BYTE);
  Serial2.print(tamano,BYTE);
  //delay(1);
  oled_espera_respuesta();
}
