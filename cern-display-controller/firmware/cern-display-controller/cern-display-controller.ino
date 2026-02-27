#include <SimpleCLI.h>
#include <pins.h>
#include <link_proto.h>
#include <boards.h>
#include "lhc.h"
#include "storage.h"

LargeHadronCollider *lhc;
long lastLcdUpdate;
long lastStatusUpdate;
long lastMiniLHCUpdate;

void UpdateTestDisplay(void) {
  DisplayClear();
  DisplayWrite(lhc->accel_mode);
  DisplayMoveCursor(2, 1);
  DisplayWrite(lhc->beam_mode);
  DisplayMoveCursor(3, 1);
  char buf[21];
  if(lhc->t_sb.hours == 0 and lhc->t_sb.mins == 0) {
    sprintf(buf, "Energy: %ld GeV", lhc->energy);
  } else {
    sprintf(buf, "%ld GeV  tSB: %d:%d", lhc->energy, lhc->t_sb.hours, lhc->t_sb.mins);
  }
  DisplayWrite(buf);
  DisplayMoveCursor(4, 1);
  sprintf(buf, "Chg: %x", lhc->changed_fields);
  DisplayWrite(buf);
}

void UpdateDisplay(void) {
  char buf[21];

  DisplayClear();
  DisplayWrite("CERN Display Ctlr v1");
  DisplayMoveCursor(2, 1);
  sprintf(buf, "SSID: %0.14s", StorageGet(STORAGE_KEY_WIFI_SSID));
  DisplayWrite(buf);
  DisplayMoveCursor(3, 1);
  sprintf(buf, "IP: %s", WiFiGetIPAddress());
  DisplayWrite(buf);
  sprintf(buf, "Last Update: %ds ago", (millis()-lastLcdUpdate)/1000);
  DisplayMoveCursor(4, 1);
  DisplayWrite(buf);
}

void setup() {
  char buf[21];

  Serial.begin(9600);
  sleep(2);
  Serial.println("CERN Display Controller is online!");

  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);

  Serial.println("Initializing display...");
  DisplayInit();

  Serial.println("Initializing storage...");
  StorageInit();
  
  Serial.println("Initializing UI...");
  UiInit();

  Serial.println("Initializing Link interfaces...");
  LinkInit();

  // TODO: replace this with an overall boot progress screen
  DisplayClear();
  DisplayMoveCursor(1, 1);
  sprintf(buf, "SSID: %0.14s", StorageGet(STORAGE_KEY_WIFI_SSID));
  DisplayWrite(buf);
  DisplayMoveCursor(2, 1);
  DisplayWrite("Connecting...");

  Serial.println("Initializing WiFi...");
  WiFiInit();

  Serial.println("Initializing MiniLHC...");
  MiniLHCInit();

  UpdateDisplay();
  
  Serial.println("Starting LHC refresh...");
  lhc = new LargeHadronCollider();
  MiniLHCUpdate();

  //LinkSetText(BOARD_MINILHC, MINILHC_ACCELMODE, lhc->accel_mode);
  delay(100);
  //LinkSetText(BOARD_MINILHC, MINILHC_BEAMMODE, lhc->beam_mode);

  lastLcdUpdate = millis();
  lastStatusUpdate = millis();
  //lastMiniLHCUpdate = millis();
  Serial.println("Done refresh!");  
  // do some "it's alive" displaying

  /*
  // text comment display
  LinkSetText(BOARD_MINILHC, MINILHC_COMMENT, "Hello, World!");
  
  // energy display
  LinkSetNumeric(BOARD_MINILHC, MINILHC_ENERGY, 1234);

  // tSB display
  LinkSetNumeric(BOARD_MINILHC, MINILHC_TSB, 5678);
  */
  // accel mode/beam mode displays
  //Serial.println("Starting tx to MiniLHC board...");
  //LinkSetText(BOARD_MINILHC, MINILHC_ACCELMODE, "ACCELERATOR MODE");
  //LinkSetText(BOARD_MINILHC, MINILHC_BEAMMODE, "BEAM MODE");
}

void loop() {
  /*
  // flash the cryo wrapup green LED on the mini LHC display as part of the "it's alive" demo
  LinkSetLED(BOARD_MINILHC, MINILHC_CRYO_WRAPUP, 0x00, 0xFF);
  sleep(1);
  LinkSetLED(BOARD_MINILHC, MINILHC_CRYO_WRAPUP, 0xFF, 0x00);
  sleep(1);
  */
  if(millis() - lastLcdUpdate > 60000) {
    DisplayClearRow(4);
    lhc->refresh();
    MiniLHCUpdate();
    //UpdateDisplay();
    lastLcdUpdate = millis();
  }

  //if(millis() - lastMiniLHCUpdate > 50) {
  //  MiniLHCUpdate();
  //  lastMiniLHCUpdate = millis();
  //}

  if(millis() - lastStatusUpdate > 1000) {
    digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
    UpdateDisplay();
    lastStatusUpdate = millis();
  }

  UiLoop();
  delay(100);
}
