#define LED 11  //Indicador de estado

//Pines del motor
#define PHASE 8      //rojo
#define ENABLE 9     //negro
                                                       
void setup() {
  pinMode(PHASE,OUTPUT);
  pinMode(ENABLE,OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() 
{
  digitalWrite(LED, HIGH);
  digitalWrite(PHASE,LOW);
  digitalWrite(ENABLE,HIGH);
  delay(1000);

  digitalWrite(LED, LOW);
  digitalWrite(ENABLE, LOW);
  delay(500);

  digitalWrite(LED, HIGH);
  digitalWrite(PHASE,HIGH);
  digitalWrite(ENABLE,HIGH);
  delay(1000);

  digitalWrite(LED, LOW);
  digitalWrite(ENABLE, LOW);
  delay(500);
}
