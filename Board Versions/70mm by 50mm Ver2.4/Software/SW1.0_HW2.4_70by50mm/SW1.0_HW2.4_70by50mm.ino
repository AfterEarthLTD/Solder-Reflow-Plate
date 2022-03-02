/* Solder Reflow Plate Sketch
    H/W - Ver 2.4
    S/W - Ver 1.0
    by Chris Halsall     */

/* To prepare
   1) Install MiniCore in additional boards; (copy into File->Preferences->Additional Boards Manager URLs)
       https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
   2) Then add MiniCore by searching and installing (Tools->Board->Board Manager)
   3) Install Adafruit_GFX and Adafruit_SSD1306 libraries (Tools->Manage Libraries)
*/

/* To program
    1) Select the following settings under (Tools)
        Board->Minicore->Atmega328
        Clock->Internal 8MHz
        BOD->BOD 2.7V
        EEPROM->EEPROM retained
        Compiler LTO->LTO Disabled
        Variant->328P / 328PA
        Bootloader->No bootloader
    2) Set programmer of choice, e.g.'Arduino as ISP (MiniCore)', 'USB ASP', etc, and set correct port.
    3) Burn bootloader (to set fuses correctly)
    4) Compile and upload
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <Bounce2.h>

//Version Definitions
static const PROGMEM float hw = 2.4;
static const PROGMEM float sw = 1.1;

//Screen Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C   //I2C Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Create Display

//Pin Definitions
#define mosfet 3
#define upsw 6
#define dnsw 5
#define temp 16   //A2
#define vcc 14    //A0

Bounce2::Button upButton;
Bounce2::Button downButton;

//Temperature Info
const byte maxTempArray[] = { 140, 150, 160, 170, 180 };
const byte maxTempCount = sizeof(maxTempArray) / sizeof(maxTempArray[0]);

byte maxTempIndex = 0;
byte tempIndexAddr = 1;

//Voltage Measurement Info
const float vConvert = 52.00;
const float vMin = 10.50;

//Solder Reflow Plate Logo
static const uint8_t PROGMEM logo[] = {
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x31, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x1f, 0xe0, 0x03, 0x01, 0x80, 0x00, 0x00, 0x30, 0x70, 0x00, 0x21, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x20, 0x03, 0x00, 0xc7, 0x80, 0x00, 0x20, 0x18, 0xf0, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x00, 0x03, 0x3e, 0xcc, 0xc0, 0xc0, 0x04, 0x19, 0x98, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x1c, 0x01, 0xf3, 0x77, 0xd8, 0xc7, 0xe0, 0x06, 0x33, 0x18, 0x61, 0x8f, 0x88, 0x00, 0x00, 0x00,
  0x06, 0x03, 0x3b, 0x61, 0xd0, 0xc6, 0x00, 0x07, 0xe2, 0x18, 0x61, 0x98, 0xd8, 0x04, 0x00, 0x00,
  0x01, 0xc6, 0x0b, 0x60, 0xd9, 0x86, 0x00, 0x06, 0x03, 0x30, 0xff, 0xb0, 0x78, 0x66, 0x00, 0x00,
  0x40, 0xe4, 0x0f, 0x60, 0xdf, 0x06, 0x00, 0x07, 0x03, 0xe0, 0x31, 0xe0, 0x78, 0x62, 0x00, 0x00,
  0x40, 0x3c, 0x0f, 0x61, 0xd8, 0x06, 0x00, 0x07, 0x83, 0x00, 0x31, 0xe0, 0x78, 0x63, 0x00, 0x00,
  0x60, 0x36, 0x1b, 0x63, 0xc8, 0x02, 0x00, 0x02, 0xc1, 0x00, 0x18, 0xb0, 0xcc, 0xe2, 0x00, 0x00,
  0x30, 0x33, 0x3b, 0x36, 0x4e, 0x03, 0x00, 0x02, 0x61, 0xc0, 0x0c, 0x99, 0xcd, 0xfe, 0x00, 0x00,
  0x0f, 0xe1, 0xe1, 0x3c, 0x03, 0xf3, 0x00, 0x02, 0x38, 0x7e, 0x0c, 0x8f, 0x07, 0x9c, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0x84, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0xe4, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x3c, 0x3c, 0x18, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x1e, 0x06, 0x7f, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x3e, 0x03, 0x18, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x36, 0x7f, 0x19, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xe6, 0xc7, 0x19, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x07, 0x83, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x07, 0x81, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x06, 0xc3, 0x98, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x04, 0x7e, 0x08, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t logo_width = 128;
static const uint8_t logo_height = 27;

//Heating Animation
static const uint8_t PROGMEM heat_animate[] = {
  0b00000001, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000001, 0b01000000,
  0b00000010, 0b01000000,
  0b00100010, 0b01000100,
  0b00100100, 0b00100100,
  0b01010101, 0b00100110,
  0b01001001, 0b10010110,
  0b10000010, 0b10001001,
  0b10100100, 0b01000001,
  0b10011000, 0b01010010,
  0b01000100, 0b01100010,
  0b00100011, 0b10000100,
  0b00011000, 0b00011000,
  0b00000111, 0b11100000
};
static const uint8_t heat_animate_width = 16;
static const uint8_t heat_animate_height = 16;

//Tick
static const uint8_t PROGMEM tick[] = {
  0b00000000, 0b00000100,
  0b00000000, 0b00001010,
  0b00000000, 0b00010101,
  0b00000000, 0b00101010,
  0b00000000, 0b01010100,
  0b00000000, 0b10101000,
  0b00000001, 0b01010000,
  0b00100010, 0b10100000,
  0b01010101, 0b01000000,
  0b10101010, 0b10000000,
  0b01010101, 0b00000000,
  0b00101010, 0b00000000,
  0b00010100, 0b00000000,
  0b00001000, 0b00000000,
  0b01111111, 0b11100000
};
static const uint8_t tick_width = 16;
static const uint8_t tick_height = 15;

enum { IDLE, HEATING, COOL_DOWN, COMPLETE, CANCEL_PB, CANCEL_TIMEOUT };
int currentState = IDLE;

unsigned long heatStartTime;
unsigned long heatTimeout = 1000UL * 60 * 8;  // 8 minute timeout

void setup() {

  //Pin Direction control
  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, LOW);
  //  pinMode(upsw, INPUT);
  //  pinMode(dnsw, INPUT);
  pinMode(temp, INPUT);
  pinMode(vcc, INPUT);

  upButton.attach(upsw);
  upButton.setPressedState(LOW);
  upButton.interval(10);

  downButton.attach(dnsw);
  downButton.setPressedState(LOW);
  downButton.interval(10);

  //Pull saved values from EEPROM
  maxTempIndex = EEPROM.read(tempIndexAddr) % maxTempCount;

  //Enable Fast PWM with no prescaler
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);

  //Start-up Diplay
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, logo, logo_width, logo_height, SSD1306_WHITE);
  display.setCursor(80, 16);
  display.print(F("S/W V"));
  display.print(sw, 1);
  display.setCursor(80, 24);
  display.print(F("H/W V"));
  display.print(hw, 1);
  display.display();
  delay(3000);

  currentState = -1;
  beginState( IDLE );
}

void loop() {

  upButton.update();
  downButton.update();
  checkState();
}


void beginState( int state ) {
  if ( currentState == state ) return;  // already in this state

  currentState = state;
  switch ( state ) {
    case IDLE:
      // display IDLE screen
      display.clearDisplay();
      display.setTextSize(1);
      display.drawRoundRect( 0, 0, 83, 32, 2, SSD1306_WHITE);

      break;

    case HEATING:
      // begin heating
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(22, 4);
      display.print(F("HEATING"));
      display.setTextSize(1);
      display.setCursor(52, 24);
      display.print(maxTempArray[maxTempIndex]);
      display.print(F("C"));
      display.display();
      delay(3000);
      heatStartTime = millis();
      break;

    case COOL_DOWN:
      // begin cool down
      digitalWrite(mosfet, LOW);
      display.clearDisplay();
      display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
      display.setCursor(25, 4);
      display.print(F("  COOL DOWN"));
      display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );
      display.setCursor(25, 14);
      display.println("  Still Hot");
      break;

    case COMPLETE:
      // all done
      display.clearDisplay();
      display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
      display.setCursor(25, 4);
      display.print(F("  COMPLETED  "));
      display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );
      display.setCursor(25, 14);
      display.println(" Push button");
      display.setCursor(25, 22);
      display.println("  to return");
      display.drawBitmap( 0, 9, tick, tick_width, tick_height, SSD1306_WHITE);
      display.drawBitmap( 112, 9, tick, tick_width, tick_height, SSD1306_WHITE);
      display.display();
      break;

    case CANCEL_PB:
      // heating got cancelled by push button
      digitalWrite(mosfet, LOW);
      display.clearDisplay();
      display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
      display.setCursor(25, 4);
      display.print(F("  CANCELLED"));
      display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );
      display.setCursor(25, 14);
      display.println(" Push button");
      display.setCursor(25, 22);
      display.println("  to return");
      display.setTextSize(3);
      display.setCursor(5, 4);
      display.print(F("!"));
      display.setTextSize(3);
      display.setCursor(108, 4);
      display.print(F("!"));
      display.setTextSize(1);
      display.display();
      break;

    case CANCEL_TIMEOUT:
      // heating got cancelled by timeout
      digitalWrite(mosfet, LOW);
      cancelledTimeout();
      break;

    default:
      // tried to switch to an unknown state
      // TODO: write error message to display
      break;
  }
}

void checkState() {

  switch (currentState) {
    case IDLE:
      updateIdleDisplay();
      //If both buttons pressed, start heating
      if (upButton.pressed() && downButton.pressed()) {
        beginState(HEATING);
      }
      else if (upButton.pressed() ) {
        // increment max temperature unless at top
        if ( maxTempIndex < maxTempCount - 1) {
          maxTempIndex++;
          EEPROM.update(tempIndexAddr, maxTempIndex);
        }
      }
      else if ( downButton.pressed()) {
        //decrement max temperature unless at bottom
        if ( maxTempIndex > 0 ) {
          maxTempIndex--;
          EEPROM.update(tempIndexAddr, maxTempIndex);
        }
      }
      break;

    case HEATING:
      if ( millis() - heatStartTime >= heatTimeout ) {
        beginState( CANCEL_TIMEOUT );
        return;
      }

      if ( checkHeat() ) {
        // done heating so begin cool down
        beginState(COOL_DOWN);
        return;
      }

      // any button presses results in cancelation
      if (upButton.pressed() || downButton.pressed()) {
        beginState(CANCEL_PB);
      }
      break;

    case COOL_DOWN:
      if ( checkCoolDown() ) {
        // done cooling so all done
        beginState(COMPLETE);
        return;
      }

      // any button presses results in cancelation
      if (upButton.pressed() || downButton.pressed()) {
        beginState(COMPLETE);
      }
      break;

    case COMPLETE:
    case CANCEL_PB:
    case CANCEL_TIMEOUT:
      // any button presses returns to idle
      if (upButton.pressed() || downButton.pressed()) {
        beginState(IDLE);
      }
      break;
  }
}


void updateIdleDisplay() {
  const unsigned long interval = 1000;  // toggle every second
  static unsigned long lastTime;
  static bool state = true;

  if ( millis() - lastTime >= interval ) {
    //Change Display (left-side)
    if ( state ) {
      display.setCursor(3, 4);
      display.print(F("PRESS BUTTONS"));
      display.drawLine( 3, 12, 79, 12, SSD1306_WHITE);
      display.setCursor(3, 14);
      display.print(F(" Change  MAX"));
      display.setCursor(3, 22);
      display.print(F(" Temperature"));
    }
    else {
      display.setCursor(3, 4);
      display.print(F("HOLD  BUTTONS"));
      display.drawLine( 3, 12, 79, 12, SSD1306_WHITE );
      display.setCursor(3, 18);
      display.print(F("Begin Heating"));
    }
    state = !state;
  }

  //Update Display (right-side)
  display.setCursor(95, 6);
  display.print(F("TEMP"));
  display.setCursor(95, 18);
  display.print(maxTempArray[maxTempIndex]);
  display.print(F("C"));
  display.display();
}


bool checkHeat() {

  //Heater Control Variables
  /*  Heater follows industry reflow graph. Slow build-up to 'warmUp' temp. Rapid ascent
      to 'maxTemp'. Then descent to room temperature.
  */
  byte maxTemp = maxTempArray[maxTempIndex];
  byte maxPWM = 0.70 * maxTemp; //Temperatures (in PWM / 255) influenced by paste temperature
  byte warmUpTemp = 0.75 * maxTemp;
  byte warmUpPWM = 0.72 * warmUpTemp;
  float t; //Used to store current temperature
  float v; //Used to store current voltage
  byte pwmVal = 0; //PWM Value applied to MOSFET

  //Other control variables
  static int x = 0;  //Heat Animate Counter
  const int y = 80; //Heat Animate max (modulused below)


  //Measure Values
  t = getTemp();
  v = getVolts();

  //Reflow Profile
  if (t < warmUpTemp) { //Warm Up Section
    if (pwmVal != warmUpPWM) {
      pwmVal++;  //Slowly ramp to desired PWM Value
    }
    if (v < vMin && pwmVal > 1) {
      pwmVal = pwmVal - 2;  //Reduce PWM Value if V drops too low but not unless it is still above 1 (avoid overflow/underflow)
    }
  }
  else if (t < maxTemp) { //Push to maximum temp
    if (pwmVal != maxPWM) {
      pwmVal++;  //Slowly ramp to desired PWM Value
    }
    if (v < vMin && pwmVal > 1) {
      pwmVal = pwmVal - 2;  //Reduce PWM Value if V drops too low but not unless it is still above 1 (avoid overflow/underflow)
    }
  }
  else {
    //Heating Complete, return
    x = 0;  // reset for next time
    return true;
  }
  if (pwmVal > maxPWM ) {
    pwmVal = maxPWM;  //Catch incase of runaway
  }

  //MOSFET Control
  analogWrite(mosfet, pwmVal);

  //Heat Animate Control
  display.clearDisplay();
  display.drawBitmap( 0, 3, heat_animate, heat_animate_width, heat_animate_height, SSD1306_WHITE);
  display.drawBitmap( 112, 3, heat_animate, heat_animate_width, heat_animate_height, SSD1306_WHITE);
  display.fillRect( 0, 3, heat_animate_width, heat_animate_height * (y - x) / y, SSD1306_BLACK);
  display.fillRect( 112, 3, heat_animate_width, heat_animate_height * (y - x) / y, SSD1306_BLACK);
  x = ( x + 1 ) % y; //Heat animate increment and modulus

  //Update display
  display.setTextSize(2);
  display.setCursor(22, 4);
  display.print(F("HEATING"));
  display.setTextSize(1);
  display.setCursor(20, 24);
  display.print(F("~"));
  display.print(v, 1);
  display.print(F("V"));
  if ( t >= 100 )      {
    display.setCursor(78, 24);
  }
  else if ( t >= 10 ) {
    display.setCursor(81, 24);
  }
  else {
    display.setCursor(84, 24);
  }
  display.print(F("~"));
  display.print(t, 0);
  display.print(F("C"));
  display.display();
  return false;   // not finished yes
}


void cancelledTimeout() {

  const unsigned long interval = 1000;
  static unsigned long lastTime;
  static byte msg = 0;

  if ( millis() - lastTime >= interval ) {
    lastTime = millis();
    display.clearDisplay();
    display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
    display.setCursor(25, 4);
    display.print(F("  TIMED OUT"));
    display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );

    //Swap Main Text
    if ( msg == 0) {
      display.setCursor(25, 14);
      display.println(" Took longer");
      display.setCursor(25, 22);
      display.println(" than 5 mins");
    }
    else if ( msg == 1) {
      display.setCursor(28, 14);
      display.println("Try a higher");
      display.setCursor(25, 22);
      display.println(" current PSU");
    }
    else {
      display.setCursor(25, 14);
      display.println(" Push button");
      display.setCursor(25, 22);
      display.println("  to return");
    }
    msg = ( msg + 1 ) % 3;

    display.setTextSize(3);
    display.setCursor(5, 4);
    display.print(F("!"));
    display.setTextSize(3);
    display.setCursor(108, 4);
    display.print(F("!"));
    display.setTextSize(1);
    display.display();
  }
}

bool checkCoolDown() {

  float t = getTemp(); //Used to store current temperature

  if ( t < 45.0) {
    return true;  // all done
  }

  if ( t >= 100 ) {
    display.setCursor(49, 22);
  }
  else           {
    display.setCursor(52, 22);
  }
  display.print(F("~"));
  display.print(t, 0);
  display.print(F("C"));
  display.setTextSize(3);
  display.setCursor(5, 4);
  display.print(F("!"));
  display.setTextSize(3);
  display.setCursor(108, 4);
  display.print(F("!"));
  display.setTextSize(1);
  display.display();
  return false;
}


float getTemp() {
  unsigned long total = 0;
  for (byte i = 0; i < 100; i++) { //Poll temp reading 100 times
    total += analogRead(temp);
  }
  return ((total / 100.0) * -1.46) + 434; //Average, convert to C, and return
}

float getVolts() {
  unsigned long total = 0;
  for (byte i = 0; i < 20; i++) { //Poll Voltage reading 20 times
    total += analogRead(vcc);
  }
  return total / 20.0 / vConvert; //Average, convert to V, and return
}
