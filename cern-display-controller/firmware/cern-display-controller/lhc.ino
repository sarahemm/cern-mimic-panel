#include <HTTPClient.h>
#include <ArduinoJson.h>

LargeHadronCollider::LargeHadronCollider() {
  strcpy(this->data_url, CERNDATA_PROD_URL);
  // make sure all of the experiments get marked as changed at first
  this->experiment[0].inst_lumi = -1;
  this->experiment[1].inst_lumi = -1;
  this->experiment[2].inst_lumi = -1;
  this->experiment[3].inst_lumi = -1;
  this->refresh();
}

// TODO: this should probably not return void
void LargeHadronCollider::refresh() {
  JsonDocument doc;
  HTTPClient http;

  int responseCode;
  char buf[64];

  Serial.println("Sending HTTP GET...");
  Serial.println(this->data_url);
  http.begin(this->data_url);
  responseCode = http.GET();
  Serial.print("GET complete, response code is ");
  Serial.println(responseCode);
  deserializeJson(doc, http.getString());

  this->changed_fields = 0;

  // energy level
  if(this->energy != doc["energy"].as<uint16_t>()) changed_fields |= LHC_CHANGE_ENERGY;
  this->energy = doc["energy"].as<uint16_t>();

  // page1 comment
  if(strncmp(this->comment, doc["comments"]["text"], 255) != 0) changed_fields |= LHC_CHANGE_COMMENT;
  strncpy(this->comment, doc["comments"]["text"], 255);
  this->comment[254] = 0;

  // accelerator mode
  char new_mode[17];
  strncpy(new_mode, doc["accmode"], 16);
  new_mode[16] = 0;
  if(strcmp(new_mode, "MACHINE DEVELOPM") == 0) {
    strcpy(new_mode, "MACHINE DEVEL");
  }
  if(strcmp(this->accel_mode, new_mode) != 0) {
    strcpy(this->accel_mode, new_mode);
    changed_fields |= LHC_CHANGE_ACCEL_MODE;
  }
  
  // beam mode
  strncpy(new_mode, doc["beammode"], 16);
  new_mode[16] = 0;
  if(strcmp(new_mode, "INJECTION PROBE ") == 0) {
    strcpy(new_mode, "INJ PROBE BEAM");
  } else if(strcmp(new_mode, "INJECTION PHYSIC") == 0) {
    strcpy(new_mode, "INJ PHYSICS BEAM");
  }
  if(strcmp(this->beam_mode, new_mode) != 0) {
    strcpy(this->beam_mode, new_mode);
    changed_fields |= LHC_CHANGE_BEAM_MODE;
  }
  
  // BIS/SMP Flags
  if(
    flags[0].permits_linked != doc["flags"]["link_status"][0] ||
    flags[1].permits_linked != doc["flags"]["link_status"][1] ||
    flags[0].beam_permit != doc["flags"]["global_beam_permit"][0] ||
    flags[1].beam_permit != doc["flags"]["global_beam_permit"][1] ||
    flags[0].setup_beam != doc["flags"]["setup_beam"][0] ||
    flags[1].setup_beam != doc["flags"]["setup_beam"][1] ||
    flags[0].beam_present != doc["flags"]["beam_presence"][0] ||
    flags[1].beam_present != doc["flags"]["beam_presence"][1] ||
    flags[0].movables_allowed_in != doc["flags"]["moveable_devices"][0] ||
    flags[1].movables_allowed_in != doc["flags"]["moveable_devices"][1] ||
    flags[0].stable_beams != doc["flags"]["stable_beams"][0] ||
    flags[1].stable_beams != doc["flags"]["stable_beams"][1]
  ) changed_fields |= LHC_CHANGE_FLAGS;

  flags[0].permits_linked = doc["flags"]["link_status"][0];
  flags[1].permits_linked = doc["flags"]["link_status"][1];
  flags[0].beam_permit = doc["flags"]["global_beam_permit"][0];
  flags[1].beam_permit = doc["flags"]["global_beam_permit"][1];
  flags[0].setup_beam = doc["flags"]["setup_beam"][0];
  flags[1].setup_beam = doc["flags"]["setup_beam"][1];
  flags[0].beam_present = doc["flags"]["beam_presence"][0];
  flags[1].beam_present = doc["flags"]["beam_presence"][1];
  flags[0].movables_allowed_in = doc["flags"]["moveable_devices"][0];
  flags[1].movables_allowed_in = doc["flags"]["moveable_devices"][1];
  flags[0].stable_beams = doc["flags"]["stable_beams"][0];
  flags[1].stable_beams = doc["flags"]["stable_beams"][1];

  // cryo is handled separately than other flags
  if((doc["cryo"]["lost_count"] == 0) != this->cryo_ok) {
    this->changed_fields |= LHC_CHANGE_CRYO;
    this->cryo_ok = (doc["cryo"]["lost_count"] == 0);
  }

  // parse out the tSB hours and minutes
  if(strcmp(doc["tsb"], "PT0S") == 0) {
    this->t_sb.hours = 0;
    this->t_sb.mins = 0;
  } else {
    char tsb[16];
    strncpy(tsb, doc["tsb"].as<const char*>()+2, 15);
    uint8_t new_hours, new_mins;
    new_hours = atoi(strtok(tsb, "HMS"));
    new_mins = atoi(strtok(NULL, "HMS"));
    if(this->t_sb.hours != new_hours || this->t_sb.mins != new_mins) changed_fields |= LHC_CHANGE_T_SB;
    this->t_sb.hours = new_hours;
    this->t_sb.mins = new_mins;
  }

  // parse out the total number of injected bunches from the AFS
  char *token;
  char afs[64];
  strncpy(afs, doc["fillscheme"].as<const char*>(), 63);
  strtok(afs, "_");
  while(token = strtok(NULL, "_")) {
    if(strchr(token, 'b') != NULL) {
      token[strlen(token)-1] = 0;
      this->total_bunches = atoi(token);
      break;
    }
  }

  // now for the experiments
  uint8_t i;
  for(i=0; i<3; i++) {
    this->experiment[i].changed_fields = 0;
    if(this->experiment[i].inst_lumi == 0 and doc["experiment"][i]["operation"]["inst_lumi"] != 0 ||
       this->experiment[i].inst_lumi != 0 and doc["experiment"][i]["operation"]["inst_lumi"] == 0) this->experiment[i].changed_fields |= LHC_EXP_CHANGE_INSTLUMI;
    this->experiment[i].inst_lumi = doc["experiment"][i]["operation"]["inst_lumi"];
  }

  http.end();
}