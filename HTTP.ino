void HTTPGET(float temp1, float temp2, int tni) {
  static bool get_failed = false, fail_flag = true;

  String req = ; 
  req += temp1;
  req += "&p2=";
  req += tni;
  req += "&p4=";
  req += average_ping;
  req += "&p5=";
  req += temp2;
    
  HTTPClient http;
  http.begin(req);
    
  int result = http.GET();
    
  if (result <= 0) {
     if (get_failed)  {
        if (fail_flag) {
          //bot.sendMessage("Ошибка HTTP-запроса", Users[0]);  закомментировано, из-за частных ошибок отправки. Пробуйте раскомментировать, если не будет постоянных ошибок
          fail_flag = false;
        }
     }
     get_failed = true;      
  }
  else {
    fail_flag = true;
    get_failed = false;
  }
  
  http.end();
}