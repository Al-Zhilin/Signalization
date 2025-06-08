void Fire(bool f_mode) {    // 0 - опрос датчиков,   1 - сброс питания
  static uint32_t mess_delay = millis(), fire_res_timer = millis();
  static bool fres_mode = false;
  String message = "";

  for (byte i = 0; i < (sizeof(f_pins)/sizeof(f_pins[0])); i++) {
    if (digitalRead(f_pins[i])) {
      if (FireSensFiltration(i) && millis() - mess_delay >= FIRE_MESS_PERIOD) {
        message += AlarmString(2, 3+i);
        mess_delay = millis();
      }
    }
  }

  if (message != "") {
    bot.notify(true);
    bot.sendMessage(message + F("Сброс: /fireres \n-----------------------------------\n") + AlarmString(3, false));
    bot.notify(false);
  }

  if (f_mode) {
    if (!fres_mode) {
      digitalWrite(r_pins[FIRE_RELE_NUM-1], LOW);
      fres_mode = true;
      fire_res_timer = millis();
    }
    if (fres_mode && millis() - fire_res_timer >= FIRE_RESET_PERIOD) {
      fres_mode = false;
      fireres_flag = false;
      digitalWrite(r_pins[FIRE_RELE_NUM-1], HIGH);
    }
  }
}

bool FireSensFiltration(byte num) {
  static uint32_t high_period[sizeof(f_pins)/sizeof(f_pins[0])] = {}, last_call[sizeof(f_pins)/sizeof(f_pins[0])] = {};

  if (millis() - last_call[num] <= 500) {
    if (millis() - high_period[num] > 2000) return true;
  }

  else high_period[num] = millis();
  last_call[num] = millis();

  return false;
}