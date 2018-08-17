// Programa de prueba para un sensor de US con comandos de control

#define PIN_TRIG_US 2
#define PIN_ECHO_US 3

#define ID "ArduSonic V1.0"

int lectura, medida;
byte continuo = false;

void setup()
{
  pinMode(PIN_TRIG_US, OUTPUT);
  pinMode(PIN_ECHO_US, INPUT);

  Serial.begin(115200);
}

void loop()
{
  //Realizamos medida del sensor de US
  //Generamos pulso de emision
  digitalWrite(PIN_TRIG_US, LOW);
  delay(10);
  digitalWrite(PIN_TRIG_US, HIGH);
  delay(10);
  digitalWrite(PIN_TRIG_US, LOW);

  // Medimos en cm, como máximo 255 cm
  lectura = pulseIn(PIN_ECHO_US, HIGH, 20000) / 58;

  // Tomamos solo datos válidos
  if (lectura > 0)
  {
    // Ajustamos la lectura al rango máximo
    if (lectura > 255)
      lectura = 255;
    medida = lectura;
  }

  if (Serial.available() > 0)
  {
    switch (Serial.read())
    {
      case 'i':
        // Envía identificación del modulo en binario
        Serial.write(true);
        break;

      case 'I':
        // Envía identificación del modulo en ASCII
        Serial.println(ID);
        break;

      case 'm':
        // Envia una medida unica en binario
        Serial.write(medida);
        break;

     case 'M':
        // Envia una medida unica en ASCII
        Serial.println(medida);
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
    Serial.println(medida);

  delay(200);
}
