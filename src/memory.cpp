#include <EEPROM.h>

#define EEPROM_CALEFACTOR_POWER_DIR 0

#define CALEFACTOR_POWER_DEFAULT 1
#define CALEFACTOR_POWER_MAX 1
#define CALEFACTOR_POWER_MIN 0

void eeprom_init(int n_var) {
    EEPROM.begin(sizeof(uint16_t)*n_var);
}

uint16_t read_memory(int dir)
{
  uint16_t value;
  value = EEPROM.read(dir*2)&0xFF;
  value <<= 8;
  value |= EEPROM.read(dir*2+1)&0xFF;
  return value;
}


void update_memory(int dir, uint16_t value)
{
  if (read_memory(dir) != value) {
    uint16_t value_tmp = value;
    EEPROM.write(dir*2, (uint8_t) (value_tmp>>8)&0xFF);
    EEPROM.write(dir*2+1, (uint8_t) value_tmp&0xFF);
    EEPROM.commit();
  }
}

uint16_t read_eeprom_calefactor_power(void) {
  uint16_t value = read_memory(EEPROM_CALEFACTOR_POWER_DIR);
  if(value==0xFFFF)
  {
    value = CALEFACTOR_POWER_DEFAULT;
  }
  return value;
}

uint16_t update_eeprom_calefactor_power(uint16_t value) {
  uint16_t val =  value;
  if (val>CALEFACTOR_POWER_MAX) 
  {
    val = CALEFACTOR_POWER_MAX;
  } 
  else if (val<CALEFACTOR_POWER_MIN) 
  {
    val = CALEFACTOR_POWER_MIN;
  }
  update_memory(EEPROM_CALEFACTOR_POWER_DIR, val);
  return val;
}
