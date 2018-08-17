void jugar(void)
{
  digitalWrite(CAL_BRUJULA,LOW);
  
  if (digitalRead(INT_POR_DEL) == HIGH)
    jugar_por();
  else
    jugar_del();
}

