bool Sensors(byte s_mode) {     //0 - проверка на отправку сообщений, 1 - автовключить датчики (всегда при вызове)
  static uint32_t srb3_timer[2] = {millis(), millis()};
  String message = "";

  if (!s_mode) {
    bool al_flag = false;
    for (byte i = 0; i < (sizeof(d_pins)/sizeof(d_pins[0])); i++) {
      if (digitalRead(d_pins[i]) && datch[i]) {
        if (i == 3 or i == 2) {
          if (millis() - srb3_timer[i-2] < PeriodSrb * 3000) {     //времени с момента последней сработки прошло меньше, чем тройной период отправки сообщений
            message += AlarmString(1, i);
          }
          srb3_timer[i-2] = millis();
        }
        else  message += AlarmString(1, i);
        al_flag = true;
      }
    }
    if (message != "") {
      bot.notify(true);
      bot.sendMessage(message + F("-----------------------------------\n") + AlarmString(3, false));
      bot.notify(false);
    }
    return al_flag;
  }

  if (s_mode == 1) {
    autovkl_flag = false;
    for (byte i = 0; i < (sizeof(d_pins)/sizeof(d_pins[0])); i++) {
      datch[i] = true;
    }
    Sborka(1);
    return false;
  }
  return false;   //любое вроде можно (true/false)
}