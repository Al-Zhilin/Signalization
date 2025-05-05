void EEPROM_INIT() {
  EEPROM.put(1, Version);       //версия прошивки
  EEPROM.put(2, 30);            //период автовключения датчиков (0 - 255), минуты
  EEPROM.put(3, 5);             //период между сообщениями о сработках датчиков (0 - 255), секунды, всегда > 5, т.к датчики на +- 4с удерживают высокий сигнал при сработке
  EEPROM.put(4, 1);             //автовключени вкл/выкл (1/0) соответственно
  EEPROM.commit();
  bot.sendMessage("EEPROM инициализирована!", Users[0]);
}

void EEPROM_START() {
  EEPROM.get(2, PeriodVkl);
  EEPROM.get(3, PeriodSrb);
  EEPROM.get(4, a_flag);
}

void EEPROM_PUT(byte addr, byte num) {
  EEPROM.put(addr, num);
  EEPROM.commit();
  
  EEPROM_START();
}