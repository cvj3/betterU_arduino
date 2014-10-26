// includes
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

// constants
// TODO: verify preferred speed for bluetooth/android
const int baudRate = 9600;  // bits per second
const int ledDelay = 1000; // 1 second
// LED output pins
const int ledRed = 3;
const int ledGreen = 4;
const int ledBlue = 5;
// bluetooth pins
const int pinTx = 10;
const int pinRx = 11;
// led control array
const int ledPins[] = {ledRed, ledGreen, ledBlue};

// colors
const byte WHITE[] = {255, 255, 255};
const byte PINK[] = {158, 4, 79};
const byte RED[] = {255, 0, 0};
const byte ORANGE[] = {83, 4, 0};
const byte YELLOW[] = {255, 255, 0};
const byte GREEN[] = {0, 255, 0};
const byte BLUE[] = {0, 0, 255};
const byte CYAN[] = {0, 255, 255};
const byte INDIGO[] = {4, 0, 19};
const byte VIOLET[] = {23, 0, 22};
const byte MAGENTA[] = {255, 0, 255};
const byte BLACK[] = {0, 0, 0};

// variables
boolean ledBlink = false;
boolean ledState = false;
byte ledColor[3] = {0, 0, 0};
String btCommand = "";
String rsCommand = "";
SoftwareSerial btModule(pinRx, pinTx);
SimpleTimer timer;

void setup() { 
  // init LED output pins
  for (int idx=0; idx < 3; idx++) {
    pinMode(ledPins[idx], OUTPUT);
  }
  setColor(BLACK);
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
}

void setColor(const byte* color){
  for (int idx = 0; idx < 3; idx++) {
    analogWrite(ledPins[idx], color[idx]);    
    ledColor[idx] = color[idx];
  }
  
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
        // set LEDs off
        analogWrite(ledPins[0], 0);
        analogWrite(ledPins[1], 0);
        analogWrite(ledPins[2], 0);
        ledState = false;
      } else {
        setColor(ledColor);        
      }
    } else if (!ledState) {
      setColor(ledColor);
    }
}

// end program
