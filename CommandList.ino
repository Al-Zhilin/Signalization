void Command_list(String user) {
  bot.sendMessage("                  Список всех команд:                    ", user);
  bot.sendMessage(F("Звездочками обозначены места, где вы должны вставить свои значения (0..255).     "), user);        
  bot.sendMessage(F("/res - перезагрузка платы;"), user);
  bot.sendMessage(F("/тех работы - уведомление пользователей при проведении работ;"), user);
  bot.sendMessage(F("/start - начать работу бота заново (та же перезагрузка, но с приветствием);"), user);
  bot.sendMessage(F("/setPeriod** - период включения датчиков после их выключения (мин);"), user);
  bot.sendMessage(F("/timerOn - включить автовключение датчиков;"), user);
  bot.sendMessage(F("/timerOff - выключить автовключение датчиков;"), user);
  bot.sendMessage(F("/setDelay** - задержка между сработками датчиков (сек);"), user);
  //bot.sendMessage("/noSystem - отключение показа системной информации (Alexey установил период..)", user);
  //bot.sendMessage("/system - включение показа системной информации (Alexey установил период..)", user);
  //bot.sendMessage("/info - доп. информация (SSID сети, уровень сигнала, ip адрес..)", user);
  bot.sendMessage(F("/fireOn - включение пожарных датчиков"), user);
  bot.sendMessage(F("/fireOff - отключение пожарных датчиков"), user);
  bot.sendMessage(F("/fireres - сброс питания пожарных датчиков"), user);
  //bot.sendMessage("/upd - показывает изменения, внесенные в последнем обновлении", user);
  bot.sendMessage(F("/limits - показ установленных таймеров (на сработки, включения датчиков и тд"), user);
} 