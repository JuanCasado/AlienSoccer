int ajusta_velocidad(void)
{

  int valor_pot, vel = 0, power, power_anterior = 5000, x;

  while (digitalRead(PULSADOR) == HIGH)
  {
    valor_pot = lee_pot();

    vel = map(valor_pot, 0, 1000, VELOCIDAD_MINIMA, VELOCIDAD_MAXIMA);

    power = map(valor_pot, 0, 1000, 0, NUMPIXELS - 1);

    if (power != power_anterior)
    {
      beep();
    }

    for (x = 0; x < (power + 1); x++)
      leds.setPixelColor(x, leds.Color(40, 0, 0));

    for (x = power + 1; x < NUMPIXELS; x++)
      leds.setPixelColor(x, leds.Color(0, 0, 0));

    leds.show();

    delay(100);
    power_anterior = power;
  }

  leds.clear();
  leds.show();

  espera_pulsador_soltado();

  return vel;
}

int lee_pot(void)
{
  int valor_pot =  analogRead(POT);
  if (valor_pot > 1000)
    valor_pot = 1000;

  return valor_pot;
}

void espera_pulsador_pulsado(void)
{
  while (digitalRead(PULSADOR) == HIGH);
}

void espera_pulsador_soltado(void)
{
  while (digitalRead(PULSADOR) == LOW);
}

void beep(void)
{
  tone(ZUMBADOR , 370);
  delay(200);
  noTone(ZUMBADOR);
}

void doble_beep(void)
{
  beep();
  delay(30);
  beep();
}
