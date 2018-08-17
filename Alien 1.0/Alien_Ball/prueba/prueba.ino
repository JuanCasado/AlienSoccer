
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define S4 6
#define S5 7
#define S6 8
#define S7 9
#define S8 10
#define S9 11
#define S10 12
#define S11 13
#define S12 14
#define S13 15
#define S14 16
#define S15 17
#define LEDP 18
#define LEDN 19


#define TIMEOUT 1400

unsigned long valor0;
unsigned long valor1;
unsigned long valor2;
unsigned long valor3;
unsigned long valor4;
unsigned long valor5;
unsigned long valor6;
unsigned long valor7;
unsigned long valor8;
unsigned long valor9;
unsigned long valor10;
unsigned long valor11;
unsigned long valor12;
unsigned long valor13;
unsigned long valor14;
unsigned long valor15;

void setup()
{
  pinMode (2, INPUT);
  pinMode (3, INPUT);
  pinMode (4, INPUT);
  pinMode (5, INPUT);
  pinMode (6, INPUT);
  pinMode (7, INPUT);
  pinMode (8, INPUT);
  pinMode (9, INPUT);
  pinMode (10, INPUT);
  pinMode (11, INPUT);
  pinMode (12, INPUT);
  pinMode (13, INPUT);
  pinMode (14, INPUT);
  pinMode (15, INPUT);
  pinMode (16, INPUT);
  pinMode (17, INPUT);
  pinMode (18, OUTPUT);
  pinMode (19, OUTPUT);

  Serial.begin(9600);
}

void loop()
{

  valor0 = pulseIn(S0, LOW, TIMEOUT);
  valor1 = pulseIn(S1, LOW, TIMEOUT);
  valor2 = pulseIn(S2, LOW, TIMEOUT);
  valor3 = pulseIn(S3, LOW, TIMEOUT);
  valor4 = pulseIn(S4, LOW, TIMEOUT);
  valor5 = pulseIn(S5, LOW, TIMEOUT);
  valor6 = pulseIn(S6, LOW, TIMEOUT);
  valor7 = pulseIn(S7, LOW, TIMEOUT);
  valor8 = pulseIn(S8, LOW, TIMEOUT);
  valor9 = pulseIn(S9, LOW, TIMEOUT);
  valor10 = pulseIn(S10, LOW, TIMEOUT);
  valor11 = pulseIn(S11, LOW, TIMEOUT);
  valor12 = pulseIn(S12, LOW, TIMEOUT);
  valor13 = pulseIn(S13, LOW, TIMEOUT);
  valor14 = pulseIn(S14, LOW, TIMEOUT);
  valor15 = pulseIn(S15, LOW, TIMEOUT);

  if (digitalRead(S7) == LOW)   
  {
    digitalWrite(LEDP, HIGH);
    digitalWrite(LEDN, LOW);
  }
  else
    {
      digitalWrite(LEDP, LOW);
      digitalWrite(LEDN, LOW);
    }

    Serial.print("Pulso 7:"); Serial.println(valor7);
 // Serial.print("      ");
  //Serial.print("Pulso 12:"); Serial.print(valor12);
  //Serial.print("      ");
  //Serial.print("Pulso 13:"); Serial.println(valor13);
  delay(100);

}
