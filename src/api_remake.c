#include "api_remake.h"
#include "max30102_api.h"

esp_err_t i2c_remake_sensor_init(void) {
	i2c_config_t i2c_remake_configuration = {
			.mode              = I2C_MODE_MASTER,
			.sda_io_num        = SDA_PIN,
			.sda_pullup_en     = GPIO_PULLUP_ENABLE,
			.scl_io_num        = SCL_PIN,
			.scl_pullup_en     = GPIO_PULLUP_ENABLE,
			.master.clk_speed  = 100000
	};
	i2c_param_config(I2C_NUM_0, &i2c_remake_configuration);
	esp_err_t err = i2c_driver_install(I2C_NUM_0, i2c_remake_configuration.mode, 0, 0, 0);
	return err;
}

esp_err_t i2c_remake_sensor_read(/*uint8_t *data_rd, size_t size*/) {
    
    uint8_t fifo_rd_ptr;
    uint8_t fifo_wr_ptr;
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, MAX30102_ADDR, MAX30102_ADDR, 1, 1000 / portTICK_RATE_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(I2C_NUM_0, MAX30102_ADDR, &fifo_wr_ptr, 1, 1000 / portTICK_RATE_MS));

    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, MAX30102_ADDR, REG_FIFO_RD_PTR, 1, 1000 / portTICK_RATE_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(I2C_NUM_0, MAX30102_ADDR, &fifo_rd_ptr, 1, 1000 / portTICK_RATE_MS));

    
    // ##################

    printf("#################### Have wr: %d, rd: %d\n", fifo_wr_ptr, fifo_rd_ptr);

    return ESP_OK;
}