#include <mcp_can.h>
#include <mcp_can_dfs.h>

#include <Adafruit_SSD1331.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

//declare pins 
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8
// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// adafruit object 
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

// 
int batteryTemperature = 0;
int batteryLife = 90; 

void setup() {
  // display instance
  display.begin();
  // set up
  display.fillScreen(BLACK);
  delay(300);
}
void loop() 
{
  batteryTemperature = random() % 100; 
  batteryLife = random() % 50;
  if (batteryTemperature >= 55) {
    showBatteryTemperature();
  } else {
    showBatteryLife(); 
  }
  delay(1000);
 
}

void showBatteryTemperature() {
  display.fillScreen(RED);
  display.setCursor(20, 5);
  display.setTextColor(WHITE);
  display.print("Temp");
  display.setCursor(38, 25);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print(batteryTemperature);
}

void showBatteryLife() {
  display.fillScreen(GREEN);
  display.setCursor(20, 5);
  display.setTextColor(WHITE);
  display.print("Life");
  display.setCursor(38, 25);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print(batteryLife);  
}
