////////////////////////////////////////
// Archivo: delantero
//
////////////////////////////////////////

void juega_delantero(void);
void centra_delantero(void);
void manda_xbee(void);

void juega_delantero(void)
{
  rol_robot = (digitalRead(INT_JUGADOR));

  if (robot == ROBOT_NEXUS_9_2)
  {
#define CONSUMO_MAXIMO_ROLLER 30
  }
  else
  {
#define CONSUMO_MAXIMO_ROLLER 30
  }

  while (1)
  {
    //Actualizamos periodicamente todos los sensores y los motores
    actualiza_sensores();
    manda_xbee();

    centra_brujula(0);

    if (sensor_maximo > 47)
    {
      centra_delantero();
      //direccion=48;
      activa_roller(0);
    }
    else
    {
      activa_roller(1);
      voy_detras_de_la_bola();

      //Salir de la esquina
      if (((us_izquierdo + us_derecho) < 45) && (us_trasero < 25))
      {
        direccion = 12;
      }
      if (((us_izquierdo + us_derecho) > 100) && (us_trasero < 25))
      {
        if ((sensor_maximo > 24) && (sensor_maximo < 36))
          direccion = 24;
        else if (sensor_maximo > 36)
          direccion = 0;
      }
      //Intentamos tirar a la porteria
      tira_porteria();

      if ((distancia_bola > 6) || (direccion < 10) && (direccion > 14) && (consumo_roller < CONSUMO_MAXIMO_ROLLER))
      {
        velocidad_robot = 80;
      }
      else
        velocidad_robot = 100;
    }

    actualiza_motores();
  }
}

void centra_delantero(void)
{
#define MAXIMO_LATERAL_A 100
#define DISTANCIA_TRASERA_A 56
#define ERROR_US_LATERAL 10
#define ERROR_US_TRASERO 5
#define KP_US 30

  int us_error_trasero, us_error_lateral;
  static unsigned long espera_us = 0;

  us_c_izquierdo = us_izquierdo;
  us_c_derecho = us_derecho;

  // Se centra en horizontal y luego en trasero con la porteria
  ////////////////////////////////////////////////////////////////////////////
  us_error_trasero = abs(us_trasero - DISTANCIA_TRASERA_A);

  if ((us_izquierdo + us_derecho) > MAXIMO_LATERAL_A)
    us_error_lateral = abs(us_izquierdo - us_derecho);
  else
  {
    if (((us_izquierdo + us_derecho) < 45) && (us_trasero < 25))
    {
      espera_us = millis();
      while (((millis() - espera_us) < 400) && (sensor_maximo > 23))
      {
        centra_brujula(0);
        actualiza_motores();
        actualiza_sensores();
        direccion = 12;
        velocidad_robot = velocidad;
      }
      espera_us = millis();
      while (((millis() - espera_us) < 600) && (sensor_maximo > 23))
      {
        centra_brujula(0);
        actualiza_motores();
        actualiza_sensores();
        velocidad_robot = velocidad;
        if (us_derecho > us_izquierdo)
          direccion = 0;
        else
          direccion = 24;
      }
    }

    us_error_lateral = 0;
  }
  ////////////////////////////////////////////////////////////////////////////

  // Primera parte : Centrado trasero
  if (us_error_trasero > ERROR_US_TRASERO)
  {
    us_error_trasero = (us_error_trasero * KP_US) / 10;
    us_error_trasero = constrain(us_error_trasero, 0, 100);
    velocidad_robot = us_error_trasero;
    if (us_trasero > DISTANCIA_TRASERA_A)
    {
      estoy_centrado = 0;
      if (us_error_lateral > ERROR_US_LATERAL)
      {
        if (us_derecho > us_izquierdo)
          direccion = 40;
        else
          direccion = 32;
      }
      else
        direccion = 36;
    }
    else
    {
      estoy_centrado = 0;
      direccion = 12;
    }
  }

  // Segunda parte : Centrado lateral
  else if (us_error_lateral > ERROR_US_LATERAL)
  {
    us_error_lateral = (us_error_lateral * KP_US) / 10;
    us_error_lateral = constrain(us_error_lateral, 0, 100);
    velocidad_robot = us_error_lateral;
    if (us_derecho > us_izquierdo)
      direccion = 0;
    else
      direccion = 24;
  }
  else
  {
    direccion = 255;
  }
}

void manda_xbee(void)
{
#define TIME_OUT_XBEE 250

  static unsigned long time_out_xbee = 0;
  static char tengo_bola_roller = 0, tengo_bola_roller_anterior = 1, cambio = 0, primera_vez = 1;

  if (primera_vez)
  {
    time_out_xbee = millis();
    primera_vez = 0;
  }

  if (xbee)
  {
    if ((consumo_roller > CONSUMO_MAXIMO_ROLLER) && (tengo_bola_roller = 1))
      tengo_bola_roller = 1;
    else if (consumo_roller < CONSUMO_MAXIMO_ROLLER)
      tengo_bola_roller = 0;


    if (tengo_bola_roller_anterior != tengo_bola_roller)
    {
      cambio = true;
      tengo_bola_roller_anterior = tengo_bola_roller;
    }
    else
      cambio = false;

    if ((cambio) || ((millis() - time_out_xbee) > TIME_OUT_XBEE))
    {
      if (tengo_bola_roller)
        Serial3.print('C');
      else
        Serial3.print('D');

      time_out_xbee = millis();
    }
  }
}





