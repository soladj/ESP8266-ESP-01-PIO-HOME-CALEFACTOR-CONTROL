#include <Arduino.h>
#include <string.h>
#include <bits/stdc++.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "conf.h"
#include "memory.h"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

boolean connection_status;
boolean prev_connection_status;

void setup_WIFI(){
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);

  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Conectado, dirección IP: ");
  Serial.println(WiFi.localIP());
}

bool string_in_float_topic_array(char* value) {
  bool ret = false;
  for (int i=0;i<MQTT_FLOAT_TOPIC_NUMBER;i++)
  {
    if (strcmp(value, mqtt_float_topics[i])==0)
    {
      ret = true;
    }
  }
  return ret;
}

void pub_topics_refresh() {
  char num_char[6];
  for(uint8_t i=0;i<MQTT_PUB_TOPIC_NUMBER;i++) {
    if (string_in_float_topic_array((char*)mqtt_pub_topics[i]))
    {
      sprintf(num_char, "%.2f", ((double)*mqtt_pub_var[i])/100);
    }
    else 
    {
      sprintf(num_char, "%d", (int)*mqtt_pub_var[i]);
    }
    
    mqtt_client.publish(mqtt_pub_topics[i], num_char);
    Serial.print("Message published [");
    Serial.print(mqtt_pub_topics[i]);
    Serial.print("] ");
    Serial.print(num_char);
    Serial.println();
  }
}

void set_alarm_var() {
  for(uint8_t i=0;i<MQTT_ALARM_VAR_NUMBER;i++) {
    *mqtt_alarm_var[i] = (uint16_t) mqtt_alarm_val[i];
    
    Serial.print("Set alarm value [");
    Serial.print(mqtt_alarm_name[i]);
    Serial.print("] ");
    Serial.print(*mqtt_alarm_var[i]);
    Serial.println();
  }
}
void callback(char* topic, unsigned char* payload, unsigned int length) {
  char data[length+1];
  // sprintf(data, "%s", payload);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    data[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  };
  data[length] = 0;
  Serial.println();

  if (strcmp(topic, "asshome/refresh")==0){
    pub_topics_refresh();
  }
  else{
    for(uint8_t i=0; i<MQTT_SUB_TOPIC_NUMBER;i++) {
      if (strcmp(topic, mqtt_sub_topics[i])==0) {
        if (string_in_float_topic_array(topic))
        {
          *mqtt_sub_var[i] = (uint16_t) (atof(data)*100);
          Serial.println(data);
          Serial.println(atof(data));
          Serial.println(atof(data)*100);
          Serial.println((uint16_t) (atof(data)*100));
        }
        else 
        {
          *mqtt_sub_var[i] = atoi(data);
        }
        switch(mqtt_sub_topics_int[i]) {
          case MQTT_CALEFACTOR_POWER_TOPIC_ENUM:
            *mqtt_sub_var[i] = update_eeprom_calefactor_power(*mqtt_sub_var[i]);
            break;
        }
      }
    }
  }
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.print(" clientID=");
    Serial.print(clientId);
    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish(mqtt_topic, "hello world");
      // ... and resubscribe
      mqtt_client.subscribe("asshome/refresh");
        Serial.print("Subscribe to topic: ");
        Serial.println("asshome/refresh");
      for(uint8_t i=0;i<(sizeof(mqtt_sub_topics)/sizeof(*mqtt_sub_topics));i++) {
        mqtt_client.subscribe(mqtt_sub_topics[i]);
        Serial.print("Subscribe to topic: ");
        Serial.println(mqtt_sub_topics[i]);
      }
      
    } else {
      Serial.print(" failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup_comm()
{
  setup_WIFI();

  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_client.setCallback(callback);
  mqtt_reconnect();

  eeprom_init(MQTT_SUB_TOPIC_STORED);
  cpower = read_eeprom_calefactor_power();

  connection_status = prev_connection_status = true;
}

void loop_comm() {
  connection_status = mqtt_client.connected();
  if (connection_status) {
    mqtt_client.loop();
  }
  else {
    if (prev_connection_status != connection_status) {
      set_alarm_var();
    }
    mqtt_reconnect();
  }
  prev_connection_status = connection_status;
}