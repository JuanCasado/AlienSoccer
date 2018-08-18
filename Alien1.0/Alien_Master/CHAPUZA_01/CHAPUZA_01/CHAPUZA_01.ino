// Programa principal para la placa master del robot Alien Soccer 1

#include <MsTimer2.h>
#include <Adafruit_NeoPixel.h>

//Pines de dispositivos

#define PIN_NEO_PIXEL  43
#define NUMPIXELS      16

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMPIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

#define INT_POR_DEL 53
#define INT_JUG_TES 49
#define INT_A_B 47
#define PULSADOR 51

#define POT 0
#define CHUTADOR 8
#define ZUMBADOR 9

#define PWM_BRUJULA 10
#define CENTRADO_BRUJULA 11
#define CAL_BRUJULA 12

#define VELOCIDAD_MINIMA 30
#define VELOCIDAD_MAXIMA 100

#define VELOCIDAD_SERIE 115200

#define SUELO1  21
#define SUELO2  3
#define SUELO3  2
#define SUELO4  20
byte sensores_suelo [4] = {SUELO1, SUELO2, SUELO3, SUELO4};


//FLAGS DE PERIODICA
boolean flag_brujula = false;
boolean flag_us = false;
boolean flag_bola = false;

//MEDIDASA DE LOS SENSORES
byte velocidad_maxima = 100;
byte velocidad_actual = 100;
int angulo_brujula = 0;
int angulo_brujula_abs = 0;
#define NUMERO_US 4
int medidas_us [NUMERO_US] = {0, 0, 0, 0};
byte bola_actual = 255;
byte distancia_bola = 100;
boolean bola_cerca = false;
volatile byte caso_suelo = 0;
unsigned long crono_chutador = 0;
byte contador_salida = 0;

//GLOBALES DE CENTRADO
#define MARGEN_TRASERO 2
#define MARGEN_LATERAL 5
#define CENTRADO_TRASERO_P 30
#define CENTRADO_TRASERO_D 75
#define MARGEN_I 550
#define MARGEN_BRUJULA 1580

int error_trasero = 0;
int error_trasero_abs = 0;
int KP = 150;
int KI = 10;
long integral_t = 0;
byte ajuste = 15;

int error_lateral = 0;
int error_lateral_abs = 0;
long integral_l = 0;

void setup()
{
  int time_out = 0;
  pinMode(CAL_BRUJULA, OUTPUT);
  digitalWrite(CAL_BRUJULA, LOW);

  pinMode(SUELO1, INPUT);
  pinMode(SUELO2, INPUT);
  pinMode(SUELO3, INPUT);
  pinMode(SUELO4, INPUT);


  pinMode(INT_POR_DEL, INPUT_PULLUP);
  pinMode(INT_JUG_TES, INPUT_PULLUP);
  pinMode(INT_A_B, INPUT_PULLUP);
  pinMode(PULSADOR, INPUT_PULLUP);

  pinMode(CHUTADOR, OUTPUT);

  pinMode(PWM_BRUJULA, INPUT);
  pinMode(CENTRADO_BRUJULA, INPUT);


  //Inicializa ledes RGB
  leds.begin();
  leds.clear();
  leds.show();

  Serial.begin(VELOCIDAD_SERIE);
  Serial1.begin(VELOCIDAD_SERIE);  // Motores
  Serial2.begin(VELOCIDAD_SERIE);  // Sensor Bola
  Serial3.begin(VELOCIDAD_SERIE);  // Ultrasonidos

  Serial1.write('x'); // Para el robot

  //Configura periodica
  //MsTimer2::set(1, periodica); // se ejecuta cada 1ms
  //MsTimer2::start();

  if (!digitalRead(PULSADOR))
  {
    digitalWrite(CAL_BRUJULA, HIGH);
    while ((pulseIn(PWM_BRUJULA, HIGH, 10000) < 2000) || (time_out > 1000))
    {
      time_out++;
    }
    if (time_out < 1000)
    {
      digitalWrite(CAL_BRUJULA, LOW);

      for (int x = 0; x < 16; x++)
        leds.setPixelColor(x, leds.Color(0, 40, 40)); // Azul
      leds.show();

      espera_pulsador_soltado();

      leds.clear();



      Serial1.write('g');
      Serial1.write(-18);
      unsigned long crono = millis();
      while ((millis() - crono) < 23000)
      {
        for (int x = 0; x < 16; x++)
        {
          leds.clear();
          leds.setPixelColor(x, leds.Color(0, 40, 40)); // Amarillo
          leds.show();
          delay(60);
        }
      }

      for (int x = 0; x < 16; x++)
        leds.setPixelColor(x, leds.Color(0, 0, 40)); // Azul
      leds.show();

      while (!digitalRead(CENTRADO_BRUJULA));

      digitalWrite(CAL_BRUJULA, LOW);


      Serial1.write('g');
      Serial1.write(0);
      Serial1.write('t');
      delay(100);
      Serial1.write('t');
    }
  }

  pinMode(ZUMBADOR, OUTPUT);
  beep();
  circulo_azul();
  circulo_blanco();

}

void loop()
{ /*
    if (prueba_com())
     doble_beep(); // Error en las comunicaciones
    else
     beep(); // Comunicaciones correctas

  */
  if (digitalRead(INT_JUG_TES) == HIGH)
    test();
  else
    jugar();
}

void periodica ()
{
#define VECES_BRUJULA 2
#define VECES_US 2
#define VECES_BOLA 2
#define VECES_SUELO 2

  static byte flag_conta_brujula = VECES_BRUJULA;
  static byte flag_conta_us = VECES_US;
  static byte flag_conta_bola = VECES_BOLA;
  static byte flag_conta_suelo = VECES_SUELO;

  //Actualiza brujula
  flag_conta_brujula--;
  if (flag_conta_brujula == 0)
  {
    flag_brujula = true;
    flag_conta_brujula = VECES_BRUJULA;
  }

  //Actualiza ultrasonidos
  flag_conta_us--;
  if (flag_conta_us == 0)
  {
    flag_us = true;
    flag_conta_us = VECES_US;
  }

  //Actualiza bola
  flag_conta_bola--;
  if (flag_conta_bola == 0)
  {
    flag_bola = true;
    flag_conta_bola = VECES_BOLA;
  }

}

