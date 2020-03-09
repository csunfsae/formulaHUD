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

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

int gear = 3;
int rpm = 0; 


void setup() {
  // put your setup code here, to run once:
  display.begin();
  Serial.begin(9600); 
  display.fillScreen(BLACK);
  delay(300); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  rpm = random() % 4000;
  setRPM();
  display.setCursor(20, 5);
  display.setTextColor(BLACK); 
  display.print("Gear"); 
  display.setCursor(38, 25);
  display.setTextColor(BLACK);
  display.setTextSize(2);
  display.print(gear);
  Serial.print(rpm);
  Serial.print("\t");
  delay(1000);
}

void setRPM() {
  if (rpm <= 1000) {
    display.fillScreen(GREEN);
  }
  else if(rpm >= 1000 && rpm <= 2000) {
    display.fillScreen(YELLOW);
  }
  else {
    display.fillScreen(RED);
  }
}
