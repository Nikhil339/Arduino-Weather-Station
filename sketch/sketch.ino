#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,-1);


#define DHTPIN 2
#define DHTTYPE 22

DHT dht(DHTPIN, DHTTYPE);


const byte ldrpin = A0;
const float gamma = 0.7;
const float rl10 = 50;


#define ledpin 3


const int ledcount = 10;
int ledpins[] = {
  4,5,6,7,8,9,10,11,12,13
};

void setup() {
  Serial.begin(115200);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  pinMode(ledpin, OUTPUT);

  for (int thisled = 0; thisled < ledcount; thisled++){
    pinMode(ledpins[thisled], OUTPUT);
  }
}

void loop() {
  delay(5000);

  //read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  //read lux
  int analogValue = analogRead(ldrpin);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(rl10 * 1e3 * pow(10, gamma) / resistance, (1 / gamma));

  //clear display
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Humidity: ");
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print(h);
  display.print(" %"); 

  // display lux
  display.setTextSize(1);
  display.setCursor(0,40);
  display.print("Brighness: ");
  display.setTextSize(1);
  display.setCursor(0,50);
  display.print(lux);
  display.print(" lux");

  // controlling led
  if (lux<=300) {
    digitalWrite(ledpin, HIGH);
  }
  else{
    digitalWrite(ledpin, LOW);
  }

  // controlling led strip
  int ledlevel = map(t, -40, 80, 0, ledcount);
  for (int thisled = 0; thisled<ledcount; thisled++){
    if (thisled<ledlevel){
      digitalWrite(ledpins[thisled], HIGH);
    }
    else{
      digitalWrite(ledpins[thisled], LOW);
    }
  }

  display.display(); 
}
