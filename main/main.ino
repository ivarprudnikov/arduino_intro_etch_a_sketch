#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Setup display
//////////////////////////

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define BUTTON_LEFT  4
#define BUTTON_RIGHT 5
#define SERIAL_UPDATE_INTERVAL_MILLIS 1000

void setup() {
  Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  
  drawClear();
}

void drawClear(void) {
  
  // Fill white from top to bottom
  for (int y = 0; y < SSD1306_LCDHEIGHT; y++) {
    display.drawFastHLine(0, y, SSD1306_LCDWIDTH, 1);
    display.display();
    delay(8);
  }
  
  // Fill black from bottom to top
  for (int y = SSD1306_LCDHEIGHT; y > 0; y--) {
    if (y>0) {
      display.drawFastHLine(0, y, SSD1306_LCDWIDTH, 0);
    }
    display.display();
    delay(8);
  }
  
  // Clear the buffer.
  display.clearDisplay();
}

void loop() {
  static bool penDown = false;
  static int phase = 0;
  static unsigned long next_serial_timestamp = 0;
  int H = map(analogRead(A0), 0, 728, 0, SSD1306_LCDWIDTH - 1);
  int V = map(analogRead(A1), 0, 728, 0, SSD1306_LCDHEIGHT - 1);

  if (digitalRead(BUTTON_LEFT) == LOW) {
    penDown = false;
    drawClear();
    while (digitalRead(BUTTON_LEFT) == LOW) ;
  }

  if (digitalRead(BUTTON_RIGHT) == LOW) {
    penDown = !penDown;
    while (digitalRead(BUTTON_RIGHT) == LOW) ;
  }

  if (next_serial_timestamp < millis()) {
    Serial.print("Pen is ");
    Serial.println(penDown ? "down" : "up");
    Serial.print("H: ");
    Serial.print(H);
    Serial.print(" - V: ");
    Serial.println(V);
    Serial.println(analogRead(A0));
    next_serial_timestamp = millis() + SERIAL_UPDATE_INTERVAL_MILLIS;
  }

  if (penDown) {
    display.drawPixel(H, V, 1);
  } else {
    display.drawPixel(H, V, 1);
    display.display();
    display.drawPixel(H, V, 0);
  }
  display.display();
}
