/*
Назначение выводов платы KBU OF KD/E-25/37
1:  LCD contrast
2:  HC148.EI
3: GM6486.LOAD
4:  LCD RS
5:  LCD Enable
6:  HC148.EO  
7:  LCD Data0; HC148.A0
8:  LCD Data1; HC148.A1
9:  LCD Data2; HC148.A2
10: LCD Data3; HC138A.A0; GM6486.DATA IN
11: LCD Data4; HC138A.A1; GM6486.CLOCK IN
12: LCD Data5; HC138A.A2
13: LCD Data6
14: LCD Data7
15: GND
16: +5v
*/
// Подключаем необходимые библиотеки для работы с прерываниями, DF Player Mini

#include <MsTimer2.h>
#include <LiquidCrystal.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
//  Описание пинов
//  GM6486:
const int latchPin = 3; // Пин "защелка"
const int clockPin = 18; // Пин синхронизации
const int dataPin = 17; // Пин для передачи данных
//  LCD:
const int v0 = 10;
const int rs = 11;
const int en = 12;
const int d4 = 18;
const int d5 = 19;
const int d6 = 8;
const int d7 = 9;
//DF
const int busyDF=13;
const int dfTX = 4;
const int dfRX = 5;

//Задаюся константы соответствия кнопки и её "коду"
const int BTN_0=54;
const int BTN_1=47;
const int BTN_2=57;
const int BTN_3=67;
const int BTN_4=46;
const int BTN_5=56;
const int BTN_6=66;
const int BTN_7=45;
const int BTN_8=55;
const int BTN_9=65;
const int BTN_ASTER=44;
const int BTN_SHARP=64;
const int BTN_REDIAL=62;
const int BTN_PGM=53;
const int BTN_VOLUP=61;
const int BTN_VOLDOWN=60;
const int BTN_ICM=77;
const int BTN_MON=50;
const int BTN_HOLD=63;
const int BTN_CBK=42;
const int BTN_CONF=52;
const int BTN_DND=41;
const int BTN_MUTE=40;
const int BTN_SPD=43;
const int BTN_FLASH=51;
const int BTN_FN01=17;
const int BTN_FN02=16;
const int BTN_FN03=15;
const int BTN_FN04=14;
const int BTN_FN05=13;
const int BTN_FN06=12;
const int BTN_FN07=11;
const int BTN_FN08=10;
const int BTN_FN09=27;
const int BTN_FN10=26;
const int BTN_FN11=25;
const int BTN_FN12=24;
const int BTN_FN13=23;
const int BTN_FN14=22;
const int BTN_FN15=21;
const int BTN_FN16=20;
const int BTN_FN17=37;
const int BTN_FN18=36;
const int BTN_FN19=35;
const int BTN_FN20=34;
const int BTN_FN21=33;
const int BTN_FN22=32;
const int BTN_FN23=31;
const int BTN_FN24=30;

int keycode = 0;
int keycodeold = 0;
int hc138 = 1;
int pin = 0; // какой пин будет установлен из 33
int f=1;  //  Установка начальных сигналов на MC74HC138A
int t=7;  //  Установка конечных сигналов на MC74HC138A
byte chartst =0;
int k;
int volume=5;
int oldvolume;
int song = 0; // Набранная на клавиатуре песня
bool keypressed = HIGH;
bool keyoldstate = LOW;
bool loopsingle = false;
bool loopall = false;
//#define _DEBUG    //Для отладки

SoftwareSerial mySoftwareSerial(dfTX,dfRX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void printDetail(uint8_t type, int value);

void setup() {
  // GM6486
  pinMode ( latchPin, OUTPUT);
  digitalWrite( latchPin, LOW);
/*          digitalWrite(dataPin, LOW);                  
        for (uint8_t i = 0; i < 33; i++)  {
                digitalWrite(clockPin,HIGH);
                digitalWrite(clockPin, LOW);
        }
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
*/        
//setPin(dataPin, clockPin,0);
//  resetGM(dataPin, clockPin, latchPin);
//  delay(2000);
  // LCD
  pinMode(v0,OUTPUT);
  analogWrite(v0,0);
  lcd.begin(24, 2);
  lcd.print(F("Initializing Phone...")); 
// DF
  pinMode ( busyDF, INPUT);  
  mySoftwareSerial.begin(9600);
  // put your setup code here, to run once:
  DDRC = B111000;
  PORTC = hc138<<3;
  pinMode(2, INPUT_PULLUP); 
#ifdef _DEBUG
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
#endif
  
  if (!myDFPlayer.begin(mySoftwareSerial,false)) {  //Use softwareSerial to communicate with mp3.
#ifdef _DEBUG
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
#endif
    while(true);
  }
#ifdef _DEBUG
  Serial.println(F("DFPlayer Mini online."));
#endif
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(volume);  //Set volume value (0~30).
//  myDFPlayer.volumeUp(); //Volume Up
//  myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  Serial.println(myDFPlayer.readVolume());
  MsTimer2::set(100, timerInterupt); // задаем период прерывания по таймеру 60 мс
  MsTimer2::start();               // разрешаем прерывание по таймеру
}

void loop() {
  if (myDFPlayer.available()) {
//    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }  

//  delay(1000);
//  Serial.println(myDFPlayer.readVolume());
}

void  timerInterupt() {
#ifdef _DEBUG
  Serial.println(~PINC & B111);
#endif
  if (digitalRead(busyDF)){
    lcd.clear();
  }
  for (k=f;k<=t;k++) {
    PORTC=k<<3;
    keypressed=digitalRead(2);
    if (keypressed && ((keycode=(~PINC & B111)+(k*10))!=74)) {
        if (!keyoldstate){
          keyoldstate=HIGH;          
#ifdef _DEBUG
        Serial.print("Keycode: ");
        Serial.println(keycode);
#endif        
        switch(keycode) {
          case BTN_MUTE:
            myDFPlayer.volume(0);  
            break;
          case BTN_VOLUP:
            myDFPlayer.volumeUp();  
            break;
          case BTN_VOLDOWN:
            myDFPlayer.volumeDown();  
            break;
          case BTN_ICM:
            myDFPlayer.previous();  
            break;
          case BTN_MON:
            if (digitalRead(busyDF)){
              myDFPlayer.start();
            } else {
              myDFPlayer.pause();
            }
            break;
          case BTN_HOLD:
            myDFPlayer.next();  
            break;
          case BTN_FN01:
            myDFPlayer.loopFolder(1);  
            break;
          case BTN_FN02:
            myDFPlayer.loopFolder(2);  
            break;
          case BTN_FN03:
            myDFPlayer.loopFolder(3);  
            break;
          case BTN_FN04:
            myDFPlayer.loopFolder(4);  
            break;
          case BTN_FN05:
            myDFPlayer.loopFolder(5);  
            break;
          case BTN_FN06:
            myDFPlayer.loopFolder(6);  
            break;
          case BTN_FN07:
            myDFPlayer.loopFolder(7);  
            break;
          case BTN_FN08:
            myDFPlayer.loopFolder(8);  
            break;
          case BTN_FN09:
            myDFPlayer.loopFolder(9);  
            break;
          case BTN_FN10:
            myDFPlayer.loopFolder(10);  
            break;
          case BTN_FN11:
            myDFPlayer.loopFolder(11);  
            break;
          case BTN_FN12:
            myDFPlayer.loopFolder(12);  
            break;
          case BTN_FN13:
            myDFPlayer.playMp3Folder(2);  
            break;
          case BTN_FN14:
            myDFPlayer.playMp3Folder(4);  
            break;
          case BTN_FN15:
            myDFPlayer.playMp3Folder(6);  
            break;
          case BTN_FN16:
            myDFPlayer.playMp3Folder(8);  
            break;
          case BTN_FN17:
            myDFPlayer.playMp3Folder(10);  
            break;
          case BTN_FN18:
            myDFPlayer.playMp3Folder(12);  
            break;
          case BTN_FN19:
            myDFPlayer.playMp3Folder(20);  
            break;
          case BTN_FN20:
            myDFPlayer.playMp3Folder(38);  
            break;
          case BTN_FN21:
            myDFPlayer.playMp3Folder(48);  
            break;
          case BTN_FN22:
            myDFPlayer.playMp3Folder(56);  
            break;
          case BTN_FN23:
            myDFPlayer.playMp3Folder(60);  
            break;
          case BTN_FN24:
            myDFPlayer.playMp3Folder(65);  
            break;
          case BTN_CBK:
            if (loopsingle) {
              myDFPlayer.disableLoop();  
              loopsingle=false;
            }
            else {
              myDFPlayer.enableLoop();  
              loopsingle=true;
            }
            break;
          case BTN_CONF:
            if (loopall) {
              myDFPlayer.disableLoopAll();  
              loopall=false;
            }
            else {
              myDFPlayer.enableLoopAll();  
              loopall=true;
            }
            break;
          case BTN_SPD:
            MsTimer2::stop();
            digitalWrite(latchPin, HIGH);
            digitalWrite(latchPin, LOW);
            for (int i=0; i<8; i++){
              shiftOut33(dataPin, clockPin, 1<<i, 1<<i, 1<<i, 1<<i ,B0);
              delay (30000);
            }
              resetGM(dataPin, clockPin, latchPin);
              digitalWrite(latchPin, HIGH);
              digitalWrite(latchPin, LOW);
            MsTimer2::start();
            break;
          case BTN_FLASH:
            MsTimer2::stop();
            if(pin>33){
              pin=0;
            }
            setPin(dataPin, clockPin,pin++);
            MsTimer2::start();
            break;
          case BTN_REDIAL:
            lcd.clear();
            lcd.write(chartst);
            chartst++;
            break;
          case BTN_PGM:
            lcd.clear();
            lcd.print(String(digitalRead(busyDF)));
            break;
          case BTN_1:
            if ((song!=0) &&(song<10)) {
              song=song*10+1;
            } else {
              song=1;
            }
              lcd.setCursor(0, 1);
              lcd.print(String(song));
            break;
          case BTN_2:
            if ((song!=0) &&(song<10)) {
              song=song*10+2;
            } else {
              song=2;
            }
              lcd.setCursor(0, 1);
              lcd.print(String(song));
            break;
          case BTN_3:
            if ((song!=0) &&(song<10)) {
              song=song*10+3;
            } else {
              song=3;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_4:
            if ((song!=0) &&(song<10)) {
              song=song*10+4;
            } else {
              song=4;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_5:
            if ((song!=0) &&(song<10)) {
              song=song*10+5;
            } else {
              song=5;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_6:
            if ((song!=0) &&(song<10)) {
              song=song*10+6;
            } else {
              song=6;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_7:
            if ((song!=0) &&(song<10)) {
              song=song*10+7;
            } else {
              song=7;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_8:
            if ((song!=0) &&(song<10)) {
              song=song*10+8;
            } else {
              song=8;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_9:
            if ((song!=0) &&(song<10)) {
              song=song*10+9;
            } else {
              song=9;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_0:
            if ((song!=0) &&(song<10)) {
              song=song*10;
            }
            lcd.setCursor(0, 1);
            lcd.print(String(song));
            break;
          case BTN_SHARP:
            if (song!=0){
              lcd.setCursor(0, 1);
              lcd.print(String(song)+" track is playing now");
              myDFPlayer.playMp3Folder(song);  
            }
            break;            
        }
        f=t=k;          
        break;
      }
    }
    else if (keyoldstate) {
      keyoldstate = LOW;
      f=1;
      t=7;
    }
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}      

void shiftOut33(uint8_t dataPin, uint8_t clockPin, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool byte5)
{
        uint8_t i;
          digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);

        for (i = 0; i < 8; i++)  {
                digitalWrite(clockPin, HIGH);
                digitalWrite(dataPin, !!(byte1 & (1 << (7-i))));
                digitalWrite(clockPin, LOW);
        }
        for (i = 0; i < 8; i++)  {
                digitalWrite(clockPin, HIGH);
                digitalWrite(dataPin, !!(byte2 & (1 << (7-i))));
                digitalWrite(clockPin, LOW);
        }
        for (i = 0; i < 8; i++)  {
                digitalWrite(clockPin, HIGH);
                digitalWrite(dataPin, !!(byte3 & (1 << (7-i))));
                digitalWrite(clockPin, LOW);
        }
        for (i = 0; i < 8; i++)  {
                digitalWrite(clockPin, HIGH);
                digitalWrite(dataPin, !!(byte4 & (1 << (7-i))));
                digitalWrite(clockPin, LOW);
        }
        digitalWrite(clockPin, HIGH);
        digitalWrite(dataPin, !!(byte5 & 1));
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
}

void setPin(uint8_t dataPin, uint8_t clockPin, uint8_t byte1)
{
        uint8_t i;
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
        for (i = 1; i < 34; i++)  {
#ifdef _DEBUG
        Serial.print("i: ");
        Serial.println(i);
#endif        
                digitalWrite(clockPin,HIGH);
                if (i==byte1){
                  digitalWrite(dataPin, HIGH);
                } else{
                  digitalWrite(dataPin, LOW);                  
                }
                digitalWrite(clockPin, LOW);
        }
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
}


void resetGM(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin)
{
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
        digitalWrite(dataPin, LOW);                  
        for (uint8_t i = 1; i < 34; i++)  {
                digitalWrite(clockPin,HIGH);
                digitalWrite(clockPin, LOW);
        }
        digitalWrite(latchPin, HIGH);
        digitalWrite(clockPin, HIGH);
        digitalWrite(latchPin, LOW);
        digitalWrite(clockPin, LOW);
}
