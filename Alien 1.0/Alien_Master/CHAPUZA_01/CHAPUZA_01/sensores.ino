void actualiza_lectura_bola(void)
{
  byte bola = 255;

  Serial2.write('t');
  while (Serial2.available() < 3);
  bola = Serial2.read();
  distancia_bola = Serial2.read();
  bola_cerca = Serial2.read();

  if (bola != 255)
  {
    if (bola % 2 != 0)
      bola++;
    bola_actual = (bola / 2);
  }
  else
    bola_actual = bola;

  Serial2.write('b');
  while (Serial2.available() < 1);
  bola_cerca = Serial2.read();
}

void actualiza_lectura_us(void)
{
  Serial3.write('m');
  while (Serial3.available() < NUMERO_US);
  for (int x = 0; x < NUMERO_US; x++)
  {
    medidas_us [x] = Serial3.read();
  }
}



void actualiza_lectura_brujula(void)
{
  int pwm = 0;
  for (int x = 0; x < 4; x++)
    pwm += pulseIn(PWM_BRUJULA, HIGH);

  angulo_brujula = (pwm / 4);
  angulo_brujula_abs = abs(angulo_brujula);
}

void centrar_p ()
{
  error_lateral = (medidas_us[0] - medidas_us[2]);
  error_lateral_abs = abs(error_lateral);

  if ((error_lateral_abs > 30))
    ajuste = 15;
  else
    ajuste = 0;

  error_trasero = (CENTRADO_TRASERO_P + ajuste) - medidas_us[1];
  error_trasero_abs = abs(error_trasero);


  if (angulo_brujula_abs < MARGEN_BRUJULA)
  {
    if (caso_suelo == 3)
    {
      Serial1.write('v');
      Serial1.write (velocidad_actual);
      if (medidas_us[0] > medidas_us[2])
        Serial1.write('B');
      else
        Serial1.write('P');
    }
    else if ((error_trasero_abs > MARGEN_TRASERO))
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
        if (medidas_us[0] > (medidas_us[2] + 30))
          Serial1.write('C');
        else if (medidas_us[0] > (medidas_us[2] + 15))
          Serial1.write('B');
        else if (medidas_us[2] > (medidas_us[0] + 30))
          Serial1.write('O');
        else if (medidas_us[2] > (medidas_us[0] + 15))
          Serial1.write('P');
        else
          Serial1.write('A');
      }
      else
      {
        if (medidas_us[0] > (medidas_us[2] + 30))
          Serial1.write('K');
        else if (medidas_us[0] > (medidas_us[2] + 15))
          Serial1.write('J');
        else if (medidas_us[2] > (medidas_us[0] + 40))
          Serial1.write('G');
        else if (medidas_us[2] > (medidas_us[0] + 15))
          Serial1.write('H');
        Serial1.write('I');
      }
    }
    else if ((error_lateral_abs > MARGEN_LATERAL ) && ((medidas_us[0] + medidas_us[2]) > 145))
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
}

void centrar_d ()
{
  error_lateral = (medidas_us[0] - medidas_us[2]);
  error_lateral_abs = abs(error_lateral);

  if ((error_lateral_abs > 30))
    ajuste = 15;
  else
    ajuste = 0;

  error_trasero = (CENTRADO_TRASERO_D + ajuste) - medidas_us[1];
  error_trasero_abs = abs(error_trasero);


  if (angulo_brujula_abs < MARGEN_BRUJULA)
  {
    if (caso_suelo == 3)
    {
      Serial1.write('v');
      Serial1.write (velocidad_actual);
      if (medidas_us[0] > medidas_us[2])
        Serial1.write('B');
      else
        Serial1.write('P');
    }
    else if ((error_trasero_abs > MARGEN_TRASERO))
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
        if (medidas_us[0] > (medidas_us[2] + 30))
          Serial1.write('C');
        else if (medidas_us[0] > (medidas_us[2] + 15))
          Serial1.write('B');
        else if (medidas_us[2] > (medidas_us[0] + 30))
          Serial1.write('O');
        else if (medidas_us[2] > (medidas_us[0] + 15))
          Serial1.write('P');
        else
          Serial1.write('A');
      }
      else
      {
        if (medidas_us[0] > (medidas_us[2] + 30))
          Serial1.write('K');
        else if (medidas_us[0] > (medidas_us[2] + 15))
          Serial1.write('J');
        else if (medidas_us[2] > (medidas_us[0] + 40))
          Serial1.write('G');
        else if (medidas_us[2] > (medidas_us[0] + 15))
          Serial1.write('H');
        Serial1.write('I');
      }
    }
    else if ((error_lateral_abs > MARGEN_LATERAL ) && ((medidas_us[0] + medidas_us[2]) > 145))
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
}

void chuta(void)
{
  if (crono_chutador < (millis() - 800))
  {
    digitalWrite(CAL_BRUJULA, HIGH);
    delay(1);
    digitalWrite(CHUTADOR, HIGH); 
    crono_chutador = millis();
  }
}

void suelo1 (void)
{
  /*switch (caso_suelo)
  {
    case 0:
      caso_suelo = 1;
      break;
    case 2:
      caso_suelo = 5;
      break;
    case 4:
      caso_suelo = 8;
      break;
  }*/
  caso_suelo = 1;
  contador_salida = 2;
}

void suelo2 (void)
{
  /*switch (caso_suelo)
  {
    case 0:
      caso_suelo = 2;
      break;
    case 1:
      caso_suelo = 5;
      break;
    case 3:
      caso_suelo = 6;
      break;
  }*/
  caso_suelo = 2;
  contador_salida = 2;
}

void suelo3 (void)
{
  /*switch (caso_suelo)
  {
    case 0:
      caso_suelo = 3;
      break;
    case 2:
      caso_suelo = 6;
      break;
    case 4:
      caso_suelo = 7;
      break;
  }*/
  caso_suelo = 3;
  contador_salida = 2;
}

void suelo4 (void)
{
  /*switch (caso_suelo)
  {
    case 0:
      caso_suelo = 4;
      break;
    case 1:
      caso_suelo = 8;
      break;
    case 3:
      caso_suelo = 7;
      break;
  }*/
  caso_suelo = 4;
  contador_salida = 2;
}

byte hay_suelo()
{
  byte suelo = 0;

  suelo += digitalRead(SUELO1);
  suelo += digitalRead(SUELO2);
  suelo += digitalRead(SUELO3);
  suelo += digitalRead(SUELO4);
  
  return suelo;
}

