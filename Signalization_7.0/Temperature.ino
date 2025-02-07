void Temp() {
  static bool error_flag = true;
  static float new_znach = 0;
  static float k;
  
  if (ds.readTemp()) { 
    new_znach = ds.getTemp();
    if (error_flag) error_flag = true;
  } 
  
  else {
    if (error_flag) {
      bot.sendMessage("Ошибка чтения датчика температуры!", Users[0]);
      error_flag = false;
    }
  }
  
  ds.requestTemp();  

  if (abs(new_znach - temp) > 0.7) k = 0.9;
  else k = 0.5;
  
  temp += (new_znach - temp) * k;
}