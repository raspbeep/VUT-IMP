#include "max30102_api.h"
#include "i2c_api.h"


void max30102_init(max_config *configuration) {
	write_max30102_reg(configuration->data2,  REG_INTR_ENABLE_2);
	write_max30102_reg(configuration->data3,  REG_FIFO_WR_PTR);
	write_max30102_reg(configuration->data4,  REG_OVF_COUNTER);
	write_max30102_reg(configuration->data5,  REG_FIFO_RD_PTR);
	write_max30102_reg(configuration->data6,  REG_FIFO_CONFIG);
	write_max30102_reg(configuration->data7,  REG_MODE_CONFIG);
	write_max30102_reg(configuration->data8,  REG_SPO2_CONFIG);
	write_max30102_reg(configuration->data9,  REG_LED1_PA);
	write_max30102_reg(configuration->data10, REG_LED2_PA);
	write_max30102_reg(configuration->data12, REG_MULTI_LED_CTRL1);
	write_max30102_reg(configuration->data13, REG_MULTI_LED_CTRL2);
}


void read_max30102_fifo(int32_t *red_data, int32_t *ir_data) {

	uint8_t fifo_wr_ptr = 0;
	uint8_t fifo_rw_reg_addr = REG_FIFO_WR_PTR;
	uint8_t fifo_rd_ptr = 0;
	uint8_t fifo_rd_reg_addr = REG_FIFO_RD_PTR;

	i2c_sensor_write(&fifo_rw_reg_addr, 1);
	i2c_sensor_read(&fifo_wr_ptr, 1);

	i2c_sensor_write(&fifo_rd_reg_addr, 1);
	i2c_sensor_read(&fifo_rd_ptr, 1);

	uint8_t available_samples = fifo_wr_ptr - fifo_rd_ptr;

	printf("###### avaliable samples: %d ######\n", available_samples);
	for (int i = 0; i < available_samples; i++) {
		uint8_t fifo_data_reg_addr = REG_FIFO_DATA;
		i2c_sensor_write(&fifo_data_reg_addr, 1);
		uint8_t fifo_data[6];
		i2c_sensor_read(fifo_data, 6);

		*red_data = (fifo_data[0] << 16) | (fifo_data[1] << 8) | fifo_data[2];
		*ir_data = (fifo_data[3] << 16) | (fifo_data[4] << 8) | fifo_data[5];
	}
	printf("%d\n", *red_data);
	printf("%d\n", *ir_data);
}


void read_max30102_reg(uint8_t reg_addr, uint8_t *data_reg, size_t bytes_to_read) {
	ESP_ERROR_CHECK(i2c_sensor_write(&reg_addr, 1));
	ESP_ERROR_CHECK(i2c_sensor_read(data_reg, bytes_to_read));
}


void write_max30102_reg(uint8_t command, uint8_t reg) {
	uint8_t data[2];
	data[0] = reg;
	data[1] = command;
	ESP_ERROR_CHECK(i2c_sensor_write(data, 2));
}


float get_max30102_temp() {
	uint8_t int_temp = 0;
	uint8_t decimal_temp = 0;
	float temp = 0;
	write_max30102_reg(1, REG_TEMP_EN); // enable one-time temperature measurement
	read_max30102_reg(REG_TEMP_INT, &int_temp, 1); // temperature in two's complement
	read_max30102_reg(REG_TEMP_FRAC, &decimal_temp, 1); // in increments of 0.0625 degrees C
	temp = (int_temp + (decimal_temp * 0.0625));
	return temp;
}


