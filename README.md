Signalization
=

Охранная сигнализация на основе микроконтроллера esp32
-

Краткий обзор возможностей:
- Управление всеми функциями через чат с телеграмм ботом
- Уведомления о сработке датчиков движения
- Уведомления о сработке пожарных датчиков (датчиков дыма/огня)
- Отслеживание температуры окружающего воздуха (датчик DS18b20) и построение графиков на open.monitoring.online
- Отслеживание связи с WiFi, проверка наличия выхода в интернет, перезагрузка модема/роутера при потере связи
- Поддержка настраиваемого количества пользователей, выбор количества "админов" среди них
- Запись в EEPROM-память важных настроек, возможность их изменения без перепрошивки
- Обновление прошивки через ArduinoOTA (в локальной сети) и с помощью бинарного файла через чат (на любом расстоянии)


Нам понадобится:
-
- ESP32 (WROOM-32 или взаимозаменяемые)
- Созданный бот (инструкцию по созданию и настройке можно найти [здесь](https://kit.alexgyver.ru/tutorials/telegram-basic/))
- Релейный модуль/модули
- Датчики движения
- Пожарные датчики (ИП212-45)
- Несколько номиналов резисторов
- Постоянное питание
- Постоянное наличие интернет-соединения
- Температурный датчик DS18b20 (опционально)

Краткий обзор настроек:
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
    #define MODEM_RELE_NUM 2       //номер реле, через которое питается роутер/модем (нумерация с 1)

Остальные дефайны можно оставить по умолчанию, либо же изменить, руководствуясь приведенными в коде комментриями.
-

Как увеличить/уменьшиить количество датчиков/реле:
-
- 1. Делаем новый дефайн, содержащий номер пина, к которому подключчен этот датчик (например #define FIRE_SENSOR3 39)
- 2. Добавляем его в соответствующий список (например, для датчиков движения, в

         const byte d_pins[])

- 3. Если добавляем реле, то придумываем ему имя и вписываем его в
     
         const char* r_names[],
     
если это датчик движения, то имя вписываем в 

         const char* d_names[]
         
- 4. Во вкладке AlarmString, руководствуясь комментариями (добавляем новый case в конструкцию), придумываем текст, который будет содержаться в сообщении о сработке датчиков дыма и движения

Для уменьшения количества датчиков/реле действуем по аналогичному принципу в обратную сторону.

Добавление пользователей и задание количества "админов":
-
- id пользователей добавляются в const String Users[], id пользователя можно узнать например через [него](https://web.telegram.org/a/#850434834) или [него](https://web.telegram.org/a/#6271337642) или через другого подобного бота.
- Количество админов задается дефайном:
    -     #define USERS_NUM 2    //кол-во пользователей (с начала общего списка), которым будет показана часть меню с управлением реле, от которых можно обновлять прошивку и т.д.

Настройка сервиса OpenMonitoring для работы с проектом:
-
Для того, чтобы иметь возможноть удаленно отслеживать графики изменения температуры окржуающего воздуха (датчик DS18), а также време без сети (см. ниже), нам нужно зарегистрироваться [на cервисе Open-Monitoring.online](https://open-monitoring.online/), а так же получить оттуда некоторые данные. Подробно процесс регистрации и создания нового устройства описан [здесь](https://dzen.ru/a/YzaJExSbgyjhJF_u), рекомендую на этапе создания параметров создать дополнительные, по 1-2 каждого типа (целые, с плавающей точкой и строковые). Их можно скрыть от видимости и они не будут мешать, а если не взять такой запас, в будущих обновлениях, возможно, вы не сможете использовать новые функции. Созданные параметры должны выглядеть следующим образом:

![image](https://github.com/user-attachments/assets/99e5ce09-e73a-4bd2-8aed-4da1f2f9cb6a)


После создания нового контроллера на сервере, раскрываем меню действий с ним -> "о контроллере" и копируем поле с заголовком "Код для отправки GET-запросом". 

![777](https://github.com/user-attachments/assets/b26f9eab-9033-4814-8e5b-54ee993110fc)

Где после cid и key будут идти ваши уникальные значения, позволяющие добавлять, с помощью http запроса, данные на сервер.
Копируем эту строку, вплость до &p1= во вкладку HTTP.ino, а именно в эти кавычки:

        String req = ""; 

Если вы создавали все параметры как на фото выше (порядок, имена и т.д.), то остальные параметры в этой вкладке можно не трогать. Иначе вам придется немного видоизменить поля сборки запроса.

Уведомления о потере связи с сигнализацией:
-
- Следите за обновлениями

Печатная плата и подключение компонентов:
-
- Компоненты подключаются приблизительным образом (для контактов, установленных по умолчанию):

![image](https://github.com/user-attachments/assets/7d2c18b1-0aeb-4898-8eaa-c8ac5e8a4074)

Питание пожарных датчиков осуществляется через нормально замкнутый контакт реле. Если у вас подключено несколько релейных модулей, то тот, через который питаются датчики, должен быть указан в:

        #define FIRE_RELE_NUM 1        //номер реле, через которое питаются датчики дыма (нумерация с 1)

Аналогичная история получается и с питание модема, если вы хотите осуществлять его программный сброс.

В зависимости от типа пожарных датчиков, у них может быть разное напряжение питания, а так же тип сигнала, который они выдают при сработке. Если у вас датчик с "сухим контактом", то вам повезло. В вашем случае датчик будет размыкать, например, пин 3.3V. Если датчик, похожий по типу срабатывания на ИП212-45, тогда есть возможность регистрировать его сработки, подогнав напряжение сработки под максимум 3.3V (максимальное входное напрыяжение на пины esp32) с помощью резисторного делителя напряжения (вот [тут](https://www.chipdip.ru/calc/voltage-divider?Uin=12&Uin_Unit=0&Uout=3.3&Uout_Unit=0&R2=1&R2_Unit=3) можно рассчитать сопротивления вашего делителя).

Как это работает?
-
- Информация скоро обновится.

