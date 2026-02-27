#include <EEPROM.h>

struct CernDisplayStorage stored_data;

void StorageInit(void) {
  EEPROM.begin(130);
  EEPROM.get(0, stored_data);
  if(stored_data.magic != 0xCD01) {
    // nothing stored, initialize it
    Serial.println("Initializing EEPROM...");
    stored_data.magic = 0xCD01;
    strcpy(stored_data.wifi_ssid, "nest");
    strcpy(stored_data.wifi_key, "redacted");
    EEPROM.put(0, stored_data);
    EEPROM.commit();
  } else {
    Serial.println("Loaded storage from EEPROM.");
  }
}

char *StorageGet(uint8_t key) {
  switch(key) {
    case STORAGE_KEY_WIFI_SSID:
      return stored_data.wifi_ssid;
      break;
    case STORAGE_KEY_WIFI_KEY:
      return stored_data.wifi_key;
      break;
  }
  
  return NULL;
}

void StoragePut(uint8_t key, const char *value) {
  switch(key) {
    case STORAGE_KEY_WIFI_SSID:
      strcpy(stored_data.wifi_ssid, value);
      EEPROM.put(0, stored_data);
      break;
    case STORAGE_KEY_WIFI_KEY:
      strcpy(stored_data.wifi_key, value);
      EEPROM.put(0, stored_data);
      break;
  }
}

void StorageCommit(void) {
  EEPROM.commit();
}