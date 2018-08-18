void inicializa_brujula(void)
{
  Wire.begin();
}

int lee_brujula(void)
{
  int medida = 0;

#define I2C_TIME 200
  static long t_i2c_out = 0;
  t_i2c_out = millis();

  Wire.beginTransmission(0X21);
  Wire.write("A");
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(0X21, 2);
  while ((Wire.available() < 2) && ((millis() - t_i2c_out) < I2C_TIME));
  medida = Wire.read() << 8;
  medida = medida + Wire.read();
  medida = medida / 10 ;
  //Serial.println(medida);


  return medida;
}

int brujula_relativa(void)
{
  int brujula, desvia;

  brujula = lee_brujula();

  if (brujula < referencia)
    desvia = 360 - abs(lee_brujula() - referencia);
  else
    desvia = lee_brujula() - referencia;

  if (desvia > 180)
    desvia = desvia - 360;

  if (desvia > 90)
    desvia = 90;

  if (desvia < -90)
    desvia = -90;

  return (desvia);

}


void calibra_brujula(void)
{
  
  Wire.beginTransmission(0X21);
  Wire.write("C");
  Wire.endTransmission();

  unsigned long contador = millis();
  
  while ((millis() - contador) < 20000)
  {
    digitalWrite(LED_CENTRADO, HIGH);
    delay(100);
    digitalWrite(LED_CENTRADO, LOW);
    delay(100);
  }
  
  Wire.beginTransmission(0X21);
  Wire.write("E");
  Wire.endTransmission();
  digitalWrite(LED_CENTRADO, LOW);
  
}
