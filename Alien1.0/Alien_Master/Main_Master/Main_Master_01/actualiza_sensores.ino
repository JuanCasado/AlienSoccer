void actualiza_sensores(void)
{
  if (flag_bola)
  {
    flag_bola = false;
    MsTimer2::stop();
    actualiza_lectura_bola();
    MsTimer2::start();
  }

  if (flag_us)
  {
    flag_us = false;
    MsTimer2::stop();
    //actualiza_lectura_us();
    MsTimer2::start();
  }

  if (flag_brujula)
  {
    flag_brujula = false;
    MsTimer2::stop();
    actualiza_lectura_brujula();
    MsTimer2::start();
  }
}

