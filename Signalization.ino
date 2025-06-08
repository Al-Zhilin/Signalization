#include <ArduinoOTA.h>
#include <ESP32Ping.h>
#include <EEPROM.h>
#include <FastBot.h>
#include <GyverDS18Array.h>
#include "passwords.h"
 
#define WIFI_RES_PERIOD 2 * 60 * 1000                                          //если по истечении этого периода (после начала попыток подключения) к WiFi не получиться подключиться, плата будет перезагружена
#define INIT_KEY 2                                                             //изменить ключ, чтобы инициализировать EEPROM
#define FIRE_SENSOR1 39                                                        //пин, к которому подключен 1-й датчик дыма
#define FIRE_SENSOR2 36                                                        //пин, к которому подключен 2-й датчик дыма
#define RELAY1 4                                                               //пин управления первым реле
#define RELAY2 18                                                              //пин управления вторым реле
#define RELAY3 17                                                              //пин управления третьим реле
#define RELAY4 16                                                              //пин управления четвертым реле
#define DATCH_HOME 22                                                          //пин, к которому подключен домашний датчик движения
#define DATCH_1 23                                                             //пин, к которому подключен первый датчик движения
#define DATCH_2 25                                                             //пин, к которому подключен второй датчик движения
#define DATCH_3 26                                                             //пин, к которому подключен третий датчик движения
#define VOLT_SENSOR 13                                                         //пин, к которому подключено реле контроля напряжения
#define ThermoPin 32                                                           //пин, к которому подключен(-ы) датчик(-и) температуры
#define Version 23                                                             //текущая версия прошивки (указывать целым числом, без точки)   !! из-за ограничений byte может быть 0-255, поломается как миннимум EEPROM !!
#define IS_VERSION_UPDATE 0                                                    //включение/отключение сообщений "Система обновлена...". Запись/чтение версий в/из память(-и) не будет прекращаться при любом значении
#define PING_PERIOD 2 * 60 * 1000                                              //период между ping-запросами, 1 число - минуты
#define HTTP_PERIOD 1.5 * 60 * 1000                                            //период отправки данных на Open Monitoring (должен быть всегда > 1 минуты), 1 число - минуты
#define PERIOD_OPROSA 200                                                      //период опроса датчиков движения, миллисекунды
#define SRB_PERIOD 10 * 1000                                                   //допустимый период, в течение которого все сработки, идущие после первой, будут посыласться пользователю (считаться не ложными)
#define MESS_UPD 10 * 1000                                                     //период обновления сообщения (дата, время, температура..)
#define SBORKA_PERIOD 7 * 1000                                                 //период, по истечении которого (без изменений состояний датчиков) меню будет пересобрано
#define FIRE_READ_PERIOD 500                                                   //период опроса датчиков дыма
#define FIRE_RESET_PERIOD 2500                                                 //время сброса питания пожарных датчиков
#define USERS_NUM 2                                                            //кол-во пользователей (с начала общего списка), которым будет показана часть меню с управлением реле, от которых можно обновлять прошивку и т.д.
#define FIRE_RELE_NUM 1                                                        //номер реле, через которое питаются датчики дыма (нумерация с 1)
#define FIRE_MESS_PERIOD 3 * 1000                                              //период между сообщениями о сработке пож. датчика(-ов) (1 число - секунды)
#define SBROS_MODEMA 1                                                         //включить функцию сброса питания модема при отсутствиие интернета
#define RESET_REASON 0                                                         //0 - модем будет сброшен при ЛЮБОМ отсутствии интернета, 1 - при отсутствии интернета ТОЛЬКО И СРАЗУ после запуска системы
#define SBROS_PERIOD1 2 * 60 * 1000                                            //период перед первой перезагрузкой модема (только при включенной функции), 1 число - минуты
#define SBROS_PERIOD2 20 * 60 * 1000                                           //период между последующими попытками перезагрузки модема (только при включенной функции), 1 число - минуты
#define POWER_RESET_TIME 10 * 1000                                             //период на который будет отключено питание модема, 1 число - секунды
#define MODEM_RELE_NUM 2                                                       //номер реле, через которое питается модем (нумерация с 1)
#define NO_ELECTRICITY 0                                                       //вкл/выкл (1/0 соотв.) уведомлений об изменении наличия напряжения питающей сети (полезно только при наличии бесперебойника на питание платы)
#define ENABLE_TERM1 1                                                         //подключить температурный датчик 1
#define ENABLE_TERM2 0                                                         //подключить температурный датчик 2

const byte r_pins[] = {RELAY1, RELAY2, RELAY3, RELAY4}, f_pins[] = {FIRE_SENSOR1, FIRE_SENSOR2};
const byte d_pins[2][4] = {{DATCH_HOME, DATCH_1, DATCH_2, DATCH_3}, {false, false, true, true}};

FastBot bot(BOT_TOKEN);
GyverDS18Array ds(14, addr, 3);

byte PeriodVkl = 0, PeriodSrb = 0;  
int32_t MenuID[(sizeof(Users) / sizeof(Users[0]))] = {}, TimeID[(sizeof(Users) / sizeof(Users[0]))] = {};
uint32_t startUnix = 0, sborka_timer = 0;
bool relays[(sizeof(r_pins) / sizeof(r_pins[0]))] = {}, datch[(sizeof(d_pins[0]) / sizeof(d_pins[0][0]))] = {}, faza_menu[(sizeof(Users) / sizeof(Users[0]))] = {}, sborka_flag = false, fire_on = true, fireres_flag = false, a_flag = true;
float average_ping = 0, temp[2] = {};



void setup() {
  Serial.begin(115200);
  ConnectWiFi();
  
  ArduinoOTA.setHostname(OTA_NAME);
  ArduinoOTA.setPassword(OTA_PASS);
  ArduinoOTA.begin();
  
  EEPROM.begin(11);

  String str = "";
  for (byte i = 0; i < (sizeof(Users) / sizeof(Users[0])); i++) {
    str += Users[i];
    str += ",";
  }

  bot.setChatID(str);
  bot.clearServiceMessages(true);                                                                           
  bot.setLimit(4);
  bot.notify(false);

  if (EEPROM.read(0) != INIT_KEY) {
    EEPROM.put(0, INIT_KEY);
    EEPROM_INIT();
  }

  bot.unpinAll();
  bot.notify(false);

  bot.sendMessage("---------------------- Здравствуйте! -----------------------\nДля просмотра списка команд жми: /list");

  delay(1000);      //самая глупая, но работающая заплатка в мире :D

  for (byte i = 0; i < (sizeof(Users) / sizeof(Users[0])); i++) {
    bot.sendMessage("скоро все будет...", Users[i]);
    bot.pinMessage(bot.lastBotMsg());
    TimeID[i] = bot.lastBotMsg();
  }

  bot.notify(true);
  EEPROM_START();
  //ds.setResolution(12);
  ds.requestTemp();
  Start();

  startUnix = bot.getUnix();
  bot.attach(newMsg);

  Sborka(0);
}

void loop() {
  static bool internet = Pingyem(), int_flag = true, start_inter = internet, need_autoOn = false;
  static uint32_t ping_timer = millis(), http_timer = millis(), mess_timer = millis(), fire_read_timer = millis(), twi = 0, sensor_read_timer = millis(), on_timer = millis();
  static int32_t time_no_int = 0;

  ArduinoOTA.handle();

  bot.tick();

  if (millis() - ping_timer >= PING_PERIOD) {             //пингуем сеть (проверка наличия интернет соединения)
    ping_timer = millis();
    if (WiFi.status() != WL_CONNECTED) {        //если проблема с интернетом заключается в отсутствующем подключении к WiFi - переподключаемся
      WiFi.disconnect();
      ConnectWiFi();
    }
    internet = Pingyem();
  }

  while (!internet) {
    static bool allow_ping = true;
    static bool first_sbros = true;

    if (int_flag) {
      twi = millis();
      int_flag = false;
    }

    if (SBROS_MODEMA) {
      if ((!RESET_REASON && !internet) || (RESET_REASON && !start_inter)) {
        SbrosModema(&allow_ping, &first_sbros);
      }
    }

    if (WiFi.status() != WL_CONNECTED) {
      WiFi.disconnect();
      ConnectWiFi();
    }

    if (allow_ping && millis() - ping_timer >= 20 * 1000) {           //пингуем сеть (проверка наличия интернет соединения)
      ping_timer = millis();
      internet = Pingyem();
    }

    if (internet) first_sbros = true;
  }

  if (internet && !start_inter) ESP.restart();        //если интернета сразу после запуска платы не было, а теперь появился (перезапускаемся для корректной отправки всех сообщений)

  if (internet && !int_flag) {                  //когда интернета не было, а теперь он появился (можно то-нибудь делать с данными о времени без подключения, например собирать статистику)
    //чото можно делать
    time_no_int = (millis() - twi) / 60000;
    int_flag = true;
  }

  if (NO_ELECTRICITY) {                         //если включена функция уведомлений об отсутствии сетевого напряжения
    static bool volt_flag = false;
    bool volts = digitalRead(VOLT_SENSOR);
    if (volts && !volt_flag) {
      volt_flag = true;
      bot.sendMessage(AlarmString(0, 1));
    }

    else if (!volts && volt_flag) {
      volt_flag = false;
      bot.sendMessage(AlarmString(0, 0));
    }
  }

  if (millis() - sensor_read_timer >= PERIOD_OPROSA) {
    sensor_read_timer = millis();
    checkSensors(&need_autoOn, &on_timer);
  }

  if (a_flag && need_autoOn && millis() - on_timer >= PeriodVkl*1000*60) {
    need_autoOn = false;
    setSensorsHigh();
  }

  if (ds.ready()) Temp();
  
  if (millis() - http_timer >= HTTP_PERIOD) {         //Отправляем данные на OpenMonitoring по таймеру
    http_timer = millis();
    HTTPGET(temp[0], temp[1], time_no_int);
    time_no_int = 0;
  }

  if (millis() - mess_timer >= MESS_UPD) {            //обновляем сообщение со временем и температурой по таймеру
    mess_timer = millis();
    Date(0);
  }

  if (sborka_flag && millis() - sborka_timer >= SBORKA_PERIOD) {                  //пересобираем меню если нужно и подошло время по таймеру
    sborka_timer = millis();
    sborka_flag = false;
    Sborka(0);
  }

  if (fire_on && millis() - fire_read_timer >= FIRE_READ_PERIOD) {               //по таймеру следим за состоянием пожарных датчиков
    fire_read_timer = millis();
    Fire(0);
  }

  if (fireres_flag) Fire(1);
}
