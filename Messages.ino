void newMsg(FB_msg& msg) {

  bool sb_flag = false;

  if (msg.unix < startUnix) return;

  if (msg.text == "/res") {
    bot.sendMessage("Есть!", msg.chatID);
    ESP.restart();
  }

  else if (msg.text == "/start") {
    bot.sendMessage("Здравствуйте!", msg.chatID);
    bot.sendMessage(msg.username + " присоединился к нам!", Users[0]);
    ESP.restart();
  }

  else if (msg.text == "/limits") {
    bot.sendMessage("Установленные таймеры:", msg.chatID);
    bot.sendMessage("Между сработками - " + String(EEPROM.read(3)) + "/" + String(PeriodSrb) + " секунд", msg.chatID);
    bot.sendMessage("Между автовключениями датчиков - " + String(EEPROM.read(2)) + "/" + String(PeriodVkl) + " минут", msg.chatID);
    bot.sendMessage("Версия (без разделителей) - " + String(EEPROM.read(1)), msg.chatID);
    bot.sendMessage("Автовключение: " + String((autovkl_flag) ? "включено" : "выключено"), msg.chatID);
  }

  else if (msg.text == "/тех работы") {
    bot.sendMessage("!! Проводятся технические работы !!");
  }

  else if (msg.text == "/list") {
    Command_list(msg.chatID);
  }

  else if (msg.OTA) {
    for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
      if (i >= USERS_NUM) break;
      if (msg.chatID == Users[i]) bot.update();
    }
  }

  else if (msg.text == "/fireOn")  {
    digitalWrite(r_pins[FIRE_RELE_NUM-1], HIGH);
    fire_on = true;
    bot.sendMessage(AlarmString(2, 1));
  }

  else if (msg.text == "/fireOff")  {
    digitalWrite(r_pins[FIRE_RELE_NUM-1], HIGH);
    fire_on = false;
    bot.sendMessage(AlarmString(2, 2));
  }

  else if (msg.text == "/fireres") {
    fireres_flag = true;
    bot.sendMessage(AlarmString(2, 0));
  }

  else if (msg.text.startsWith("/setDelay")) {
    byte parse_mess = 0;
    if (msg.text == "/setDelay") {
      bot.sendMessage("После /setDelay должно идти значение времени!", msg.chatID);
      return;
    }

    if (msg.text.length() > 11) {
      bot.sendMessage("Число должно быть двузначным!", msg.chatID);
      return;
    }

    parse_mess = msg.text[9] - '0';
    if (msg.text.length() > 10) {
      parse_mess *= 10;
      parse_mess += msg.text[10] - '0';
    }

    if (PeriodSrb != parse_mess)  EEPROM_PUT(3, parse_mess);

    bot.sendMessage("Установили период между сработками: " + String(PeriodSrb) + " сек.", msg.chatID);
  }

  else if (msg.text.startsWith("/setPeriod")) {
    byte parse_mess = 0;
    if (msg.text == "/setPeriod") {
      bot.sendMessage("После /setPeriod должно идти значение времени!", msg.chatID);
      return;
    }

    if (msg.text.length() > 12) {
      bot.sendMessage("Число должно быть двузначным!", msg.chatID);
      return;
    }

    parse_mess = msg.text[10] - '0';
    if (msg.text.length() > 11) {
      parse_mess *= 10;
      parse_mess += msg.text[11] - '0';
    }

    if (PeriodVkl != parse_mess)  EEPROM_PUT(2, parse_mess);


    bot.sendMessage("Установили автовключение через: " + String(PeriodVkl) + " мин.", msg.chatID);
  }

  else if (msg.text == "/timerOn")  {
    if (!autovkl_flag)  {
      autovkl_flag = true;
      EEPROM_PUT(4, 1);
      bot.sendMessage("Автовключение теперь активно!", msg.chatID);
      if (msg.chatID != Users[0]) bot.sendMessage(msg.username + " активировал автовключение!", Users[0]);
    }
    bot.deleteMessage(bot.lastUsrMsg());
  }

  else if (msg.text == "/timerOff")  {
    if (autovkl_flag)  {
      autovkl_flag = false;
      EEPROM_PUT(4, 0);
      bot.sendMessage("Автовключение теперь неактивно!", msg.chatID);
      if (msg.chatID != Users[0]) bot.sendMessage(msg.username + " отключил автовключение!", Users[0]);
    }
    bot.deleteMessage(bot.lastUsrMsg());
  }

  else if (msg.text == "К управлению реле") {
    for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
      if (Users[i] == msg.chatID) {
        faza_menu[i] = 1;
        Sborka(0);
        break;
      }
    }
    bot.deleteMessage(bot.lastUsrMsg());
  }

  else if (msg.text == "На главную") {
    for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
      if (Users[i] == msg.chatID) {
        faza_menu[i] = 0;
        Sborka(0);
        break;
      } 
    }
    bot.deleteMessage(bot.lastUsrMsg());
  }

  for (byte i = 0; i < (sizeof(d_pins)/sizeof(d_pins[0])); i++) {
    if (msg.text.startsWith(d_names[i])) {
      bot.deleteMessage(bot.lastUsrMsg());
      datch[i] = !datch[i];
      sb_flag = true;
    }
  }

  for (byte i = 0; i < (sizeof(r_pins)/sizeof(r_pins[0])); i++) {
    if (msg.text.startsWith(r_names[i])) {
      bot.deleteMessage(bot.lastUsrMsg());
      relays[i] = !relays[i];
      digitalWrite(r_pins[i], !relays[i]);
      sb_flag = true;
    }
  }

  if (sb_flag) {
    sborka_timer = millis();
    sborka_flag = true;
  }
  FB_Time t(msg.unix, 3);
}