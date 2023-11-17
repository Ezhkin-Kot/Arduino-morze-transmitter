#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

bool Fr=0;
bool Fl=0;
bool Fm=0;
unsigned long lt = 0;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("start");

  oled.init();
  oled.clear();
  oled.setScale(1);
  oled.setCursor(0,2);
}

String wrd;
char abc[33]={'_','А','Б','В','Г','Д','Е','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Х','Ц','Ч','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я'};
int a=0;

void loop() {
  if (millis()-lt>=100){
    // right button
    if (!Fr && !digitalRead(2)){
      Fr=1;
      a = a+1;
      Serial.println(a);
      oled.setCursor(0,2);
      oled.print("next: ");
      oled.print(abc[a]);
      oled.setCursor(0,4);
      oled.print("number: ");
      oled.clear(38,35,127,60);
      oled.print(a+1);
    }
    if (Fr && digitalRead(2)){
      Fr=0;
    }

    // left button
    if (!Fl && !digitalRead(3)){
      Fl=1;
      a = a-1;
      Serial.println(a);
      oled.setCursor(0,2);
      oled.print("next: ");
      oled.print(abc[a]);
      oled.setCursor(0,4);
      oled.print("number: ");
      oled.clear(38,35,127,60);
      oled.print(a+1);
    }
    if (Fl && digitalRead(3)){
      Fl=0;
    }
    
    // mid button
    if (!Fm && !digitalRead(4)){
      Fm=1;
      wrd += abc[a];
      oled.home();
      oled.print(wrd);
    }
    if (Fm && digitalRead(4)){
      Fm=0;
    }
    if ((Fl && Fr) == 1){
      wrd.remove(a);
      oled.home();
      oled.print(wrd);
    }
    lt=millis();
  }
}