uint8_t cur_frame;

void MiniLHCSetBeam(uint8_t beam, uint8_t density, uint8_t frame_nbr) {
  uint8_t i;
  
  uint8_t buf[24];

  for(i=0; i<24; i++) {
    if((i-frame_nbr)%(24/density) == 0) {
      buf[i] = 255;
    } else {
      buf[i] = 0;
    }
  }

  LinkSetMultiLED(BOARD_MINILHC, beam == 0 ? MINILHC_B1_FIRST : MINILHC_B2_FIRST, 24, buf);
}

void MiniLHCInit(void) {
  cur_frame = 0;
}

void MiniLHCUpdate(void) {
  // TODO: we should be tracking beams individually
  bool beam_present = lhc->flags[0].beam_present || lhc->flags[1].beam_present;

  Serial.println("Sending energy...\n");
  if((lhc->changed_fields & LHC_CHANGE_ENERGY) != 0) {
    LinkSetNumeric(BOARD_MINILHC, MINILHC_ENERGY, lhc->energy);
  }
  Serial.println("Sending tSB...\n");
  if((lhc->changed_fields & LHC_CHANGE_T_SB) != 0) {
    if(beam_present) {
      LinkSetNumeric(BOARD_MINILHC, MINILHC_TSB, (uint16_t)(lhc->t_sb.hours * 100 + lhc->t_sb.mins));
    } else {
      LinkSetNumeric(BOARD_MINILHC, MINILHC_TSB, 0);
    }
  }
  Serial.println("Sending accel mode...\n");
  if((lhc->changed_fields & LHC_CHANGE_ACCEL_MODE) != 0) {
    LinkSetText(BOARD_MINILHC, MINILHC_ACCELMODE, lhc->accel_mode);
  }
  Serial.println("Sending beam mode...\n");
  if((lhc->changed_fields & LHC_CHANGE_BEAM_MODE) != 0) {
    LinkSetText(BOARD_MINILHC, MINILHC_BEAMMODE, lhc->beam_mode);
  }
  Serial.println("Sending comment...\n");
  // TODO: lhc->comment is limited to 255 bytes which can cut the comment off! we should probably dynamically allocate that instead
  if((lhc->changed_fields & LHC_CHANGE_COMMENT) != 0) {
    Serial.println(lhc->comment);
    LinkSetLongText(BOARD_MINILHC, MINILHC_COMMENT, lhc->comment);
  }

  // figure out which experiments are getting lumi and mask off any that aren't
  if((lhc->experiment[0].changed_fields & LHC_EXP_CHANGE_INSTLUMI) != 0) {
    if(lhc->experiment[0].inst_lumi > 0)
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_1);
    else
      LinkAnimationMaskSet(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_1);
  }
  if((lhc->experiment[1].changed_fields & LHC_EXP_CHANGE_INSTLUMI) != 0) {
    if(lhc->experiment[1].inst_lumi > 0)
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_2);
    else
      LinkAnimationMaskSet(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_2);
  }
  if((lhc->experiment[2].changed_fields & LHC_EXP_CHANGE_INSTLUMI) != 0) {
    if(lhc->experiment[2].inst_lumi > 0)
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_5);
    else
      LinkAnimationMaskSet(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_5);
  }
  if((lhc->experiment[3].changed_fields & LHC_EXP_CHANGE_INSTLUMI) != 0) {
    if(lhc->experiment[3].inst_lumi > 0)
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_8);
    else
      LinkAnimationMaskSet(BOARD_MINILHC, MINILHC_ANIM_MASK_IP_8);
  }

  // figure out if there's a beam and apply beam masks if necessary
  if((lhc->changed_fields & LHC_CHANGE_FLAGS) != 0) {
    if(lhc->flags[0].beam_present && lhc->flags[1].beam_present) {
      // no masks needed, both beams present
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_BEAM_1 | MINILHC_ANIM_MASK_BEAM_2);
      Serial.println("Both beams are present");
    } else if(lhc->flags[0].beam_present) {
      // only beam 1 present, so mask off beam 2
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_BEAM_1);
      LinkAnimationMaskSet(BOARD_MINILHC, MINILHC_ANIM_MASK_BEAM_2);
      Serial.println("Beam 2 not present");
    } else if(lhc->flags[1].beam_present) {
      // only beam 2 present, so mask off beam 1
      LinkAnimationMaskSet(BOARD_MINILHC, MINILHC_ANIM_MASK_BEAM_1);
      LinkAnimationMaskRemove(BOARD_MINILHC, MINILHC_ANIM_MASK_BEAM_2);
      Serial.println("Beam 1 not present");
    } else {
      // no beam, stop any animations entirely
      LinkAnimationStop(BOARD_MINILHC);
      Serial.println("No beams.");
    }

    // all the proper masks should be applied, so start playing the appropriate animation
    if(lhc->flags[0].beam_present || lhc->flags[1].beam_present) {
      if(strstr(lhc->beam_mode, "INJ") != NULL) {
        LinkAnimationPlay(BOARD_MINILHC, MINILHC_ANIM_INJECTING);
      } else {
        // we always play the animation with experiments, we mask off the experiments as required
        // TODO: can we eliminate the non-EXP animation then?
        LinkAnimationPlay(BOARD_MINILHC, MINILHC_ANIM_BOTH_BEAMS_EXP);
      }
    }

    // push the new flags to the flag LEDs

    // beam permit
    delay(50);
    if(lhc->flags[0].beam_permit)
      LinkSetLED(BOARD_MINILHC, MINILHC_BEAMPERMIT_B1, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_BEAMPERMIT_B1, 255, 0);
    if(lhc->flags[1].beam_permit)
      LinkSetLED(BOARD_MINILHC, MINILHC_BEAMPERMIT_B2, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_BEAMPERMIT_B2, 255, 0);

    // stable beam
    delay(50);
    if(lhc->flags[0].stable_beams)
      LinkSetLED(BOARD_MINILHC, MINILHC_STABLEBEAM_B1, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_STABLEBEAM_B1, 255, 0);
    if(lhc->flags[1].stable_beams)
      LinkSetLED(BOARD_MINILHC, MINILHC_STABLEBEAM_B2, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_STABLEBEAM_B2, 255, 0);

    // movables allowed in
    delay(50);
    if(lhc->flags[0].movables_allowed_in)
      LinkSetLED(BOARD_MINILHC, MINILHC_MOVABLES_B1, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_MOVABLES_B1, 255, 0);
    if(lhc->flags[1].movables_allowed_in)
      LinkSetLED(BOARD_MINILHC, MINILHC_MOVABLES_B2, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_MOVABLES_B2, 255, 0);

    // permits linked
    delay(50);
    if(lhc->flags[0].permits_linked)
      LinkSetLED(BOARD_MINILHC, MINILHC_PERMITSLINKED, 0, 255);
    else
      LinkSetLED(BOARD_MINILHC, MINILHC_PERMITSLINKED, 255, 0);
  }

  // cryo is handled separately from other flags, so update that if required too
  if((lhc->changed_fields & LHC_CHANGE_CRYO) != 0) {
    if(lhc->cryo_ok) {
      LinkSetLED(BOARD_MINILHC, MINILHC_CRYO_WRAPUP, 0, 255);
    } else {
      LinkSetLED(BOARD_MINILHC, MINILHC_CRYO_WRAPUP, 255, 0);
    }
  }
}
