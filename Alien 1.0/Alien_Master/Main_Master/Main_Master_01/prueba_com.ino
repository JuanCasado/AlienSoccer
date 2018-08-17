int delayval = 40; // delay for half a second

int prueba_com(void)
{
  int error = false;

  //Comprueba brujula
  if (digitalRead(CENTRADO_BRUJULA) == HIGH)
  {
    leds.setPixelColor(0, leds.Color(0, 60, 0)); // verde, poco brillante
    leds.show();
    beep();
  }
  else
  {
    leds.setPixelColor(0, leds.Color(40, 0, 0)); // rojo, poco brillante
    leds.show();
    doble_beep();
    error = true;
  }

  delay(50);

  //Comprueba ultrasonidos
  Serial3.write('i');
  while (Serial3.available() < 1);

  if (Serial3.read() == 'k')
  {
    leds.setPixelColor(1, leds.Color(0, 60, 0)); // verde, poco brillante
    leds.show();
    beep();
  }
  else
  {
    leds.setPixelColor(1, leds.Color(40, 0, 0)); // rojo, poco brillante
    leds.show();
    doble_beep();
    error = true;
  }

  delay(50);
  
  // Comprueba sensores bola
  Serial2.write('I');
  while (Serial2.available() < 1);

  if (Serial2.read() == 'k')
  {
    leds.setPixelColor(2, leds.Color(0, 60, 0)); // verde, poco brillante
    leds.show();
    beep();
  }
  else
  {
    leds.setPixelColor(2, leds.Color(40, 0, 0)); // rojo, poco brillante
    leds.show();
    doble_beep();
    error = true;
  }
  delay(50);

  // Comprueba motores
  Serial1.write('i');
  
  while (Serial1.available() < 1);
  
  if (Serial1.read() == 'k')
  {
    leds.setPixelColor(3, leds.Color(0, 60, 0)); // verde, poco brillante
    leds.show();
    beep();
  }
  else
  {
    leds.setPixelColor(3, leds.Color(40, 0, 0)); // rojo, poco brillante
    leds.show();
    doble_beep();
    error = true;
  }

  delay(100);

  leds.clear();
  leds.show();

  return error;
}
