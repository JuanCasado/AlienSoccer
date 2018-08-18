#define SENSOR 2
#define LED 3
#define N_MED 32
#define TIMEOUT 1000

unsigned long tiempo_inicio, tiempo_medido, tiempo;
int x;

void setup()
{
  pinMode(SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  tiempo = 0;
  for (x = 0; x < N_MED; x++)
  {
    //El sensor da una señal LOW (OV) cuando detecta la bola
    while (!digitalRead(SENSOR) == HIGH);
    while (!digitalRead(SENSOR) == LOW);
    tiempo_inicio = micros();
    while (!digitalRead(SENSOR) == HIGH);
    tiempo_medido = micros() - tiempo_inicio;
    tiempo = tiempo + tiempo_medido;
      
    if(tiempo < TIMEOUT)
    {
      digitalWrite(LED,HIGH);
    }
    else
    {
      digitalWrite(LED,LOW);
    }  
  }
  tiempo = tiempo / N_MED;
  
  Serial.println(tiempo);

  delay(200);
}
