void pruebas(void);

void pruebas(void)
{
  while(digitalRead(GO)!=0)
  {
    oled_texto("Calibra brujula",1,0,GRANDE,BLANCO);
    velocidad_robot = 15;  
  }
  espera_go();
  oled_borra_pantalla();
  
  oled_texto("Calibrando brujula...",1,0,GRANDE,BLANCO);
  Wire.beginTransmission(0X21);
  Wire.write("C");
  Wire.endTransmission();  
  gira_robot(velocidad_robot);
  delay(30000);
  para_robot();
  Wire.beginTransmission(0X21);
  Wire.write("E");
  Wire.endTransmission();
  oled_borra_pantalla();
  oled_texto("Calibracion terminada",1,0,GRANDE,BLANCO);
  delay(2000);
}
