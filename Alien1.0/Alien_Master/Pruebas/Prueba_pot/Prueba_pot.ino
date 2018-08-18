#define POT 0

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  Serial.print("Pot: ");
  Serial.println(analogRead(POT));
  delay(300);
}
