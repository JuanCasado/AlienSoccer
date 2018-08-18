#define N_SENSORES 16
byte p_sensores[3] = {0, 0, 0};
byte puerto_d=B00000000, puerto_b=B00000000, puerto_c=B00000000, cuenta_s=0, estado=1, x = 0, i = 0;
byte lista_s[N_SENSORES];
byte lista_p[N_SENSORES];
byte estado_s[N_SENSORES];
void setup()
{
  Serial.begin(115200);
  DDRD = B00000011;
  DDRB = DDRB & B11000000;
  DDRC = DDRC & B11110000;
  for (x = 0; x < N_SENSORES; x++)
  {
    lista_p[x] = 0;
    lista_s[x] = 0;
    estado_s[x] = 0;
  }
}

void loop()
{
  puerto_d = (~PIND >> 2) & B00111111;
  puerto_b = (~PINB) & B00111111;
  puerto_c = (~PINC) & B00001111;

  cuenta_s = 0;

  p_sensores[0] =  puerto_d;
  p_sensores[1] =  puerto_b;
  p_sensores[2] =  puerto_c;

  for (x = 0; x < 3; x++)
  {
    if (p_sensores[x] > 0)
    {
      for (i = 0; i < 8; i++)
      {
        if ((p_sensores[x] & (1 << i)))
        {
          estado = estado_s[i + (8 * x)];
          
          if (estado == 0)
          {
            lista_s[cuenta_s] = i;
            lista_p[cuenta_s] = x;
            estado_s[i + (8 * x)] = 1;
            cuenta_s++;
              Serial.print("  cuenta s ");
              Serial.println(cuenta_s);
          }
        }
      }
    }
  }
  Serial.print("    ");
  Serial.print(puerto_d, BIN);
  Serial.print("    ");
  Serial.print(puerto_b, BIN);
  Serial.print("    ");
  Serial.println(puerto_c, BIN);

  delay (100);

  while (cuenta_s>0);
}
