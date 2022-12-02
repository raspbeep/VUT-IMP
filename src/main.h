#ifndef MAIN_H
#define MAIN_H

#include "esp_err.h"
#include "max30102_api.h"

void sensor_data_processor(void *pvParameters);
void sensor_data_reader(void *pvParameters);
void fill_buffers_data();

#define BUFFER_SIZE 128

max_config max30102_configuration = {

		.INT_EN_1.A_FULL_EN         = 1,        // Enable FIFO Almost Full Interrupt
		.INT_EN_1.PPG_RDY_EN        = 1,        // enable interrupt when new data in FIFO is ready
		.INT_EN_1.ALC_OVF_EN        = 1,        // triggers when ambient light affects output        
		.INT_EN_1.PWR_RDY_EN        = 0,        // disable interrupt when sensor is ready

		.INT_EN_2.DIE_TEMP_RDY_EN   = 1,        // enable interrupt when tempereture data is ready

		.FIFO_WRITE_PTR.FIFO_WR_PTR = 0,

		.OVEF_COUNTER.OVF_COUNTER   = 0,

		.FIFO_READ_PTR.FIFO_RD_PTR  = 0,

		.FIFO_CONF.SMP_AVE          = 0,        // no averaging  
		.FIFO_CONF.FIFO_ROLLOVER_EN = 1,        // disable FIFO roll on full    
		.FIFO_CONF.FIFO_A_FULL      = 0x2,      // set interrupt when there are 30 unread samples

		.MODE_CONF.SHDN             = 0,
		.MODE_CONF.RESET            = 0,
		.MODE_CONF.MODE             = 0b011,    // set SpO2 mode (Red and IR LEDs)

		.SPO2_CONF.SPO2_ADC_RGE     = 0b10,     // LSB = 31.25 pA, maximum range 8192 nA
		.SPO2_CONF.SPO2_SR          = 0b001,    // 100 samples per second
		.SPO2_CONF.LED_PW           = 0b11,     // pulse width = 411us to achieve 18bit ADC resolution

		.LED1_PULSE_AMP.LED1_PA     = 0x7F,     // 25.4mA for Red LED
		.LED2_PULSE_AMP.LED2_PA     = 0x7F,     // 25.4mA for IR LED

		.MULTI_LED_CONTROL1.SLOT2   = 0,        // slot 2 disabled(using SpO2 mode)
		.MULTI_LED_CONTROL1.SLOT1   = 0,        // slot 1 disabled(using SpO2 mode)

		.MULTI_LED_CONTROL2.SLOT4   = 0,        // slot 4 disabled(using SpO2 mode)
		.MULTI_LED_CONTROL2.SLOT3   = 0,        // slot 3 disabled(using SpO2 mode)
};

#endif
