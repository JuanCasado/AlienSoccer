/*
  SENSORES
  Nº PIN PUERTO(D0,B1,C2)
  0  2   PD2
  1  3   PD3
  2  4   PD4
  3  5   PD5
  4  6   PD6
  5  7   PD7
  6  8   PB0
  7  9   PB1
  8  10  PB2
  9  11  PB3
  10 12  PB4
  11 13  PB5
  12 14  PC0
  13 15  PC1
  14 16  PC2
  15 17  PC3
*/

#define T_BOLA 1000  //timeout en microsegundos
#define LEDN 19      //negativo led
#define LEDP 18      //positivo led
#define N_SENSORES 16 //constante que define el numero de sensores para comprobar por que sensores ve la bola
#define REF_CORTE 50
#define N_PROMEDIOS 32

byte puerto_0 = B00000000, puerto_1 = B00000000;
byte bola = 0;                            //valor de vision de la bola
byte p_sensores[2] = {0, 0};           //array donde se guardan las lecturas de los puertos

byte lista = 0, bit_sensor = 0, puerto = 0, estado = 1, s_acabado = 0;
byte cuenta_promedios = 0;
int numero_sensores_activos = 0, primer_elemento = 1;
int distancia_grupo = 0;
unsigned int acumula_distancia = 0;

byte lista_s[N_SENSORES];
byte lista_p[N_SENSORES];
byte estado_s[N_SENSORES];
byte veo[N_SENSORES];
int acumula_s[N_SENSORES];
int s[N_SENSORES];                              //Medidas de tiempo de los sensores

unsigned long crono_t = 0, t_maximo = 0;      //cronometro
unsigned long crono[N_SENSORES];                //array que guarda el instante de comienzo de un pulso

byte cuenta_s = 0, x = 0, i = 0, test = 0;
byte indice_maximo = 0;                         //Sensor por el que se ve la bola

int binariza_array_sensores(void);
int busca_sensor_maximo(void);
int busca_primer_cero_sensores(void);
int busca_primer_cero_sensores(void);
int busca_maximo_mayor_energia_grupo(void);

void setup()
{
  DDRD = B00000011;
  DDRB = DDRB & B11000000;
  DDRC = DDRC & B11110000; // recordar configurado como input todo
  pinMode(LEDP, OUTPUT);
  pinMode(LEDN, OUTPUT);
  Serial.begin(115200);

  for (x = 0; x < N_SENSORES; x++)
  {
    lista_s[x] = 0;
    lista_p[x] = 0;
    estado_s[x] = 0;
    acumula_s[x] = 0;
    crono[x] = 0;
    veo[x] = false;
    s[x] = 0;
  }
}

void loop()
{
  //Esperamos a que finalice transmision previa
  do
  {
    puerto_0 = ((~PIND >> 2)& B00111111) | (((~PINB) & B00000011) << 6);          //sensores S0-S7
    puerto_1 = (((~PINC) & B00001111) << 4) | (((~PINB) & B00111100) >> 2);       //sensores S8-S15
  }
  while ((puerto_0 + puerto_1) != 0);

  //cronometro para timeout
  crono_t = micros();

  //Esperamos comienzo de transmision o timeout (no ve la bola)
  do
  {
    puerto_0 = ((~PIND >> 2)& B00111111) | (((~PINB) & B00000011) << 6);          //sensores S0-S7
    puerto_1 = (((~PINC) & B00001111) << 4) | (((~PINB) & B00111100) >> 2);       //sensores S8-S15
    t_maximo = micros() - crono_t;              //Incremento de tiempo desde primera medida
  }
  while (((puerto_0 + puerto_1) == 0) && (t_maximo < T_BOLA));
  cuenta_s = 0;
  if (t_maximo < T_BOLA)
  {
    bola = true;
    digitalWrite(LEDP, HIGH);
    digitalWrite(LEDN, LOW);

    // Iniciamos el contador de sensores que ve a bola

    crono_t = micros();

    // Buscamos sensores activos durante un tiempo para no saltarnos algunos sensores
    // que esten desfasados
    while ((micros() - crono_t) < 120)
    {

      //Medimos para ver cuando se inicia la transmision (evitar desfase)
      p_sensores[0] = ((~PIND >> 2)& B00111111) | (((~PINB) & B00000011) << 6);          //sensores S0-S7
      p_sensores[1] = (((~PINC) & B00001111) << 4) | (((~PINB) & B00111100) >> 2);       //sensores S8-S15

      for (x = 0; x < 2; x++)
      {
        if (p_sensores[x] > 0)
        {
          //Serial.print("P");
          //Serial.print(x);
          //vemos algo en los sensores de este puerto
          for (i = 0; i < 8; i++)
          {
            // Buscamos la entrada por la que vemos algo
            if ((p_sensores[x] & (1 << i)))
            {
              //Serial.print("S");
              //Serial.print(i);
              estado = estado_s[i + (8 * x)];
              //Serial.print("E");
              //Serial.print(estado);
              if (estado == 0)
              {
                crono[cuenta_s] = micros();     //Inciciamos un cronómetro por sensor

                //Sirven para localizar los sensores activos en el futuro
                lista_s[cuenta_s] = i;
                lista_p[cuenta_s] = x;

                estado_s[i + (8 * x)] = 1;  //Lo ponemos a 1 para evitar que entre de nuevo en el if
                cuenta_s++;
              }
            }
          }
        }
      }

      s_acabado = cuenta_s;

      while (s_acabado > 0)
      { 
        //Medimos para ver cuando acaban la transmision
        p_sensores[0] = ((~PIND >> 2)& B00111111) | (((~PINB) & B00000011) << 6);          //sensores S0-S7
        p_sensores[1] = (((~PINC) & B00001111) << 4) | (((~PINB) & B00111100) >> 2);       //sensores S8-S15

        for (i = 0; i < cuenta_s; i++)
        {
          puerto = lista_p[i];
          bit_sensor = lista_s[i];
          lista = bit_sensor + puerto * 8;  //Localizador de sensores activos

          if (estado_s[lista])              //Comprobación para hacer solo una vez
          {
            if ((p_sensores[puerto] & (1 << bit_sensor )) == 0) //Busca cada sensor activo
            {
              acumula_s[lista] = (acumula_s[lista] + (int)(micros() - crono[i]) - ((i + 1) * 13));
              estado_s[lista] = 0;
              s_acabado--;
            }
            else
            {
              estado_s[lista] = 0;
              s_acabado--;
            }
          }
          else
          {
            estado_s[lista] = 0;
            s_acabado--;
          }
        }
      }
    }
  }
  else
  {
    bola = false;
    digitalWrite(LEDP, LOW);
    digitalWrite(LEDN, LOW);
  }
  Serial.print(puerto_0, BIN);
  Serial.print("\t");
  Serial.print(puerto_1, BIN);
  Serial.print("\t");
  Serial.print("C$");
  Serial.println(cuenta_s);

  for (int x = 0; x < N_SENSORES; x++)
  {
    Serial.print(acumula_s[x]);
    Serial.print("/");
  }
  Serial.println();
}
