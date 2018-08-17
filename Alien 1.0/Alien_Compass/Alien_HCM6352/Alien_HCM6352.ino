#include <Wire.h>
#include <Servo.h>

#define PIN_PWM 6
#define CENTRADO 5
#define CAL_CHUTA 7
#define LED_CENTRADO 13
#define MARGEN_CENTRADO 3

Servo salida_pwm;

int referencia = 0, pos = 0, pos_anterior = 0;

unsigned long time_out = 0;
boolean primera = true;

void setup()
{
  int time_out = 0;
  pinMode(CENTRADO, OUTPUT);
  pinMode(LED_CENTRADO, OUTPUT);
  pinMode(CAL_CHUTA, INPUT);

  salida_pwm.attach(PIN_PWM);

  Serial.begin(115200);

  // Initialize Initialize brujula
  inicializa_brujula();

  //Eperamos sincronizarnos con la Mega
  delay(1000);

  //Serial.print(digitalRead(CAL_CHUTA));
  if (digitalRead(CAL_CHUTA))
  {
    salida_pwm.writeMicroseconds(5000);
    while (digitalRead(CAL_CHUTA) || (time_out > 10000))
    {
      time_out++;
      delay(1);
    }
    if (time_out < 10000)
      calibra_brujula();
  }

  //Descartamos las primeras medidas
  while (millis() < 500)
    lee_brujula();

  referencia = lee_brujula();
}

void loop()
{
  if ((!digitalRead(CAL_CHUTA)) || (time_out > (millis() - 100)))
  {
    pos = brujula_relativa();
    primera = true;
  }
  else
  {
    pos = pos_anterior;
    if (primera)
      time_out = millis();
    primera = false;
  }

  //Serial.println(pos);

  salida_pwm.writeMicroseconds(1500 + (pos * (1000 / 90)));

  if (abs(pos) <= MARGEN_CENTRADO)
  {
    digitalWrite(CENTRADO, HIGH);
    digitalWrite(LED_CENTRADO, HIGH);
  }
  else
  {
    digitalWrite(CENTRADO, LOW);
    digitalWrite(LED_CENTRADO, LOW);
  }

  pos_anterior = pos;
  delay(20);

}
