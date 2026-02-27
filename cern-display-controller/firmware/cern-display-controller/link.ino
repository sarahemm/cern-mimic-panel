void LinkInit(void) {
  Wire.setPins(11, 10);
  Wire.begin(11, 10);
  //Wire.setClock(25000);
  Wire.setTimeout(5000); // we're running I2C over cables, so in case of data issues set up a timeout so we won't wait forever
}

void LinkSendCommand(uint8_t addr, uint16_t id, uint8_t cmd) {
  //char buf[64];
  //sprintf(buf, "Sending id 0x%X, cmd 0x%X", id, cmd);
  //Serial.println(buf);
  Wire.beginTransmission(addr);
  Wire.write((uint8_t)(id >> 8));   // high byte of id
  Wire.write((uint8_t)(id & 0xFF)); // low byte of id
  Wire.write(cmd);
  Wire.endTransmission();
  //delay(50);
} 

void LinkSendCommand(uint8_t addr, uint16_t id, uint8_t cmd, uint8_t *args, uint16_t args_len) {
  //char buf[64];
  //sprintf(buf, "Sending id 0x%X, cmd 0x%X, args_len %d", id, cmd, args_len);
  //Serial.println(buf);
  Wire.beginTransmission(addr);
  Wire.write((uint8_t)(id >> 8));   // high byte of id
  Wire.write((uint8_t)(id & 0xFF)); // low byte of id
  Wire.write(cmd);

  // chunk the args into 32-byte pieces as if we send more than that, it overruns the Arduino Wire library's buffers and corrupts
  uint8_t bytes_to_send;
  uint8_t *send_ptr = args;
  do {
    bytes_to_send = 32;
    if(args + args_len - send_ptr < 32) bytes_to_send = args + args_len - send_ptr;
    Wire.write(send_ptr, bytes_to_send);
    send_ptr += 32;
  } while(send_ptr < (args + args_len));

  Wire.endTransmission();
  delay(10);  // let the board digest this potentially large chunk of data for a few milliseconds
}

// TODO: look at combining this with the above?
void LinkSendCommandWithLength(uint8_t addr, uint16_t id, uint8_t cmd, uint8_t *args, uint8_t args_len) {
  //char buf[64];
  //sprintf(buf, "Sending id 0x%X, cmd 0x%X, args_len %d", id, cmd, args_len);
  //Serial.println(buf);
  Wire.beginTransmission(addr);
  Wire.write((uint8_t)(id >> 8));   // high byte of id
  Wire.write((uint8_t)(id & 0xFF)); // low byte of id
  Wire.write(cmd);
  Wire.write(args_len);

  // chunk the args into 32-byte pieces as if we send more than that, it overruns the Arduino Wire library's buffers and corrupts
  uint8_t bytes_to_send;
  uint8_t *send_ptr = args;
  do {
    bytes_to_send = 32;
    if(args + args_len - send_ptr < 32) bytes_to_send = args + args_len - send_ptr;
    Wire.write(send_ptr, bytes_to_send);
    send_ptr += 32;
  } while(send_ptr < (args + args_len));

  Wire.endTransmission();
}

void LinkSetText(uint8_t addr, uint16_t id, const char *text) {
  LinkSendCommand(addr, id, CMD_SET_TEXT, (uint8_t*)text, strlen(text)+1);
}

void LinkSetNumeric(uint8_t addr, uint16_t id, uint16_t value) {
  uint8_t args[] = {(uint8_t)(value >> 8), (uint8_t)(value & 0xFF)};

  LinkSendCommand(addr, id, CMD_SET_NUMERIC, args, 2);
}

/*
void LinkSetLED(uint8_t addr, uint16_t id, uint8_t chan1) {
  uint8_t args[] = {chan1};

  LinkSendCommand(addr, id, CMD_SET_LED, args, 1);
}
*/
void LinkSetLED(uint8_t addr, uint16_t id, uint8_t chan1, uint8_t chan2) {
  uint8_t args[] = {chan1, chan2};

  LinkSendCommand(addr, id, CMD_SET_LED_2CH, args, 2);
}

void LinkSetLED(uint8_t addr, uint16_t id, uint8_t chan1, uint8_t chan2, uint8_t chan3) {
  uint8_t args[] = {chan1, chan2, chan3};

  LinkSendCommand(addr, id, CMD_SET_LED_3CH, args, 3);
}

void LinkSetMultiLED(uint8_t addr, uint16_t id, uint8_t len, uint8_t *buf) {
  LinkSendCommandWithLength(addr, id, CMD_SET_LED_MULTI, (uint8_t*)buf, len);
}

void LinkAnimationPlay(uint8_t addr, uint8_t anim_id) {
  uint8_t args[] = {anim_id};
  LinkSendCommand(addr, 0, CMD_ANIMATION_PLAY, args, 1);
}

void LinkAnimationStop(uint8_t addr) {
  LinkSendCommand(addr, 0, CMD_ANIMATION_STOP);
}

#define TEXT_CHUNK_SIZE 120
// same as LinkSetLongText, but uses the flexible buffer on the disply side so we can send more than 125 bytes of text
void LinkSetLongText(uint8_t addr, uint16_t id, char *text) {
  // initialize a buffer on the display for us to use
  LinkSendCommand(addr, id, CMD_INIT_TEXT_BUFFER);

  // send data to the remote buffer TEXT_CHUNK_SIZE characters at a time until we're done
  char chunk_end_buf;
  char *send_ptr = text;
  uint8_t bytes_to_send;
  do {
    bytes_to_send = TEXT_CHUNK_SIZE;
    if(strlen(send_ptr) < TEXT_CHUNK_SIZE) bytes_to_send = strlen(send_ptr) + 1;
    chunk_end_buf = *(send_ptr+bytes_to_send-1);
    *(send_ptr+bytes_to_send-1) = 0;
    LinkSendCommand(addr, id, CMD_FILL_TEXT_BUFFER, (uint8_t*)send_ptr, bytes_to_send);
    *(send_ptr+bytes_to_send-1) = chunk_end_buf;
    send_ptr += bytes_to_send-1;
  } while(send_ptr < (text + strlen(text)));

  // ask the display board to use the buffer we've built to set the text display
  LinkSendCommand(addr, id, CMD_SET_TEXT_FROM_BUFFER);
}

void LinkAnimationMaskSet(uint8_t addr, uint8_t mask) {
  uint8_t args[] = {mask};
  LinkSendCommand(addr, 0, CMD_ANIMATION_MASK_SET, args, 1);

  char buf[32];
  sprintf(buf, "Setting mask 0x%X\n", mask);
  Serial.println(buf);
}

void LinkAnimationMaskRemove(uint8_t addr, uint8_t mask) {
  uint8_t args[] = {mask};
  LinkSendCommand(addr, 0, CMD_ANIMATION_MASK_REMOVE, args, 1);
  char buf[32];
  sprintf(buf, "Removing mask 0x%X\n", mask);
  Serial.println(buf);
}
