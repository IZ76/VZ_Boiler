const char P_index[] PROGMEM =  R"=====(
<html>
    <head>
        <!--meta name = "viewport" content = "width=device-width, initial-scale=1.0"-->
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>VZ_Boiler</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_config(submit){
                server = "/set_configs?fanTempOn=" + val('fanTempOn') + "&onOutside=" + val('onOutside') + "&deltaOutside=" + val('deltaOutside') + "&gasVolAlarm=" + val('gasVolAlarm');

                server += "&ntpServerName=" + val('ntpServerName')+ "&timeZone=" + val('timeZone') + "&isDayLightSaving=" + val_sw('isDayLightSaving') + "&boilerOnH0=" + val('boilerOnH0');
                server += "&boilerOnM0=" + val('boilerOnM0') + "&boilerTempOn0=" + val('boilerTempOn0') + "&boilerOnH1=" + val('boilerOnH1') + "&boilerOnM1=" + val('boilerOnM1') + "&boilerTempOn1=" + val('boilerTempOn1');
                server += "&boilerOnH2=" + val('boilerOnH2') + "&boilerOnM2=" + val('boilerOnM2') + "&boilerTempOn2=" + val('boilerTempOn2') + "&boilerOnH3=" + val('boilerOnH3') + "&boilerOnM3=" + val('boilerOnM3');
                server += "&boilerTempOn3=" + val('boilerTempOn3') + "&boilerOnH4=" + val('boilerOnH4') + "&boilerOnM4=" + val('boilerOnM4') + "&boilerTempOn4=" + val('boilerTempOn4');
                
                server += "&ssid=" + val('ssid') + "&password=" + encodeURIComponent(val('password'))+"&ssidAP=" + val('ssidap') + "&passwordAP=" + encodeURIComponent(val('passwordAp'));

                server += "&dallas0=" + val('dallas0') + "&dallas1=" + val('dallas1') + "&dallas2=" + val('dallas2') + "&dallas3=" + val('dallas3') + "&dallas4=" + val('dallas4')
                
                server += "&mqttOn=" + val_sw('mqttOn') + "&mqtt_server=" + val('mqtt_server') + "&mqtt_port=" + val('mqtt_port') + "&mqtt_user=" + (val('mqtt_user')) + "&mqtt_pass=" + (val('mqtt_pass'));
                server += "&mqtt_name=" + val('mqtt_name') + "&mqtt_sub_temp=" + val('mqtt_sub_temp') + "&mqtt_heating_boiler=" + val('mqtt_heating_boiler') + "&mqtt_pub_statBoiler=" + val('mqtt_pub_statBoiler');
                server += "&mqtt_pub_statFan=" + val('mqtt_pub_statFan') + "&mqtt_pub_info=" + val('mqtt_pub_info') + "&mqtt_pub_tempBoiler=" + val('mqtt_pub_tempBoiler') + "&mqtt_pub_tempUl=" + val('mqtt_pub_tempUl');
                server += "&mqtt_pub_tempFan=" + val('mqtt_pub_tempFan') + "&mqtt_pub_temp4=" + val('mqtt_pub_temp4') + "&mqtt_pub_temp5=" + val('mqtt_pub_temp5') + "&mqtt_pub_gasVol=" + val('mqtt_pub_gasVol');

                server += "&postNmon=" + val_sw('postNmon') + "&macNmon=" + val('macNmon') + "&nameNmon=" + val('nameNmon') + "&ownerNmon=" + (val('ownerNmon'));
                server += "&latNmon=" + val('latNmon') + "&lonNmon=" + val('lonNmon') + "&altNmon=" + val('altNmon') + "&periodSendNmon=" + val('periodSendNmon');
                server += "&writeNmon0=" + val('writeNmon0') + "&writeNmon1=" + val('writeNmon1') + "&writeNmon2=" + val('writeNmon2') + "&writeNmon3=" + val('writeNmon3') + "&writeNmon4=" + val('writeNmon4');
                 
                send_request(submit, server);
            }
        </script>
    </head>
    <body onload = "load();">
        <header>
            <span class="opt_cn">Управление кухней VZ_Boiler_{{ver}} (by IvanZah)</span><br>
            <span class="opt_cn">Время на ESP:</span>
            <span id="time">{{time}}</span> 
        </header> 
        <central-unit>   
        <div class="body__center">
            <br>
            <titles>
                <span class="opt_cn">Состояние устройства</span>
            </titles>
            <div id="main_unit">
                <center>
                <table>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Состояние бойлера:&nbsp&nbsp</span></td>
                        <td><span class="opt_cn">{{statusBoiler}}&nbsp({{tempBoiler}}&degC)</span></td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Подогрев бойлера:&nbsp&nbsp</span></td>
                        <td>
                          <span id="123">{{tempHotBoiler}}</span>
                          <span class="save_booton" onclick="set_heating(this);" height="25"> &nbsp &#9728;</span>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Состояние вытяжки:&nbsp&nbsp</span></td>
                        <td><span class="opt_cn">{{statusFan}}&nbsp({{tempFan}}&degC)</span></td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Включение вытяжки:&nbsp&nbsp</span></td>
                        <td><input type="number" min="0" step="1" max="100" id="fanTempOn" name="fanTempOn" value="{{fanTempOn}}"</input><span>&degC</span></td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Учитывать коррекцию:&nbsp&nbsp</span></td>
                        <td><select id = "onOutside">
                            <option value="0">Нет</option>
                            <option value="1">Да</option>
                        </select></td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">tВытяжки &ge; tВнешняя +&nbsp&nbsp</span></td>
                        <td><input type="number" min="-50" step="1" max="50" id="deltaOutside" name="deltaOutside" value="{{deltaOutside}}"</input><span>&degC</span></td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Внешняя температура:&nbsp&nbsp</span></td>
                        <td><span class="opt_cn">{{tempOutside}}&degC</span></td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Сигнал уровня газа:&nbsp&nbsp</span></td>
                        <td>
                          <input type="number" min="0" step="1" max="1023" id="gasVolAlarm" name="gasVolAlarm" value="{{gasVolAlarm}}"</input>
                          <span class="opt_cn">({{gasVol}})</span>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" height="25"><span class="opt_cn">Температура на улице:&nbsp&nbsp</span></td>
                        <td><span class="opt_cn">{{tempUl}}&degC</span></td>
                    </tr>
                </table>
                </center>
                <br> 
                <span class="save_booton" onclick="set_config(this);">Сохранить</span>  
            </div>
            <br>
            <hr>
            <br>
            <titles>
                <div class="opt_cn">Настройки Времени</div>
            </titles>
            <div id="main_unit">
                <span class="opt_cn">Адрес сервера</span>
                <input type="text" class="field" id="ntpServerName" value="{{ntpServerName}}" class="form-control" pattern="[0-9a-zA-Z. ]{1,20}"><br>
                <span class="opt_cn">Ваша часовая зона</span>
                <select id = "timeZone">
                    <option value="-12"> (GMT) -12:00 </option>
                    <option value="-11"> (GMT) -11:00 </option>
                    <option value="-10"> (GMT) -10:00 </option>
                    <option value="-9 > (GMT) -09:00 </option>
                    <option value="-8" > (GMT) -08:00 </option>
                    <option value="-7" > (GMT) -07:00 </option>
                    <option value="-6" > (GMT) -06:00 </option>
                    <option value="-5" > (GMT) -05:00 </option>
                    <option value="-4" > (GMT) -04:00 </option>
                    <option value="-3" > (GMT) -03:00 </option>
                    <option value="-2" > (GMT) -02:00 </option>
                    <option value="-1" > (GMT) -01:00 </option>
                    <option value="0"  > (GMT) +00:00 </option>
                    <option value="1"  > (GMT) +01:00 </option>
                    <option value="2"  > (GMT) +02:00 </option>
                    <option value="3"  > (GMT) +03:00 </option>
                    <option value="4"  > (GMT) +04:00 </option>
                    <option value="5"  > (GMT) +05:00 </option>
                    <option value="6"  > (GMT) +06:00 </option>
                    <option value="7"  > (GMT) +07:00 </option>
                    <option value="8"  > (GMT) +08:00 </option>
                    <option value="9"  > (GMT) +09:00 </option>
                    <option value="10" > (GMT) +10:00 </option>
                    <option value="11" > (GMT) +11:00 </option>
                    <option value="12" > (GMT) +12:00 </option>
                </select>
                <br>
                <label class="switch"><span class="opt_cn">Переход на летнее время</span>
                    <input type="checkbox" id="isDayLightSaving">
                    <span class="switch-btn"></span>
                </label>
                <br>
                <br>
                <paragraph>
                    <span class="opt_cn">Настройка работы бойлера</span>
                </paragraph>
                <center>
                    <table id="table__font" class = "table">
                        <tr>
                            <td><span class="opt_cn"><b> # </b></span></td>
                            <td><span class="opt_cn"><b> Часы </b></span></td>
                            <td><span class="opt_cn"><b> Минуты </b></span></td>
                            <td><span class="opt_cn"><b> Температура </b></span></td>
                        </tr>
                        <tr>
                            <td><span>1</span></td>
                            <td>
                                <select id="boilerOnH0">
                                    <option value = "0"> 0 </option>
                                    <option value = "1"> 1 </option>
                                    <option value = "2"> 2 </option>
                                    <option value = "3"> 3 </option>
                                    <option value = "4"> 4 </option>
                                    <option value = "5"> 5 </option>
                                    <option value = "6"> 6 </option>
                                    <option value = "7"> 7 </option>    
                                    <option value = "8"> 8 </option>
                                    <option value = "9"> 9 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                </select>
                            </td>
                            <td>
                                <select id="boilerOnM0">
                                    <option value = "0"> 00 </option>
                                    <option value = "1"> 01 </option>
                                    <option value = "2"> 02 </option>
                                    <option value = "3"> 03 </option>
                                    <option value = "4"> 04 </option>
                                    <option value = "5"> 05 </option>
                                    <option value = "6"> 06 </option>
                                    <option value = "7"> 07 </option>    
                                    <option value = "8"> 08 </option>
                                    <option value = "9"> 09 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                    <option value = "24"> 24 </option>
                                    <option value = "25"> 25 </option>
                                    <option value = "26"> 26 </option>
                                    <option value = "27"> 27 </option>    
                                    <option value = "28"> 28 </option>
                                    <option value = "29"> 29 </option>
                                    <option value = "30"> 30 </option>
                                    <option value = "31"> 31 </option>
                                    <option value = "32"> 32 </option>
                                    <option value = "33"> 33 </option>
                                    <option value = "34"> 34 </option>
                                    <option value = "35"> 35 </option>
                                    <option value = "36"> 36 </option>
                                    <option value = "37"> 37 </option>    
                                    <option value = "38"> 38 </option>
                                    <option value = "39"> 39 </option>
                                    <option value = "40"> 40 </option>
                                    <option value = "41"> 41 </option>
                                    <option value = "42"> 42 </option>
                                    <option value = "43"> 43 </option>
                                    <option value = "44"> 44 </option>
                                    <option value = "45"> 45 </option>
                                    <option value = "46"> 46 </option>
                                    <option value = "47"> 47 </option>    
                                    <option value = "48"> 48 </option>
                                    <option value = "49"> 49 </option>
                                    <option value = "50"> 50 </option>
                                    <option value = "51"> 51 </option>
                                    <option value = "52"> 52 </option>
                                    <option value = "53"> 53 </option>
                                    <option value = "54"> 54 </option>
                                    <option value = "55"> 55 </option>
                                    <option value = "56"> 56 </option>
                                    <option value = "57"> 57 </option>    
                                    <option value = "58"> 58 </option>
                                    <option value = "59"> 59 </option>
                                </select>
                            </td>
                            <td>
                                <input type="number" min="5" step="1" max="85" id="boilerTempOn0" name="boilerTempOn0" value="{{boilerTempOn0}}"><span>&degC</span>
                            </td>
                        </tr>
                        <tr>
                            <td><span>2</span></td>
                            <td>
                                <select id="boilerOnH1">
                                    <option value = "0"> 0 </option>
                                    <option value = "1"> 1 </option>
                                    <option value = "2"> 2 </option>
                                    <option value = "3"> 3 </option>
                                    <option value = "4"> 4 </option>
                                    <option value = "5"> 5 </option>
                                    <option value = "6"> 6 </option>
                                    <option value = "7"> 7 </option>    
                                    <option value = "8"> 8 </option>
                                    <option value = "9"> 9 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                </select>
                            </td>
                            <td>
                                <select id="boilerOnM1">
                                    <option value = "0"> 00 </option>
                                    <option value = "1"> 01 </option>
                                    <option value = "2"> 02 </option>
                                    <option value = "3"> 03 </option>
                                    <option value = "4"> 04 </option>
                                    <option value = "5"> 05 </option>
                                    <option value = "6"> 06 </option>
                                    <option value = "7"> 07 </option>    
                                    <option value = "8"> 08 </option>
                                    <option value = "9"> 09 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                    <option value = "24"> 24 </option>
                                    <option value = "25"> 25 </option>
                                    <option value = "26"> 26 </option>
                                    <option value = "27"> 27 </option>    
                                    <option value = "28"> 28 </option>
                                    <option value = "29"> 29 </option>
                                    <option value = "30"> 30 </option>
                                    <option value = "31"> 31 </option>
                                    <option value = "32"> 32 </option>
                                    <option value = "33"> 33 </option>
                                    <option value = "34"> 34 </option>
                                    <option value = "35"> 35 </option>
                                    <option value = "36"> 36 </option>
                                    <option value = "37"> 37 </option>    
                                    <option value = "38"> 38 </option>
                                    <option value = "39"> 39 </option>
                                    <option value = "40"> 40 </option>
                                    <option value = "41"> 41 </option>
                                    <option value = "42"> 42 </option>
                                    <option value = "43"> 43 </option>
                                    <option value = "44"> 44 </option>
                                    <option value = "45"> 45 </option>
                                    <option value = "46"> 46 </option>
                                    <option value = "47"> 47 </option>    
                                    <option value = "48"> 48 </option>
                                    <option value = "49"> 49 </option>
                                    <option value = "50"> 50 </option>
                                    <option value = "51"> 51 </option>
                                    <option value = "52"> 52 </option>
                                    <option value = "53"> 53 </option>
                                    <option value = "54"> 54 </option>
                                    <option value = "55"> 55 </option>
                                    <option value = "56"> 56 </option>
                                    <option value = "57"> 57 </option>    
                                    <option value = "58"> 58 </option>
                                    <option value = "59"> 59 </option>
                                </select>
                            </td>
                            <td>
                                <input type="number" min="5" step="1" max="85" id="boilerTempOn1" name="boilerTempOn1" value="{{boilerTempOn1}}"><span>&degC</span>
                            </td>
                        </tr>
                        <tr>
                            <td><span>3</span></td>
                            <td>
                                <select id="boilerOnH2">
                                    <option value = "0"> 0 </option>
                                    <option value = "1"> 1 </option>
                                    <option value = "2"> 2 </option>
                                    <option value = "3"> 3 </option>
                                    <option value = "4"> 4 </option>
                                    <option value = "5"> 5 </option>
                                    <option value = "6"> 6 </option>
                                    <option value = "7"> 7 </option>    
                                    <option value = "8"> 8 </option>
                                    <option value = "9"> 9 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                </select>
                            </td>
                            <td>
                                <select id="boilerOnM2">
                                    <option value = "0"> 00 </option>
                                    <option value = "1"> 01 </option>
                                    <option value = "2"> 02 </option>
                                    <option value = "3"> 03 </option>
                                    <option value = "4"> 04 </option>
                                    <option value = "5"> 05 </option>
                                    <option value = "6"> 06 </option>
                                    <option value = "7"> 07 </option>    
                                    <option value = "8"> 08 </option>
                                    <option value = "9"> 09 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                    <option value = "24"> 24 </option>
                                    <option value = "25"> 25 </option>
                                    <option value = "26"> 26 </option>
                                    <option value = "27"> 27 </option>    
                                    <option value = "28"> 28 </option>
                                    <option value = "29"> 29 </option>
                                    <option value = "30"> 30 </option>
                                    <option value = "31"> 31 </option>
                                    <option value = "32"> 32 </option>
                                    <option value = "33"> 33 </option>
                                    <option value = "34"> 34 </option>
                                    <option value = "35"> 35 </option>
                                    <option value = "36"> 36 </option>
                                    <option value = "37"> 37 </option>    
                                    <option value = "38"> 38 </option>
                                    <option value = "39"> 39 </option>
                                    <option value = "40"> 40 </option>
                                    <option value = "41"> 41 </option>
                                    <option value = "42"> 42 </option>
                                    <option value = "43"> 43 </option>
                                    <option value = "44"> 44 </option>
                                    <option value = "45"> 45 </option>
                                    <option value = "46"> 46 </option>
                                    <option value = "47"> 47 </option>    
                                    <option value = "48"> 48 </option>
                                    <option value = "49"> 49 </option>
                                    <option value = "50"> 50 </option>
                                    <option value = "51"> 51 </option>
                                    <option value = "52"> 52 </option>
                                    <option value = "53"> 53 </option>
                                    <option value = "54"> 54 </option>
                                    <option value = "55"> 55 </option>
                                    <option value = "56"> 56 </option>
                                    <option value = "57"> 57 </option>    
                                    <option value = "58"> 58 </option>
                                    <option value = "59"> 59 </option>
                                </select>
                            </td>
                           <td>
                                <input type="number" min="5" step="1" max="85" id="boilerTempOn2" name="boilerTempOn2" value="{{boilerTempOn2}}"><span>&degC</span>
                            </td>
                        </tr>
                        <tr>
                            <td><span>4</span></td>
                            <td>
                                <select id="boilerOnH3">
                                    <option value = "0"> 0 </option>
                                    <option value = "1"> 1 </option>
                                    <option value = "2"> 2 </option>
                                    <option value = "3"> 3 </option>
                                    <option value = "4"> 4 </option>
                                    <option value = "5"> 5 </option>
                                    <option value = "6"> 6 </option>
                                    <option value = "7"> 7 </option>    
                                    <option value = "8"> 8 </option>
                                    <option value = "9"> 9 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                </select>
                            </td>
                            <td>
                                <select id="boilerOnM3">
                                    <option value = "0"> 00 </option>
                                    <option value = "1"> 01 </option>
                                    <option value = "2"> 02 </option>
                                    <option value = "3"> 03 </option>
                                    <option value = "4"> 04 </option>
                                    <option value = "5"> 05 </option>
                                    <option value = "6"> 06 </option>
                                    <option value = "7"> 07 </option>    
                                    <option value = "8"> 08 </option>
                                    <option value = "9"> 09 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                    <option value = "24"> 24 </option>
                                    <option value = "25"> 25 </option>
                                    <option value = "26"> 26 </option>
                                    <option value = "27"> 27 </option>    
                                    <option value = "28"> 28 </option>
                                    <option value = "29"> 29 </option>
                                    <option value = "30"> 30 </option>
                                    <option value = "31"> 31 </option>
                                    <option value = "32"> 32 </option>
                                    <option value = "33"> 33 </option>
                                    <option value = "34"> 34 </option>
                                    <option value = "35"> 35 </option>
                                    <option value = "36"> 36 </option>
                                    <option value = "37"> 37 </option>    
                                    <option value = "38"> 38 </option>
                                    <option value = "39"> 39 </option>
                                    <option value = "40"> 40 </option>
                                    <option value = "41"> 41 </option>
                                    <option value = "42"> 42 </option>
                                    <option value = "43"> 43 </option>
                                    <option value = "44"> 44 </option>
                                    <option value = "45"> 45 </option>
                                    <option value = "46"> 46 </option>
                                    <option value = "47"> 47 </option>    
                                    <option value = "48"> 48 </option>
                                    <option value = "49"> 49 </option>
                                    <option value = "50"> 50 </option>
                                    <option value = "51"> 51 </option>
                                    <option value = "52"> 52 </option>
                                    <option value = "53"> 53 </option>
                                    <option value = "54"> 54 </option>
                                    <option value = "55"> 55 </option>
                                    <option value = "56"> 56 </option>
                                    <option value = "57"> 57 </option>    
                                    <option value = "58"> 58 </option>
                                    <option value = "59"> 59 </option>
                                </select>
                            </td>
                            <td>
                                <input type="number" min="5" step="1" max="85" id="boilerTempOn3" name="boilerTempOn3" value="{{boilerTempOn3}}"><span>&degC</span>
                            </td>
                        </tr>
                        <tr>
                            <td><span>5</span></td>
                            <td>
                                <select id="boilerOnH4">
                                    <option value = "0"> 0 </option>
                                    <option value = "1"> 1 </option>
                                    <option value = "2"> 2 </option>
                                    <option value = "3"> 3 </option>
                                    <option value = "4"> 4 </option>
                                    <option value = "5"> 5 </option>
                                    <option value = "6"> 6 </option>
                                    <option value = "7"> 7 </option>    
                                    <option value = "8"> 8 </option>
                                    <option value = "9"> 9 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                </select>
                            </td>
                            <td>
                                <select id="boilerOnM4">
                                    <option value = "0"> 00 </option>
                                    <option value = "1"> 01 </option>
                                    <option value = "2"> 02 </option>
                                    <option value = "3"> 03 </option>
                                    <option value = "4"> 04 </option>
                                    <option value = "5"> 05 </option>
                                    <option value = "6"> 06 </option>
                                    <option value = "7"> 07 </option>    
                                    <option value = "8"> 08 </option>
                                    <option value = "9"> 09 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "11"> 11 </option>
                                    <option value = "12"> 12 </option>
                                    <option value = "13"> 13 </option>
                                    <option value = "14"> 14 </option>
                                    <option value = "15"> 15 </option>
                                    <option value = "16"> 16 </option>
                                    <option value = "17"> 17 </option>
                                    <option value = "18"> 18 </option>
                                    <option value = "19"> 19 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "21"> 21 </option>
                                    <option value = "22"> 22 </option>
                                    <option value = "23"> 23 </option>
                                    <option value = "24"> 24 </option>
                                    <option value = "25"> 25 </option>
                                    <option value = "26"> 26 </option>
                                    <option value = "27"> 27 </option>    
                                    <option value = "28"> 28 </option>
                                    <option value = "29"> 29 </option>
                                    <option value = "30"> 30 </option>
                                    <option value = "31"> 31 </option>
                                    <option value = "32"> 32 </option>
                                    <option value = "33"> 33 </option>
                                    <option value = "34"> 34 </option>
                                    <option value = "35"> 35 </option>
                                    <option value = "36"> 36 </option>
                                    <option value = "37"> 37 </option>    
                                    <option value = "38"> 38 </option>
                                    <option value = "39"> 39 </option>
                                    <option value = "40"> 40 </option>
                                    <option value = "41"> 41 </option>
                                    <option value = "42"> 42 </option>
                                    <option value = "43"> 43 </option>
                                    <option value = "44"> 44 </option>
                                    <option value = "45"> 45 </option>
                                    <option value = "46"> 46 </option>
                                    <option value = "47"> 47 </option>    
                                    <option value = "48"> 48 </option>
                                    <option value = "49"> 49 </option>
                                    <option value = "50"> 50 </option>
                                    <option value = "51"> 51 </option>
                                    <option value = "52"> 52 </option>
                                    <option value = "53"> 53 </option>
                                    <option value = "54"> 54 </option>
                                    <option value = "55"> 55 </option>
                                    <option value = "56"> 56 </option>
                                    <option value = "57"> 57 </option>    
                                    <option value = "58"> 58 </option>
                                    <option value = "59"> 59 </option>
                                </select>
                            </td>
                            <td>
                                <input type="number" min="5" step="1" max="85" id="boilerTempOn4" name="boilerTempOn4" value="{{boilerTempOn4}}"><span>&degC</span>
                            </td>
                        </tr>
                    </table>
                </center>
                <br>
                <center>
                    <span class="save_booton" onclick="set_config(this);">Сохранить</span>
                </center>
            </div>
            <br>
            <hr>
            <br>
            <titles>
                <span class="opt_cn">Настройки WIFI</span>
            </titles>
            <div id="main_unit">
                <paragraph>
                    <span class="opt_cn">Подключение к локальной WiFi сети</span>
                </paragraph>
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Введите название WiFi сети</span></td>
                            <td><input type="text" class="field form-control" id="ssid" value="{{ssid}}" pattern="[-\w\s#@/]{1,15}" placeholder="name WiFi network"></td>
                        </tr>
                        <tr>  
                            <td align = "right"><span class="opt_cn">Пароль для доступа к этой сети</span></td>
                            <td><input class="field form-control" id="password" value="{{password}}" pattern=".{8,15}" onfocus="this.type='text'" type="password"></td>
                        </tr>
                    </table>
                </center>
                <paragraph>
                    <span class="opt_cn">Точка доступа часов (IP_192.168.4.1)</span>
                </paragraph>
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Введите имя точки доступа часов</span></td>
                            <td><input type="text" class="field" id="ssidap" value="{{ssidAP}}" class="form-control" pattern="[-\w\s#@/]{1,15}" placeholder="access point name"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Пароль к точке доступа</span></td>  
                            <td><input class="field" id="passwordAp" value="{{passwordAP}}" class="form-control" pattern=".{0,15}" onfocus="this.type='text'" type="password" class="form-control"></td>
                        <tr>
                    </table>
                </center>
            </div>
            <center>
                <table>
                    <tr>
                        <td align = "center"><span class="opt_cn">Состояние соединения:</span></td>
                        <td><div id="connectionstate">Нет данных</div></td>
                    </tr>
                </table>
                <table class = "table">
                    <tr>
                        <td>
                            <div id="networks">Сканирование...</div>
                        </td>
                    </tr>
                </table>
                <a  align="center" href="javascript:GetState()" >Обновить список сетей</a>
                <br>
                <br> 
                <span class="save_booton" onclick="set_config(this);">Сохранить</span>  
            </center>
            <br>
            <hr>
            <br>
            <titles>
                <span class="opt_cn">Настройка датчиков DS18B20</span>
            </titles>
            <div id="main_unit">
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Датчик №1 - Бойлер</span></td>
                            <td><input type="text" class="field form-control" id="dallas0" value="{{dallas0}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Датчик №2 - Вытяжка</span></td>
                            <td><input type="text" class="field form-control" id="dallas1" value="{{dallas1}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Датчик №3 - Улица</span></td>
                            <td><input type="text" class="field form-control" id="dallas2" value="{{dallas2}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Датчик №4 - Ванная</span></td>
                            <td><input type="text" class="field form-control" id="dallas3" value="{{dallas3}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Датчик №5 - Свободный</span></td>
                            <td><input type="text" class="field form-control" id="dallas4" value="{{dallas4}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                    </table>
                </center>
            </div>
            <br>
            <center>
                <span class="save_booton" onclick="set_config(this);">Сохранить</span>
            </center>
            <br>
            <hr>
            <br>
            <titles>
                <span class="opt_cn">Настройки MQTT</span>
            </titles>
            <div id="main_unit">
                <label class="switch">
                    <span class="opt_cn">Использовать MQTT сервер</span>
                    <input type="checkbox" class="checkbox" id="mqttOn" name="switch-btn" onclick="set_mqtt_on(this);">
                    <span class="switch-btn"></span>
                </label>
                <paragraph><span class="opt_cn">Параметры MQTT сервера:</span></paragraph>
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Адрес сервера</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_server" value="{{mqtt_server}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Порт сервера</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_port" value="{{mqtt_port}}" pattern="[\d]{4,6}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Имя пользователя</span></td>
                            <td><input class="field form-control" id="mqtt_user" value="{{mqtt_user}}" pattern="[-\w\s#@/]{0,30}" onfocus="this.type='text'" type="password"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Пароль пользователя</span></td>
                            <td><input class="field form-control" id="mqtt_pass" value="{{mqtt_pass}}" pattern="[-\w\s#@/]{0,30}" onfocus="this.type='text'" type="password"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Имя устройства (ESP)</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_name" value="{{mqtt_name}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                    </table>
                </center>  
                <paragraph><span class="opt_cn">Топики сообщений</span></paragraph>
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Получение входящих данных</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_sub_temp" value="{{mqtt_sub_temp}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Топик комманды подогрева</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_heating_boiler" value="{{mqtt_heating_boiler}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация состояния бойлера</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_statBoiler" value="{{mqtt_pub_statBoiler}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация состояния вытяжки</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_statFan" value="{{mqtt_pub_statFan}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация информационных сообщений</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_info" value="{{mqtt_pub_info}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикаци температуры бойлера</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_tempBoiler" value="{{mqtt_pub_tempBoiler}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация температуры улицы</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_tempUl" value="{{mqtt_pub_tempUl}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация температуры вытяжки</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_tempFan" value="{{mqtt_pub_tempFan}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация температуры датчик4</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_temp4" value="{{mqtt_pub_temp4}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация температуры датчик5</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_temp5" value="{{mqtt_pub_temp5}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Публикация уровня газа</span></td>
                            <td><input type="text" class="field form-control" id="mqtt_pub_gasVol" value="{{mqtt_pub_gasVol}}" pattern="[-\w\s#@/]{1,30}"></td>
                        </tr>
                    </table>
                </center>
            </div>
            <br>
            <center>
                <span class="save_booton" onclick="set_config(this);">Сохранить</span>
            </center>
            <br>
            <hr>
            <br>
            <titles>
                <span class="opt_cn">Настройки narodmon.ru</span>
            </titles>
            <div id="main_unit">
                <label class="switch">
                    <span class="opt_cn">Отправлять данные на narodmon сервер</span>
                    <input type="checkbox" class="checkbox" id="postNmon" name="switch-btn" onclick="set_nmon_on(this);">
                    <span class="switch-btn"></span>
                </label>
                <paragraph><span class="opt_cn">Параметры сервера:</span></paragraph>
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">MAC адрес устройства:&nbsp&nbsp</span></td>
                            <td><span id="macNmon">VZ{{macNmon}}</span></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Название (необязательно)</span></td>
                            <td><input type="text" class="field form-control" id="nameNmon" value="{{nameNmon}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Владелец (необязательно)</span></td>
                            <td><input type="text" class="field form-control" id="ownerNmon" value="{{ownerNmon}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Широта (необязательно)</span></td>
                            <td><input type="text" class="field form-control" id="latNmon" value="{{latNmon}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Долгота (необязательно)</span></td>
                            <td><input type="text" class="field form-control" id="lonNmon" value="{{lonNmon}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Высота (необязательно)</span></td>
                            <td><input type="text" class="field form-control" id="altNmon" value="{{altNmon}}" pattern="[- 0-9a-zA-Z._/]{1,30}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Период отправки (мин.)</span></td>
                            <td>
                                <select id="periodSendNmon">
                                    <option value = "1"> 1 </option>
                                    <option value = "2"> 2 </option>
                                    <option value = "3"> 3 </option>
                                    <option value = "4"> 4 </option>
                                    <option value = "5"> 5 </option>
                                    <option value = "10"> 10 </option>
                                    <option value = "20"> 20 </option>
                                    <option value = "30"> 30 </option>
                                    <option value = "45"> 45 </option>
                                    <option value = "60"> 60 </option>
                                </select>
                            </td>
                        </tr>
                        </table>
                </center>  
                <paragraph><span class="opt_cn">Каналы отправки данных</span></paragraph>
                <center>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">1&nbsp&nbsp</span></td>
                            <td>
                                <select id="writeNmon0">
                                    <option value = "0"> Не использовать </option>
                                    <option value = "1"> Датчик №1 </option>
                                    <option value = "2"> Датчик №2 </option>
                                    <option value = "3"> Датчик №3 </option>
                                    <option value = "4"> Датчик №4 </option>
                                    <option value = "5"> Датчик №5 </option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">2&nbsp&nbsp</span></td>
                            <td>
                                <select id="writeNmon1">
                                    <option value = "0"> Не использовать </option>
                                    <option value = "1"> Датчик №1 </option>
                                    <option value = "2"> Датчик №2 </option>
                                    <option value = "3"> Датчик №3 </option>
                                    <option value = "4"> Датчик №4 </option>
                                    <option value = "5"> Датчик №5 </option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">3&nbsp&nbsp</span></td>
                            <td>
                                <select id="writeNmon2">
                                    <option value = "0"> Не использовать </option>
                                    <option value = "1"> Датчик №1 </option>
                                    <option value = "2"> Датчик №2 </option>
                                    <option value = "3"> Датчик №3 </option>
                                    <option value = "4"> Датчик №4 </option>
                                    <option value = "5"> Датчик №5 </option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">4&nbsp&nbsp</span></td>
                            <td>
                                <select id="writeNmon3">
                                    <option value = "0"> Не использовать </option>
                                    <option value = "1"> Датчик №1 </option>
                                    <option value = "2"> Датчик №2 </option>
                                    <option value = "3"> Датчик №3 </option>
                                    <option value = "4"> Датчик №4 </option>
                                    <option value = "5"> Датчик №5 </option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">5&nbsp&nbsp</span></td>
                            <td>
                                <select id="writeNmon4">
                                    <option value = "0"> Не использовать </option>
                                    <option value = "1"> Датчик №1 </option>
                                    <option value = "2"> Датчик №2 </option>
                                    <option value = "3"> Датчик №3 </option>
                                    <option value = "4"> Датчик №4 </option>
                                    <option value = "5"> Датчик №5 </option>
                                </select>
                            </td>
                        </tr>
                    </table>
                </center>
            </div>
            <br>
            <center>
                <span class="save_booton" onclick="set_config(this);">Сохранить</span>
            </center>
            <br>
            <hr>
            <br>
            <titles><span class="opt_cn">Прошивка и сброс устройства...</span></titles>
            <div class="informer"><span class="opt_cn">ВНИМАНИЕ!!! Некоторые изменения в этом блоке могут быть необратимыми !!!</span>
            </div>            
            <center>
                <div>
                    <b><span class="opt_cn">Обновление: пожалуйста выберите и загрузите прошивку (bin)</span></b>
                    <form method="POST" action="/update" enctype="multipart/form-data">
                        <div>
                            <input type="file"  name="update" style="height: 33px;">
                            <input type="submit" class="save_booton" value="Загрузить" onclick="this.value='Ожидайте...';">
                        </div>
                    </form>
                </div>
            </center>
            <div class="menu_unit">
                <a class="link__img" href="#"><span class="ico__text" onclick="restart(this,'Перезагрузить ESP?');">Перезагрузить устройство</span></a>
            </div>
            <div class="menu_unit">
                <a class="center" href="#"><span class="ico__text" onclick="resetConfig(this,'Сбросить настройки устройства?');">Сброс настроек по умолчанию</span></a>
            </div>
            <div class="informer"><a href="https://play.google.com/store/apps/details?id=com.vz.kitchen">ANDROID приложение</a>
            </div>  
            <div>
                <a href="http://forum.amperka.ru/threads/умный-бойлер.20700/">Forum</a>
                <a href="https://github.com/IZ76/VZ_Boiler" class = "right">GitHub</a>
        </div>
        </central-unit>
    </body>
</html>
)=====";
