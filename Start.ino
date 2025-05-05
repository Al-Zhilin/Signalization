void Start() {

  pinMode(DATCH_HOME, INPUT_PULLUP); 
  pinMode(DATCH_1, INPUT_PULLUP);
  pinMode(DATCH_2, INPUT_PULLUP);
  pinMode(DATCH_3, INPUT_PULLUP);
  pinMode(VOLT_SENSOR, INPUT_PULLUP);
  pinMode(ThermoPin, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  pinMode(FIRE_SENSOR1, INPUT);
  pinMode(FIRE_SENSOR2, INPUT);

  if (Version != EEPROM.read(1)) {
    if (IS_VERSION_UPDATE && Version > EEPROM.read(1)) bot.sendMessage("Система обновлена до версии: " + String((Version * 0.1), 1));
    EEPROM_PUT(0, Version);
  }
}