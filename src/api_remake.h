#ifndef I2C_API_REMAKE_H
#define I2C_API_REMAKE_H

#include "esp_err.h"
#include "driver/i2c.h"


#define ACK_CHECK_EN 0x1
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0
#define ACK_VAL 0x0
#define NACK_VAL 0x1
#define MAX30102_ADDR 0x57

#define SDA_PIN 26
#define SCL_PIN 25

esp_err_t i2c_remake_sensor_init(void);
esp_err_t i2c_remake_sensor_read();
esp_err_t i2c_remake_sensor_write(/*uint8_t *data_wr, size_t size*/);



#endif