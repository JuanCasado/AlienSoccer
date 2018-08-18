void jugar_por(void)
{
  for (int i = 4; i < 13; i++) {
    leds.setPixelColor(i, leds.Color(40, 0, 40)); // Morado
  }


  char sigue_bola [16] = {'A', 'B', 'F', 'G', 'G', 'I', 'I', 'L', 'E', 'F', 'I', 'I', 'K', 'K', 'L', 'P'};
  //char sigue_bola_lejos [16] = {'A', 'B', 'D', 'E', 'G', 'H', 'J', 'K', 'M', 'G', 'H', 'J', 'K', 'M', 'N', 'P'};
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

    if (bola_actual != 255)
    {
      Serial1.write('v');
      Serial1.write (velocidad_maxima);
      
        if (bola_cerca)
        {
          chuta();
        }
        Serial1.write(sigue_bola[bola_actual]);
    }
    else
    { 
        centrar_p();
    }
    if (contador_salida != 0)
      contador_salida --;
  }
}

