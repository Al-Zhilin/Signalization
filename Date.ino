String Date(byte d_mode) {        //0 - для сообщения, 1 - дата и время,  2 - чисто время
  FB_Time t = bot.getTime(3);
  String DaT = F("");
  
  if (!d_mode) {
    DaT += F("Дата: ");
    DaT += t.dateString();
    DaT += ", ";
    DaT += t.timeString()[0]; 
    DaT += t.timeString()[1]; 
    DaT += t.timeString()[2]; 
    DaT += t.timeString()[3]; 
    DaT += t.timeString()[4];
    DaT += F(".\n");
 
    if (ENABLE_TERM1) {
      DaT += TERM1_NAME;
      DaT += ": ";
      DaT += temp[0];
      DaT += F("°С,  ");
      DaT += "\n";
    }

    if (ENABLE_TERM2) {
      DaT += TERM2_NAME;
      DaT += ": ";
      DaT += temp[1];
      DaT += F("°С,  ");
      DaT += "\n";
    }
    
    DaT += F("Ping: ");
    DaT += average_ping;

    for (byte i = 0; i < (sizeof(Users)/sizeof(Users[0])); i++) {
      bot.editMessage(TimeID[i], DaT);
    }
    return "";
  }

  if (d_mode == 1) {
    DaT += t.dateString();
    DaT += ", ";
    DaT += t.timeString();
    return DaT;
  }

  if (d_mode == 2) {
    return t.timeString();
  }

  bot.sendMessage("Ошибка Date!", Users[0]);
  return "";
}