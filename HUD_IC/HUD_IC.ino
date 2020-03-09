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

/* ADAFRUIT OBJECT */
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

/* DATA */
int gear;
int rpm;

/* FOR READING DATA  */
// time
unsigned long lastTime = 0;
unsigned long watchdog = 0;
// booleans
bool gearReading = false;
bool rpmReading = false;
// messages
String gearMessage = "";
String rpmMessage = "";

void setup() {
  // put your setup code here, to run once:
  display.begin();
  Serial.begin(115200);
}

/* SEND DATA IN THE FORM OF
  < GEAR , RPM >
  < 1, 1000 >
*/

void loop() {
  if (gearReading) { // Start Reading GEAR
    int c = Serial.read();
    if (c == ',') { // stop reading gear at ','
      gearReading = false;
      rpmReading = true; // Set rpmReading to true to start reading temp
      Serial.println("in gear " + gearMessage);
      gear = gearMessage.toInt();
      watchdog = millis();
    } else {
      gearMessage = (char) c;
    }
  } else if (rpmReading) { // Start Reading RPM
    int c = Serial.read();
    if (c == '>') { // stop reading rpm at '>'
      rpmReading = false;
      Serial.println("in rpm " + rpmMessage);
      rpm = rpmMessage.toInt();
      watchdog = millis();
    } else {
      rpmMessage = rpmMessage + (char) c;
    }
  } else { // Start Reading Data
    int c = Serial.read();
    if (c >= 0) { // msg available
      gearReading = true;
      gearMessage = "";
      rpmMessage = "";
    }
  }
  auto current = millis();
  // check if data is not being sent
  // display warning if there is no data NEW for 5 seconds
  if (current - watchdog > 5000) {
    displayWarning();
  } else {
    if (current - lastTime > 1000) {
      lastTime = current;
      // update screen
      showGear();
    }
  }
  delay(1);
}

void showGear() {
  setRPM();
  display.setCursor(38, 20);
  display.setTextColor(BLACK);
  display.setTextSize(4);
  display.print(gear);
}

void setRPM() {
  if (rpm <= 1000) {
    display.fillScreen(GREEN);
  } else if (rpm >= 1000 && rpm <= 2000) {
    display.fillScreen(YELLOW);
  } else {
    display.fillScreen(RED);
  }
}

void displayWarning() {
  display.fillScreen(YELLOW);
}
