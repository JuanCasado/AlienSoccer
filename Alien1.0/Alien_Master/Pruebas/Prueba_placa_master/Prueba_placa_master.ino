#include <Adafruit_NeoPixel.h>

#define PIN            43
#define NUMPIXELS      16

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define INT_POR_DEL 45
#define INT_JUG_TES 47
#define INT_3 51
#define INT_4 53
#define PUL 49
#define POT 0

int valor_pot;
int valor_in1;
int valor_in2;
int valor_in3;
int valor_in4;
int valor_pul;
int valor_ant;

void setup()
{
  pinMode(INT_POR_DEL, INPUT_PULLUP);
  pinMode(INT_JUG_TES, INPUT_PULLUP);
  pinMode(INT_3, INPUT_PULLUP);
  pinMode(INT_4, INPUT_PULLUP);
  pinMode(PUL, INPUT_PULLUP);

  Serial.begin(9600);
  leds.begin();
}


void loop()
{
  valor_pot = analogRead(POT);
  valor_in1 = digitalRead(INT_POR_DEL);
  valor_in2 = digitalRead(INT_JUG_TES);
  valor_in3 = digitalRead(INT_3);
  valor_in4 = digitalRead(INT_4);
  valor_pul = digitalRead(PUL);

  valor_pot = map(valor_pot, 0, 1023, 0, 15);

  if (valor_pul == LOW)
  {
    for (int i = 0; i <= 16; i++)
      leds.setPixelColor(i, leds.Color(100, 0 , 0));
    leds.show();
    while (valor_pul == HIGH);

    for (int i = 0; i <= 16; i++)
      leds.setPixelColor(i, leds.Color(0, 0 , 0));
      leds.show();
  }
  else
  {

    if (valor_ant != valor_pot)
    {

      if (valor_in1 == HIGH)
      {
        leds.setPixelColor(valor_pot, leds.Color(0, 255, 0));
      }
      else if (valor_in2 == HIGH)
      {
        leds.setPixelColor(valor_pot, leds.Color(10, 100, 10));
      }
      else if (valor_in3 == HIGH)
      {
        leds.setPixelColor(valor_pot, leds.Color(20, 0, 50));
      }
      else if (valor_in4 == HIGH)
      {
        leds.setPixelColor(valor_pot, leds.Color(255, 255, 255));
      }
      else
      {
        leds.setPixelColor(valor_pot, leds.Color(0, 0, 255));
      }
      leds.show();
      valor_ant = valor_pot;
      leds.setPixelColor(valor_ant, leds.Color(0, 0, 0));

    }
  }
}

