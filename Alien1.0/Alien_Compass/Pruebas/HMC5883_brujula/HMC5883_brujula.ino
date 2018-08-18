#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

Servo servo;

int referencia=0, m_servo=0;


int lee_brujula(void);
int brujula_relativa(void);

void setup() 
{
  servo.attach(9);
  Serial.begin(9600);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  referencia=lee_brujula();
}

void loop() 
{
  
  Serial.print("Medida_brujula: ");
  Serial.print(brujula_relativa());

  if(brujula_relativa() <=90)
    m_servo=map(brujula_relativa(),0,90,90,0);
  else if(brujula_relativa() >=270)
    m_servo=map(brujula_relativa(),270,360,180,91);
  else if(brujula_relativa() <180)
    m_servo=0;
  else
    m_servo=180;

  Serial.print("  Salida servo: ");
  Serial.println(m_servo);
  servo.write(m_servo);
  //delay(100);
}

int lee_brujula(void)
{
  unsigned long int acumula=0;
  int contador=0;

  for(contador=0;contador<4;contador++)
  {
    //medida de la brujula
    sensors_event_t event; 
    mag.getEvent(&event);

    float heading = atan2(event.magnetic.y, event.magnetic.x);
    float headingDegrees = (heading*180/M_PI) + 180; 
    acumula+=(int)headingDegrees;
  }
  return acumula/4;
}

int brujula_relativa(void)
{
  if(lee_brujula()<referencia)
    return (360-abs(lee_brujula()-referencia));
  else
    return (lee_brujula()-referencia);
}

