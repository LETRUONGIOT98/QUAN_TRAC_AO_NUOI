#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
#define BLYNK_TEMPLATE_ID "TMPL62y0ve9oF"
#define BLYNK_TEMPLATE_NAME "QUAN TRAC HO NUOI"
#define BLYNK_AUTH_TOKEN "l5LbK1K2Lsx528noGGidpybOvV2ce1is"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Thu Thao";
char pass[] = "12052002";

// Định nghĩa các chân cảm biến
#define TURBIDITY_PIN 36 // Độ đục
#define NH3_PIN 39       // Nồng độ NH3
#define SOIL_MOISTURE_PIN 35 // Độ ẩm đất
#define ONE_WIRE_BUS 16 // DS18B20

// Thiết lập cho cảm biến DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Khởi tạo màn hình LCD I2C 1602
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Khởi tạo kết nối WiFi và Blynk
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

  // Khởi tạo màn hình LCD
  lcd.init();
  lcd.backlight();

  // Khởi tạo cảm biến nhiệt độ DS18B20
  sensors.begin();

  // Thiết lập các chân cảm biến
  pinMode(TURBIDITY_PIN, INPUT);
  pinMode(NH3_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
}

void loop() {
  Blynk.run();

  // Đọc dữ liệu từ các cảm biến
  float temperature = getTemperature();
  int turbidity = analogRead(TURBIDITY_PIN);
  turbidity = map(turbidity,0,4095,100,0);
  int nh3 = analogRead(NH3_PIN);
  nh3 = map(nh3,0,4095,0,300);
  int sum;
  for(int i = 0; i < 100; i ++){
  int val4 = analogRead(SOIL_MOISTURE_PIN);
  sum = sum + val4;
  delay(10);
  }
  sum = sum /100;
  Serial.println(sum);
  if(sum>=1020)sum = 1020;
  int doman = map(sum,0,1020,100,0);
  // Hiển thị lên LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temperature,1); 
  lcd.print(" Man: ");
  lcd.print(doman);
  lcd.print("%");


  lcd.setCursor(0, 1);
  lcd.print("D: ");
  lcd.print(turbidity); 
  lcd.print("% NH3: ");
  lcd.print(nh3);
  lcd.print(" Ppm  ");

  // Đẩy dữ liệu lên Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, turbidity);
  Blynk.virtualWrite(V2, doman);
  Blynk.virtualWrite(V3, nh3);
  Serial.print("T: ");
  Serial.println(temperature);
  Serial.print("Man: ");
  Serial.println(doman);
  Serial.print("Do duc: ");
  Serial.println(turbidity);
  Serial.print("NH3: ");
  Serial.println(nh3);
}

// Hàm đọc nhiệt độ từ cảm biến DS18B20
float getTemperature() {
  sensors.requestTemperatures(); 
  return sensors.getTempCByIndex(0);
}
