#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"
// #include "font8x8_basic.h"

#include "main.h"
#include "max30102_api.h"
#include "algorithm.h"
#include "i2c_api.h"

#define tag "SSD1306"
#define DEBUG 1
#define DELAY_AMOSTRAGEM 40


SSD1306_t dev;
TaskHandle_t sensor_reader_handle = NULL;

int32_t red_data = 0;
int32_t ir_data = 0;
int32_t red_data_buffer[BUFFER_SIZE];
int32_t ir_data_buffer[BUFFER_SIZE];
double auto_correlationated_data[BUFFER_SIZE];
char *data = NULL;

#define LINE_COUNT 8
#define LINE_LENGTH 20

char lines[LINE_COUNT][LINE_LENGTH] = {0};


void clean_lines() {
    for (int i = 0; i < LINE_COUNT; i++) {
        memset(lines[i], 0, LINE_LENGTH);
    }
    
    char bpm_legend[] = "BPM: ";
    char spo_legend[] = "SPO2: ";
    strcpy(lines[0], bpm_legend);
    strcpy(lines[1], spo_legend);
}

void write_to_display() {
    for (int i = 0; i < LINE_COUNT; i++) {
        ssd1306_display_text(&dev, 0, lines[i], strlen(lines[i]), false);
    }
    clean_lines();
}

void update_display(int bpm, float spo2, double pearson_correlation) {
    ssd1306_clear_screen(&dev, false);

    for (int i = 0; i < LINE_COUNT; i++) {
        memset(lines[i], 0, LINE_LENGTH);
    }

    sprintf(lines[0], "BPM: %d", bpm);
    sprintf(lines[1], "SPO: %f", spo2);
    sprintf(lines[2], "PC: %f", pearson_correlation);

    for (int i = 0; i < LINE_COUNT; i++) {
        ssd1306_display_text(&dev, i, lines[i], strlen(lines[i]), false);
    }
}


void init_display() {
    spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
}

void app_main(void) {
	init_display();

    xTaskCreatePinnedToCore(sensor_data_reader, "Data", 10240, NULL, 2, &sensor_reader_handle, 1);
    while(1){};
    
    ssd1306_display_text_x3(&dev, 0, "Hello", 5, false);
    vTaskDelay(3000 / portTICK_PERIOD_MS);


	
	ssd1306_display_text(&dev, 0, "SSD1306 128x64", 14, false);
	ssd1306_display_text(&dev, 1, "ABCDEFGHIJKLMNOP", 16, false);
	ssd1306_display_text(&dev, 2, "abcdefghijklmnop",16, false);
	ssd1306_display_text(&dev, 3, "Hello World!!", 13, false);
	ssd1306_display_text(&dev, 4, "SSD1306 128x64", 14, true);
	ssd1306_display_text(&dev, 5, "ABCDEFGHIJKLMNOP", 16, true);
	ssd1306_display_text(&dev, 6, "abcdefghijklmnop",16, true);
	ssd1306_display_text(&dev, 7, "Hello World!!", 13, true);


	vTaskDelay(3000 / portTICK_PERIOD_MS);

	// Restart module
	esp_restart();
}

void sensor_data_reader(void *pvParameters) {
    uint8_t sta_mac[6] = {0};
	i2c_sensor_init();
	vTaskDelay(pdMS_TO_TICKS(100));
	max30102_init(&max30102_configuration);
	init_time_array();
	uint64_t ir_mean;
	uint64_t red_mean;
	float temperature;
	double r0_autocorrelation;
	size_t size;

	for(;;){
		//vTaskDelay(pdMS_TO_TICKS(100));
		fill_buffers_data();
		temperature = get_max30102_temp();
		remove_dc_part(ir_data_buffer, red_data_buffer, &ir_mean, &red_mean);
		remove_trend_line(ir_data_buffer);
		remove_trend_line(red_data_buffer);
		double pearson_correlation = correlation_datay_datax(red_data_buffer, ir_data_buffer);
		int heart_rate = calculate_heart_rate(ir_data_buffer, &r0_autocorrelation, auto_correlationated_data);

		// printf("\n");
		// printf("HEART_RATE %d\n", heart_rate);
		// printf("correlação %f\n", pearson_correlation);
		// printf("Temperature %f\n", temperature);

		//if(pearson_correlation >= 0.7){
		double spo2 = spo2_measurement(ir_data_buffer, red_data_buffer, ir_mean, red_mean);
        update_display(heart_rate, spo2, pearson_correlation);
		//	printf("SPO2 %f\n", spo2);

	    //    size = asprintf(&data, "{\"mac\": \"%02x%02x%02x%02x%02x%02x\", \"spo2\":%f, \"heart_rate\":%d}",MAC2STR(sta_mac), spo2, heart_rate);
			// mqtt_publish(data, size);
		//}
		printf("\n");

// #if DEBUG
// 		for(int i = 0; i < BUFFER_SIZE; i++){
// 			printf("%d", ir_data_buffer[i]);
// 			printf(" ");
// 			printf("%d", red_data_buffer[i]);
// 			printf(" ");
// 			printf("%f\n", auto_correlationated_data[i]);
// 		}
// #endif
	}
}


void fill_buffers_data()
{
	for(int i = 0; i < BUFFER_SIZE; i++){
		read_max30102_fifo(&red_data, &ir_data);
		ir_data_buffer[i] = ir_data;
		red_data_buffer[i] = red_data;
		//printf("%d\n", red_data);
		ir_data = 0;
		red_data = 0;
		vTaskDelay(pdMS_TO_TICKS(DELAY_AMOSTRAGEM));
	}
}
