#include <Arduino.h>
#include <WiFi.h>

#include <PubSubClient.h>
#define LED 2
// #define
const char*ssid="Oneplus Ace5";
const char*password="qwqee2333";
const char*mqtt_server="broker.hivemq.com";
const int mqtt_port=1883;

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char*topic,byte*payload,unsigned int length) {

    String msg="";
    for (int i=0;i<length;i++) {
        msg+=(char)payload[i];
    }

    Serial.print("收到指令:");
    Serial.println(msg);
    if (String(topic)=="esp32/led") {
        if (msg=="ON") {
            digitalWrite(LED,HIGH);
            Serial.println("开灯");
        }else if (msg=="OFF") {
            digitalWrite(LED,LOW);
            Serial.println("关灯");
        }

    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("尝试连接 MQTT...");
        // 使用随机的客户端 ID，避免冲突
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("连接成功");
            // 重新订阅主题
            client.subscribe("esp32/led");
        } else {
            Serial.print("失败, rc=");
            Serial.print(client.state());
            Serial.println(" 5秒后重试");
            delay(5000);
        }
    }
}

void setup() {
// write your initialization code here
    Serial.begin(115200);
    pinMode(2,OUTPUT);
    WiFi.begin(ssid,password);
    while (WiFi.status()!=WL_CONNECTED) {
        delay(500);
        Serial.print('.');
    }
    Serial.println("WiFi已连接");
    client.setServer(mqtt_server,mqtt_port);
    client.setCallback(callback);


}

void loop() {
// write your code here
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    //jfgjhsgfjhsgjfgjsdgjsgfdhghjsdgjfhgjsdhfghjsdgjhfghjds
}