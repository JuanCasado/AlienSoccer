/* Author = helscream (Omer Ikram ul Haq)
Last edit date = 2014-06-22
Website: http://hobbylogs.me.pn/?p=17
Location: Pakistan
Ver: 0.1 beta --- Start
Ver: 0.2 beta --- Debug feature included
*/

#include <Wire.h>
#include "compass.h"

#define Task_t 10          // Task Time in milli seconds

int dt=0;
unsigned long t;
// Main code -----------------------------------------------------------------
void setup(){
  Serial.begin(9600);
  // Serial.print("Setting up I2C ........\n");
  Wire.begin();
  compass_x_offset = -17.27;
  compass_y_offset = 151.91;
  compass_z_offset = 399.15;
  compass_x_gainError = 1.05;
  compass_y_gainError = 1.08;
  compass_z_gainError = 0.97;
  
  
  
  compass_init(2);
  //compass_debug = 1;
  //compass_offset_calibration(3);


}

// Main loop 
// Main loop -----------------------------------------------------------------
void loop(){
  
  t = millis();
 
  float load;
 
  
  
  /*compass_scalled_reading();
  
  Serial.print("x = ");
  Serial.println(compass_x_scalled);
  Serial.print("y = ");
  Serial.println(compass_y_scalled);
  Serial.print("z = ");
  Serial.println(compass_z_scalled);*/
  

  compass_heading();
  //Serial.print ("Heading angle = ");
  Serial.println (bearing);
  //Serial.println(" Degree");
  
  /*dt = millis()-t;
  load = (float)dt/(Task_t/100);
  Serial.print ("Load on processor = ");
  Serial.print(load);
  Serial.println("%");*/

  
  //delay(3000);

  delay(200);
  
}











