#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

#define NUM_LEDS 8     //LED灯数目
#define DATA_PIN 4     //引脚 引脚4 = GPIO4
//arduino D1 的D4引脚
//对应ESP8266的D2

CRGB leds[NUM_LEDS];  //第几个灯

int color = 20;       //颜色
int dim = 80;         //亮度
int modus = 3;        //模式
int confiG = 80;

// WiFi network
const char* ssid      =     "xxxxxxxxx";
const char* password  =     "xxxxxxxxx";

ESP8266WebServer server ( 80 );

char htmlResponse[3000];

void handleRoot() {
  snprintf(htmlResponse, 3000, 
    "<!DOCTYPE html>\
     <html>\
     <head>\
     <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
     <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\
     <title>WIFI彩灯</title>\
     <script type=\"text/javascript\">function led(){\
                                   var mode_id = cc.value;\
                                   var color_id = color_slider.value;\
                                   var dim_id = dim_slider.value;\
                                   var xmlhttp=new XMLHttpRequest();\
                                   xmlhttp.open(\"GET\",\"/save?mode_id=\"+mode_id+\"&color_id=\"+color_id+\"&dim_id=\"+dim_id,true);\
                                   xmlhttp.send();xmlhttp.onload = function(e){alert(this.responseText);}}\
     </script>\
     </head>\
       <body>WIFI彩灯\
       <form>\
         <input type='range' name='color_slider' id='color_slider' size=2 min='5' max='100' value='50'> 变换速度<br> \
         <input type='range' name='dim_slider' id='dim_slider' size=2 min='1' max='254' value='125'> 亮度<br> \
         <select id=\"cc\">\
           <option value='1'>关闭</option>\
           <option value='2'>暂停</option>\
           <option value='3'>水滴</option>\
           <option value='4'>旋风</option>\
           <option value='5'>彩虹</option>\
           <option value='6'>渐变</option>\
           <option value='7'>橙色-绿色</option>\
           <option value='8'>蓝色-紫色</option>\
           <option value='9'>青色-蓝色</option>\
         </select>\
         <br><input type=\"button\" value=\"保存\" onclick=\"led()\">\
       </form>\
       </body>\
     </html>"
  );
  server.send ( 200, "text/html", htmlResponse );
}

void handleSave() {
  if (server.arg("color_id") != "") {
//    Serial.println("color: " + server.arg("color_id"));
    color = 105 - server.arg("color_id").toInt();
  }

  if (server.arg("dim_id") != "") {
//    Serial.println("brightness: " + server.arg("dim_id"));
    dim = server.arg("dim_id").toInt();
  }

  if (server.arg("mode_id") != "") {
//    Serial.println("moduds: " + server.arg("mode_id"));
    modus = server.arg("mode_id").toInt();
  }
}

void setup() {
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // Start serial
  Serial.begin(115200);
  delay(10);

  // Connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(0, 255, 100);
    }
    FastLED.show();
    delay(250);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(0, 0, 0);
    }
    FastLED.show();
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on ( "/", handleRoot );
  server.on ("/save", HTTP_GET, handleSave);
  //开启服务器
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
  switch (modus) {
    case 1: Modus1(); break;
    case 2: Modus2(); break;
    case 3: Modus3(); break;
    case 4: Modus4(); break;
    case 5: Modus5(); break;
    case 6: Modus6(); break;
    case 7: Modus7(); break;
    case 8: Modus8(); break;
    case 9: Modus9(); break;
    default: Modus1(); break;
  }
}

void Modus1() {
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CHSV(i, 0, 0);
  }
  FastLED.show();
  delay(20);
}

void Modus2() {
  delay(20);
}

void Modus3() {
  static float counter = 0;
  static float C;
  int nummerdrup = map(color, 0, 255, 0, 10);
  counter = counter + 0.001;
  for (int i = 0; i <= nummerdrup; i++) {
    int pos = random16(NUM_LEDS);
    C = sin(counter) * 200;
    leds[pos] += CHSV(C + random8(30), 255, dim);
  }
  delay(30);
  Fadeall();
  if (counter > 10000) {
    counter = 0;
  }
  FastLED.show();
}

void Modus4() {
  static uint8_t hue = 0;
  static int counter = 0;
  static boolean dir = true;
  if (dir == true) {
    counter++;
  } else if (dir == false) {
    counter--;
  }
  if (counter > NUM_LEDS) {
    dir = false;
  }
  if (counter < 0) {
    dir = true;
  }
  leds[counter] = CHSV(hue++, 255, dim);
  FastLED.show();
  Fadeall();
  delay(map(color, 5, 100, 5, 50));
}

void Modus5() {
  static int counter = 0;
  delay(map(color, 5, 100, 5, 500));
  if (counter > 255) {
    counter = 0;
  }
  counter++;
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CHSV(counter + i, 255, dim);
  }
  FastLED.show();
}

void Modus6() {
  static int counter = 0;
  counter++;
  if (counter > 255) {
    counter = 0;
  }
  delay(map(color, 5, 100, 5, 500));
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(counter, 255, dim);
  }
  FastLED.show();
}

void Modus7() {
  int min_num = 0;
  int max_num = 90;
  
  CommonModus(min_num, max_num);
}

void Modus8() {
  int min_num = 90;
  int max_num = 180;
  
  CommonModus(min_num, max_num);
}

void Modus9() {
  int min_num = 160;
  int max_num = 240;

  CommonModus(min_num, max_num);
}

void CommonModus(int min_num, int max_num) {
  int avg = (min_num + max_num) / 2;
  int difference = max_num - min_num;
  float si = map(confiG, 5, 100, 1, 100);
  static float counter;
  
  counter += si / 1000;

//  Serial.println(difference / 2 * sin(counter) + avg);

  for (int i = 0; i <= NUM_LEDS; i++) {
    float Delay = i * si / 1000;
    float colorgradient = difference / 2 * sin(counter + Delay) + avg;
    leds[i] = CHSV(colorgradient , 255, dim);
  }

  FastLED.show();
  delay(map(color, 5, 100, 5, 100));
}

void Fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(240);
  }
}
