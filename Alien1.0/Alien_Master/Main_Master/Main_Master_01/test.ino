void test(void)
{
  if (prueba_com())
     doble_beep(); // Error en las comunicaciones
    else
     beep(); // Comunicaciones correctas
     
  switch (selecciona_test())
  {
    case 0:
      test_brujula();
      break;

    case 1:
      test_movimientos();
      break;

    case 2:
      test_us();
      break;

    case 3:
      test_bola();
      break;

    case 4:
      test_chutador();
      break;

    case 5:
      test_bola_cerca();
      break;

    case 6:
      test_velocidades();
      break;
  }
}

int selecciona_test(void)
{
  int s_actual = -1, s_anterior = -2;

  while (digitalRead(PULSADOR) == HIGH)
  {
    s_actual = map(lee_pot(), 0, 1000, 0, 6);

    if (s_actual != s_anterior)
    {
      leds.setPixelColor(s_anterior, leds.Color(0, 0, 0)); // Negro
      leds.setPixelColor(s_actual, leds.Color(40, 0, 40)); // Morado
      leds.show();

      s_anterior = s_actual;

      beep();
    }
  }

  espera_pulsador_soltado();
  beep();

  leds.clear();
  leds.show();

  return s_actual;
}



void test_brujula(void)
{
  int pwm, conta, pos, ultima_pos = 255;
  Serial1.write('t');

  while (true)
  {
    pwm = 0;
    for (conta = 0; conta < 4; conta++)
      pwm += pulseIn(PWM_BRUJULA, HIGH);

    pwm /= 4;


    if (pwm < 1000)
      pwm = 1000;

    if (pwm > 2000)
      pwm = 2000;

    if (pwm < 1063)
      pos = 4;
    else if (pwm < 1188)
      pos = 3;
    else if (pwm < 1313)
      pos = 2;
    else if (pwm < 1438)
      pos = 1;
    else if (pwm < 1563)
      pos = 0;
    else if (pwm < 1688)
      pos = 15;
    else if (pwm < 1813)
      pos = 14;
    else if (pwm < 1938)
      pos = 13;
    else
      pos = 12;

    if (pos != ultima_pos)
    {
      //borrar posicion anterior
      leds.setPixelColor(ultima_pos, leds.Color(0, 0, 0)); // negro

      //mostrar posición actual
      leds.setPixelColor(pos, leds.Color(0, 0, 40)); // azul

      leds.show();

      ultima_pos = pos;
    }
  }
}

void test_movimientos(void)
{
  int velocidad;
  int mov_actual, mov_anterior = -1;

  espera_pulsador_soltado();
  velocidad = ajusta_velocidad();

  Serial1.write('v');
  Serial1.write(velocidad);

  espera_pulsador_soltado();
  while (digitalRead(PULSADOR))
  {
    if (digitalRead(INT_A_B))
    {
      leds.clear();
      for (int x = 0; x < 8; x++)
        leds.setPixelColor(x * 2, leds.Color(0, 0, 40)); // Azul
      leds.show();
    }
    else
    {
      leds.clear();
      for (int x = 0; x < 8; x++)
        leds.setPixelColor(x * 2 + 1, leds.Color(0, 40, 40)); // Amarillo
      leds.show();
    }
  }

  leds.clear();
  if (digitalRead(INT_A_B))
    Serial1.write('t');

  while (true)
  {

    mov_actual = map(lee_pot(), 0, 1000, 0, 15);

    if (mov_actual != mov_anterior)
    {

      leds.setPixelColor(mov_anterior, leds.Color(0, 0, 0)); // Negro
      leds.setPixelColor(mov_actual, leds.Color(0, 40, 40)); // Amarillo
      leds.show();

      mov_anterior = mov_actual;

      beep();
    }

    if (digitalRead(PULSADOR) == LOW)
    {
      Serial1.write(65 + mov_actual);
      espera_pulsador_soltado();
      delay(200);
      espera_pulsador_pulsado();
      delay(200);
      espera_pulsador_soltado();
      Serial1.write('x');
      delay(200);
    }
  }
}

void test_us(void)
{
  byte medida;
  byte medidas[NUMERO_US];
  int x = 0;
  int y = 0;
  int j = 0;
  Serial1.write('t');

  velocidad_maxima = ajusta_velocidad();
  Serial1.write('v');
  Serial1.write(velocidad_maxima);

  while (true)
  {
    actualiza_lectura_brujula();
    Serial3.write('m');
    while (Serial3.available() < NUMERO_US);
    for (x = 0; x < NUMERO_US; x++)
    {
      medidas [x] = Serial3.read();
    }

    error_lateral = medidas[0] - medidas[2];
    error_lateral_abs = abs(error_lateral);

    if ((error_lateral_abs > 30))
      ajuste = 15;
    else
      ajuste = 0;

    error_trasero = (CENTRADO_TRASERO_D + ajuste) - medidas[1];
    error_trasero_abs = abs(error_trasero);


    if (angulo_brujula_abs < MARGEN_BRUJULA)
    {
      if ((error_trasero_abs > MARGEN_TRASERO))
      {
        integral_t += error_trasero;
        integral_t = constrain(integral_t, -MARGEN_I, MARGEN_I);

        if (error_trasero_abs < (MARGEN_TRASERO + 1))
          integral_t = 0;

        velocidad_actual = (((error_trasero_abs + ((integral_t * KI) / 1000.0)) * KP) / 100.0);
        velocidad_actual  = constrain(velocidad_actual, VELOCIDAD_MINIMA, velocidad_maxima);

        Serial1.write('v');
        Serial1.write (velocidad_actual);
        if (error_trasero > 0)
        {
          if (medidas[0] > (medidas[2] + 30))
            Serial1.write('C');
          else if (medidas[0] > (medidas[2] + 15))
            Serial1.write('B');
          else if (medidas[2] > (medidas[0] + 30))
            Serial1.write('O');
          else if (medidas[2] > (medidas[0] + 15))
            Serial1.write('P');
          else
            Serial1.write('A');
        }
        else
        {
          if (medidas[0] > (medidas[2] + 30))
            Serial1.write('K');
          else if (medidas[0] > (medidas[2] + 15))
            Serial1.write('J');
          else if (medidas[2] > (medidas[0] + 40))
            Serial1.write('G');
          else if (medidas[2] > (medidas[0] + 15))
            Serial1.write('H');
          Serial1.write('I');
        }
      }
      else if ((error_lateral_abs > MARGEN_LATERAL ) && ((medidas[0] + medidas[2]) > 145))
      {
        integral_l += error_lateral;
        integral_l = constrain(integral_l, -MARGEN_I, MARGEN_I);

        if (error_lateral_abs < (MARGEN_LATERAL + 1))
          integral_l = 0;

        velocidad_actual = (((error_lateral_abs + ((integral_l * KI) / 1000.0)) * KP) / 100.0);
        velocidad_actual  = constrain(velocidad_actual, VELOCIDAD_MINIMA, velocidad_maxima);

        Serial1.write('v');
        Serial1.write (velocidad_actual);
        if (error_lateral > 0)
          Serial1.write('E');
        else
          Serial1.write('M');
      }
      else
      {
        Serial1.write('v');
        Serial1.write (velocidad_maxima);
        Serial1.write('x');
      }
    }
    else
    {
      Serial1.write('v');
      Serial1.write (velocidad_maxima);
      Serial1.write('x');
    }



    for (x = 0; x < 3; x++)
    {

      medida = medidas [x];

      if (medida < 34)
      {
        leds.setPixelColor((x * 4), leds.Color(0, 0, 40)); // Azul
        for (j = 1; j < 4; j++)
        {
          leds.setPixelColor((x * 4) + j, leds.Color(0, 0, 0)); // Negro
        }
      }
      else if (medida > 121)
      {
        for (y = 0; y < 4; y++)
        {
          leds.setPixelColor(((x * 4) + y), leds.Color((y * 10), 0, 40)); // Azul
        }
      }
      else if ((121 > medida) && (medida > 91))
      {
        for (y = 0; y < 3; y++)
        {
          leds.setPixelColor(((x * 4) + y), leds.Color((y * 10), 0, 40)); // Azul
        }

        leds.setPixelColor((x * 4) + 3, leds.Color(0, 0, 0)); // Negro

      }
      else if ((91 > medida) && (medida > 35))
      {
        for (y = 0; y < 2; y++)
        {
          leds.setPixelColor(((x * 4) + y), leds.Color((y * 10), 0, 40)); // Azul
        }

        for (j = 2; j < 4; j++)
        {
          leds.setPixelColor((x * 4) + j, leds.Color(0, 0, 0)); // Negro
        }
      }
      leds.show();
    }
  }
}

void test_bola(void)
{
  byte bola_actual;
  byte bola_anterior = 100;
  byte bola_cerca;

  int conta;

  beep();
  beep();
  beep();

  while (true)
  {
    Serial2.write('t');
    conta = 0;

    while ((Serial2.available() < 3) && conta < 100)
    {
      delay(5);
      conta ++;
    }

    if (conta < 100)
    {
      bola_actual = Serial2.read();
      distancia_bola = Serial2.read();
      bola_cerca = Serial2.read();
      Serial.println(distancia_bola);
      if (bola_actual != bola_anterior)
      {
        if (bola_actual != 255)
        {
          if (bola_actual % 2 != 0)
            bola_actual++;

          bola_actual = (bola_actual / 2);

          for (int x = 0; x < 16; x++)
            leds.setPixelColor(x, leds.Color(0, 0, 0)); // Negro
          if (distancia_bola < 4)
          {
            leds.setPixelColor(bola_actual, leds.Color(0, 40, 40)); // Amarillo
          }
          else
          {
            leds.setPixelColor(bola_actual, leds.Color(0, 0, 40)); // Azul
          }
          leds.show();

        }
        else
        {
          for (int x = 0; x < 16; x++)
          {
            leds.setPixelColor(x, leds.Color(40, 0, 0)); // Rojo poco brillante
          }
          leds.show();
        }
      }

      bola_anterior = bola_actual;

      //Bola_cerca
      if (bola_cerca)
      {
        leds.setPixelColor(0, leds.Color(40, 0, 0)); // Rojo poco brillante
        leds.show();
      }
    }
  }
}


void test_chutador(void)
{
  leds.setPixelColor(15, leds.Color(40, 0, 0));
  leds.setPixelColor(0, leds.Color(40, 0, 0));
  leds.setPixelColor(1, leds.Color(40, 0, 0));
  leds.show();

  while (true)
  {
    //Test del chutador
    espera_pulsador_pulsado();
    delay(200);
    espera_pulsador_soltado();
    digitalWrite(CAL_BRUJULA, HIGH);
    delay(1);
    digitalWrite(CHUTADOR, HIGH);
    delay(100);
    digitalWrite(CHUTADOR, LOW);
    delay(1);
    digitalWrite(CAL_BRUJULA, LOW);
    delay(1000);
  }
}

void test_bola_cerca(void)
{
  int x;
  byte bola_cerca;
  while (true)
  {
    for (x = 0; x < 5 ; x++)
    {
      leds.setPixelColor(x, leds.Color(0, 0, 40)); // azul
    }
    for (x = 12; x < 16 ; x++)
    {
      leds.setPixelColor(x, leds.Color(0, 0, 40)); // azul
    }
    leds.show();


    Serial2.write('b');
    delay(10);
    if (Serial2.available() > 0)
    {
      bola_cerca = Serial2.read();

      if (bola_cerca)
      {
        for (x = 0; x < 5 ; x++)
        {
          leds.setPixelColor(x, leds.Color(40, 0, 0)); // Rojo poco brillante
        }
        for (x = 12; x < 16 ; x++)
        {
          leds.setPixelColor(x, leds.Color(40, 0, 0)); // Rojo poco brillante
        }
        leds.show();

        digitalWrite(CAL_BRUJULA, HIGH);
        delay(1);
        digitalWrite(CHUTADOR, HIGH);
        delay(100);
        digitalWrite(CHUTADOR, LOW);
        delay(1);
        digitalWrite(CAL_BRUJULA, LOW);
        delay(1000);
      }
    }
  }
}

void test_velocidades()
{
  int velocidad;
  boolean movimiento = false;
  int power = 0;

  while (true)
  {

    if (movimiento)
    {
      delay(10);
      Serial1.write('A');
      espera_pulsador_pulsado();
      espera_pulsador_soltado();
    }
    else
    {
      Serial1.write('x');
      espera_pulsador_soltado();
      velocidad = ajusta_velocidad();

      power = map(velocidad, VELOCIDAD_MINIMA, VELOCIDAD_MAXIMA, 0, NUMPIXELS - 1);
      Serial.println(power);
      for (int x = 0; x < (power + 1); x++)
      {
        leds.setPixelColor(x, leds.Color(0, 0, 40)); // azul
      }
      leds.show();
      Serial1.write('v');
      Serial1.write(velocidad);
    }
    movimiento = (! movimiento);
  }
}





