#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void WiFiInit(void) {
  Serial.print("Connecting to WiFi...");

  WiFi.setHostname("cerndiplay");
  WiFi.begin(StorageGet(STORAGE_KEY_WIFI_SSID), StorageGet(STORAGE_KEY_WIFI_KEY));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(PIN_LED_WIFI, !digitalRead(PIN_LED_WIFI));
    if(millis() > 60000) {
      // give up on connecting and let the user in to fix whatever settings are wrong
      UiFailsafe();
    }
  }
  
  digitalWrite(PIN_LED_WIFI, LOW);

  Serial.print("\nConnected to network, IP address is ");
  Serial.println(WiFi.localIP());
}

String WiFiGetIPAddress(void) {
  return WiFi.localIP().toString();
}
