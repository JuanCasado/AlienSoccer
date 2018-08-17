#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_LEDS 43
#define NUMERO_LEDS 16
#define POT 0

#define VELOCIDAD_MINIMA 30
#define VELOCIDAD_MAXIMA 100

int valor_pot, power, x;
int tiempo = 50;

int velocidad;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMERO_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();
  pixels.clear();
  pixels.show();
}


void loop()
{
  valor_pot =  analogRead(POT);
  if (valor_pot > 1000)
    valor_pot = 1000;

  velocidad = map(valor_pot, 0, 1000, VELOCIDAD_MINIMA, VELOCIDAD_MAXIMA);

  power = map(valor_pot, 0, 1000, 0, NUMERO_LEDS-1);

  for (x = 0; x < (power + 1); x++)
    pixels.setPixelColor(x, pixels.Color(70, 0, 0));

  for (x = power + 1; x < NUMERO_LEDS; x++)
    pixels.setPixelColor(x, pixels.Color(0, 0, 0));

  pixels.show();

  delay(100);

}

