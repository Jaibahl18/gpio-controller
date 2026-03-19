#include <ESPAsyncWebServer.h>
#include "secrets.h"//to store wifi passwords before pushing on git
#include<WiFi.h>

AsyncWebServer server(80);
const int LED = 2;


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

const char* html = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Digital Controller</title>
    
</head>
<style>
    h1 {
        font-family:system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
        text-align: center;

    }

    .buttons{
        display: flex;
        height: 100vh;
        flex-direction: column;
        margin-top: 10%;
        gap: 10%;
        align-items: center;

    }

    button{
        width: 50%;
        height: 5%;
    }
    p {
        font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
        color: red;
    }



</style>

<body>
    <h1>Digital GPIO Controller</h1>
    <div class="buttons">
<button id="bON">On</button>
    <button id="bOFF">Off</button>
    <input type="range" name="pwm" id="pwm" min="0" max="255">
    <p id="stat">Status</p>
    </div>
<script>
    document.getElementById("bON").addEventListener("click",function(){fetch("/led/on") .then(function(res){return res.text();}).then(function(data){document.getElementById("stat").innerText=data})});
    document.getElementById("bOFF").addEventListener("click",function(){fetch("/led/off").then(function(res){return res.text();}).then(function(data){document.getElementById("stat").innerText=data})});
    document.getElementById("pwm").addEventListener("input",function(){fetch("/led/brightness?val="+this.value)});
    
</script>
</body>
</html>)";
void setup(){
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(LED, 0); 
  
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.println("connecting to WiFi ");
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    Serial.print(".");
  }
  Serial.println("connected to the WiFi");
  Serial.println("IP : ");
  Serial.print(WiFi.localIP());

  server.on("/",HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200,"text/html",html);
  });
  server.on("/led/brightness",HTTP_GET,[] (AsyncWebServerRequest *request){
    int val = request->getParam("val")->value().toInt();
    ledcWrite(0,val);
    request->send(200,"text/plain","PWM value set");
  });

  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request){
    ledcWrite(0,255);
    request->send(200,"text/plain","LED is ON");
  });
  server.on("/led/off",HTTP_GET,[](AsyncWebServerRequest *request){
    ledcWrite(0,0);
    request->send(200,"text/plain","LED is OFF");
  });
  server.begin();
}

void loop(){

}