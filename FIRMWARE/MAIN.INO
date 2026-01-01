#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <Adafruit_NeoPixel.h>



// PIN CONFRIGRATION
#define LED_PIN     48        // RGB NeoPixel pin
#define LED_COUNT   1         // only one LED for test

Adafruit_NeoPixel rgbLed(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

BLEScan* pBLEScan;


// VAR
unsigned long lastPrint   = 0;
int scanTime = 5;   // BLE scan time (seconds)



void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println(" ESP32 TEST STARTING .... ");
  Serial.println(" ------------------------ ");
  Serial.println();



  // ---- RGB LED INIT ----
  rgbLed.begin();
  rgbLed.clear();
  rgbLed.show();

  Serial.println(" NeoPixel Init Done ");
  delay(300);



  // ---- WIFI INIT ----
  WiFi.mode(WIFI_STA);
   WiFi.disconnect();
  delay(100);

  Serial.println(" WiFi set to STA mode ");
  Serial.println();



  // ---- BLE INIT ----
  BLEDevice::init("ESP32_TEST_DEVICE");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);  // better results but more power
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(80);

  Serial.println(" BLE Init Done ");
  Serial.println();



  // random LED color at boot
  rgbLed.setPixelColor(0, rgbLed.Color(20, 0, 20));
  rgbLed.show();

  Serial.println(" Setup Finished .... ");
  Serial.println();
}



void loop()
{
  // ---- SERIAL HEARTBEAT ----
  if (millis() - lastPrint > 3000)
  {
    lastPrint = millis();

    Serial.print(" Uptime (ms): ");
    Serial.print(millis());

    Serial.print(" | Free Heap: ");
    Serial.println(ESP.getFreeHeap());
  }



  // ---- WIFI SCAN ----
  Serial.println();
  Serial.println(" Starting WiFi Scan .... ");

  int n = WiFi.scanNetworks();

  if (n == 0)
  {
    Serial.println(" No WiFi Networks Found ");
    rgbLed.setPixelColor(0, rgbLed.Color(50, 0, 0)); // red
  }
  else
  {
    Serial.print(" WiFi Networks Found: ");
    Serial.println(n);

    rgbLed.setPixelColor(0, rgbLed.Color(0, 50, 0)); // green

    for (int i = 0; i < n; i++)
    {
      Serial.print("  ");
      Serial.print(i + 1);
      Serial.print(" -> ");
      Serial.print(WiFi.SSID(i));
      Serial.print("  RSSI: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm ");
      Serial.println();
      delay(10);
    }
  }

  rgbLed.show();
  delay(800);



  // ---- BLE SCAN ----
  Serial.println();
  Serial.println(" Starting BLE Scan .... ");

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  Serial.print(" BLE Devices Found: ");
  Serial.println(foundDevices.getCount());

  pBLEScan->clearResults();   // free memory

  rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 50)); // blue
  rgbLed.show();



  // ---- SMALL DELAY ----
  Serial.println(" Scan Cycle Done .... ");
  Serial.println(" --------------------- ");
  Serial.println();

  delay(4000);   // slow DOWN
}
