//FUNCIONES

int binariza_array_sensores (void)
{
  int contador = 0, numero_unos = 0;

  for (contador = 0; contador < N_SENSORES; contador++ )
  {
    if (s[contador] > 0)
    {
      veo[contador] = true;
      numero_unos++;
    }
    else
      veo[contador] = false;
  }
  /*for (contador = 0; contador < N_SENSORES; contador++ )
  {
    Serial.print(veo[contador]);
  }
  Serial.println();*/
  return numero_unos;
}

int busca_sensor_maximo(void)
{
  int v_maximo = 0, i_maximo = 255;
  byte contador = 0;
  for (contador = 0; contador < N_SENSORES; contador++)
  {
    if (v_maximo < s[contador])
    {
      v_maximo = s[contador];
      i_maximo = contador;
    }
  }
  distancia_grupo = v_maximo;
  return i_maximo *2;
}

int busca_primer_cero_sensores(void)                       //Busca el primer uno y lo traduce a sensores virtuales (32)
{
  int contador = 0;

  while ((veo[contador] == true) && (contador < 16))
    contador++;

  return contador+1;
}

byte busca_maximo_mayor_energia_grupo(void)
{
  byte j = 0, comienzo_grupo = false, cuenta_grupos = 0, primero = 0;
  byte indice_max = 0, movi = 0, contador = 0, elementos = 0, x = 0;
  int energia = 0, a_energia[16], a_comienzo[16], a_longitud[16], max_centro = 0;

  for (x = 0; x < 16; x++)
  {
    a_energia[x] = 0;
    a_comienzo[x] = 0;
    a_longitud[x] = 0;
  }

  j = primer_elemento;

  //Buscamos grupos
  for (contador = 0; contador < 16; contador++)
  {
    if (veo[j] == 0)                        //El control se realiza según el array binario
    {
      //Fuera de grupo o final de grupo
      if (comienzo_grupo == true)           //deja todo como estaba (sensor que no ve)-->cambio de grupo
      {
        //Final de grupo
        a_energia[cuenta_grupos] = energia;
        a_comienzo[cuenta_grupos] = primero;
        a_longitud[cuenta_grupos] = elementos;
        cuenta_grupos++;
        comienzo_grupo = false;
      }
    }
    else
    {
      //Comienzo de grupoo o elemento de grupo
      if (comienzo_grupo == false)        //Primer sensor que está viendo; da valores iniciales al grupo.
      {
        //Comienzo de grupo
        energia = s[j];
        primero = j;
        elementos = 1;
        comienzo_grupo = true;
      }
      else
      {
        //Elemento de grupo
        energia = energia + s[j];         //Añade el cronómetro de un sensor que está viendo al grupo  
        elementos++;
      }
    }
    j++;

    //Da la vuelta ala array (en la lectura)
    if (j > 15)
      j -= 16;
  }
  //Buscamos el grupo con ,mayor energia
  indice_max = 0;
  max_centro = 0;
  for (j = 0; j < cuenta_grupos; j++)
  {
    if (max_centro < a_energia[j])    //Busca el grupo con mayor energía --> conjunto contínuo de sensores que tienen un cronómetro mayor
    {
      max_centro = a_energia[j];      
      indice_max = j;
    }
  }

  distancia_grupo = a_energia[indice_max];

  //Buscamos el elemento central del grupo
  movi = ((a_comienzo[indice_max]*2) + a_longitud[indice_max] -1); //Devuelve el centro del grupo que más ve traducido a movimientos virtuales

  //Control para dar la vuelta al array
  if (movi > 31)
    movi -= 32;
    
  return movi;
  
}











