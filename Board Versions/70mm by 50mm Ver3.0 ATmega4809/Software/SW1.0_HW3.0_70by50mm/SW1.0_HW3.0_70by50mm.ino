 /* Solder Reflow Plate Sketch
 *  H/W - Ver 3.0
 *  S/W - Ver 1.0
 *  by Chris Halsall     */

/* To prepare
 * 1) Install MegaCoreX via Board Manager; See https://github.com/MCUdude/MegaCoreX
 * 2) Install Bounce2, Adafruit_GFX and Adafruit_SSD1306 libraries (Tools->Manage Libraries)
 * 3) Have a UPDI programmer. eg. Arduino Nano with JTAG2UPDI loaded - https://github.com/ElTangas/jtag2updi
 */

/* To program
 *  1) Select the following settings under (Tools)
 *      Board->MegaCoreX->Atmega4209 
 *      Clock->Internal 16MHz
 *      BOD->BOD 2.7V
 *      EEPROM->EEPROM retained
 *      Pinout->48 pin Standard
 *      Reset Pin->Reset
 *      Bootloader->No bootloader
 *  2) Set programmer of choice, e.g.'JTAG2UPDI', 'Atmel ICE', etc, and set correct port.
 *  4) Compile and upload
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <Bounce2.h>

//Version Definitions
static const PROGMEM float hw = 3.0;
static const PROGMEM float sw = 1.0;

//Screen Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C   //I2C Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //Create Display

//Pin Definitions
#define mosfet PIN_PC3
#define upsw PIN_PD1
#define dnsw PIN_PD0
#define temp PIN_PF5 
#define vcc PIN_PF3

Bounce2::Button upButton;
Bounce2::Button downButton;

//Temperature Info
byte maxTempArray[] = { 140, 150, 160, 170, 180, 190, 200, 210, 220 };
byte maxTempIndex = 0;
byte tempIndexAddr = 1;

//Voltage Measurement Info
float vConvert = 52.00;
float vMin = 10.50;

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

void setup() {
  
  //Pin Direction control
  pinMode(mosfet,OUTPUT);
  digitalWrite(mosfet,LOW);
  pinMode(temp,INPUT);
  pinMode(vcc,INPUT);

  upButton.attach(upsw);
  upButton.setPressedState(LOW);
  upButton.interval(10);

  downButton.attach(dnsw);
  downButton.setPressedState(LOW);
  downButton.interval(10);

  //Pull saved values from EEPROM
  maxTempIndex = EEPROM.read(tempIndexAddr) % sizeof(maxTempArray);

  //Enable Fast PWM with no prescaler
  //TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  //TCCR2B = _BV(CS20);

  //Start-up Diplay
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.drawBitmap(0, 0, logo, logo_width, logo_height, SSD1306_WHITE);
  display.setCursor(80,16);
  display.print(F("S/W V"));
  display.print(sw, 1);
  display.setCursor(80,24);
  display.print(F("H/W V"));
  display.print(hw, 1);
  display.display();
  delay(3000);

  //Go to main menu
  main_menu();
}

void main_menu() {
  
  int x = 0;  //Display change counter
  int y = 200; //Display change max (modulused below)
  while(1) {
    upButton.update();
    downButton.update();
    analogWrite(mosfet,0); //Ensure MOSFET off
    display.clearDisplay();
    display.setTextSize(1);
    display.drawRoundRect( 0, 0, 83, 32, 2, SSD1306_WHITE);

    //Button Logic
    if(downButton.pressed() || upButton.pressed()) { //If either button pressed
      delay(100);
      //upButton.update();
      //downButton.update();
      if(downButton.pressed() && upButton.pressed()) { //If both buttons pressed
        if(!heat(maxTempArray[maxTempIndex])) {
          cancelledPB();
          main_menu();
        }
        else {
          coolDown();
          completed();
          main_menu();
        }
      }
      if(upButton.pressed() && maxTempIndex < sizeof(maxTempArray) - 1) { //If upper button pressed
        maxTempIndex++;
        EEPROM.update(tempIndexAddr, maxTempIndex);
      }
      if(downButton.pressed() && maxTempIndex > 0) { //If lower button pressed
        maxTempIndex--;
        EEPROM.update(tempIndexAddr, maxTempIndex);
      }
    }

    //Change Display (left-side)
    if( x < (y * 0.5)) {
      display.setCursor(3,4);
      display.print(F("PRESS BUTTONS"));
      display.drawLine( 3, 12, 79, 12, SSD1306_WHITE); 
      display.setCursor(3,14);
      display.print(F(" Change  MAX"));
      display.setCursor(3,22);
      display.print(F(" Temperature"));
    }
    else {
      display.setCursor(3,4);
      display.print(F("HOLD  BUTTONS"));
      display.drawLine( 3, 12, 79, 12, SSD1306_WHITE ); 
      display.setCursor(3,18);
      display.print(F("Begin Heating"));
    }
    x = ( x + 1 ) % y; //Display change increment and modulus
    
    //Update Display (right-side)
    display.setCursor(95,6);
    display.print(F("TEMP"));
    display.setCursor(95,18);
    display.print(maxTempArray[maxTempIndex]);
    display.print(F("C"));
    display.display();
  }
}

bool heat(byte maxTemp) {
  //Heating Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(22,4);
  display.print(F("HEATING"));
  display.setTextSize(1);
  display.setCursor(52,24);
  display.print(maxTemp);
  display.print(F("C"));
  display.display();
  delay(3000);

  //Heater Control Variables
  /*  Heater follows industry reflow graph. Slow build-up to 'warmUp' temp. Rapid ascent
   *  to 'maxTemp'. Then descent to room temperature. 
   */
  //byte maxTemp; //Declared in function call
  byte maxPWM = 0.70 * maxTemp; //Temperatures (in PWM / 255) influenced by paste temperature
  byte warmUpTemp = 0.75 * maxTemp;
  byte warmUpPWM = 0.72 * warmUpTemp;
  float t; //Used to store current temperature
  float v; //Used to store current voltage
  byte pwmVal = 0; //PWM Value applied to MOSFET
  unsigned long eTime = (millis() / 1000) + (8*60); //Used to store the end time of the heating process, limited to 8 mins

  //Other control variables
  int x = 0;  //Heat Animate Counter
  int y = 80; //Heat Animate max (modulused below)
  
  while(1) {

    upButton.update();
    downButton.update();
    
    //Button Control
    if(downButton.pressed() || upButton.pressed()) {
      analogWrite(mosfet, 0);
      return 0;
    }

    //Check Heating not taken more than 8 minutes
    if(millis() / 1000 > eTime) {
      analogWrite(mosfet, 0);
      cancelledTimer();
    }

    //Measure Values
    t = getTemp();
    v = getVolts();

    //Reflow Profile
    if (t < warmUpTemp) { //Warm Up Section
      if (pwmVal != warmUpPWM) { pwmVal++; } //Slowly ramp to desired PWM Value
      if (v < vMin && pwmVal > 1) { pwmVal = pwmVal - 2; } //Reduce PWM Value if V drops too low but not unless it is still above 1 (avoid overflow/underflow)
    }
    else if (t < maxTemp) { //Push to maximum temp
      if (pwmVal != maxPWM) { pwmVal++; } //Slowly ramp to desired PWM Value
      if (v < vMin && pwmVal > 1) { pwmVal = pwmVal - 2; } //Reduce PWM Value if V drops too low but not unless it is still above 1 (avoid overflow/underflow)
    }
    else { //Heating Complete, return
      analogWrite(mosfet, 0);
      break;
    }
    if (pwmVal > maxPWM ) { pwmVal = maxPWM; } //Catch incase of runaway 

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
    display.setCursor(22,4);
    display.print(F("HEATING"));
    display.setTextSize(1);
    display.setCursor(20,24);
    display.print(F("~"));
    display.print(v,1);
    display.print(F("V"));
    if( t >= 100 )      { display.setCursor(78,24); }
    else if ( t >= 10 ) { display.setCursor(81,24); }
    else                { display.setCursor(84,24); }
    display.print(F("~"));
    display.print(t,0);
    display.print(F("C"));
    display.display();
  }
}

void cancelledPB() { //Cancelled via push button

  //Update Display
  display.clearDisplay();
  display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
  display.setCursor(25,4);
  display.print(F("  CANCELLED"));
  display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );
  display.setCursor(25,14);
  display.println(" Push button");
  display.setCursor(25,22);
  display.println("  to return");
  display.setTextSize(3);
  display.setCursor(5,4);
  display.print(F("!"));
  display.setTextSize(3);
  display.setCursor(108,4);
  display.print(F("!"));
  display.setTextSize(1);
  display.display();
  delay(50);

  //Wait to return on any button press
  while(!downButton.pressed() && !upButton.pressed()) { 
    upButton.update();
    downButton.update(); 
  }
}

void cancelledTimer() { //Cancelled via 5 minute Time Limit

  //Initiate Swap Display
  int x = 0;  //Display change counter
  int y = 150; //Display change max (modulused below)

  //Wait to return on any button press
  while(!downButton.pressed() || !downButton.pressed()) {
    upButton.update();
    downButton.update();
    //Update Display
    display.clearDisplay();
    display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
    display.setCursor(25,4);
    display.print(F("  TIMED OUT"));
    display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );

    //Swap Main Text
    if( x < (y * 0.3)) {
      display.setCursor(25,14);
      display.println(" Took longer");
      display.setCursor(25,22);
      display.println(" than 5 mins");
    }
    else if( x < (y * 0.6)) {
      display.setCursor(28,14);
      display.println("Try a higher");
      display.setCursor(25,22);
      display.println(" current PSU");
    }
    else {
      display.setCursor(25,14);
      display.println(" Push button");
      display.setCursor(25,22);
      display.println("  to return");
    }
    x = ( x + 1 ) % y; //Display change increment and modulus
    
    display.setTextSize(3);
    display.setCursor(5,4);
    display.print(F("!"));
    display.setTextSize(3);
    display.setCursor(108,4);
    display.print(F("!"));
    display.setTextSize(1);
    display.display();
    delay(50);
  }
  main_menu();
}

void coolDown() {
  
  float t = getTemp(); //Used to store current temperature
  
  //Wait to return on any button press, or temp below threshold
  while(!downButton.pressed() || !downButton.pressed() && t > 45.00) {
    upButton.update();
    downButton.update();
    display.clearDisplay();
    display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
    display.setCursor(25,4);
    display.print(F("  COOL DOWN"));
    display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );
    display.setCursor(25,14);
    display.println("  Still Hot");
    t = getTemp();
      if( t >= 100 ) { display.setCursor(49,22); }
      else           { display.setCursor(52,22); }
      display.print(F("~"));
      display.print(t,0);
      display.print(F("C"));
    display.setTextSize(3);
    display.setCursor(5,4);
    display.print(F("!"));
    display.setTextSize(3);
    display.setCursor(108,4);
    display.print(F("!"));
    display.setTextSize(1);
    display.display();
  }
}

void completed() {
  //Update Display
  display.clearDisplay();
  display.drawRoundRect( 22, 0, 84, 32, 2, SSD1306_WHITE );
  display.setCursor(25,4);
  display.print(F("  COMPLETED  "));
  display.drawLine( 25, 12, 103, 12, SSD1306_WHITE );
  display.setCursor(25,14);
  display.println(" Push button");
  display.setCursor(25,22);
  display.println("  to return");
  display.drawBitmap( 0, 9, tick, tick_width, tick_height, SSD1306_WHITE);
  display.drawBitmap( 112, 9, tick, tick_width, tick_height, SSD1306_WHITE);
  display.display();

  //Wait to return on any button press
  while(downButton.pressed() || upButton.pressed()) {
    upButton.update();
    downButton.update();
  }
}

float getTemp(){
  float t = 0;
  for (byte i = 0; i < 100; i++){ //Poll temp reading 100 times
    t = t + analogRead(temp);
  }
  return ((t / 100) * -1.46) + 434; //Average, convert to C, and return
}

float getVolts(){
  float v = 0;
  for (byte i = 0; i < 20; i++){ //Poll Voltage reading 20 times
    v = v + analogRead(vcc);
  }
  return v / 20 / vConvert; //Average, convert to V, and return
}

void loop() {
  // Not used
}
