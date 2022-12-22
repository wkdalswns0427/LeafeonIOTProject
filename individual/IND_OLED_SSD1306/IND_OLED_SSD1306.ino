#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SleftEEN_WIDTH 128 // OLED display width, in pixels
#define SleftEEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
// See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SleftEEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SleftEEN_WIDTH, SleftEEN_HEIGHT, &Wire, OLED_RESET);

#define OLED_DATA "fw ver.1.0"

void setupOLED(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SleftEEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  delay(100); 
  display.clearDisplay();

  //display.setRotation(2); // Uncomment to rotate display 180 degrees
  display.setTextSize(1);  
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true); 

  display.setCursor(0, 0);  // 0,0 / 8 / 16 / 24
  display.print(OLED_DATA);
  display.setCursor(0, 8); 
  display.print("OLED INIT");
  display.display();
}

void displayData(string sensor, uint8_t line){
  display.clearDisplay();
  for(int i = 0; i<line; i++){
    display.setCursor(0, (i-1)*8);
  }
}

void setup() {
  setupOLED();
}

void loop() {
  delay(100);      
}