void Fire(bool f_mode) {    // 0 - опрос датчиков,   1 - сброс питания
  static uint32_t mess_delay = millis(), fire_res_timer = millis();
  static bool fres_mode = false, f1_flag = false, f2_flag = false;
  String message = "";

  if (!f_mode && millis() - mess_delay >= FIRE_MESS_PERIOD) {
    f1_flag = false;
    for (byte i = 0; i < (sizeof(f_pins)/sizeof(f_pins[0])); i++) {
      if (digitalRead(f_pins[i])) {
        if (f2_flag)  {
          message += AlarmString(2, 3+i);
        }
        f1_flag = true; 
        if (f2_flag)  mess_delay = millis();
      }
    }
    f2_flag = f1_flag;

    if (message != "") {
      bot.notify(true);
      bot.sendMessage(message + F("Сброс: /fireres \n-----------------------------------\n") + AlarmString(3, false));
      bot.notify(false);
    }
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