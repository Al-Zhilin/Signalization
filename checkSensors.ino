void checkSensors(bool *need_autoOn, uint32_t *on_timer) {
  String message = "";                  //в эту строку будем добавлять все сообщения о сработке датчика(-ов)
  *need_autoOn = false;
  static uint32_t srb_timer[sizeof(d_pins[0])/sizeof(d_pins[0][0])] = {};

  for (byte i = 0; i < sizeof(d_pins[0])/sizeof(d_pins[0][0]); i++) {                 //проходим по всем датчикам
    if (digitalRead(d_pins[0][i])) {                          //если датчик включен && датчик сработал
      if (datch[i] && millis() - srb_timer[i] >= PeriodSrb*1000 && SensorsFiltration(i)) {
        message += AlarmString(1, i);          //прогоняем через фильтр, отсеиваем (или хотя бы пытаемся) разные шумы
        srb_timer[i] = millis();
      }
      *on_timer = millis();
    }
    
    else if (!datch[i] && !(*need_autoOn)) {
      *need_autoOn = true;
      *on_timer = millis();
    }
  }

  if (message != "") {                     //если в строке хоть что-то есть
      bot.notify(true);
      bot.sendMessage(message + F("-----------------------------------\n") + AlarmString(3, false));
      bot.notify(false);
  }
}

void setSensorsHigh () {
  for (byte i = 0; i < sizeof(datch)/sizeof(datch[0]); i++) datch[i] = true;
  Sborka(1);
}

bool SensorsFiltration(byte snum) {                                           //функция фильтрации сработок датчиков
  static uint32_t last_high[sizeof(d_pins[0])/sizeof(d_pins[0][0])] = {};
  if (!d_pins[1][snum]) return true;
  if (millis() - last_high[snum] < SRB_PERIOD) return true;
  last_high[snum] = millis();
  return false;
}