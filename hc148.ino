#include <MsTimer2.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

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
int f=1;  //  Установка начальных сигналов на MC74HC138A
int t=7;  //  Установка конечных сигналов на MC74HC138A
int k;
int volume=5;
int oldvolume;
bool keypressed = HIGH;
bool keyoldstate = LOW;
bool loopsingle = false;
bool loopall = false;
bool playing = false;
//#define _DEBUG

SoftwareSerial mySoftwareSerial(4,5); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  // put your setup code here, to run once:
  DDRC = B111000;
  PORTC = hc138<<3;
  pinMode(2, INPUT_PULLUP); 
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial,false)) {  //Use softwareSerial to communicate with mp3.
//  if (!myDFPlayer.begin(Serial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
#ifdef _DEBUG
            Serial.print("volume=");
            Serial.println(volume);
#endif  
delay (1000);
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
  MsTimer2::set(60, timerInterupt); // задаем период прерывания по таймеру 60 мс
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
  for (k=f;k<=t;k++) {
    PORTC=k<<3;
    keypressed=digitalRead(2);
    if (keypressed && ((keycode=(~PINC & B111)+(k*10))!=74)) {
        if (!keyoldstate){
//        keycode=(~PINC & B111)+(k*10);
//        if (keycode!=74){
          keyoldstate=HIGH;          
//        }
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
            if (playing){
              myDFPlayer.pause();
              playing=false;
            } else {
              myDFPlayer.start();
              playing=true;
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
            myDFPlayer.start();  
            break;
          case BTN_FLASH:
            myDFPlayer.randomAll();
            break;
          case BTN_1:
            myDFPlayer.randomAll();
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
