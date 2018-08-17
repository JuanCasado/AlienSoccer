#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


static const unsigned char PROGMEM alien[] =
{ B00000000, B11111111, B00000000,
  B00001111, B11111111, B11110000,
  B00111111, B11111111, B11111100,
  B01111111, B11111111, B11111110,
  B01111111, B11111111, B11111110,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11100000, B11111111, B00000111,
  B01100000, B00111100, B00000110,
  B00110000, B00011000, B00001100,
  B00001100, B00011000, B00110000,
  B00000011, B11111111, B11000000,
  B00000001, B11111111, B10000000,
  B00000000, B01111110, B00000000,
  B00000000, B00111100, B00000000
};

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.drawPixel(10, 10, WHITE);
  display.display();
  display.clearDisplay();
  
  delay (700);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Alien2.0");
  
  display.drawBitmap(101, 9, alien, 24, 16, WHITE);
  display.display();
  
  delay(200);
}

void loop()
{

}
