// includes
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

// constants
// TODO: verify preferred speed for bluetooth/android
const int baudRate = 9600;  // bits per second
const int ledDelay = 1000; // 1 second
// LED output pins
// const int ledRed = 3;
// const int ledGreen = 5;
// const int ledBlue = 6;
const int ledPIN = 9;
const int ledMAX = 12;
// bluetooth pins
const int pinTx = 10;
const int pinRx = 11;
// led control array
//const int ledPins[] = {ledRed, ledGreen, ledBlue};

// colors
const byte BLACK[] = {0, 0, 0};
const byte PINK[] = {158, 4, 79};
const byte RED[] = {255, 0, 0};
const byte ORANGE[] = {255, 128, 0};
const byte YELLOW[] = {255, 255, 0};
const byte GREEN[] = {0, 255, 0};
const byte BLUE[] = {0, 0, 255};
const byte CYAN[] = {0, 255, 255};
const byte INDIGO[] = {4, 0, 19};
const byte VIOLET[] = {23, 0, 22};
const byte MAGENTA[] = {255, 0, 255};
const byte WHITE[] = {255, 255, 255};
const byte RAINBOW[] = {-1, -1, -1};

// variables
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledMAX , ledPIN, NEO_GRB + NEO_KHZ800);
boolean ledBlink = false;
boolean ledState = false;
byte ledColor[3] = {0, 0, 0};
String btCommand = "";
String rsCommand = "";
SoftwareSerial btModule(pinRx, pinTx);
SimpleTimer timer;

void setup() { 
  // init LED output pins
//  for (int idx=0; idx < 3; idx++) {
//    pinMode(ledPins[idx], OUTPUT);
//  }
//  setColor(BLACK);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // init bluetooth (for serial communications)
  Serial.begin(baudRate);
  Serial.println("Enter AT commands!");
  
  while (!Serial) {
    ;
  }
  
  btModule.begin(baudRate);
  btModule.println("Hello, world!");
  
  timer.setInterval(ledDelay, doBlink);
}

void loop() {
  // check bluetooth buffer
  if (btModule.available()) {
    char rxBuffer = (char)btModule.read();

    if (rxBuffer != '\n') {
      btCommand += rxBuffer;
      // debug output
      Serial.print(rxBuffer);
    } else {
      parseCommand(btCommand);
      btCommand = "";
      // debug output
      btModule.println("ACK");
      Serial.println("");
    }
  }

  // wait for at least two bytes before processing
  // rsCommand
  // AT...
  int iCnt = 0;
  int iBytes = Serial.available();
  while ((iBytes < 2) && (iBytes !=0)) {
    iCnt++;
    delay(10);
    iBytes = Serial.available();
    if (iCnt > 5)
      break;
  }
 if (iBytes != 0) {
    Serial.print("bytes recieved: ");
    Serial.println(iBytes, DEC);
  
    if (Serial.available()) {
        String txBuffer = "";
        while (Serial.available()) {
          txBuffer += (char)Serial.read();
        }
        Serial.println(txBuffer);
        Serial.println("ACK");
    }      
  }
  timer.run();
}

void parseCommand(String commInput) {
  String output;
  String param;
  
  int idx = commInput.indexOf(":");
  output = commInput.substring(0, idx);
  param = commInput.substring(idx + 1);
  param.replace("\r", "");
  param.replace("\n", "");
  
  if (output.equalsIgnoreCase("BLINK")) {
    Serial.print(" - BLINK:");
    if (param.equalsIgnoreCase("ON"))
      ledBlink = true;
    else
      ledBlink = false;
    Serial.println(param);
  }
  // LED command
  else if (output.equalsIgnoreCase("LED")) {
      Serial.print(" - LED:");
      if (param.equalsIgnoreCase("RED"))
        setColor(RED);
      else if (param.equalsIgnoreCase("BLUE"))
        setColor(BLUE);
      else if (param.equalsIgnoreCase("GREEN"))
        setColor(GREEN);
      else if (param.equalsIgnoreCase("CYAN"))
        setColor(CYAN);
      else if (param.equalsIgnoreCase("MAGENTA"))
        setColor(MAGENTA);
      else if (param.equalsIgnoreCase("INDIGO"))
        setColor(INDIGO);
      else if (param.equalsIgnoreCase("ORANGE"))
        setColor(ORANGE);
      else if (param.equalsIgnoreCase("VIOLET"))
        setColor(VIOLET);
      else if (param.equalsIgnoreCase("YELLOW"))
        setColor(YELLOW);
      else if (param.equalsIgnoreCase("PINK"))
        setColor(PINK);
      else if (param.equalsIgnoreCase("WHITE"))
        setColor(WHITE);
      else if (param.equalsIgnoreCase("BLACK"))
        setColor(BLACK);
      else if (param.equalsIgnoreCase("OFF"))
        setColor(BLACK);
      else if (param.equalsIgnoreCase("ON"))
        setColor(WHITE);      
      else if (param.equalsIgnoreCase("RAINBOW"))
        setColor(RAINBOW);
      else {
        Serial.print("unknown parameter ");
        /*
        char char_debug[param.length() + 1];
        char tmp[16];
        param.toCharArray(char_debug, param.length() + 1);
        for (int il = 0; il < param.length(); il++) {
          sprintf(tmp, "0x%.2X", char_debug[il]);
          Serial.print(tmp); Serial.print(" ");
        } 
 */       
        setColor(BLACK);
      }
      Serial.print("'");
      Serial.print(param);
      Serial.println("'.");
  }
  else if (output.equalsIgnoreCase("HELP")) {
    Serial.println("BLINK:[ON/OFF]");
    Serial.println("LED:[COLOR]");
    Serial.println("..BLUE");
    Serial.println("..GREEN");
    Serial.println("..INDIGO");    
    Serial.println("..MAGENTA");    
    Serial.println("..ORANGE");    
    Serial.println("..PINK");
    Serial.println("..RAINBOW");    
    Serial.println("..RED");
    Serial.println("..VIOLET");
    Serial.println("..YELLOW");
    
    btModule.println("BLINK:[ON/OFF]");
    btModule.println("LED:[COLOR]");
    btModule.println("..BLUE");
    btModule.println("..GREEN");
    btModule.println("..INDIGO");    
    btModule.println("..MAGENTA");    
    btModule.println("..ORANGE");    
    btModule.println("..PINK");
    btModule.println("..RAINBOW");    
    btModule.println("..RED");
    btModule.println("..VIOLET");
    btModule.println("..YELLOW");
  }
}

void setColor(const byte* color){
  if ( (color[0] == RAINBOW[0]) && (color[1] == RAINBOW[1]) && (color[2] == RAINBOW[2]) ) {
    strip.setPixelColor(6, RED[0], RED[1], RED[2]);
    strip.setPixelColor(5, ORANGE[0], ORANGE[1], ORANGE[2]);
    strip.setPixelColor(4, YELLOW[0], YELLOW[1], YELLOW[2]);
    strip.setPixelColor(3, GREEN[0], GREEN[1], GREEN[2]);
    strip.setPixelColor(2, BLUE[0], BLUE[1], BLUE[2]);
    strip.setPixelColor(1, INDIGO[0], INDIGO[1], INDIGO[2]);
    strip.setPixelColor(0, VIOLET[0], VIOLET[1], VIOLET[2]);
    
    ledColor[0] = RAINBOW[0];
    ledColor[1] = RAINBOW[1];
    ledColor[2] = RAINBOW[2];
  }
  else
  {
    for (int idx = 0; idx < ledMAX; idx++) {
      strip.setPixelColor(idx, color[0], color[1], color[2]);
    }
    ledColor[0] = color[0];
    ledColor[1] = color[1];
    ledColor[2] = color[2];
  }
  strip.show();
/*
  for (int idx = 0; idx < 3; idx++) {
    analogWrite(ledPins[idx], color[idx]);    
    ledColor[idx] = color[idx];
  }
*/  
  
  if (color == BLACK) {
    Serial.println("DEBUG - setColor:BLACK");
    ledState = false;
  } else {
    ledState = true;
  }
}

void doBlink() {
    if (ledBlink) {
      // if already off, then set color 
      if ( ledState ) {
        for (int idx = 0; idx < ledMAX; idx++) {
          strip.setPixelColor(idx, 0, 0, 0);
        }
        strip.show();        
        // set LEDs off
//        analogWrite(ledPins[0], 0);
//        analogWrite(ledPins[1], 0);
//        analogWrite(ledPins[2], 0);
        ledState = false;
      } else {
        setColor(ledColor);        
      }
    } else if (!ledState) {
      setColor(ledColor);
    }
}

// end program
