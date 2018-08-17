////////////////////////////////////////
// Archivo: oled_160.h
//
////////////////////////////////////////


#ifndef oled_160_h
#define oled_160_h

#define OLED_BAUDRATE 115200

//Colores de los elementos del display
#define BLANCO 		0xFFFF
#define AZUL_CLARO	0x07FF
#define AZUL		0x001F
#define ROJO		0xF800
#define VERDE		0x07E0
#define AMARILLO	0xFFE0
#define NEGRO		0x0000
#define MAGENTA		0xF81F
#define ROSA		0xF856
#define NARANJA		0xD546

#define TRANSPARENTE    0x00		//para el texto
#define OPACO 		0x01

#define SOLIDO 		0x00		//para la pluma(pen size)
#define HUECO 		0x01

#define OFF 		0x00
#define ON              0x01

//Tamaño de caracter
#define PEQUE		0x00
#define MEDIANO		0x01
#define GRANDE		0x02

//Leds
#define LED_MUY_PEQUE 2
#define LED_PEQUE 3
#define LED_MEDIANO 4
#define LED_GRANDE 5

void oled_ini(void);
byte oled_espera_respuesta(void);
void oled_demo(void);

//
void oled_texto(char *mensaje, unsigned char columna, unsigned char fila, unsigned char fuente, int color);
void oled_opaco_transparente(char opcion);
void oled_boton(char *mensaje, unsigned char estado, unsigned char x, unsigned char y);
void oled_rectangulo(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, int color);
void oled_circulo(unsigned char x, unsigned char y, unsigned char rad, int color);
void oled_linea(unsigned char x1, unsigned char y1, unsigned char x2, unsigned int y2, int color);
void oled_opaco_transparente(char opcion);
void oled_pluma(char opcion);
void oled_texto_sin_formato(char *mensaje, unsigned char x, unsigned char y, char fuente, int color);
void oled_caracter(char caracter, unsigned char columna, unsigned char fila, char tamano, int color);
void oled_caracter_sin_formato(char caracter, unsigned char x, unsigned char y, char tamano, int color);

//Objetos
void oled_led(unsigned char estado, unsigned char x, unsigned char y, int tamano, int color);
void oled_bat(char *nombre_bat, int valor_bat, unsigned char x, unsigned char y, int color, unsigned char actualizar);
void oled_indicador_lineal(unsigned char valor, unsigned char x, unsigned char y, unsigned char actualizar);
void oled_indicador_lineal_vertical(unsigned char valor, unsigned char x, unsigned char y);
void oled_indicador(unsigned char valor, unsigned char x, unsigned char y, int tamano);
void oled_visualiza_entero(int valor, unsigned char x, unsigned char y, int tamano);
//
void oled_borra_pantalla(void);
void oled_color(int color);
void oled_fuente(char tamano);

#endif
