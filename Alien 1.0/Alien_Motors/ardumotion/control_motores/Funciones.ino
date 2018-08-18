//Funcion para medir el encoder
void contar()
{                                                                
  if(digitalRead(ENCODER1)==digitalRead(ENCODER2))
  {
    encoderPos++;
  }
  else
  {
    encoderPos--;
  }
}

//Funciones para controlar el movimiento de los motores
void avanzar (int v,int tipo)
{
  if(tipo==0){
    digitalWrite(PHASE,LOW);
    analogWrite(ENABLE,v);
  }
  else if(tipo==1){
    digitalWrite(PHASE,HIGH);
    analogWrite(ENABLE,v);
  }
  else if (tipo==2)
    digitalWrite(ENABLE,LOW);
}
