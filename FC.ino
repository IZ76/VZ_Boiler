bool loadConfig(){
  File configFile=SPIFFS.open("/config.json", "r");
  if(!configFile){
    Serial.println("NOT FOUND FILE config.json");
    saveConfig();
    configFile.close();
//  Serial.println("OK NEW config.json");
    return false;
  }
  size_t size=configFile.size();
  if(size>2048){
    configFile.close();
    Serial.println("ERROR LOAD SIZE config.json");    
    return false;
  }
  jsonConfig=configFile.readString();
//DynamicJsonBuffer jsonBuffer;                                  //JSON V5
  DynamicJsonDocument doc(2048);                                 //JSON V6
  deserializeJson(doc, jsonConfig);                              //JSON V6
  configFile.close();
//JsonObject& root=jsonBuffer.parseObject(jsonConfig);           //JSON V5

  fanTempOn=doc["fanTempOn"];
  onOutside=doc["onOutside"];
  deltaOutside=doc["deltaOutside"];
  gasVolAlarm=doc["gasVolAlarm"];

  ntpServerName=doc["ntpServerName"].as<String>();
  timeZone=doc["timeZone"];
  isDayLightSaving=doc["isDayLightSaving"];
  boilerOn[0]=doc["boilerOnH0"];
  boilerOn[1]=doc["boilerOnM0"];
  boilerTempOn[0]=doc["boilerTempOn0"];
  boilerOn[2]=doc["boilerOnH1"];
  boilerOn[3]=doc["boilerOnM1"];
  boilerTempOn[1]=doc["boilerTempOn1"];
  boilerOn[4]=doc["boilerOnH2"];
  boilerOn[5]=doc["boilerOnM2"];
  boilerTempOn[2]=doc["boilerTempOn2"];
  boilerOn[6]=doc["boilerOnH3"];
  boilerOn[7]=doc["boilerOnM3"];
  boilerTempOn[3]=doc["boilerTempOn3"];
  boilerOn[8]=doc["boilerOnH4"];
  boilerOn[9]=doc["boilerOnM4"];
  boilerTempOn[4]=doc["boilerTempOn4"];

  ssidAP=doc["ssidAP"].as<String>();
  passwordAP=doc["passwordAP"].as<String>();
  ssid=doc["ssid"].as<String>();
  password=doc["password"].as<String>();

  dallas[0]=doc["dallas0"].as<String>();
  dallas[1]=doc["dallas1"].as<String>();
  dallas[2]=doc["dallas2"].as<String>();
  dallas[3]=doc["dallas3"].as<String>();
  dallas[4]=doc["dallas4"].as<String>();
  
  snprintf(mqtt_server, 30, "%s", (doc["mqtt_server"].as<String>()).c_str());
  mqtt_port = doc["mqtt_port"];
  snprintf(mqtt_user, 29, "%s", (doc["mqtt_user"].as<String>()).c_str());
  snprintf(mqtt_pass, 29, "%s", (doc["mqtt_pass"].as<String>()).c_str());
  snprintf(mqtt_name, 20, "%s", (doc["mqtt_name"].as<String>()).c_str());
  snprintf(mqtt_sub_temp, 29, "%s", (doc["mqtt_sub_temp"].as<String>()).c_str());
  snprintf(mqtt_heating_boiler, 29, "%s", (doc["mqtt_heating_boiler"].as<String>()).c_str());
  snprintf(mqtt_pub_statBoiler, 29, "%s", (doc["mqtt_pub_statBoiler"].as<String>()).c_str());
  snprintf(mqtt_pub_statFan, 29, "%s", (doc["mqtt_pub_statFan"].as<String>()).c_str());
  snprintf(mqtt_pub_info, 29, "%s", (doc["mqtt_pub_info"].as<String>()).c_str());
  snprintf(mqtt_pub_tempBoiler, 29, "%s", (doc["mqtt_pub_tempBoiler"].as<String>()).c_str());
  snprintf(mqtt_pub_tempUl, 29, "%s", (doc["mqtt_pub_tempUl"].as<String>()).c_str());
  snprintf(mqtt_pub_tempFan, 29, "%s", (doc["mqtt_pub_tempFan"].as<String>()).c_str());
  snprintf(mqtt_pub_temp4, 29, "%s", (doc["mqtt_pub_temp4"].as<String>()).c_str());
  snprintf(mqtt_pub_temp5, 29, "%s", (doc["mqtt_pub_temp5"].as<String>()).c_str());
  snprintf(mqtt_pub_gasVol, 29, "%s", (doc["mqtt_pub_gasVol"].as<String>()).c_str());
  mqttOn = doc["mqttOn"];
  
  Serial.println("OK LOAD config.json");
  bip(3);
  return true;
}

//--------------------------------------------------------------------------
bool saveConfig(){
//DynamicJsonBuffer jsonBuffer;                             //JSON V5
//JsonObject& json=jsonBuffer.parseObject(jsonConfig);      //JSON V5
  DynamicJsonDocument doc(2048);                            //JSON V6
  deserializeJson(doc, jsonConfig);                         //JSON V6

  doc["fanTempOn"] = fanTempOn;
  doc["onOutside"] = onOutside;
  doc["deltaOutside"] = deltaOutside;
  doc["gasVolAlarm"] = gasVolAlarm;

  doc["ntpServerName"] = ntpServerName;
  doc["timeZone"] = timeZone;
  doc["isDayLightSaving"] = isDayLightSaving;
  doc["boilerOnH0"] = boilerOn[0];
  doc["boilerOnM0"] = boilerOn[1];
  doc["boilerTempOn0"] = boilerTempOn[0];
  doc["boilerOnH1"] = boilerOn[2];
  doc["boilerOnM1"] = boilerOn[3];
  doc["boilerTempOn1"] = boilerTempOn[1];
  doc["boilerOnH2"] = boilerOn[4];
  doc["boilerOnM2"] = boilerOn[5];
  doc["boilerTempOn2"] = boilerTempOn[2];
  doc["boilerOnH3"] = boilerOn[6];
  doc["boilerOnM3"] = boilerOn[7];
  doc["boilerTempOn3"] = boilerTempOn[3];
  doc["boilerOnH4"] = boilerOn[8];
  doc["boilerOnM4"] = boilerOn[9];
  doc["boilerTempOn4"] = boilerTempOn[4];

  doc["ssidAP"] = ssidAP;
  doc["passwordAP"] = passwordAP;
  doc["ssid"] = ssid;
  doc["password"] = password;

  doc["dallas0"] = dallas[0];
  doc["dallas1"] = dallas[1];
  doc["dallas2"] = dallas[2];
  doc["dallas3"] = dallas[3];
  doc["dallas4"] = dallas[4];
  
  doc["mqtt_server"] = mqtt_server;
  doc["mqtt_port"] = mqtt_port;
  doc["mqtt_user"] = mqtt_user;
  doc["mqtt_pass"] = mqtt_pass;
  doc["mqtt_name"] = mqtt_name;
  doc["mqtt_sub_temp"] = mqtt_sub_temp;
  doc["mqtt_heating_boiler"] = mqtt_heating_boiler;
  doc["mqtt_pub_statBoiler"] = mqtt_pub_statBoiler;
  doc["mqtt_pub_statFan"] = mqtt_pub_statFan;
  doc["mqtt_pub_info"] = mqtt_pub_info;
  doc["mqtt_pub_tempBoiler"] = mqtt_pub_tempBoiler;
  doc["mqtt_pub_tempUl"] = mqtt_pub_tempUl;
  doc["mqtt_pub_tempFan"] = mqtt_pub_tempFan;
  doc["mqtt_pub_temp4"] = mqtt_pub_temp4;
  doc["mqtt_pub_temp5"] = mqtt_pub_temp5;
  doc["mqtt_pub_gasVol"] = mqtt_pub_gasVol;
  doc["mqttOn"] = mqttOn;
//json.printTo(jsonConfig);                                 //JSON V5
//Serial.println(jsonConfig);                               //JSON V5
  jsonConfig = "";
  if(serializeJson(doc, jsonConfig)==0){                    //JSON V6 
    Serial.println(F("Failed to write to jsonConfig"));
  }
  File configFile = SPIFFS.open("/config.json", "w");
  if(!configFile){
    configFile.close();
    Serial.println("ERROR OPEN FILE config.json");
    return false;
  }
//json.printTo(configFile);                                 //JSON V5
  if(serializeJson(doc, configFile)==0){                    //JSON V6 
    Serial.println(F("Failed to write to file"));
  }
  Serial.print("Save Config : ");
  Serial.println(jsonConfig);
  configFile.close();
  bip(1);
  return true;
}
