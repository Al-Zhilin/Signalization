Signalization
=

Охранная сигнализация на основе микроконтроллера esp32
-

Краткий обзор возможностей:
- Управление всеми функциями через чат с телеграмм ботом
- Уведомления о сработке датчиков движения
- Уведомления о сработке пожарных датчиков (датчиков дыма/огня)
- Отслеживание температуры окружающего воздуха (датчик DS18) и построение графиков на open.monitoring.online
- Отслеживание связи с WiFi, проверка наличия выхода в интернет, перезагрузка модема/роутера при потере связи
- Поддержка настраиваемого количества пользователей, выбор количества "админов" среди них
- Запись в EEPROM память важных настроек, возможность их изменения без перепрошивки
- Обновление прошивки через ArduinoOTA (в локальной сети) и с помощью бинарного файла через чат


Нам понадобится:
-
- ESP32
- Созданный бот (инструкцию по созданию и настройке можно найти [здесь](https://kit.alexgyver.ru/tutorials/telegram-basic/)
- Релейный модуль/модули
- Датчики движения
- Пожарные датчики (ИП212-45)
- Несколько номиналов резисторов
- Постоянное питание
- Постоянное наличие интернет-соединения
- Температурный датчик DS18 (опционально)

Краткий обзор кода:
-

Обязательные дефайны (проект не запуститься без них):
-
    #define ssid ""            //имя WiFi сети
    #define password ""        //пароль от WiFi сети
    #define BOT_TOKEN ""       //идентификатор бота в телеге (получаем у BotFather после создания)

Важные дефайны (проект будет работать неправильно без их настройки):
-
Необходимо заменить значения по умолчанию своими данными:

    #define FIRE_SENSOR1 39        //пин, к которому подключен 1-й датчик дыма
    #define FIRE_SENSOR2 36        //пин, к которому подключен 2-й датчик дыма
    #define RELAY1 4               //пин управления первым реле
    #define RELAY2 18              //пин управления вторым реле
    #define RELAY3 17              //пин управления третьим реле
    #define RELAY4 16              //пин управления четвертым реле
    #define DATCH_HOME 22          //пин, к которому подключен домашний датчик движения
    #define DATCH_1 23             //пин, к которому подключен первый датчик движения
    #define DATCH_2 25             //пин, к которому подключен второй датчик движения
    #define DATCH_3 26             //пин, к которому подключен третий датчик движения
    #define ThermoPin 32           //пин, к которому подключен датчик температуры
    #define USERS_NUM 2            //кол-во пользователей (с начала общего списка), которым будет показана часть меню с управлением реле, от которых можно обновлять прошивку и т.д.
    #define FIRE_RELE_NUM 1        //номер реле, через которое питаются датчики дыма (нумерация с 1)
    #define MODEM_RELE_NUM 2       //номер реле, через которое питается модем (нумерация с 1)

Остальные дефайны можно оставить по умолчанию, либо же изменить, руководствуясь приведенными в коде комментриями.

Как увеличить/уменьшиить количество датчиков/реле:
-
- 1. Делаем новый дефайн, содержащий номер пина, к которому подключчен этот датчик (например #define FIRE_SENSOR3 39)
- 2. Добавляем его в соответствующий список (например, для датчиков движения, в const byte d_pins[])
- 3. Если добавляем реле, то придумываем ему имя и вписываем его в const char* r_names[], если это датчик движения, то имя вписываем в const char* d_names[]
- 4. Во вкладке AlarmString, руководствуясь комментариями (добавляем новый case в конструкцию), придумываем текст, который будет содержаться в сообщении о сработке датчиков дыма и движения

Для уменьшения количества датчиков/реле действуем по аналогичному принципу в обратную сторону.

Добавление пользователей и задания количества "админов":
-
- id пользователей добавляются в const String Users[], id пользователя можно узнать например через [него](https://web.telegram.org/a/#6271337642) или другого подобного бота.
- Количество админов задается дефайном:
    -     #define USERS_NUM 2    //кол-во пользователей (с начала общего списка), которым будет показана часть меню с управлением реле, от которых можно обновлять прошивку и т.д. 


