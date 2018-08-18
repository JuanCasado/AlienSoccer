void jugar(void)
{
  attachInterrupt(digitalPinToInterrupt(SUELO1), suelo1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SUELO2), suelo2, FALLING);
  attachInterrupt(digitalPinToInterrupt(SUELO3), suelo3, FALLING);
  attachInterrupt(digitalPinToInterrupt(SUELO4), suelo4, FALLING);

  digitalWrite(CAL_BRUJULA,LOW);
  
  if (digitalRead(INT_POR_DEL) == HIGH)
    jugar_por();
  else
    jugar_del();
}

