void SbrosModema(bool *ping_flag, bool *first_sbros) {
  static uint32_t tmr = millis(), hold_timer = millis();
  static bool faza_sbrosa = true;

  if ((*first_sbros && millis() - tmr >= SBROS_PERIOD1) || (!(*first_sbros) && millis() - tmr >= SBROS_PERIOD2)) {
    *ping_flag = false;
    if (faza_sbrosa) {                                                   //питание подается
      digitalWrite(r_pins[MODEM_RELE_NUM-1], LOW);
      hold_timer = millis();
      faza_sbrosa = false;
    }

    else if (millis() - hold_timer >= POWER_RESET_TIME) {                //питание выключено
      digitalWrite(r_pins[MODEM_RELE_NUM-1], HIGH);
      faza_sbrosa = true;
      tmr = millis();
      *first_sbros = false;
      *ping_flag = true;
    }
  }

  else  *ping_flag = true;
}