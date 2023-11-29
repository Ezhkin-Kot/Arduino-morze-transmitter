#include <GyverOLED.h>
#define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
#define EB_NO_CALLBACK      // отключить обработчик событий attach (экономит 2 байта оперативки)
#define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки)
#define EB_NO_BUFFER        // отключить буферизацию энкодера (экономит 1 байт оперативки)
#include <EncButton.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
EncButton eb(2, 3, 4);      //назначение выводов энкодера

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
  oled.print("number: ");
}

String wrd;
char abc[33] = {'_','А','Б','В','Г','Д','Е','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Х','Ц','Ч','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я'};
int a = 0;
unsigned int count = 0;
char s;

void loop() {
  eb.tick();

  //обработка поворота энкодера
  if (eb.turn()){
    a += eb.dir();
    oled.setCursor(0, 2);
    oled.print("next: ");
    oled.print(abc[a]);
    oled.setCursor(0, 4);
    oled.print("number: ");
    oled.clear(38, 35, 127, 60);
    oled.print(a + 1);
    }
  //обработка клика (печать символа)
  if (eb.click()){
    wrd += abc[a];
    count++;
    oled.home();
    oled.print(wrd);
  }
  //обработка удержания (удаление символа)
  if (eb.hold()){
    wrd.remove(count-1, 1);
    count--;
    oled.clear(0, 0, 127, 10);
    oled.home();
    oled.print(wrd);
  }
}