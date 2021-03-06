#include <iostream>
#include "data.h"

#define MQTT_SUB_TOPIC_NUMBER 2
#define MQTT_SUB_TOPIC_STORED 1
#define MQTT_PUB_TOPIC_NUMBER 2
#define MQTT_FLOAT_TOPIC_NUMBER 0
#define MQTT_ALARM_VAR_NUMBER 2

#define MQTT_MAIN_TOPIC "asshome"
#define MQTT_CALEFACTOR_TOPIC "calefactor"

#define MQTT_POWER_TOPIC "power"
#define MQTT_STATUS_TOPIC "status"

#define MQTT_CALEFACTOR_POWER_TOPIC MQTT_MAIN_TOPIC "/" MQTT_CALEFACTOR_TOPIC "/" MQTT_POWER_TOPIC
#define MQTT_CALEFACTOR_STATUS_TOPIC MQTT_MAIN_TOPIC "/" MQTT_CALEFACTOR_TOPIC "/" MQTT_STATUS_TOPIC

#define MQTT_CALEFACTOR_POWER_TOPIC_ENUM 1
#define MQTT_CALEFACTOR_STATUS_TOPIC_ENUM 2

#define MQTT_CALEFACTOR_POWER_VALUE_ALARM 1
#define MQTT_CALEFACTOR_STATUS_VALUE_ALARM 0

#define MQTT_CALEFACTOR_POWER_NAME_ALARM MQTT_CALEFACTOR_POWER_TOPIC
#define MQTT_CALEFACTOR_STATUS_NAME_ALARM MQTT_CALEFACTOR_STATUS_TOPIC

const char* mqtt_sub_topics[MQTT_SUB_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_POWER_TOPIC,
  MQTT_CALEFACTOR_STATUS_TOPIC
};
int mqtt_sub_topics_int[MQTT_SUB_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_POWER_TOPIC_ENUM,
  MQTT_CALEFACTOR_STATUS_TOPIC_ENUM
};
const char* mqtt_pub_topics[MQTT_PUB_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_POWER_TOPIC,
  MQTT_CALEFACTOR_STATUS_TOPIC
};
const char* mqtt_float_topics[MQTT_FLOAT_TOPIC_NUMBER] = {
};

uint16_t cpower;
uint16_t cstatus;

uint16_t *mqtt_sub_var[MQTT_SUB_TOPIC_NUMBER] = {
  &cpower,
  &cstatus
};
const uint16_t *mqtt_pub_var[MQTT_PUB_TOPIC_NUMBER] = {
  &cpower,
  &cstatus
};
uint16_t *mqtt_alarm_var[MQTT_ALARM_VAR_NUMBER] = {
  &cpower,
  &cstatus
};
const uint16_t mqtt_alarm_val[MQTT_ALARM_VAR_NUMBER] = {
  MQTT_CALEFACTOR_POWER_VALUE_ALARM,
  MQTT_CALEFACTOR_STATUS_VALUE_ALARM
};
const char* mqtt_alarm_name[MQTT_ALARM_VAR_NUMBER] = {
  MQTT_CALEFACTOR_POWER_NAME_ALARM,
  MQTT_CALEFACTOR_STATUS_NAME_ALARM
};
