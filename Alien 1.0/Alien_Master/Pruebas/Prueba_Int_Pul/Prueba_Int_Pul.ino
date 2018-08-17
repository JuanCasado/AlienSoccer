#define INT_POR_DEL 53
#define INT_JUG_TES 49
#define INT_A_B 47
#define PUL 51


void setup()
{
  pinMode(INT_POR_DEL, INPUT_PULLUP);
  pinMode(INT_JUG_TES, INPUT_PULLUP);
  pinMode(INT_A_B, INPUT_PULLUP);
  pinMode(PUL, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(INT_POR_DEL) == HIGH)
    Serial.print("Portero");
  else
    Serial.print("Delantero");

  Serial.print("   ");

  if (digitalRead(PUL) == HIGH)
    Serial.print("No Go");
  else
    Serial.print("Go");

  Serial.print("   ");

  if (digitalRead(INT_JUG_TES) == HIGH)
    Serial.print("Test");
  else
    Serial.print("Jugar");

  Serial.print("   ");

  if (digitalRead(INT_A_B) == HIGH)
    Serial.print("B");
  else
    Serial.print("A");

  Serial.println("");

  delay(1000);
}
