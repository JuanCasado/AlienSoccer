void jugar_por(void)
{
  for (int i = 4; i < 13; i++) {
    leds.setPixelColor(i, leds.Color(40, 0, 40)); // Morado
  }


  char sigue_bola [16] = {'A', 'B', 'F', 'G', 'G', 'I', 'I', 'L', 'E', 'F', 'I', 'I', 'K', 'K', 'L', 'P'};
  char sigue_bola_lejos [16] = {'A', 'B', 'D', 'E', 'G', 'H', 'J', 'K', 'M', 'G', 'H', 'J', 'K', 'M', 'N', 'P'};
  char fuera_campo1 [16] = {'x', 'x', 'x', 'x', 'F', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'L', 'x', 'x', 'x'};
  char fuera_campo2 [16] = {'P', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'J', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
  char fuera_campo3 [16] = {'A', 'B', 'B', 'B', 'D', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'N', 'P', 'P', 'P'};
  char fuera_campo4 [16] = {'x', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'H', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  char fuera_campo5 [16] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'J', 'K', 'L', 'x', 'x', 'x', 'x'};
  char fuera_campo6 [16] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'N', 'O', 'P'};
  char fuera_campo7 [16] = {'x', 'B', 'C', 'D', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  char fuera_campo8 [16] = {'x', 'x', 'x', 'x', 'x', 'F', 'G', 'H', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  leds.show();

  delay(50);
  leds.clear();
  leds.show();

  Serial1.write('t');

  velocidad_maxima = ajusta_velocidad();
  Serial1.write('v');
  Serial1.write(velocidad_maxima);

  espera_pulsador_soltado();

  leds.clear();
  leds.show();
  while (true)
  {
    actualiza_lectura_brujula();
    actualiza_lectura_bola();
    actualiza_lectura_us();

    //Apaga el chutador
    if (crono_chutador < (millis() - 80))
    {
      digitalWrite(CHUTADOR, LOW);
      delay(1);
      digitalWrite(CAL_BRUJULA, LOW);
    }

    if ((caso_suelo > 0) && (contador_salida == 4))
      Serial1.write('z');

    if ((contador_salida == 0))
    {
      caso_suelo = 0;
    }

    if (bola_actual != 255)
    {
      Serial1.write('v');
      Serial1.write (velocidad_maxima);
      if (caso_suelo > 0)
      {
        switch (caso_suelo)
        {

          case 1:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }

            while (medidas_us[3] < 38)
            {
              Serial1.write('I');
              actualiza_lectura_us();
            }



            /*delay(20);
              Serial1.write('I');
              delay(110);*/


            break;
          case 2:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }
            while (medidas_us[0] < 38)
            {
              Serial1.write('M');
              actualiza_lectura_us();
            }
            break;
          case 3:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }
            while (medidas_us[1] < 38)
            {
              Serial1.write('A');
              actualiza_lectura_us();
            }
            break;
          case 4:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }
            while (medidas_us[2] < 38)
            {
              Serial1.write('E');
              actualiza_lectura_us();
            }
            break;
        }
      }
      else
      {
        if (bola_cerca)
        {
          chuta();
        }
        Serial1.write(sigue_bola[bola_actual]);
      }
    }
    else
    {
      if (caso_suelo > 0)
      {
        /*switch (caso_suelo)
          {

          case 1:
            Serial1.write('z');
            delay(50);
            Serial1.write('I');
            delay(40);
            break;
          case 2:
            Serial1.write('z');
            delay(50);
            Serial1.write('M');
            delay(40);
            break;
          case 3:
            Serial1.write('z');
            delay(50);
            Serial1.write('A');
            delay(40);
            break;
          case 4:
            Serial1.write('z');
            delay(50);
            Serial1.write('E');
            delay(40);
            break;
          }*/
        switch (caso_suelo)
        {

          case 1:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }

            while (medidas_us[3] < 38)
            {
              Serial1.write('I');
              actualiza_lectura_us();
            }



            /*delay(20);
              Serial1.write('I');
              delay(110);
            */

            break;
          case 2:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }
            while (medidas_us[0] < 38)
            {
              Serial1.write('M');
              actualiza_lectura_us();
            }
            break;
          case 3:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }
            while (medidas_us[1] < 38)
            {
              Serial1.write('A');
              actualiza_lectura_us();
            }
          case 4:
            if (contador_salida > 2)
            {
              Serial1.write('z');
              delay(100);
            }
            while (medidas_us[2] < 38)
            {
              Serial1.write('E');
              actualiza_lectura_us();
            }
            break;
        }
      }
      else
      {
        centrar_p();
      }
    }
    if (contador_salida != 0)
      contador_salida --;
  }
}

