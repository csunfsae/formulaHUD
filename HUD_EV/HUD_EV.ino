#include <mcp_can_dfs.h>
#include <mcp_can.h>

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

/* Screen Dimension */
// 96×64 

/* ADAFRUIT OBJECT */
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

/* DATA */
int batteryLife;
int batteryTemperature;

/* FOR READING DATA  */
// time
unsigned long lastTime = 0;
unsigned long watchdog = 0;
// booleans
bool lifeReading = false;
bool tempReading = false;
// messages
String lifeMessage = "";
String tempMessage = "";

/*START CODE */
void setup() {
  // display instance
  display.begin();
  // Serial BAUD
  Serial.begin(115200);
}

/* SEND DATA IN THE FORM OF
  < BatteryLife , BatteryTemperature >
  < 32 , 55 >
*/

void loop()
{
  if (lifeReading) { // Start Reading Battery Life
    int c = Serial.read();
    if (c == ',') { // stop reading battery life at ','
      lifeReading = false;
      tempReading = true; // Set tempReading to true to start reading Temp
      Serial.println("in blife " + lifeMessage);
      batteryLife = lifeMessage.toInt();
      watchdog = millis();
    } else {
      lifeMessage = lifeMessage + (char) c;
    }
  } else if (tempReading) { // Start Reading Battery Temperature
    int c = Serial.read();
    if (c == '>') { // stop reading battery temp at '>'
      tempReading = false;
      Serial.println("in btemp " + tempMessage);
      batteryTemperature = tempMessage.toInt();
      watchdog = millis();
    } else {
      tempMessage = tempMessage + (char) c;
    }
  } else { // Read Data
    int c = Serial.read();
    if (c >= 0) { // will be less then 0 if nothing
      if (c == '<') { // start reading
        lifeReading = true;
        lifeMessage = "";
        tempMessage = "";
      }
    }
  }
  auto current = millis();
  // checks if data is not being sent
  // display warning if there is no data NEW for 5 seconds
  if (current - watchdog > 5000) {
    // Display not connected message on screen
    displayWarning();
  } else {
    if (current - lastTime > 1000) {
      lastTime = current;
      // Update screen
      if (batteryTemperature >= 55) {
        showBatteryTemperature();
      } else {
        showBatteryLife();
      }
    }
  }
  delay(1);
}

void showBatteryTemperature() {
  display.fillScreen(RED);
  display.setCursor(20, 5);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print("Temp");
  
  display.setCursor(38, 25);
  display.setTextColor(BLACK);
  display.setTextSize(3);
  display.print(batteryTemperature);
}

void showBatteryLife() {
  display.fillScreen(GREEN);
  display.setCursor(20, 5);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print("Life");
  
  display.setCursor(35, 25);
  display.setTextColor(BLACK);
  display.setTextSize(3);
  display.print(batteryLife);
}

void displayWarning() {
  display.fillScreen(YELLOW);
}
