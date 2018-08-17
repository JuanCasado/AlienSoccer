//Código brújula digital con sensor magnetometro HMC5883

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Asignacion de una identificacion a la brujula
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("ID unica:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Valor Maximo:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Valor Minimo:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolucion:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
 
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Magnetometro HMC5883 Test"); Serial.println("");
 
  /* Inicializamos el sensor */
  if(!mag.begin())
  {
    // Si hay algun problema con el HMC5883 sale el aviso de que revise las conexiones
    Serial.println("Ooops, no se ha detectado el HMC5883 ... revisa las conexiones!");
    while(1);
  }
 
  // Muesta la información básica del sensor
  displaySensorDetails();
}

void loop(void)
{
  // Hacemos que el sensor tome una nueva muestra
  sensors_event_t event;
  mag.getEvent(&event);
 
  /*
     Sostenga el módulo de manera que Z está señalando arriba y se pueda medir con el título X e Y
     Calcular la medida cuando el magnetómetro esté nivelado, y luego corregir los signos de eje
  */
 
  float muestra = atan2(event.magnetic.y, event.magnetic.x);
 
  /* Una vez tengamos la muestra tomada, a continuación debemos agregar el "ángulo de declinación"
    el ángulo de declinación es el "error" del campo magnético en su ubicación.
  Puedes encontrar el tuyo aquí: http://www.magnetic-declination.com/
   Si no encuentras tu ángulo de declinación, comenta las dos línias siguientes, la brújula estará  
  ligeramente desviada*/
 
  float declinacionAngulo = 0.23;
  muestra += declinacionAngulo;
 
  // corrige los valores negativos
  if(muestra < 0)
    muestra += 2*PI;
   
  // Comprueba si hay error debido a la adición de la declinación.
  if(muestra > 2*PI)
    muestra -= 2*PI;
   
  // Convierte los radianes a grados
  float muestraangulo = muestra * 180/M_PI;
 
/* A continuación vamos a mostrar por pantalla los diferentes valores y puntos cardinales en función de las muestras tomadas*/
  if (muestraangulo > 350){
    Serial.print("N      "); Serial.println(muestraangulo);
  delay(500);
  }
 
  if (muestraangulo <= 15){
    Serial.print("N      "); Serial.println(muestraangulo);
  delay(500);
  }
  if (muestraangulo > 15 && muestraangulo <= 75){
    Serial.print("NE    "); Serial.println(muestraangulo);
  delay(500);
  }
 
  if (muestraangulo > 75 && muestraangulo <= 105){
    Serial.print("E      "); Serial.println(muestraangulo);
    delay(500);
  }
 
   if (muestraangulo > 105 && muestraangulo <= 165){
    Serial.print("SE    "); Serial.println(muestraangulo);
    delay(500);
  }
 
  if(muestraangulo > 165 && muestraangulo <= 195){
    Serial.print ("S    "); Serial.println(muestraangulo);
    delay(500);
  }
 
  if(muestraangulo > 195 && muestraangulo <= 255){
    Serial.print ("SW    "); Serial.println(muestraangulo);
    delay(500);
  }
 
  if(muestraangulo > 255 && muestraangulo <= 285){
    Serial.print ("W    "); Serial.println(muestraangulo);
    delay(500);
  }
 
  if(muestraangulo > 285 && muestraangulo <= 350){
    Serial.print ("NW    "); Serial.println(muestraangulo);
  delay(500);
  }  
}
