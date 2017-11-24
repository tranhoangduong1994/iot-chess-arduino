/* YourDuino.com Example Software Sketch
 20 character 4 line I2C Display
 Backpack Interface labelled "LCM1602 IIC  A0 A1 A2"
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
//none
/*-----( Declare objects )-----*/
// set the LCD address to 0x20 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


/*-----( Declare Variables )-----*/
#define MENU 2
#define UP 14
#define DOWN 15
#define OK 3

#define STOPPER_PIN_X 19
void NotEnable_X() ;


void onMenuPressed();
void onOKPressed();
void onUpPressed();
void onDownPressed();

void setup()
{
  // Configure stopper pin to read HIGH unless grounded
  pinMode(MENU , INPUT_PULLUP);//keo chan len + nguon
  pinMode(UP, INPUT_PULLUP);//keo chan len + nguon
  pinMode(DOWN, INPUT_PULLUP);//keo chan len + nguon
  pinMode(OK, INPUT_PULLUP);//keo chan len + nguon
//  attachInterrupt(0, NgatMenu, LOW);//ngat khi xay ra chuyen tu cao xuong thap
//  attachInterrupt(1,NgatOK, LOW);//ngat khi xay ra chuyen tu cao xuong thap
 // attachInterrupt(2, UP, FALLING);//ngat khi xay ra chuyen tu cao xuong thap
  //attachInterrupt(3, notEnable_X, FALLING);//ngat khi xay ra chuyen tu cao xuong thap
  
  pinMode(STOPPER_PIN_X, INPUT_PULLUP);
  Serial.begin(9600);  // Used to type in characters
  attachInterrupt(4, NotEnable_X, LOW);
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines and turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 2; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  
  
//-------- Write characters on the display ----------------
  lcd.setCursor(0,0);
  lcd.print("WCB version 1.0");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("By HUST WCB team");
  delay(1000);  
  lcd.setCursor(0,2);
  lcd.print("Booting up system...");
}/*--(end setup )---*/

const int CMD_BUFFER_LENGTH = 102;
const int STRING_LENGTH = 100;

const char SERVICE_PREFIX_CLEAR_SCREEN = '0';
const char SERVICE_PREFIX_PRINT = '1';
const char SERVICE_PREFIX_MOVE = '2';
const char SERVICE_PREFIX_CAPTURE = '3';
const char SERVICE_PREFIX_SCAN_BOARD = '4';
const char SERVICE_PREFIX_RESET_BOARD = '5';

const char MESSAGE_PREFIX_PLAYER_FINISHED_MOVE = '0';
const char MESSAGE_PREFIX_OPPONENT_FINISHED_MOVE = '1';
const char MESSAGE_PREFIX_KEY_PRESSED = '2';
const char MESSAGE_PREFIX_PLAYER_CHANGED_BOARD_STATE = '3';
const char MESSAGE_PREFIX_BOARD_SCANNED = '4';
const char MESSAGE_PREFIX_BOARD_RESETTED = '5';

boolean connected = false;

char commandBuffer[CMD_BUFFER_LENGTH];

void readMessage() {
  int i = 0;
  // read all the available characters
  while (Serial.available() > 0) {
    char character = (char)(Serial.read());
    if (character == '|') {
      break;
    }
    commandBuffer[i] = character;
    i++;
  }
}

void tryPerformingCommand() {
  if (commandBuffer[0] == SERVICE_PREFIX_CLEAR_SCREEN) {
    //clear screen
    lcd.clear();
  } else if (commandBuffer[0] == SERVICE_PREFIX_PRINT) {
    //print
    char line = commandBuffer[1] - 48;
    lcd.setCursor(0, line);
    char stringBuffer[STRING_LENGTH] = "";
    int i = 2;
    while(true) {
      char character = commandBuffer[i];
      if (character == 0) break;
      stringBuffer[i - 2] = character;
      i++;
    }
    lcd.print(stringBuffer);
  }
}

void clearCommandBuffer() {
  for (int i = 0; i < CMD_BUFFER_LENGTH; i++) {
    commandBuffer[i] = 0;
  }
}

boolean upPressed = false;
boolean downPressed = false;
boolean menuPressed = false;
boolean okPressed = false;

void readButtons() {
  if(digitalRead(DOWN) == LOW && !downPressed) {
    onDownPressed();
    downPressed = true;
  }
  
  if (digitalRead(DOWN) == HIGH) {
    downPressed = false;
  }
  
  if(digitalRead(UP) == LOW && !upPressed) {
    onUpPressed();
    upPressed = true;
  }
  
  if(digitalRead(UP) == HIGH) {
    upPressed = false;
  }
  
  if(digitalRead(MENU) == LOW && !menuPressed) {
    onMenuPressed();
    menuPressed = true;
  }
  
  if (digitalRead(MENU) == HIGH) {
    menuPressed = false;
  }
  
  if(digitalRead(OK) == LOW && !okPressed) {
    onOKPressed();
    okPressed = true;
  }
  
  if (digitalRead(OK) == HIGH) {
    okPressed = false;
  }
}

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{ 
  if (!Serial) {
    if (connected) {
      connected = false;
      onSerialDisconnected();
    }
    return;
  }
  connected = true;
  
  readButtons();
  
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);

    clearCommandBuffer();
    readMessage();
    tryPerformingCommand();
  }
}
void NotEnable_X() {
  Serial.println("hello Pi 3 \n");
  delay(100);
}

void onMenuPressed() {
  Serial.print("23|");
  Serial.flush();
}

void onOKPressed() {
  Serial.print("22|");
    Serial.flush();
}

void onUpPressed() {
  Serial.print("21|");
    Serial.flush();
}

void onDownPressed() {
  Serial.print("20|");
    Serial.flush();
}

void onSerialDisconnected() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System is restarting.");
  lcd.setCursor(0,1);
  lcd.print("Please wait...");
}
