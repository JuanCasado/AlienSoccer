////////////////////////////////////////
// Archivo: p_baterias
//
////////////////////////////////////////


void monitor_baterias(void);

void monitor_baterias(void)
{
  unsigned long bat;

  oled_texto("MONITOR DE BATERIAS",0,1,GRANDE,BLANCO);

  while(espera_cambio_menu(digitalRead(GO)))
  {
    bat = (promedia_ai(BAT_POT,16) * 59.0)/25.0;
    oled_bat("Pot", bat, 30, 45, VERDE, 0);
    bat = (promedia_ai(BAT_ELECTRO,16) * 59.0)/25.0;
    oled_bat("Elec",bat, 80, 45, VERDE, 0);
    delay(200);
  }
  
  espera_go();
}  
