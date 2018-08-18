////////////////////////////////////////
// Archivo: nexus.h
//
////////////////////////////////////////


#ifndef nexus_h
#define nexus_h

#define ROBOT_NEXUS_9_2 2
#define ROBOT_NEXUS_9_1 0

#define TIPO_ROBOT_A 39
#define TIPO_ROBOT_B 38

//Pulsador de GO
#define GO 4

//Interruptores
#define INT_XBEE 41
#define INT_JUGADOR 40
#define INT_PORTERIA 13
#define INT_MODO 12

//LEDs
#define LED_1 28
#define LED_2 29
#define LED_3 30
#define LED_4 31

//Selector
#define SELECTOR_1 8
#define SELECTOR_2 9
#define SELECTOR_4 10
#define SELECTOR_8 11

#define ZUMBADOR 34
#define RESET 35
#define CHUTADOR 48

//Entradas analogicas
#define BAT_ELECTRO 0
#define BAT_POT 1
#define POT_VEL 2
#define SUELO_DERECHO_DEL 3
#define SUELO_IZQUIERDO_DEL 4
#define CONSUMO_ROLLER 5
#define SUELO_IZQUIERDO_TRA 6
#define SUELO_DERECHO_TRA 7

#define UMBRAL_SUELO_BLANCO 900


#define ROLLER_ON 1023
#define ROLLER_OFF 0

// Entendemos que esta es la salida PWM de control del Roller, antes ponia 0
#define ROLLER 3

//Funciones basicas del robot
void beep(void);
void doble_beep(void);
void resetea_perifericos(void);
void espera_go(void);
void configura_interruptores(void);
void configura_selector(void);
byte lee_selector(void);
void comprueba_led(byte maneja_led);
int promedia_ai(int ai, int promedios);
void pantalla_presentacion(void);
void visualiza_menu(void);
byte espera_cambio_menu(byte p_go);
byte espera_go_timeout(void);
int lee_pot_digital(byte modo, int value);

#endif
