void ConnectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint32_t start_time = millis();
  
  while (WiFi.status() != WL_CONNECTED && millis() - start_time < WIFI_RES_PERIOD) {        
    delay(1000);
  }

  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
} 