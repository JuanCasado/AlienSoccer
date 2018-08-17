/*
  SENSORES
  Nº PIN PUERTO(D0,B1,C2) PUERTOS_FINALES(0,1)  
  0  2   PD0              P00  
  1  3   PD1              P01
  2  4   PD2              P02
  3  5   PD3              P03
  4  6   PD4              P04
  5  7   PD5              P05
  6  8   PB0              P06
  7  9   PB1              P07
  8  10  PB2              P10
  9  11  PB3              P11
  10 12  PB4              P12
  11 13  PB5              P12
  12 14  PC0              P14
  13 15  PC1              P15
  14 16  PC2              P16
  15 17  PC3              P17
*/

#define T_BOLA 1000  //timeout en microsegundos
#define CERCA 19      //Bola Cerca
#define N_SENSORES 16 //constante que define el numero de sensores para comprobar por que sensores ve la bola

byte puerto_0 = B00000000, puerto_1 = B00000000;         //configura puertos
byte bola = 0;                            //valor de vision de la bola
byte cuenta_s = 0;

unsigned long crono_t = 0, t_maximo = 0;        //cronometros

void setup() {
  DDRD = DDRD & B00000011;
  DDRB = DDRB & B11000000;
  DDRC = DDRC & B11110000; // recordar configurado como input todo
  Serial.begin(115200);

}

void loop() {
  //Esperamos a que finalice transmision previa
  do
  {
    puerto_0 = ((~PIND >> 2) & B00111111) | (((~PINB) & B00000011) << 6);          //sensores S0-S7
    puerto_1 = (((~PINC) & B00001111) << 4) | (((~PINB) & B00111100) >> 2);       //sensores S8-S15
  }
  while ((puerto_0 + puerto_1) != 0);

  //cronometro para timeout
  crono_t = micros();
  
  //Esperamos comienzo de transmision o timeout (no ve la bola)
  do
  {
    puerto_0 = ((~PIND >> 2)& B00111111) | (((~PINB) & B00000011) << 6);          //sensores S0-S7
    puerto_1 = (((~PINC) & B00001111) << 4) | (((~PINB) & B00111100) >> 2);       //sensores S8-S15
    t_maximo =(micros() - crono_t);                                                //Incremento de tiempo desde primera medida
  }
  while (((puerto_0 + puerto_1) == 0) && (t_maximo < T_BOLA));
  

  if (t_maximo < T_BOLA)
  {
      bola = true;
      cuenta_s = 0;
      
      // Iniciamos el contador de sensores que ve a bola
      crono_t = micros();
  
  }
}
