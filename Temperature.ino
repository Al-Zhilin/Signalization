void Temp() {
  if (!ENABLE_TERM1 && !ENABLE_TERM2) return;

  static bool error_flag[2] = {true, true};
  float new_znach[2] = {0, 0};
  
  if (ENABLE_TERM1) {
    if (!ds.readTemp(0)) {
      if (error_flag[0]) {
        bot.sendMessage("Ошибка чтения датчика 1", Users[0]);
        error_flag[0] = false;
      }
    }
    else {
      new_znach[0] = ds.getTemp();
      error_flag[0] = true;
    }
  }

  if (ENABLE_TERM2) {
    if (!ds.readTemp(1)) {
      if (error_flag[1]) {
        bot.sendMessage("Ошибка чтения датчика 2", Users[0]);
        error_flag[1] = false;
      }
    }
    else {
      new_znach[1] = ds.getTemp();
      error_flag[0] = true;
    }
  }

  ds.requestTemp();
  filtration(new_znach);
}

void filtration(float newVal[2]) {
  float k;
  
  for (byte i = 0; i < 2; i++) {
    if (abs(newVal[i] - temp[i]) > 1.0) k = 0.9;
    else if (abs(newVal[i] - temp[i]) > 0.5) k = 0.75;
    else k = 0.05;
    
    temp[i] += (newVal[i] - temp[i]) * k;
  }
}