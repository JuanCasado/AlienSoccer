int velocidades_calibradas[4]={-130,90,90,-130};
int v_max;
boolean signo;
int velocidad;

void setup() {Serial.begin(115200);}
void loop()
{
  for (int x = 0; x < 4; x++)
  {
    /*
    while (Serial.available() < 1);
    velocidades_calibradas[x] = Serial.read();
    Serial.println(velocidades_calibradas[x]);
    */
    if (x == 0)
      v_max = abs(velocidades_calibradas[x]);
    else
    {
      if (velocidades_calibradas[x] > v_max)
        v_max = velocidades_calibradas[x];
    }
  }

  Serial.print(v_max);
  Serial.print("\t");
  Serial.print("==>");
  Serial.print("\t");
  for (int motor = 0; motor < 4; motor++)
  {
    velocidad = velocidades_calibradas[motor];

    //Normaliza las medidas
    if (v_max > 100)
    {
      signo = (velocidad > 0);
      velocidad = abs (velocidad);
      velocidad = ((velocidad * 100)/v_max);
      if (!signo)
        velocidad *=-1;
    }
    

    Serial.print(velocidad);
    Serial.print("\t");
  }
  Serial.println("");
}
