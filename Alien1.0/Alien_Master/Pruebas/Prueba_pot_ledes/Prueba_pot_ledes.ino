#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL  43
#define NUMPIXELS      16

#define POT 0

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMPIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

int x, valor_pot, ledes;

void setup()
{
  Serial.begin(115200);

  //Inicializa ledes RGB
  leds.begin();
  leds.clear();
  leds.show();
}

void loop()
{
  valor_pot = analogRead(POT);
  if (valor_pot > 1000)
    valor_pot = 1000;

  Serial.print("Pot: ");
  Serial.println(valor_pot);

  ledes = map(valor_pot, 0, 1000, 0, NUMPIXELS - 1);

  for (x = 0; x < (ledes + 1); x++)
    leds.setPixelColor(x, leds.Color(40, 0, 0));

  for (x = ledes + 1; x < NUMPIXELS; x++)
    leds.setPixelColor(x, leds.Color(0, 0, 0));

  leds.show();
  delay(100);
}
