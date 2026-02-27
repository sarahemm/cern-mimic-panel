#define STORAGE_KEY_WIFI_SSID 0
#define STORAGE_KEY_WIFI_KEY  1

struct CernDisplayStorage {
  uint16_t magic;
  char wifi_ssid[64];
  char wifi_key[64];
};