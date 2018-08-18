#define VELOCIDAD_SERIE 115200
#define PIN_BRUJULA 3
#define LED 11    //Indicador de estado

#define PROMEDIOS 8

#define T_MAX 1600
#define T_MIN 1400

volatile boolean estado = false;
volatile unsigned long crono;
volatile unsigned long medida[PROMEDIOS]={0,0,0,0,0,0,0,0};
volatile unsigned long control1 = 0;
volatile unsigned long control2 = 0;
volatile int x = 0;

void setup()
{
  Serial.begin(VELOCIDAD_SERIE);

  pinMode(PIN_BRUJULA, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  
  attachInterrupt(1, guardar_tiempo , CHANGE);

  delay(200); //--> espera para llenar el array
}

void loop()
{
  Serial.print(">");
  Serial.println(medir());
  digitalWrite(LED, digitalRead(PIN_BRUJULA));
}
