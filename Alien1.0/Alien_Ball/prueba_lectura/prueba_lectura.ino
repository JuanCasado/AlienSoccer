//Prueba

#define T_BOLA 1000  //timeout en microsegundos
#define LEDN 19      //negativo led
#define LEDP 18      //positivo led
#define N_SENSORES 16 //constante que define el numero de sensores para comprobar por que sensores ve la bola

byte puerto[2];

void setup()
{
  DDRD = B00000010;
  DDRB = DDRB & B11000000;
  DDRC = DDRC & B11110000; // recordar configurado como input todo
  pinMode(LEDP, OUTPUT);
  pinMode(LEDN, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  puerto[0] = ((~PIND >> 2)& B00111111)|(((~PINB) & B00000011)<<6);              //sensores S0-S7
  puerto[1] = (((~PINC) & B00001111)<<4)|(((~PINB) & B00111100)>>2);             //sensores S8-S15
  for (int x =0; x<2;x++)
  {
      Serial.print(puerto[x],BIN);
    Serial.print("|");
  }
  Serial.println();
  delay(200);
}
