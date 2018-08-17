void guardar_tiempo()
{
  if (digitalRead(PIN_BRUJULA))
    crono = micros();
  else
  {
      medida[x] = (micros() - crono);
      x++;
      if(x == PROMEDIOS)
        x=0;
  }
}

int medir()
{
  unsigned long acumula;
  for(int z = 0; z < PROMEDIOS; z++)
    acumula += medida [z];
  return ((acumula/PROMEDIOS)/10.0);
}

