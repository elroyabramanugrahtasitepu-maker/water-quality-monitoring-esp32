#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const char* ssid = "";
const char* password = "";

// Ganti dengan IP laptop yang menjalankan XAMPP
const char* server = "";

#define TDS_PIN A0
#define ONE_WIRE_BUS D4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

   // Ganti jika nanti pakai DS18B20

void setup() {
  Serial.begin(115200);
  sensors.begin();
  WiFi.begin(ssid, password);

Serial.print("Menghubungkan WiFi");

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

Serial.println();
Serial.println("WiFi Terhubung");
Serial.println(WiFi.localIP());

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.setTextColor(SSD1306_WHITE);

  // ===============================
  // Loading Screen
  // ===============================
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(22,0);
  display.println("AKPRIND");

  display.setTextSize(2);
  display.setCursor(8,15);
  display.println("WATER");

  display.setCursor(5,35);
  display.println("QUALITY");

  display.setTextSize(1);
  display.setCursor(15,53);
  display.println("Initializing...");

  display.drawRect(14,58,100,5,WHITE);
  display.display();

  for(int i=0;i<=100;i++){

    display.fillRect(15,59,98,3,BLACK);
    display.fillRect(15,59,i,3,WHITE);

    display.display();
    delay(25);
  }

  delay(500);
  display.clearDisplay();
  display.display();
}

void loop() {
  sensors.requestTemperatures();
  delay(50);
float temperature = sensors.getTempCByIndex(0);

if (temperature == DEVICE_DISCONNECTED_C) {
  Serial.println("DS18B20 Tidak Terbaca!");
  temperature = 25.0;
}

  long total = 0;

  // Ambil 20 sampel agar lebih stabil
  for (int i = 0; i < 20; i++) {
    total += analogRead(TDS_PIN);
    delay(10);
  }

  float adcValue = total / 20.0;
float voltage = adcValue / 1024.0 * 3.3;

  // Temperature Compensation
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
  float compensationVoltage = voltage / compensationCoefficient;

  float tds = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
             - 255.86 * compensationVoltage * compensationVoltage
             + 857.39 * compensationVoltage) * 0.5;

  if (adcValue < 10) {
    tds = 0;
  }

  String statusAir;

if (tds < 150)
    statusAir = "Sangat Baik";
else if (tds < 400)
    statusAir = "Baik";
else if (tds < 700)
    statusAir = "Normal";
else
    statusAir = "Perlu Cek";

  Serial.println("======================");
  Serial.print("ADC : ");
  Serial.println(adcValue);

  Serial.print("Voltage : ");
  Serial.println(voltage, 3);

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("TDS : ");
  Serial.print(tds, 1);
  Serial.println(" ppm");

  Serial.print("Status : ");
  Serial.println(statusAir);

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(10,0);
  display.println("Monitoring Air");

  display.setCursor(0,18);
  display.print("TDS  : ");
  display.print(tds,1);
  display.println(" ppm");

  display.setCursor(0,34);
  display.print("Suhu : ");
  display.print(temperature,1);
  display.println(" C");

  display.setCursor(0,50);
  display.print("Status: ");
  display.print(statusAir);

  display.display();
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    String url = "http://" + String(server) +
                 "/sensor_air/kirim_data.php?tds=" +
                 String(tds,1) +
                 "&suhu=" +
                 String(temperature,1);

    http.begin(client, url);

    int httpCode = http.GET();

    Serial.print("HTTP Code : ");
    Serial.println(httpCode);

    http.end();
}

  delay(1000);
}
