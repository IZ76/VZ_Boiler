# VZ_Boiler
Данный код написан в приложении Arduino IDE для платы ESP8266.
Настройку и управление устройством возможно производить или через WEB интерфейс, 
или через Android приложение https://play.google.com/store/apps/details?id=com.vz.kitchen
При первом включении устройство создает свою собственную точку доступа для настроки подключения к существующей WiFi сети.
Минимально воможно использовать устройство как модуль управления бойлером по времени.
Вы можете выставить до 5 временных интервалов для установки заданной температуры нагрева воды.
Для этого будет достаточно использовать сам модуль ESP8266 (ESP12, NodeMCU  и прочие) + часовой модуль (DS1307-DS3231) + модуль реле + термодатчик DS18B20
Схема проста как и в любом ардуино проекте
<a href="http://piccy.info/view3/13776070/e8a9d4927672ad9b3846bf3b5eb640e4/" target="_blank"><img src="http://i.piccy.info/i9/e68cec836123d75b2eb9152bd024939a/1587929547/210292/1367845/boiler_shemat.png" alt="Piccy.info - Free Image Hosting" border="0" /></a><a href="http://i.piccy.info/a3c/2020-04-26-19-32/i9-13776070/500x600-r" target="_blank"><img src="http://i.piccy.info/a3/2020-04-26-19-32/i9-13776070/500x600-r/i.gif" alt="" border="0" /></a>

По I2C можно подключить экран LCD 1602. На нем будет отображаться текущее время, порог нагрева бойлера, до какого времени действует этот временной интервал
и температуру бойлера и уличного датчика.

При подключении еще одного DS18B20 можно контролировать температуру за окном.
Так же можно подключить реле, которое будет управлять кухонной вытяжкой + датчик DS18B20 возле вытяжки.

При необходимости можно подключить любой датчик газа, и по его срабатыванию получать уведомления, или на внутренний динамик устройства, или 
на другие устройства подключенные по MQTT протоколу.

Так же возможно использовать устройство проасто как датчик уличной (любой) тепературы.
Или к примеру управлять только вытяжкой.

Подключать дополнительные модули можно согласно описания ниже
/***********************************************************************
I2C device found at address 0x3F  ! - Екран
I2C device found at address 0x68  ! - RTC модуль
buzzerPin        - D0 GPIO 16 динамик
I2C (SCL)        - D1 GPIO 05 шина экрана и часового модуля
I2C (SDA)        - D2 GPIO 04 шина экрана и часового модуля
DS18B20          - D3 GPIO 00 (t- бойлера, t- кухни, t- другие
ledConnectedWifi - D4 GPIO 02 выход на LED индикатор подключения к WiFi B-LED
releFan          - D5 GPIO 14 выход на реле вытяжки
buttonWarmingUp  - D6 GPIO 12 вход кнопки подогрева бойлера
ledWarmingUp     - D7 GPIO 13 выход на LED индикатор ручного подогрвева (синий)BLUE
releBoiler       - D8 GPIO 15 выход на реле бойлера          (красный светодиод)RED
                 - D9 GPIO 03/
                 - D10 GPIO 01
//               - D11 GPIO 09
//               - D12 GPIO 10
sensorGas        - A0 ADCO    вход от датчика газа или любого другого датчика
************************************************************************/