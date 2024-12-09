#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Constants
constexpr int RAIN_THRESHOLD = 500;
constexpr int OLED_RESET = -1; // OLED reset pin
constexpr int FRAME_DELAY = 42; // Animation speed
constexpr int WARNING_BLINK_DELAY = 700; // Blinking speed for the warning message
constexpr int FRAME_WIDTH = 32;
constexpr int FRAME_HEIGHT = 32;

// Messages
const String WarningMessage = "SLOW DOWN!";
const String RegularMessage = "NICE DAY!!";

// Global variables
Adafruit_SSD1306 HighwayDisplay(128, 64, &Wire, OLED_RESET);
const byte PROGMEM Rainframes[][128] = { 
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,16,132,0,0,0,0,0,33,12,96,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,1,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,8,32,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,1,255,255,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,0,0,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,5,255,255,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,66,0,24,66,16,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,66,0,24,194,16,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,194,0,24,194,16,128,16,0,0,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,194,0,26,210,16,128,16,132,16,128,4,16,132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,15,255,255,192,13,255,255,64,0,0,0,0,3,24,194,0,26,210,150,128,16,134,48,128,4,16,132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,3,24,194,0,26,210,150,128,16,134,48,128,6,48,132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,3,24,194,0,26,210,150,128,16,134,48,128,6,48,132,0,0,0,0,0,33,8,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,2,24,66,0,18,82,146,128,16,134,48,128,6,48,132,0,0,0,0,0,33,12,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,2,16,2,0,2,16,130,0,16,134,48,128,6,48,132,0,0,0,0,0,33,12,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,1,255,255,0,0,0,0,0,0,0,0,0,2,16,130,0,16,134,48,128,6,48,132,0,0,0,0,0,33,12,32,0,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,1,255,255,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,48,132,0,0,0,0,0,33,12,96,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,5,255,255,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,48,132,0,0,0,0,0,33,12,96,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,66,0,24,66,16,128,0,0,0,0,4,16,132,0,0,0,0,0,33,8,0,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,66,0,24,194,16,128,0,0,0,0,0,0,0,0,0,0,0,0,33,0,0,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,194,0,24,194,16,128,16,0,0,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,2,24,194,0,26,210,16,128,16,132,16,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,15,255,255,192,13,255,255,64,0,0,0,0,3,24,194,0,26,210,150,128,16,134,48,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,3,24,194,0,26,210,150,128,16,134,48,128,4,16,132,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,13,255,255,64,0,0,0,0,3,24,194,0,26,210,150,128,16,134,48,128,6,48,132,0,0,0,0,0,33,8,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,2,24,66,0,18,82,146,128,16,134,48,128,6,48,132,0,0,0,0,0,33,12,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,2,16,2,0,2,16,130,0,16,134,48,128,6,48,132,0,0,0,0,0,33,12,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,0,0,0,0,2,16,130,0,16,134,48,128,6,48,132,0,0,0,0,0,33,12,32,0,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,48,132,0,0,0,0,0,33,12,96,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,31,192,0,0,96,32,0,0,64,16,0,0,192,24,0,0,128,15,192,0,128,8,96,1,128,0,16,15,128,0,24,8,0,0,8,16,0,0,8,16,0,0,8,16,0,0,8,16,0,0,24,16,0,0,16,8,0,0,32,7,255,255,192,0,255,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,48,132,0,0,0,0,0,33,12,96,0,33,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
const byte PROGMEM Sunnyframes[][128] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,48,0,0,0,48,0,0,0,48,0,0,24,48,96,0,12,0,192,0,4,49,128,0,0,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,242,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,0,156,0,2,6,56,0,3,12,64,0,1,24,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,48,0,0,0,48,0,0,24,48,96,0,12,0,192,0,6,49,128,0,0,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,242,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,0,156,0,2,12,56,0,3,28,64,0,1,24,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,16,0,0,8,16,0,0,12,16,32,0,4,0,224,0,0,49,192,0,0,252,0,0,1,4,120,0,2,1,254,0,242,3,3,0,114,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,4,156,0,2,12,56,0,3,24,64,0,1,16,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,16,0,0,8,16,0,0,12,16,32,0,6,0,96,0,2,48,192,0,0,252,128,0,1,4,120,0,2,1,254,0,242,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,4,156,0,2,12,56,0,3,24,64,0,1,16,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,24,0,0,8,16,0,0,4,16,32,0,6,0,96,0,0,48,192,0,0,252,128,0,1,4,120,0,2,1,254,0,242,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,4,156,0,2,28,56,0,3,48,64,0,1,0,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,24,0,0,12,24,0,0,4,16,0,0,2,0,112,0,0,48,224,0,0,252,128,0,1,4,120,0,66,1,254,0,242,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,12,156,0,2,24,56,0,3,48,64,0,1,0,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,8,0,0,4,8,0,0,6,24,0,0,2,0,48,0,0,48,96,0,0,252,192,0,1,4,120,0,226,1,254,0,50,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,12,156,0,2,56,56,0,3,48,64,0,1,0,64,0,1,0,192,0,1,0,192,0,3,0,192,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,8,0,0,4,8,0,0,2,8,0,0,2,0,48,0,0,48,112,0,0,252,192,0,1,4,120,0,114,1,254,0,18,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,24,156,0,2,56,56,0,3,32,64,0,1,0,192,0,1,0,192,0,1,0,192,0,3,0,192,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,12,0,0,6,8,0,0,2,8,0,0,2,0,16,0,0,48,112,0,0,252,224,0,1,4,120,0,114,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,24,156,0,2,48,56,0,3,0,64,0,1,0,192,0,1,0,192,0,1,0,192,0,3,0,192,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,6,12,0,0,2,8,0,0,0,0,0,0,0,48,48,0,0,252,96,0,65,4,120,0,114,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,9,136,0,12,56,156,0,2,48,56,0,3,0,64,0,1,0,192,0,1,0,192,0,1,0,192,0,3,0,192,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,2,4,0,0,2,4,0,0,3,12,0,0,0,0,0,0,0,48,48,0,0,252,112,0,97,4,120,0,50,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,9,136,0,12,56,156,0,2,48,56,0,3,0,64,0,1,0,192,0,1,0,192,0,1,0,192,0,3,0,192,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,0,0,2,4,0,0,1,4,0,0,0,0,0,0,0,48,16,0,0,252,112,0,113,4,120,0,18,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,25,136,0,12,56,156,0,2,32,56,0,3,0,64,0,1,0,192,0,1,0,192,0,1,1,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,6,0,0,3,4,0,0,1,4,0,0,0,0,0,0,0,48,16,0,0,252,112,0,113,4,120,0,18,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,57,136,0,12,112,156,0,2,0,56,0,3,0,64,0,1,0,192,0,1,1,192,0,1,1,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,1,6,0,0,1,4,0,0,0,0,0,0,0,48,0,0,64,252,48,0,113,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,57,136,0,12,112,156,0,2,0,56,0,3,0,64,0,1,1,192,0,1,1,192,0,1,1,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,1,6,0,0,1,4,0,0,0,0,0,0,0,48,0,0,96,252,48,0,49,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,3,12,0,248,57,136,0,12,96,156,0,2,0,56,0,3,0,64,0,1,1,192,0,1,1,192,0,1,1,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,1,130,0,0,0,134,0,0,0,0,0,0,0,48,0,0,112,252,16,0,25,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,19,12,0,248,113,136,0,12,0,156,0,2,0,56,0,3,0,64,0,1,1,192,0,1,1,192,0,1,3,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,0,0,1,130,0,0,0,130,0,0,0,0,0,0,0,48,0,0,48,252,0,0,25,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,19,12,0,248,113,136,0,12,0,156,0,2,0,56,0,3,0,64,0,1,1,192,0,1,3,192,0,1,2,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,129,0,0,0,131,0,0,0,130,0,0,0,0,0,0,32,48,0,0,56,252,0,0,9,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,51,12,0,248,113,136,0,12,0,156,0,2,0,56,0,3,1,64,0,1,1,64,0,1,2,192,0,1,2,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,129,0,0,0,129,0,0,0,130,0,0,0,2,0,0,32,48,0,0,56,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,115,12,0,248,113,136,0,12,0,156,0,2,0,56,0,3,1,64,0,1,3,64,0,1,2,192,0,1,2,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,129,0,0,0,129,0,0,0,131,0,0,0,2,0,0,48,48,0,0,24,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,2,4,1,128,115,12,0,248,65,136,0,12,0,156,0,2,0,56,0,3,1,64,0,1,2,64,0,1,6,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,128,0,0,0,193,128,0,0,67,0,0,0,2,0,0,48,48,0,0,24,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,18,4,1,128,115,12,0,248,1,136,0,12,0,156,0,2,0,56,0,3,3,64,0,1,2,64,0,1,6,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,192,0,0,0,65,128,0,0,65,0,0,32,3,0,0,56,48,0,0,8,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,114,4,1,128,243,12,0,248,1,136,0,12,0,156,0,2,0,56,0,3,2,64,0,1,6,64,0,1,4,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,64,128,0,0,65,128,0,48,3,0,0,24,48,0,0,8,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,114,4,1,128,227,12,0,248,1,136,0,12,0,156,0,2,0,56,0,3,2,64,0,1,6,64,0,1,12,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,64,192,0,0,65,128,0,48,1,0,0,24,48,0,0,4,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,2,6,1,0,242,4,1,128,67,12,0,248,1,136,0,12,0,156,0,2,0,56,0,3,2,64,0,1,4,64,0,1,12,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,96,0,0,0,96,192,0,0,32,128,0,48,1,0,0,12,48,0,0,4,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,18,6,1,0,242,4,1,128,3,12,0,248,1,136,0,12,0,156,0,2,2,56,0,3,6,64,0,1,12,64,0,1,8,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,96,0,0,0,32,64,0,16,32,192,0,24,1,128,0,12,49,0,0,0,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,114,6,1,0,242,4,1,128,3,12,0,248,1,136,0,12,0,156,0,2,2,56,0,3,6,64,0,1,12,64,0,1,8,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,32,0,0,0,32,64,0,16,32,192,0,24,1,128,0,12,49,0,0,0,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,242,6,1,0,242,4,1,128,3,12,0,248,1,136,0,12,0,156,0,2,2,56,0,3,4,64,0,1,8,64,0,1,8,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,32,0,0,0,32,0,0,16,32,64,0,12,0,128,0,4,49,0,0,0,252,0,0,1,4,120,0,2,1,254,0,2,3,3,0,242,6,1,0,2,4,1,128,3,12,0,248,1,136,0,12,0,156,0,2,6,56,0,3,12,64,0,1,24,64,0,1,0,192,0,1,0,192,0,3,0,64,0,2,0,96,0,12,0,63,255,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
constexpr int FRAME_COUNT_RAIN = sizeof(Rainframes) / sizeof(Rainframes[0]);
constexpr int FRAME_COUNT_SUN = sizeof(Sunnyframes) / sizeof(Sunnyframes[0]);

int frame = 0;
bool isMessageVisible = true;
unsigned long lastBlinkTime = 0;
unsigned long lastFrameTime = 0;

void setup() {
    // Initialize the display
    HighwayDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    HighwayDisplay.clearDisplay();
    HighwayDisplay.setTextSize(2);  // Set text size
    HighwayDisplay.setTextColor(WHITE); // Text color
}

void loop() {
    int sensorValue = analogRead(A0);
    unsigned long currentTime = millis();

    if (sensorValue < RAIN_THRESHOLD) {
        // Rain animation
        if (currentTime - lastFrameTime >= FRAME_DELAY) {
            updateRainAnimation();
            lastFrameTime = currentTime;
        }

        // Warning message blinking
        if (currentTime - lastBlinkTime >= WARNING_BLINK_DELAY) {
            updateWarningMessage();
            lastBlinkTime = currentTime;
        }

        HighwayDisplay.display(); // Refresh the display once per loop
    } else {
        // Sunny animation and regular message
        HighwayDisplay.clearDisplay();
        displayRegularMessage();
        updateSunnyAnimation();
        HighwayDisplay.display();
    }
}

// Function Definitions

void updateWarningMessage() {
    if (isMessageVisible) {
        // Draw the warning message
        HighwayDisplay.setCursor(7, 0);
        HighwayDisplay.print(WarningMessage);
    } else {
        // Clear the warning message area
        HighwayDisplay.fillRect(7, 0, 120, 16, BLACK);
    }
    isMessageVisible = !isMessageVisible;
}

void displayRegularMessage() {
    HighwayDisplay.setCursor(7, 0);
    HighwayDisplay.print(RegularMessage);
}

void updateRainAnimation() {
    // Clear only the rain animation area
    HighwayDisplay.fillRect(48, 25, FRAME_WIDTH, FRAME_HEIGHT, BLACK);

    // Draw the current frame of the rain animation
    HighwayDisplay.drawBitmap(48, 25, Rainframes[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);

    // Update the frame index
    frame = (frame + 1) % FRAME_COUNT_RAIN;
}

void updateSunnyAnimation() {
    // Draw the current frame of the sunny animation
    HighwayDisplay.drawBitmap(48, 25, Sunnyframes[frame], FRAME_WIDTH, FRAME_HEIGHT, WHITE);

    // Update the frame index
    frame = (frame + 1) % FRAME_COUNT_SUN;
}
