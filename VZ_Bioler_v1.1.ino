/***********************************************************************
I2C device found at address 0x3F  ! - Екран
I2C device found at address 0x57  ! - EEPROM RTC модуля
I2C device found at address 0x68  ! - RTC модуль
buzzerPin        - D0 GPIO 16 гучномовець)
I2C (SCL)        - D1 GPIO 05 шина екрану та модуля годинника
I2C (SDA)        - D2 GPIO 04 шина екрану та модуля годинника
DS18B20          - D3 GPIO 00 (t- бойлера, t- кухні, t- та інші)        FLASH
ledConnectedWifi - D4 GPIO 02 вихід на LED індікатор підкл. до WiFi B-LED
releFan          - D5 GPIO 14 вихід на релє витяжки
buttonWarmingUp  - D6 GPIO 12 вхід кнопки підогріва бойлера    (зелений)GREEN
ledWarmingUp     - D7 GPIO 13 вихід на LED інд. ручного підігріва (синій)BLUE
releBoiler       - D8 GPIO 15 вихід на релє бойлера             (червоний)RED
                 - D9 GPIO 03/
                 - D10 GPIO 01
//               - D11 GPIO 09
//               - D12 GPIO 10
sensorGas        - A0 ADCO    вхід сигнал від датчика газу
************************************************************************/
#define ver "v1.1"

#include <FS.h>
#include <Wire.h>
#include <OneWire.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPUpdateServer.h>
#define sensorGas        A0
#define buzzerPin        D0
#define ledConnectedWifi D4
#define releFan          D5
#define buttonWarmingUp  D6
#define ledWarmingUp     D7
#define releBoiler       D8
OneWire ds(D3);              // На цей пін підключаемо датчики температури (резистор на 4.7к обовязковий)
LiquidCrystal_I2C lcd(0x3F,16,2);
byte degree[8] = {B00110, B01001, B01001, B00110, B00000, B00000, B00000};  // кодуємо символ градуса
byte termometru[8] = {B00100, B01010, B01010, B01010, B01110, B11111, B11111, B01110};  // кодуємо симол термометру

IPAddress apIP(192, 168, 4, 1);
String ssid       = "ALISA";
String password   = "lera060476margo";
String ssidAP     = "WiFi_Kitchen";
String passwordAP = "11223344";
// ----------змінні для роботи з mqtt сервером
char mqtt_server[31] =      "192.168.88.200";
int  mqtt_port =            1883;
char mqtt_user[30] =        "";
char mqtt_pass[30] =        "";
char mqtt_name[30] =        "ESP-Kitchen";
char mqtt_sub_temp[30] =    "ESPzimmer/temp";
char mqtt_heating_boiler[30] = "Kitchen/heatingBoiler";
char mqtt_pub_statBoiler[30] = "Kitchen/statusBoiler";
char mqtt_pub_statFan[30] = "Kitchen/statusFan";
char mqtt_pub_info[30] =    "Inform/mess";
char mqtt_pub_tempBoiler[30] = "Kitchen/tempBoiler";
char mqtt_pub_tempUl[30] =  "Kitchen/tempUl";
char mqtt_pub_tempFan[30] = "Kitchen/tempFan";
char mqtt_pub_temp4[30] =   "Kitchen/temp4";
char mqtt_pub_temp5[30] =   "Kitchen/temp5";
char mqtt_pub_gasVol[30] =  "Kitchen/gasVol";
bool mqttOn = true;

WiFiClient ESPclient;
PubSubClient MQTTclient(ESPclient);
ESP8266HTTPUpdateServer httpUpdater;
File fsUploadFile;
String jsonConfig = "{}";
//----------Налаштування часу та його інтернет оновлення----------------
int rtcAddr = 0x68;
IPAddress timeServerIP;
WiFiUDP udp;
String ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
unsigned int localPort = 2390;
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#define LEAP_YEAR(Y)(((1970+Y)>0)&&!((1970+Y)%4)&&(((1970+Y)%100)||!((1970+Y)%400)))
boolean statusUpdateNtpTime = 0;
int hour = 1, minute = 58, second = 0;
int year = 2017, month = 8, day = 31, dayOfWeek = 5;
int secFr, lastSec = 1;
int timeZone = 2;
bool isDayLightSaving = true;
uint8_t hourTest[3], minuteTest[3];
//----------Налаштування праці з бойлером-------------------------------
int tempOnBoiler = 35;
bool tempHotBoiler = false;
bool oldTempHotBoiler = false;
unsigned long buttonTime = millis();                        
bool buttonLast = false;
int boilerOn[10] = {0,00, 4,00, 6,55, 23,00, 23,30};
int boilerTempOn[5] = {10, 60, 40, 60, 50};
bool statusBoiler = false;
bool oldStatusBoiler = false;
//----------Налаштування праці з витяжкою-------------------------------
int fanTempOn = 40;
bool statusFan = false;
bool oldStatusFan = false;
int tempOutside = 0;
bool onOutside = false;
int deltaOutside = 10;
int gasVol = 0;
int gasVolAlarm = 900;
//----------Налаштування роботи датчиків температури--------------------
String dallas[5]{
  "18168162224156",          // датчик бойлера
  "21513118122359",          // датчик витяжки
  "292471672237",            // датчик на вулиці
  "81122164225178",          // датчик в ванній кімнаті
  "585344265378"             // вільний датчик
};
float tempDallas[5];
int timePubTemp = 30;
int numberIntervalTime = 0;

ESP8266WebServer server(80);
#include "P_index.h"
#include "P_js.h"
#include "P_css.h"
//======================================================================
void setup(){
  Serial.begin(115200);
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  lcd.createChar(1, degree);
  lcd.createChar(2, termometru);
  udp.begin(localPort);
  pinMode(sensorGas, INPUT);
  pinMode(releFan, OUTPUT);
  digitalWrite(releFan, LOW);
  pinMode(releBoiler, OUTPUT);
  digitalWrite(releBoiler, LOW);
  pinMode(buttonWarmingUp, INPUT);
  digitalWrite(buttonWarmingUp, HIGH);
  pinMode(ledConnectedWifi, OUTPUT);
  digitalWrite(ledConnectedWifi, HIGH);
  pinMode(ledWarmingUp, OUTPUT);
  digitalWrite(ledWarmingUp, LOW);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  server_init();
  //----------OTA SETUP---------------------------------------------------
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("ESP-Kitchen");
  ArduinoOTA.onEnd([](){ESP.restart();});
  ArduinoOTA.onError([](ota_error_t error){Serial.printf("Error[%u]: ", error); ESP.restart();});  //
  ArduinoOTA.begin();                                                   //
  //----------MQTT client-------------------------------------------------
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback(callback);
  MQTTclient.connect(mqtt_name);
  MQTTclient.subscribe(mqtt_heating_boiler);
  MQTTclient.subscribe(mqtt_sub_temp);
  //----------Ініціалізуємо файлову систему-------------------------------
  SPIFFS.begin();
  //{
  //  Dir dir = SPIFFS.openDir("/");
  //  while (dir.next()){
  //    String fileName = dir.fileName();
  //    size_t fileSize = dir.fileSize();
  //  }
  //}
  //----------------------------------------------------------------------
  loadConfig();
  wifiConnect();
  timeUpdateNTP();
  buttonTime = millis();
}
//==========END SETUP===================================================
void callback(char* topic, byte* payload, unsigned int length){
  if(String(topic)==mqtt_heating_boiler){
    if((char)payload[0]=='1'){
      tempHotBoiler=1;
      digitalWrite(ledWarmingUp, HIGH); 
    }
    else{
      tempHotBoiler=0;
      digitalWrite(ledWarmingUp, LOW); 
    }
  }
  if(String(topic)==mqtt_sub_temp){
    tempOutside=0;
    for(int i=0; i<length; i++){
      if(payload[i]>=48&&payload[i]<58){
        tempOutside=(tempOutside*10)+(payload[i]-48);
      }
      if((char)payload[i+1]=='.') i=length;
    }
  }
}
//======================================================================
void reconnect(){ 
  if(!MQTTclient.connected()&& WiFi.status()==WL_CONNECTED){
    Serial.print("Attempting MQTT connection..."); 
    if(MQTTclient.connect(mqtt_name, mqtt_user, mqtt_pass)){
      Serial.println("...reconnected OK!!!");
        MQTTclient.subscribe(mqtt_heating_boiler);
        MQTTclient.subscribe(mqtt_sub_temp);
        }
    else{
      Serial.print("...ERROR...failed, rc=");
      Serial.print(MQTTclient.state());
    }
  }
}
//======================================================================
//==========LOOP========================================================
void loop(){
  ArduinoOTA.handle();
  server.handleClient();
  getRTCDateTime();
  MQTTclient.loop();
  if(second!=lastSec){
    lastSec=second;
    Serial.print("secFr=");
    Serial.println(secFr);
    secFr=0;
    timePubTemp++;
  } else secFr++;
  //----------Провека подключения к WiFi---------------------------------- 
  if(WiFi.status()!=WL_CONNECTED&&secFr==50&&minute%5==1) wifiConnect();
  //----------Перевірка рівня газу в кухні--------------------------------  
  if(secFr==0&&second%5==0){
    gasVol=analogRead(sensorGas);
    if(gasVol>=gasVolAlarm){
      for(int i=0; i<4; i++){
        tone(buzzerPin, 2000, 40);
        delay(200);
      }
      if(mqttOn) MQTTclient.publish(mqtt_pub_gasVol, (String(gasVol)).c_str());
    }
  }
  //----------Перевірка перевищення температури бойлера-------------------  
  if(secFr==0&&second%5==0){
    if(tempDallas[0]>70){
      lcd.setCursor(0, 0);
      lcd.print(" ALARM BOILER!!! ");  
      lcd.setCursor(0, 1);
      lcd.print("  TEMPER="); 
      lcd.print(tempDallas[0], 1);  
      lcd.print("*C    "); 
      for(int i=0; i<8; i++){
        tone(buzzerPin, 1950, 80);
        delay(220);
        tone(buzzerPin, 1700, 80);
        delay(250);
      }
    }
  }
  //----------Работа с кнопкой управления---------------------------------
  if(digitalRead(buttonWarmingUp)==LOW&&buttonLast==false){
    buttonTime=millis();
    buttonLast=true;
  }
  if(digitalRead(buttonWarmingUp)==HIGH&&buttonLast==true){
    buttonLast=false;
    if((millis()-buttonTime)>50){
       tempHotBoiler=!tempHotBoiler;
       digitalWrite(ledWarmingUp, tempHotBoiler);
       tone(buzzerPin, 2000, 40);
       delay(200);
    }
  }
  //----------Работа с бойлером------------------------------------------- 
  if(secFr==0){
    temper();
    if(tempHotBoiler==true && tempDallas[0]>50) tempHotBoiler=false;
    numberIntervalTime = 0;
    if(tempHotBoiler==true) tempOnBoiler=50;
    else{
      numberIntervalTime = 0;
      for(int i=0; i<5; i++){
        if((hour*60+minute) >= (boilerOn[i*2]*60+boilerOn[i*2+1]) && (boilerOn[i*2]*60+boilerOn[i*2+1])>=(boilerOn[numberIntervalTime*2]*60+boilerOn[numberIntervalTime*2+1])){
          tempOnBoiler = boilerTempOn[i];
          numberIntervalTime = i;
        }
      }
    }
    if(tempDallas[0]<(tempOnBoiler-2)){
      digitalWrite(releBoiler, HIGH);
      statusBoiler = true;
    } else if(tempDallas[0]>=tempOnBoiler){
      digitalWrite(releBoiler, LOW);
      statusBoiler = false;
    }
  }
  //----------Синхронизация часов со временем из интернета----------------
  if((statusUpdateNtpTime==0&&second==5)||(hour==3 && minute==1 && second==5)) timeUpdateNTP();
  //----------Выводим данные на LCD экран---------------------------------
  if(millis()%100==0) LCD();
  //----------Выводим данные на MQTT сервер-------------------------------   
  if(WiFi.status()==WL_CONNECTED && secFr==0 && mqttOn){
    if(!MQTTclient.connected()) reconnect();
    else{
      if(timePubTemp>59){
        MQTTclient.publish(mqtt_pub_tempBoiler, (String((int)tempDallas[0])+"."+String((int)(tempDallas[0]*(tempDallas[0]<0?-10:10))%10)).c_str());
        MQTTclient.publish(mqtt_pub_tempFan,    (String((int)tempDallas[1])+"."+String((int)(tempDallas[1]*(tempDallas[1]<0?-10:10))%10)).c_str());
        MQTTclient.publish(mqtt_pub_tempUl,  ((String)tempDallas[2]).c_str());
        if(oldStatusBoiler==statusBoiler) MQTTclient.publish(mqtt_pub_statBoiler, (String(statusBoiler)).c_str());
        if(oldTempHotBoiler==tempHotBoiler) MQTTclient.publish(mqtt_heating_boiler, (String(tempHotBoiler)).c_str());
        if(oldStatusFan==statusFan) MQTTclient.publish(mqtt_pub_statFan, (String(statusFan)).c_str());
        MQTTclient.publish(mqtt_pub_gasVol, (String(gasVol)).c_str());
        timePubTemp=0;
        Serial.println("MQTT publish DATA");
      }
      if(oldStatusBoiler!=statusBoiler){
        MQTTclient.publish(mqtt_pub_statBoiler, (String(statusBoiler)).c_str());
        oldStatusBoiler=statusBoiler;
        Serial.println("MQTT publish STATUS Boiler");
      }
      if(oldTempHotBoiler!=tempHotBoiler){
        MQTTclient.publish(mqtt_heating_boiler, (String(tempHotBoiler)).c_str());
        oldTempHotBoiler=tempHotBoiler;
        Serial.println("MQTT publish STATUS PODOGREV");
        tone(buzzerPin, 2000, 40);
        delay(200);
      }
      if(oldStatusFan!=statusFan){
        MQTTclient.publish(mqtt_pub_statFan, (String(statusFan)).c_str());
        oldStatusFan=statusFan;
        Serial.println("MQTT publish STATUS FAN");
      }
      if(gasVol>=gasVolAlarm&&second%15==0) MQTTclient.publish(mqtt_pub_info, ("       ВНИМАНИЕ!!! Повышенный уровень газа "+String(analogRead(sensorGas))+" ед.!!!      ").c_str());
      if(tempDallas[0]>70&&second%15==0) MQTTclient.publish(mqtt_pub_info, ("       ВНИМАНИЕ!!! ПЕРЕГРЕВ БОЙЛЕРА  "+String(tempDallas[0])+" *С!!!      ").c_str());
    }
  }
  //----------Провека температуры вытяжки---------------------------------  
  if(secFr==0){
    if(tempDallas[1]>=fanTempOn||((tempDallas[1]>=(tempOutside+deltaOutside))&&(tempOutside>9&&tempOutside<=30)&&onOutside)){
      digitalWrite(releFan, HIGH);
      statusFan=true;
    }
    if(tempDallas[1]<(fanTempOn-2)||((tempDallas[1]<(tempOutside+deltaOutside))&&(tempOutside>9&&tempOutside<=30)&&onOutside)){
      digitalWrite(releFan, LOW);
      statusFan=false;
    }
  }
}
//==========END LOOP====================================================
//======================================================================
void LCD(){
  lcd.setCursor(0, 0);
  if(gasVol<gasVolAlarm){
    String mess=(hour>9?String(hour):" "+String(hour))+":"+(minute>9?String(minute):"0"+String(minute))+":"+(second>9?String(second):"0"+String(second));
    lcd.print (mess);
    lcd.setCursor(8, 0);
    lcd.print("|do");
    byte nit = (numberIntervalTime<4?numberIntervalTime+1:0);
    if(boilerOn[nit*2]<10) lcd.print(" ");
    lcd.print(boilerOn[nit*2]);
    lcd.print(":");
    if(boilerOn[nit*2+1]<10) lcd.print("0");
    lcd.print(boilerOn[nit*2+1]);
    lcd.setCursor(0, 1);
    lcd.write((byte)2);
    if(second%8<4){
      lcd.print("B");
      lcd.print(tempDallas[0], 1);                                        // выводим температуру бойлера с одним знаком после запятой      
    } else {
      lcd.print("U");
      if(tempDallas[2]>=0&&tempDallas[2]<10) lcd.print(" ");
      lcd.print(tempDallas[2], 1);                                        // выводим температуру улицы с одним знаком после запятой
    }
    lcd.write((byte)1);                                                 // Вывод значка градусов
    lcd.print("C  ");
    lcd.setCursor(8, 1);
    lcd.print("|on=");
    lcd.print(tempOnBoiler);
    lcd.write((byte)1);                                                 // Вывод значка градусов
    lcd.print("C ");
  }else{
    lcd.print("  ALARM GAS!!!  ");
    lcd.setCursor(0, 1);
    lcd.print(gasVol);
    lcd.print("ed.  iz");
    lcd.print(gasVolAlarm);
    lcd.print("ed.    ");
  }
}
//======================================================================
void temper(){
  byte i;
  byte present=0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if(!ds.search(addr)){
    ds.reset_search();
    delay(250);
    return;
  }
  if(OneWire::crc8(addr, 7)!=addr[7]) return;
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(500);
  present=ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  String aDDr="";
  for(i=0; i<9; i++){
    if(i>1&&i<8) aDDr+=String (addr[i]);
    data[i]=ds.read();
  }
  Serial.println(aDDr);
  int16_t raw=(data[1]<<8) | data[0];
  if(type_s){
    raw=raw<<3;
    if(data[7]==0x10){
      raw=(raw&0xFFF0)+12-data[6];
    }
  }
  else{
    byte cfg=(data[4]&0x60);
    if(cfg==0x00) raw=raw& ~7;
    else if(cfg==0x20) raw=raw& ~3;
    else if(cfg==0x40) raw=raw& ~1;
  }
  for(int i=0; i<5; i++){
    if(aDDr==dallas[i]){
      tempDallas[i]=(float)raw/16.0 - (i==3?0.65:0)+(i==0?0:0);         // Градусник уличный корретируем на 0.65С
      Serial.print("tempDallas[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(tempDallas[i]);
      Serial.println("");
    }
  }
}

//==========Підключення до мережі WiFi з виводом інфо на LCD============
void wifiConnect(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  lcd.clear();
  lcd.setCursor(0, 0);
  for(int i=0; i<16; i++){
    if(WiFi.status()==WL_CONNECTED){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print ("IP address:     ");
      lcd.setCursor(0, 1);
      lcd.print (WiFi.localIP());
      Serial.print("WiFi connect :");
      Serial.println(WiFi.localIP());
      digitalWrite(ledConnectedWifi, LOW);
      delay(3000);
      lcd.clear();
      return;
    }
    if(i==16) lcd.setCursor(0, 1);
    lcd.print(".");
    Serial.println(".");
    delay(500);
  }
  digitalWrite(ledConnectedWifi, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Start AP mode");
  lcd.setCursor(0, 1);
  lcd.print("IP 192.168.4.1");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());
  Serial.println("WiFi AP start");
  delay(2000);
  lcd.clear();
}
//==========ОНОВЛЕННЯ МЕРЕЖЕВОГО ЧАСУ (перевірка в три проходи)=========
void timeUpdateNTP(){
  if(WiFi.status()!=WL_CONNECTED) return;
  statusUpdateNtpTime=1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proba # ");
  for(int timeTest=0; timeTest<3; timeTest++){
    lcd.print(timeTest+1);
    lcd.print(" ");
    getNTPtime();
    hourTest[timeTest]=hour;
    minuteTest[timeTest]=minute;
    if(statusUpdateNtpTime==0){
      lcd.setCursor(0, 1);
      lcd.print("ERROR DateTime");
      delay(3000);
      lcd.clear();
      return;
    }
    if(timeTest>0){
      if((hourTest[timeTest]!=hourTest[timeTest-1]||minuteTest[timeTest]!=minuteTest[timeTest-1])){
        statusUpdateNtpTime=0;
        lcd.setCursor(0, 1);
        lcd.print("ERROR timeUpdate");
        delay(3000);
        lcd.clear();
        return;
      }
    }
  }
  statusUpdateNtpTime=1;
  setRTCDateTime();
  lcd.setCursor(0, 1);
  lcd.print("Time update OK");
  delay(1000);
  lcd.clear();
}
//==========ОТРИМАННЯ ДАТИ ТА ЧАСУ ВІД СЕРВЕРА ТОЧНОГО ЧАСУ ============
void getNTPtime(){
  WiFi.hostByName(ntpServerName.c_str(), timeServerIP);
  int cb;
  for(int i=0; i<3; i++){
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0]=0b11100011;
    packetBuffer[1]=0;
    packetBuffer[2]=6;
    packetBuffer[3]=0xEC;
    packetBuffer[12]=49;
    packetBuffer[13]=0x4E;
    packetBuffer[14]=49;
    packetBuffer[15]=52;
    udp.beginPacket(timeServerIP, 123);
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
    delay(500);
    cb=udp.parsePacket();
    if(!cb){
      lcd.setCursor(0, 1);
      lcd.print("no packet yet  "+String(i+1));
    }
    if(!cb&&i==2){
      statusUpdateNtpTime=0;
      return;
    }
    if(cb) i=3;
  }
  if(cb){
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord=word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord=word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900=highWord<<16 | lowWord;
    const unsigned long seventyYears=2208988800UL;
    unsigned long epoch=secsSince1900-seventyYears;
    boolean summerTime;
    if(month<3||month>10) summerTime=false;
    if(month>3&&month<10) summerTime=true;
    if(month==3&&(hour+24*day)>=(3+24*(31-(5*year/4+4)%7))||month==10&&(hour+24*day)<(3+24*(31-(5*year/4+1)%7))) summerTime=true; 
    epoch=epoch+timeZone*3600+(3600*(isDayLightSaving && summerTime));
    year=0;
    int days=0;
    uint32_t time;
    time=epoch/86400;
    hour=(epoch%86400L)/3600;
    minute=(epoch%3600)/60;
    second=epoch%60;
    dayOfWeek=(((time)+4)%7)+1;
    while((unsigned)(days+=(LEAP_YEAR(year)?366:365))<=time){
      year++;
    }
    days-=LEAP_YEAR(year)?366:365;
    time-=days;
    days=0;
    month=0;
    uint8_t monthLength=0;
    for(month=0; month<12; month++){
      if(month==1){
        if(LEAP_YEAR(year)) monthLength=29;
        else monthLength=28;
      }
      else monthLength=monthDays[month];
      if(time>=monthLength) time-=monthLength;
      else break;
    }
    month=month+1;
    day=time+1;
    year+=1970;
    return;
  }
  Serial.println("Nie ma czasu(((");
}
// ---------------Конвертирование шестнацатиричного числа в десятичное-----
unsigned char h2int(char c){
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

// Декодировани url адресов
String urldecode(String input){
  char c;
  String ret="";
  for(byte t=0; t<input.length(); t++){
    c=input[t];
    if(c=='+') c=' ';
    if(c=='%'){
      t++;
      c=input[t];
      t++;
      c=(h2int(c)<<4) | h2int(input[t]);
    }
    ret.concat(c);
  }
  return ret;
}
//--------------------------------------------------------------------------------
void bip(int res){
  for(int i=0; i<res; i++){
    tone(buzzerPin, 1950, 80);
    delay(220);  
  }
}
