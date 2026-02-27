

SimpleCLI cli;
bool failsafe_mode;

void UiCmdAnimate(cmd *c) {
  uint8_t addr, anim;
  Command cmd(c);

  addr = cmd.getArg("board").getValue().toInt();
  anim = cmd.getArg("anim").getValue().toInt();

  Serial.println("OK, starting animation.");
  LinkSendCommand(addr, 0, CMD_ANIMATION_PLAY, &anim, 1);

  UiPrompt();
}

void UiCmdSource(cmd *c) {
  Command cmd(c);

  if(cmd.getArg("test").isSet()) {
    strcpy(lhc->data_url, CERNDATA_TEST_URL);
    Serial.println("OK, switching to TEST data source.");
    lastLcdUpdate = 0;
  } else if(cmd.getArg("prod").isSet()) {
    strcpy(lhc->data_url, CERNDATA_PROD_URL);
    Serial.println("OK, switching to PROD data source.");
    lastLcdUpdate = 0;
  }

  UiPrompt();
}

void UiCmdRefresh(cmd *c) {
  Serial.println("OK, forcing a refresh now.");
  lastLcdUpdate = 0;

  UiPrompt();
}

void UiCmdSysFlag(cmd *c) {
  uint8_t addr;
  char response[48];
  uint8_t buf;
  Command cmd(c);

  // TODO: support hex inputs too
  addr = cmd.getArg("board").getValue().toInt();
  buf = cmd.getArg("flag").getValue().toInt();

  if(cmd.getArg("set").isSet()) {
    sprintf(response, "Setting sysflag %d on device 0x%X.", buf, addr);
    Serial.println(response);
    LinkSendCommand(addr, 0, CMD_SET_SYSFLAG, &buf, 1);
  } else if(cmd.getArg("clear").isSet()) {
    sprintf(response, "Clearing sysflag %d on device 0x%X.", buf, addr);
    Serial.println(response);
    LinkSendCommand(addr, 0, CMD_CLEAR_SYSFLAG, &buf, 1);
  } else {
    Serial.println("ERR, you must specify either -set or -clear for flags.");
  }

  UiPrompt();
}

void UiCmdWifiSetup(cmd *c) {
  Command cmd(c);

  StoragePut(STORAGE_KEY_WIFI_SSID, cmd.getArg("ssid").getValue().c_str());
  StoragePut(STORAGE_KEY_WIFI_KEY, cmd.getArg("key").getValue().c_str());
  StorageCommit();

  Serial.println("OK, wifi setup updated. Restarting...");
  sleep(2);
  ESP.restart();
}

void UiInit(void) {
  failsafe_mode = false;

  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
  pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BTN_SELECT, INPUT_PULLUP);

  pinMode(PIN_LCD_nRESET, OUTPUT);
  pinMode(PIN_LED_STATUS, OUTPUT);
  pinMode(PIN_LED_WIFI, OUTPUT);

  Command cmdSource = cli.addCommand("source", UiCmdSource);
  cmdSource.addFlagArg("test");
  cmdSource.addFlagArg("prod");
  
  Command cmdRefresh = cli.addCommand("refresh", UiCmdRefresh);

  Command cmdSysFlag = cli.addCommand("sysflag", UiCmdSysFlag);
  cmdSysFlag.addFlagArg("set");
  cmdSysFlag.addFlagArg("clear");
  cmdSysFlag.addArg("board");
  cmdSysFlag.addArg("flag");

  Command cmdAnimate = cli.addCommand("animate", UiCmdAnimate);
  cmdAnimate.addArg("board");
  cmdAnimate.addArg("anim");

  Command cmdWifiSetup = cli.addCommand("wifi-setup", UiCmdWifiSetup);
  cmdWifiSetup.addArg("ssid");
  cmdWifiSetup.addArg("key");
  
  UiPrompt();
}

void UiLoop(void) {
  uint8_t buf;
  
  if(cli.errored()) {
    CommandError e = cli.getError();

    // Print the error, or do whatever you want with it
    Serial.println(e.toString());
  }

  if(Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
    if(input.length() == 0 or input == "\n") {
      UiPrompt();
    } else {
      cli.parse(input);
    }
  }
}

void UiPrompt(void) {
  if(failsafe_mode) {
    Serial.print("ctlr> ");
  } else {
    Serial.print("ctld-failsafe> ");
  }
}

// we couldn't connect to wifi, so enter a failsafe mode where the user can reconfigure things
void UiFailsafe(void) {
  Serial.println("Couldn't connect to Wifi, entering failsafe mode.");
  UiPrompt();
  
  while(true) {
    UiLoop();
    sleep(1);
  }
}