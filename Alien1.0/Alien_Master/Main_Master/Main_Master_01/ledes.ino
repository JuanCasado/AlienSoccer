void circulo_azul (void)
{
#define WAIT 1
  leds.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {

    leds.setPixelColor(i, leds.Color(0, 0, 40)); // Azul

    leds.show(); // This sends the updated pixel color to the hardware.

    delay(WAIT); // Delay for a period of time (in milliseconds).
  }

}

void circulo_blanco (void)
{
#define ESPERA 10
  leds.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {

    leds.setPixelColor(i, leds.Color(0, 0, 0)); // Moderately bright green color.

    leds.show(); // This sends the updated pixel color to the hardware.

    delay(ESPERA); // Delay for a period of time (in milliseconds).
  }
}

void muestra_bola (void)
{
  leds.clear();
  if (bola_actual != 255)
  {
    leds.setPixelColor(bola_actual, leds.Color(0, 40, 40));   // Amarillo
  }
  else
  {
    for (int x = 0; x < 16; x++)
      leds.setPixelColor(x, leds.Color(40, 0, 0));            // Rojo poco brillante
  }
  if (bola_cerca || bola_actual == 15 || bola_actual == 0 || bola_actual == 1)
  {
    leds.setPixelColor(0, leds.Color(40, 0, 0));            // Rojo poco brillante
    doble_beep();
    doble_beep();
  }
  leds.show();
  leds.clear();
}

void muestra_centrado(void)
{
  leds.clear();
  leds.setPixelColor(0, leds.Color(0, 60, 0)); // verde, poco brillante
  leds.setPixelColor(1, leds.Color(0, 60, 0)); // verde, poco brillante
  leds.setPixelColor(2, leds.Color(0, 60, 0)); // verde, poco brillante
  leds.setPixelColor(3, leds.Color(0, 60, 0)); // verde, poco brillante
  int medida;
  for (int x = 1; x < 4; x++)
  {

    medida = medidas_us [x-1];

    if (medida < 34)
    {
      leds.setPixelColor((x * 4), leds.Color(0, 0, 40)); // Azul
      for (int j = 1; j < 4; j++)
      {
        leds.setPixelColor((x * 4) + j, leds.Color(0, 0, 0)); // Negro
      }
    }
    else if (medida > 121)
    {
      for (int y = 0; y < 4; y++)
      {
        leds.setPixelColor(((x * 4) + y), leds.Color((y * 10), 0, 40)); // Azul
      }
    }
    else if ((121 > medida) && (medida > 91))
    {
      for (int y = 0; y < 3; y++)
      {
        leds.setPixelColor(((x * 4) + y), leds.Color((y * 10), 0, 40)); // Azul
      }

      leds.setPixelColor((x * 4) + 3, leds.Color(0, 0, 0)); // Negro

    }
    else if ((91 > medida) && (medida > 35))
    {
      for (int y = 0; y < 2; y++)
      {
        leds.setPixelColor(((x * 4) + y), leds.Color((y * 10), 0, 40)); // Azul
      }

      for (int j = 2; j < 4; j++)
      {
        leds.setPixelColor((x * 4) + j, leds.Color(0, 0, 0)); // Negro
      }
    }
    leds.show();
  }
}

