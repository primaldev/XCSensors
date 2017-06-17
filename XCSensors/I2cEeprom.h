/*
  XCSensors http://XCSensors.org

  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
void writeSizeValue(int point, int sizeValue);
int readSizeValue(int point);
void i2c_eeprom_write_psize( int deviceaddress, unsigned int eeaddresspage, byte* data );
void i2c_eeprom_read_psize( int deviceaddress, unsigned int eeaddress, byte *buffer);
bool writeI2CBin(const uint8_t id, uint16_t adr, char data[], const uint16_t len, const uint8_t pageSize);
bool readI2CBin(const uint8_t id, uint16_t adr, byte *data, const uint16_t len, const uint8_t pageSize);
