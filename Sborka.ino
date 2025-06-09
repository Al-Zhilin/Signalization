void Sborka(bool vk_flag) {

  static byte old_faza[(sizeof(Users)/sizeof(Users[0]))] = {254};
  static byte old_datch[(sizeof(d_pins[0])/sizeof(d_pins[0][0]))];
  static byte old_rele[(sizeof(r_pins)/sizeof(r_pins[0]))]; 

  if (old_faza[0] == 254) {
    memset(old_faza, 255, (sizeof(Users)/sizeof(Users[0])));
    memset(old_datch, 255, (sizeof(d_pins[0])/sizeof(d_pins[0][0])));
    memset(old_rele, 255, (sizeof(r_pins)/sizeof(r_pins[0])));
  }

  for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
    //if (faza_menu[i] == old_faza[i] && faza_menu[i] && memcmp(old_datch, datch, (sizeof(d_pins[0])/sizeof(d_pins[0][0])))) faza_menu[i] = 0;    //возвращать пользователя к 0 фазе меню, при изменении кем-то состояний датчиков (проверить работоспособность)

    String main_menu = F("");

    switch (faza_menu[i]) {
      case 0:
        if (!memcmp(old_datch, datch, (sizeof(d_pins[0])/sizeof(d_pins[0][0]))) && faza_menu[i] == old_faza[i]) continue;

        for (byte j = 0; j < (sizeof(d_pins[0])/sizeof(d_pins[0][0])); j++) {
          main_menu += d_names[j];
    
          if (datch[j]) main_menu += " [✅] ";
          else  main_menu += " [❌] ";

          if (j % 3 == 0) main_menu += "\n";
          else  main_menu += "\t";
        }
      
      if (i < USERS_NUM)  main_menu += F("\n К управлению реле");
      break;


      case 1:
        if (!memcmp(old_rele, relays, (sizeof(d_pins[0])/sizeof(r_pins[0]))) && faza_menu[i] == old_faza[i]) continue;

        for (byte j = 0; j <(sizeof(r_pins)/sizeof(r_pins[0])); j++)  {
          if (j == FIRE_RELE_NUM-1 || j == MODEM_RELE_NUM-1) continue;

          main_menu += r_names[j];

          if (relays[j])  main_menu += " [✅] ";
          else  main_menu += " [❌] ";

          if (j < FIRE_RELE_NUM-1 && j < MODEM_RELE_NUM-1 && j % 3 == 2) main_menu += "\n";
          else if (j > FIRE_RELE_NUM-1 && j > MODEM_RELE_NUM-1 && (j-2) % 3 == 2) main_menu += "\n";
          else if ((j > FIRE_RELE_NUM-1 || j > MODEM_RELE_NUM-1) && (j-1) % 3 == 2) main_menu += "\n";
          else  main_menu += "\t";
        }
      main_menu += "\n На главную";
      break;
    }

    old_faza[i] = faza_menu[i];

    if (!vk_flag) {
      if (MenuID[i])  bot.deleteMessage(MenuID[i]);
      bot.showMenuText("Версия прошивки: " + String((Version * 0.1), 1), main_menu, Users[i]);
      MenuID[i] = bot.lastBotMsg();
    }

    else {
      if (MenuID[i])  bot.deleteMessage(MenuID[i]);
      bot.showMenuText("Датчики включены по таймеру в " + Date(2) + ". Версия прошивки: " + String((Version * 0.1), 1), main_menu, Users[i]);
      MenuID[i] = bot.lastBotMsg();
    }
  }

  memcpy(old_datch, datch, (sizeof(d_pins[0])/sizeof(d_pins[0][0])));
  memcpy(old_rele, relays, (sizeof(r_pins)/sizeof(r_pins[0])));
}


//старая сборка, пусть на всякий пока полежит, возможно при раскомментировании даже скомпилируется/заработает

/*void Sborka(bool vk_flag) {
  autovkl_flag = false;
  String main_menu = F("");
  
  for (byte i = 0; i < (sizeof(d_pins)/sizeof(d_pins[0])); i++) {
    main_menu += d_names[i];
    
    if (datch[i]) main_menu += " [✅] ";
    else  {
      main_menu += " [❌] ";
      autovkl_flag = true;
    }

    if (i % 3 == 0) main_menu += "\n";
    else  main_menu += "\t";
  }
  
  String ReleMenu = F("\n");

  for (byte i = 0; i < (sizeof(r_pins)/sizeof(r_pins[0])); i++) {
    if (i == FIRE_RELE_NUM-1) continue;

    ReleMenu += r_names[i];

    if (relays[i])  ReleMenu += " [✅] ";
    else  ReleMenu += " [❌] ";

    if (i < FIRE_RELE_NUM-1 && i % 3 == 2) ReleMenu += "\n";
    else if (i > FIRE_RELE_NUM-1 && (i-1) % 3 == 2) ReleMenu += "\n";
    else  ReleMenu += "\t";
  }
  bot.notify(false);

  if (!vk_flag) {
    for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
      if (MenuID[i])  bot.deleteMessage(MenuID[i]);
      if (i < USERS_NUM)  bot.showMenuText("Версия прошивки: " + String((Version * 0.1), 1), main_menu + ReleMenu, Users[i]);
      else  bot.showMenuText("Версия прошивки: " + String((Version * 0.1), 1), main_menu, Users[i]);
      MenuID[i] = bot.lastBotMsg();
    }
  }

  else {
    for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
      if (MenuID[i])  bot.deleteMessage(MenuID[i]);
      if (i < USERS_NUM)  bot.showMenuText("Датчики включены по таймеру в " + Date(2) + ". Версия прошивки: " + String((Version * 0.1), 1), main_menu + ReleMenu, Users[i]);
      else  bot.showMenuText("Датчики включены по таймеру в " + Date(2) + ". Версия прошивки: " + String((Version * 0.1), 1), main_menu, Users[i]);
      MenuID[i] = bot.lastBotMsg();
    }
  }

  bot.notify(true);
}*/
