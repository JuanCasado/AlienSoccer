// Programa de prueba para cuatro sensores de US con comandos de control

#define PIN_TRIG_US_2 7
#define PIN_ECHO_US_2 8
#define PIN_TRIG_US_3 10
#define PIN_ECHO_US_3 9
#define PIN_TRIG_US_4 19
#define PIN_ECHO_US_4 18

#define LED 13

#define ID "ArduSonic V1.0"

#define ESPERA_ENTRE_MEDIDAS 10


int medida = 0, medida_2 = 0, medida_3 = 0, medida_4 = 0;
byte continuo = false;
unsigned long crono = 0;
byte sensor = 2;

void setup()
{
  pinMode(PIN_TRIG_US_2, OUTPUT);
  pinMode(PIN_ECHO_US_2, INPUT);
  pinMode(PIN_TRIG_US_3, OUTPUT);
  pinMode(PIN_ECHO_US_3, INPUT);
  pinMode(PIN_TRIG_US_4, OUTPUT);
  pinMode(PIN_ECHO_US_4, INPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Serial.begin(115200);
  crono = millis();
}

void loop()
{


  //Medida 2
  if (((millis() - crono) > ESPERA_ENTRE_MEDIDAS) && (sensor == 2))
  {
    medida = mide_us(PIN_TRIG_US_2, PIN_ECHO_US_2);
    if (medida > 0)
    {
      medida_2 = constrain(medida, 0, 255); 
    }
    sensor = 3;
    crono = millis();
  }




  //Medida 3
  if (((millis() - crono) > ESPERA_ENTRE_MEDIDAS) && (sensor == 3))
  {
    medida = mide_us(PIN_TRIG_US_3, PIN_ECHO_US_3);
    if (medida > 0)
    {
      medida_3 = constrain(medida, 0, 255); 
      
    }
    sensor = 4;
    crono = millis();
  }



  //Medida 4
  if (((millis() - crono) > ESPERA_ENTRE_MEDIDAS) && (sensor == 4))
  {
    medida = mide_us(PIN_TRIG_US_4, PIN_ECHO_US_4);
    if (medida > 0)
    {
      medida_4 = constrain(medida, 0, 255); 
    }
    sensor = 2;
    crono = millis();
  }



  if (Serial.available() > 0)
  {
    switch (Serial.read())
    {
      case 'i':
        // Envía identificación del modulo en binario
        Serial.write('k'); //minuscula
        break;

      case 'I':
        // Envía identificación del modulo en ASCII
        Serial.println(ID);
        break;

      case 'm':
        // Envia una medida unica en binario
        Serial.write(medida_2);
        Serial.write(medida_3);
        Serial.write(medida_4);
        break;

      case 'M':
        // Envia una medida unica en ASCII
        Serial.print(medida_2);
        Serial.print("   ");
        Serial.print(medida_3);
        Serial.print("   ");
        Serial.println(medida_4);
        break;

      case 'S':
        // Inicia medidas continuas en ASCII
        continuo = true;
        break;

      case 'T':
        // Detiene medidas continuas en ASCII
        continuo = false;
        break;
    }
  }

  // Envia medidas ASCII si esta activo el modo continuo
  if (continuo)
  {
    Serial.print(medida_2);
    Serial.print("   ");
    Serial.print(medida_3);
    Serial.print("   ");
    Serial.println(medida_4);

    //delay(10);
  }
}

int mide_us(int trig, int echo)
{
  int lectura;

  //Realizamos medida del sensor de US
  //Generamos pulso de emision
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Medimos en cm, como máximo 255 cm
  lectura = pulseIn(echo, HIGH, 20000) / 58;

  // Ajustamos la lectura al rango máximo
  if (lectura > 255)
    lectura = 255;

  return lectura;
}

