#include <GyverOLED.h>
#define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
#define EB_NO_CALLBACK      // отключить обработчик событий attach (экономит 2 байта оперативки)
#define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки)
#define EB_NO_BUFFER        // отключить буферизацию энкодера (экономит 1 байт оперативки)
#include <EncButton.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
EncButton eb(2, 3, 4);      //назначение выводов энкодера

void LightOn(unsigned short seconds) {
  digitalWrite(5, HIGH);
  delay(seconds * 1000);
  digitalWrite(5, LOW);
}

void setup() {
  eb.setEncReverse(0);
  eb.setEncType(EB_STEP4_LOW); //тип энкодера
  // инициализация дисплея
  oled.init();
  oled.clear();
  oled.setScale(1);
  oled.setCursor(0,2);
  oled.print("next: ");
  oled.setCursor(0,4);
  oled.print("mrz: ");
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  pinMode(12, INPUT_PULLUP);
}

String wrd;
String wmrz;
//Задаём алфавит, откуда будем брать символы
const char abc[43] = {'0','1','2','3','4','5','6','7','8','9','_','А','Б','В','Г','Д','Е','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Х','Ц','Ч','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я'};
//Здоровенный массив с азбукой Морзе
const char *mrz[] = {"11111 ","01111 ","00111 ","00011 ","00001 ","00000 ","10000 ","11000 ","11100 ","11110 ","  ","01 ","1000 ","011 ","110 ","100 ","0 ","0001 ","1100 ","00 ","0111 ","101 ","0100 ","11 ","10 ","111 ","0110 ","010 ","000 ","1 ","001 ","0010 ","0000 ","1010 ","1110 ","1111 ","1101 ","011010 ","1011 ","1001 ","00100 ","0011 ","0101 "};
int n = 0;
unsigned int count = 0;
char s;
const char DOT = '0';
const char DASH = '1';
const char SPACE = ' ';

//Функции-преобразователи
void abc_to_mrz() {
  for (auto i : wmrz){
    switch (*i)
    {
    case DASH:
      LightOn(seconds: 1.5);
      break;
    case DOT:
      LightOn(seconds: 3);
      break;
    case SPACE:
      delay(5000);
      break;
    default:
      break; 
    }
      delay(500);
  }
}
void loop() {
  eb.tick();

  //обработка поворота энкодера (выбор символа)
  if (eb.turn()){
    if (eb.pressing()) {
      n += 5 * eb.dir(); //быстрое пролистывание
    }
    else {n += eb.dir();}
    oled.setCursor(0, 2);
    oled.print("next: ");
    oled.print(abc[n]);
    oled.setCursor(0, 4);
    oled.print("mrz: ");
    oled.clear(38, 35, 127, 60);
    oled.print(mrz[n]);
    }
  //обработка клика (печать символа)
  if (eb.click()){
    wrd += abc[n];
    wmrz += mrz[n];
    count++;
    oled.home();
    oled.print(wrd);
    Serial.println(wmrz);
  }
  //обработка удержания (удаление символа)
  if (eb.hold()){
    wrd.remove(count-1, 1);
    wmrz.remove(count-1, 1);
    count--;
    oled.clear(0, 0, 127, 10);
    oled.home();
    oled.print(wrd);
  }
  if (digitalRead(12) != 1) {
    abc_to_mrz();
    wrd = "";
    wmrz = "";
    oled.clear(0, 0, 127, 10);
    oled.home();
    oled.print(wrd);
  }
}
