#include <ESPAsyncWebServer.h>
#include<WiFi.h>

AsyncWebServer server(80);
const int LED = 2;

const char* ssid = "iPhone";
const char* password = "jebel123";

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
    <p id="stat">Status</p>
    </div>
<script>
    document.getElementById("bON").addEventListener("click",function(){fetch("/led/on") .then(function(res){return res.text();}).then(function(data){document.getElementById("stat").innerText=data})});
    document.getElementById("bOFF").addEventListener("click",function(){fetch("/led/off").then(function(res){return res.text();}).then(function(data){document.getElementById("stat").innerText=data})});
</script>
</body>
</html>)";
void setup(){
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,0);
  

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

  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request ){
    digitalWrite(LED,HIGH);
    request->send(200,"text/plain","LED is ON");
  });
  server.on("/led/off",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(LED,LOW);
    request->send(200,"text/plain","LED is OFF");
  });
  server.begin();
}

void loop(){

}