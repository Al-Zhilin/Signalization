#define ssid ""                                                                //имя WiFi сети
#define password ""                                                            //пароль от WiFi сети
#define BOT_TOKEN ""                                                           //идентификатор бота в телеге
#define MONITORING "" 							                                           //начало запроса на OpenMonitoring
#define TERM1_NAME "Дом"                                                       //имя для первого температурного датчика
#define TERM1_NAME "Улица"                                                     //имя для второго температурного датчика
#define OTA_NAME ""						                                                 //имя для OTA
#define OTA_PASS ""		                    			                               //пароль для OTA
 
 
const String Users[] = {
  "",   // админ
  "",
  "",
};
 
const char* d_names[]  = {
  "Датч0",
  "Датч1",
  "Датч2",
  "Датч3",
};
 
const char* r_names[]  = {
  "Реле1",
  "Реле2",
  "Реле3",
  "Реле4",
};
 
uint64_t addr[] = {
    0x760000006AB3CC28,     //1 датчик
    0x950000006B043C28,     //2 датчик
};