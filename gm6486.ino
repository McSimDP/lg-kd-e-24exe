// Тестировалось на Arduino IDE 1.0.6
const int clockPin = 18; // Пин синхронизации
const int latchPin = 3; // Пин "защелка"
const int dataPin = 17; // Пин для передачи данных

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  
  // В функцию Shiftout передается число, которое состоит и 8 бит.
  // Для двух модулей необходимо послать 16 бит.
  // Включим 3 и 11ый вывод.

  digitalWrite(latchPin, LOW);
  shiftOut33(dataPin, clockPin, B10000000, B00000000, B00000000, B00000001, B1);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}
void loop() 
{

  for (int i=0; i<8; i++){
    shiftOut33(dataPin, clockPin, 1<<i, 1<<i, 1<<i, 1<<i ,B0);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    delay (100);
  }

}

void shiftOut33(uint8_t dataPin, uint8_t clockPin, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool byte5)
{
        uint8_t i;
  
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
  digitalWrite(latchPin, LOW);
}

void shiftOut33_old(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, unsigned long byte1, byte byte2)
{
        uint8_t i;
  
        for (i = 0; i < 33; i++)  {
                digitalWrite(dataPin, !!(byte1 & (1 << (32-i))));
                digitalWrite(clockPin, HIGH);
                digitalWrite(clockPin, LOW);
        }
        digitalWrite(dataPin, !!(byte2 & 1));
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
}
