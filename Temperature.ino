void filtration(float newVal[ENABLED_TERMS]);     //прототип функции. О чудо, компилятор ругается на отсутствие прототипа, хотя в Arduino IDE должен делать его сам. Магия :)

void Temp() {
  static bool error_flag[ENABLED_TERMS] = {false};
  float new_znach[ENABLED_TERMS] = {};
  if (!error_flag[0]) for (byte i = 0; i < ENABLED_TERMS; i++)  error_flag[i] = true;

  #if (ENABLED_TERMS == 1)
      if (ds.readTemp()) {
          new_znach[0] = ds.getTemp();
          error_flag[0] = true;
      } else {
          if (error_flag[0]) {
            bot.sendMessage("Ошибка чтения 1 датчика температуры!", Users[0]);
            error_flag[0] = false;
        }
      }
  #endif

  #if (ENABLED_TERMS >= 2)
    for (byte iter = 0; iter < ENABLED_TERMS; iter++) {
      if (ds.readTemp(iter)) {
          new_znach[iter] = ds.getTemp();
          error_flag[iter] = true;
      } else {
          if (error_flag[iter]) {
            bot.sendMessage("Ошибка чтения " + String(iter+1) + " датчика температуры!", Users[0]);
            error_flag[iter] = false;
        }
      }
    }
  #endif

  ds.requestTemp();
  filtration(new_znach);
}

void filtration(float newVal[ENABLED_TERMS]) {
  float k;
  
  for (byte i = 0; i < ENABLED_TERMS; i++) {
    if (abs(newVal[i] - temp[i]) > 1.0) k = 0.9;
    else if (abs(newVal[i] - temp[i]) > 0.5) k = 0.75;
    else k = 0.05;
    
    temp[i] += (newVal[i] - temp[i]) * k;
  }
}