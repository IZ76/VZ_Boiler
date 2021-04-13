void server_init(){
  server.on("/", [](){server.send_P(200, "text/html", P_index);});
  server.on("/index.html", [](){server.send_P(200, "text/html", P_index);});
  server.on("/favicon.ico", [](){server.send(200, "text/html", "");});
  server.on("/configs.json", handle_configs);
  server.on("/set_configs", set_configs);
  server.on("/get_list_networks", get_list_networks);
  server.on("/restart", handle_Restart);
  server.on("/resetConfig", handle_resetConfig);
  server.on("/heating", handle_heating);
  server.on("/style.css", [](){server.send_P ( 200, "text/plain", P_css );});
  server.on("/function.js", [](){server.send_P ( 200, "text/plain", P_js);});
  server.onNotFound([](){Serial.println("Page Not Found");
  server.send(400, "text/html", "Page not Found");});
  server.on("/buf", [](){server.send(200, "text/plain", buf);}); //Для тестов отправки запросов на народмон
  httpUpdater.setup(&server);
  server.begin();
}
//======================================================================================================
void handle_configs() {
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));

  json += "\",\"tempUl\":\"";
  json += tempDallas[2];
  json += "\",\"tempBoiler\":\"";
  json += tempDallas[0];
  json += "\",\"statusBoiler\":\"";
  json += (statusBoiler==true?"Греет":"Не греет");
  json += "\",\"tempHotBoiler\":\"";
  json += (tempHotBoiler==true?"Включено":"Выключено");
  json += "\",\"tempFan\":\"";
  json += tempDallas[1];
  json += "\",\"statusFan\":\"";
  json += (statusFan==true?"Работает":"Выключено");
  json += "\",\"fanTempOn\":\"";
  json += fanTempOn;
  json += "\",\"tempOutside\":\"";
  json += tempOutside;
  json += "\",\"onOutside\":\"";
  json += onOutside;
  json += "\",\"deltaOutside\":\"";
  json += deltaOutside;
  json += "\",\"gasVol\":\"";
  json += gasVol;
  json += "\",\"gasVolAlarm\":\"";
  json += gasVolAlarm;
  json += "\",\"temp4\":\"";
  json += tempDallas[3];
  json += "\",\"temp5\":\"";
  json += tempDallas[4];

  json += "\",\"ntpServerName\":\"";
  json += ntpServerName;
  json += "\",\"timeZone\":\"";
  json += timeZone;
  json += "\",\"isDayLightSaving\":\"";
  json += (isDayLightSaving==1?"checked":"");
  json += "\",\"boilerOnH0\":\"";
  json += boilerOn[0];
  json += "\",\"boilerOnM0\":\"";
  json += boilerOn[1];
  json += "\",\"boilerTempOn0\":\"";
  json += boilerTempOn[0];
  json += "\",\"boilerOnH1\":\"";
  json += boilerOn[2];
  json += "\",\"boilerOnM1\":\"";
  json += boilerOn[3];
  json += "\",\"boilerTempOn1\":\"";
  json += boilerTempOn[1];
  json += "\",\"boilerOnH2\":\"";
  json += boilerOn[4];
  json += "\",\"boilerOnM2\":\"";
  json += boilerOn[5];
  json += "\",\"boilerTempOn2\":\"";
  json += boilerTempOn[2];
  json += "\",\"boilerOnH3\":\"";
  json += boilerOn[6];
  json += "\",\"boilerOnM3\":\"";
  json += boilerOn[7];
  json += "\",\"boilerTempOn3\":\"";
  json += boilerTempOn[3];
  json += "\",\"boilerOnH4\":\"";
  json += boilerOn[8];
  json += "\",\"boilerOnM4\":\"";
  json += boilerOn[9];
  json += "\",\"boilerTempOn4\":\"";
  json += boilerTempOn[4];

  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\",\"ssid\":\"";
  json += ssid;
  json += "\",\"password\":\"";
  json += password;
  json += "\",\"ssidAP\":\"";
  json += ssidAP;
  json += "\",\"passwordAP\":\"";
  json += passwordAP;

  
  json += "\",\"tempCorr0\":\"";
  json += tempCorr[0];
  json += "\",\"tempCorr1\":\"";
  json += tempCorr[1];
  json += "\",\"tempCorr2\":\"";
  json += tempCorr[2];
  json += "\",\"tempCorr3\":\"";
  json += tempCorr[3];
  json += "\",\"tempCorr4\":\"";
  json += tempCorr[4];

  json += "\",\"dallas0\":\"";
  json += dallas[0];
  json += "\",\"dallas1\":\"";
  json += dallas[1];
  json += "\",\"dallas2\":\"";
  json += dallas[2];
  json += "\",\"dallas3\":\"";
  json += dallas[3];
  json += "\",\"dallas4\":\"";
  json += dallas[4];

  json += "\",\"mqttOn\":\"";
  json += (mqttOn==1?"checked":"");
  json += "\",\"mqtt_server\":\"";
  json += mqtt_server;
  json += "\",\"mqtt_port\":\"";
  json += mqtt_port;
  json += "\",\"mqtt_user\":\"";
  json += mqtt_user;
  json += "\",\"mqtt_pass\":\"";
  json += mqtt_pass;
  json += "\",\"mqtt_name\":\"";
  json += mqtt_name;
  json += "\",\"mqtt_sub_temp\":\"";
  json += mqtt_sub_temp;
  json += "\",\"mqtt_heating_boiler\":\"";
  json += mqtt_heating_boiler;
  json += "\",\"mqtt_pub_statBoiler\":\"";
  json += mqtt_pub_statBoiler;
  json += "\",\"mqtt_pub_statFan\":\"";
  json += mqtt_pub_statFan;
  json += "\",\"mqtt_pub_info\":\"";
  json += mqtt_pub_info;
  json += "\",\"mqtt_pub_tempBoiler\":\"";
  json += mqtt_pub_tempBoiler;
  json += "\",\"mqtt_pub_tempUl\":\"";
  json += mqtt_pub_tempUl;
  json += "\",\"mqtt_pub_tempFan\":\"";
  json += mqtt_pub_tempFan;
  json += "\",\"mqtt_pub_temp4\":\"";
  json += mqtt_pub_temp4;
  json += "\",\"mqtt_pub_temp5\":\"";
  json += mqtt_pub_temp5;
  json += "\",\"mqtt_pub_gasVol\":\"";
  json += mqtt_pub_gasVol;

  json += "\",\"macNmon\":\"";
  json += macNmon;
  json += "\",\"postNmon\":\"";
  json += (postNmon==1?"checked":"");
  json += "\",\"nameNmon\":\"";
  json += nameNmon;
  json += "\",\"ownerNmon\":\"";
  json += ownerNmon;
  json += "\",\"latNmon\":\"";
  json += latNmon;
  json += "\",\"lonNmon\":\"";
  json += lonNmon;
  json += "\",\"altNmon\":\"";
  json += altNmon;
  json += "\",\"periodSendNmon\":\"";
  json += periodSendNmon;
  json += "\",\"writeNmon0\":\"";
  json += writeNmon[0];
  json += "\",\"writeNmon1\":\"";
  json += writeNmon[1];
  json += "\",\"writeNmon2\":\"";
  json += writeNmon[2];
  json += "\",\"writeNmon3\":\"";
  json += writeNmon[3];
  json += "\",\"writeNmon4\":\"";
  json += writeNmon[4];

  json += "\"}";
  server.send(200, "text/json", json);
}

void set_configs(){
  if(server.arg("fanTempOn")!="") fanTempOn = server.arg("fanTempOn").toInt();
  if(server.arg("onOutside")!="") onOutside = (server.arg("onOutside")=="1"?true:false);
  if(server.arg("deltaOutside")!="") deltaOutside = server.arg("deltaOutside").toInt();
  if(server.arg("gasVolAlarm")!="") gasVolAlarm = server.arg("gasVolAlarm").toInt();
  
  if(server.arg("ntpServerName")!="") ntpServerName = server.arg("ntpServerName").c_str();
  if(server.arg("timeZone")!="") timeZone = server.arg("timeZone").toInt();
  if(server.arg("isDayLightSaving")!="") isDayLightSaving = (server.arg("isDayLightSaving")=="1"?true:false);
  if(server.arg("boilerOnH0")!="") boilerOn[0] = server.arg("boilerOnH0").toInt();
  if(server.arg("boilerOnM0")!="") boilerOn[1] = server.arg("boilerOnM0").toInt();
  if(server.arg("boilerTempOn0")!="") boilerTempOn[0] = server.arg("boilerTempOn0").toInt();
  if(server.arg("boilerOnH1")!="") boilerOn[2] = server.arg("boilerOnH1").toInt();
  if(server.arg("boilerOnM1")!="") boilerOn[3] = server.arg("boilerOnM1").toInt();
  if(server.arg("boilerTempOn1")!="") boilerTempOn[1] = server.arg("boilerTempOn1").toInt();
  if(server.arg("boilerOnH2")!="") boilerOn[4] = server.arg("boilerOnH2").toInt();
  if(server.arg("boilerOnM2")!="") boilerOn[5] = server.arg("boilerOnM2").toInt();
  if(server.arg("boilerTempOn2")!="") boilerTempOn[2] = server.arg("boilerTempOn2").toInt();
  if(server.arg("boilerOnH3")!="") boilerOn[6] = server.arg("boilerOnH3").toInt();
  if(server.arg("boilerOnM3")!="") boilerOn[7] = server.arg("boilerOnM3").toInt();
  if(server.arg("boilerTempOn3")!="") boilerTempOn[3] = server.arg("boilerTempOn3").toInt();
  if(server.arg("boilerOnH4")!="") boilerOn[8] = server.arg("boilerOnH4").toInt();
  if(server.arg("boilerOnM4")!="") boilerOn[9] = server.arg("boilerOnM4").toInt();
  if(server.arg("boilerTempOn4")!="") boilerTempOn[4] = server.arg("boilerTempOn4").toInt();

  if(server.arg("ssid")!="") ssid = server.arg("ssid").c_str();
  if(server.arg("password")!="") password = server.arg("password").c_str();
  if(server.arg("ssidAP")!="") ssidAP = server.arg("ssidAP").c_str();
  if(server.arg("passwordAP")!="") passwordAP = server.arg("passwordAP").c_str();

  if(server.arg("dallas0")!="") dallas[0] = server.arg("dallas0").c_str();
  if(server.arg("dallas1")!="") dallas[1] = server.arg("dallas1").c_str();
  if(server.arg("dallas2")!="") dallas[2] = server.arg("dallas2").c_str();
  if(server.arg("dallas3")!="") dallas[3] = server.arg("dallas3").c_str();
  if(server.arg("dallas4")!="") dallas[4] = server.arg("dallas4").c_str();

  
  if(server.arg("mqtt_server")!=""){
    snprintf(mqtt_server, 30, "%s", server.arg("mqtt_server").c_str());
    mqtt_port = server.arg("mqtt_port").toInt();
    snprintf(mqtt_user, 29, "%s", server.arg("mqtt_user").c_str());
    snprintf(mqtt_pass, 29, "%s", server.arg("mqtt_pass").c_str());
    snprintf(mqtt_name, 29, "%s", server.arg("mqtt_name").c_str());
  }
  if(server.arg("mqtt_sub_temp")!="") snprintf(mqtt_sub_temp, 29, "%s", server.arg("mqtt_sub_temp").c_str());
  if(server.arg("mqtt_heating_boiler")!="") snprintf(mqtt_heating_boiler, 29, "%s", server.arg("mqtt_heating_boiler").c_str());
  if(server.arg("mqtt_pub_statBoiler")!="") snprintf(mqtt_pub_statBoiler, 29, "%s", server.arg("mqtt_pub_statBoiler").c_str());
  if(server.arg("mqtt_pub_statFan")!="") snprintf(mqtt_pub_statFan, 29, "%s", server.arg("mqtt_pub_statFan").c_str());
  if(server.arg("mqtt_pub_info")!="") snprintf(mqtt_pub_info, 29, "%s", server.arg("mqtt_pub_info").c_str());
  if(server.arg("mqtt_pub_tempBoiler")!="") snprintf(mqtt_pub_tempBoiler, 29, "%s", server.arg("mqtt_pub_tempBoiler").c_str());
  if(server.arg("mqtt_pub_tempUl")!="") snprintf(mqtt_pub_tempUl, 29, "%s", server.arg("mqtt_pub_tempUl").c_str());
  if(server.arg("mqtt_pub_tempFan")!="") snprintf(mqtt_pub_tempFan, 29, "%s", server.arg("mqtt_pub_tempFan").c_str());
  if(server.arg("mqtt_pub_temp4")!="") snprintf(mqtt_pub_temp4, 29, "%s", server.arg("mqtt_pub_temp4").c_str());
  if(server.arg("mqtt_pub_temp5")!="") snprintf(mqtt_pub_temp5, 29, "%s", server.arg("mqtt_pub_temp5").c_str());
  if(server.arg("mqtt_pub_gasVol")!="") snprintf(mqtt_pub_gasVol, 29, "%s", server.arg("mqtt_pub_gasVol").c_str());
  if(server.arg("mqttOn")!="") mqttOn = server.arg("mqttOn").toInt();

  if(server.arg("postNmon")!="") postNmon = (server.arg("postNmon")=="1"?true:false);
  nameNmon = server.arg("nameNmon").c_str();
  ownerNmon = server.arg("ownerNmon").c_str();
  latNmon = server.arg("latNmon").c_str();
  lonNmon = server.arg("lonNmon").c_str();
  altNmon = server.arg("altNmon").c_str();
  if(server.arg("periodSendNmon")!="") periodSendNmon = server.arg("periodSendNmon").toInt();
  if(server.arg("writeNmon0")!="") writeNmon[0] = server.arg("writeNmon0").toInt();
  if(server.arg("writeNmon1")!="") writeNmon[1] = server.arg("writeNmon1").toInt();
  if(server.arg("writeNmon2")!="") writeNmon[2] = server.arg("writeNmon2").toInt();
  if(server.arg("writeNmon3")!="") writeNmon[3] = server.arg("writeNmon3").toInt();
  if(server.arg("writeNmon4")!="") writeNmon[4] = server.arg("writeNmon4").toInt();

  if(server.arg("tempCorr0")!="") tempCorr[0] = server.arg("tempCorr0").toFloat();
  if(server.arg("tempCorr1")!="") tempCorr[1] = server.arg("tempCorr1").toFloat();
  if(server.arg("tempCorr2")!="") tempCorr[2] = server.arg("tempCorr2").toFloat();
  if(server.arg("tempCorr3")!="") tempCorr[3] = server.arg("tempCorr3").toFloat();
  if(server.arg("tempCorr4")!="") tempCorr[4] = server.arg("tempCorr4").toFloat();

  if(mqttOn) reconnect();
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}
//=========================================================================================================================================
void get_list_networks(){
  String state= "Нет данных";
  String line="";
  if(WiFi.status()==0) state="Idle";
  else if(WiFi.status()==1) state=" Нет доступных сетей";
  else if(WiFi.status()==2) state=" Сканирование завершино";
  else if(WiFi.status()==3) state=" Соединение установлено";
  else if(WiFi.status()==4) state=" Ошибка соединения";
  else if(WiFi.status()==5) state=" Отключено";
  else if(WiFi.status()==6) state=" Нет соединения";
  int n = WiFi.scanNetworks();
  if(n == 0){
    line="<font color='#FF0000'>Не найдено доступных сетей!</font>";
  }
  else{
    line = "Доступно сетей: "+String(n)+"<br>";
    line += "<table border='0' cellspacing='0' cellpadding='3'>";
    line += "<tr bgcolor='#DDDDDD' ><td><span><pre>     Название сети     </pre></span></td><td><span><pre>  Качество  </pre></span></td><td><span><pre>  Защита  </pre></span></td><tr>";
    for(int i=0; i<n; ++i){
      int quality=0;
      if(WiFi.RSSI(i)<= -100) quality=0;
      else if(WiFi.RSSI(i)>= -50) quality=100;
      else quality=2*(WiFi.RSSI(i)+100);
        line += "<tr><td height=\"25\"><a href='javascript:selssid(\""+String(WiFi.SSID(i))+"     \")'>"+String(WiFi.SSID(i))+"</a></td><td align = \"center\">"+String(quality)+"%</td><td align = \"center\">"+String((WiFi.encryptionType(i)==ENC_TYPE_NONE)?"-":"+")+"</td></tr>";
    }
    line += "</table>";
  }
  String values="";
  values += "connectionstate|"+state+"|div\n";
  values += "networks|"+line+"|div\n";
  server.send(200, "text/plain", values);
  Serial.println(values); 
}
//======================================================================================================
void handle_heating() {
  tempHotBoiler = !tempHotBoiler;
  server.send(200, "text/plain", (String)tempHotBoiler);
}
//======================================================================================================
void handle_resetConfig() {
  if(server.arg("device") == "ok"){
    //SPIFFS.format();
    SPIFFS.remove("/config.json");
    Serial.println("ESP erase Config file");
    delay(3000);
    server.send(200, "text/plain", "OK");
    delay(3000);
    bip(5);
    ESP.reset();
  }
}
//======================================================================================================
void handle_Restart() {
  if(server.arg("device") == "ok"){
    server.send(200, "text/plain", "OK"); 
    ESP.reset();
  }
}
