/*
  HMC5883L Triple Axis Digital Compass. Compass Example.
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-magnetometr-hmc5883l.html
  GIT: https://github.com/jarzebski/Arduino-HMC5883L
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <Servo.h>
#include <HMC5883L.h>

#define PIN_PWM 6
#define CENTRADO 5
#define CAL_CHUTA 7
#define LED_CENTRADO 13
#define MARGEN_CENTRADO 3

HMC5883L compass;

Servo salida_pwm;

float declinationAngle = 0;

int referencia = 0, pos = 0;

void setup()
{
  pinMode(CENTRADO, OUTPUT);
  pinMode(LED_CENTRADO, OUTPUT);
  pinMode(CAL_CHUTA, INPUT);

  salida_pwm.attach(PIN_PWM);

  //Serial.begin(115200);

  // Initialize Initialize HMC5883L
  //Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    //Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(-37, -96); //-54,-111

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  declinationAngle = (0.0 - (43.0 / 60.0)) / (180 / M_PI);

  referencia = lee_brujula();
}

void loop()
{
  pos = brujula_relativa();
  
  //Serial.println(pos);

  salida_pwm.writeMicroseconds(1500 + (pos * (1000 / 90)));

  //salida_pwm.write(pos + 90);

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
  //delay(100);
}

int lee_brujula(void)
{
#define PROMEDIOS 4
  int conta;
  float x, y;

  x = 0;
  y = 0;
  for (conta = 0; conta < PROMEDIOS; conta++)
  {
    Vector norm = compass.readNormalize();
    x += norm.XAxis;
    y += norm.YAxis;
  }

  x /= PROMEDIOS;
  y /= PROMEDIOS;

  // Calculate heading
  float heading = atan2(y, x );

  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180 / M_PI;

  return ((int) headingDegrees);
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
