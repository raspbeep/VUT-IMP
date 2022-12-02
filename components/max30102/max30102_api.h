#ifndef MAX30102_H
#define MAX30102_H

#include "esp_err.h"

// STATUS
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03

#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07

// CONFIGURATION

#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A

// RESERVED 0x0B

#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D

// RESERVED 0x0E
// RESERVED 0x0F
// UNKNOWN 0x10

#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12

// RESERVED 0x13 - 0x1E

#define REG_TEMP_INT 0x1F
#define REG_TEMP_FRAC 0x20

#define REG_TEMP_EN 0x21

// RESERVED 0x22 - 0x25

#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

typedef struct{
	union{
		uint8_t data1;  //0x02  REG_INTR_ENABLE_1
		struct {
			uint8_t PWR_RDY_EN         :1;	// Power Ready interrupt enable after power-up or brownout to signal readiness
			uint8_t	RESERVED           :4;	// RESERVED
			uint8_t ALC_OVF_EN         :1;	// Ambient light cancellation overflow interrupt enable
			uint8_t PPG_RDY_EN         :1;  // New FIFO data is ready interrupt enable
			uint8_t A_FULL_EN          :1;	// FIFO Almost Full interrupt enable
		} INT_EN_1;
	};

	union{
		uint8_t data2;  //0x03 REG_INTR_ENABLE_2
		struct {
			uint8_t	RESERVED1          :1; // RESERVED
			uint8_t DIE_TEMP_RDY_EN    :1;	// Die temperature ready interrupt enable, signals temperature conversion is ready
			uint8_t RESERVED2          :6;	// RESERVED
		} INT_EN_2;
	};

	union{
		uint8_t data3;  //0x04   REG_FIFO_WR_PTR
		struct {
			uint8_t	FIFO_WR_PTR        :5;	// location for the next sample, pointer advances with each new sample in FIFO
			uint8_t RESERVED           :3;  // RESERVED
		} FIFO_WRITE_PTR;
	};

	union{
		uint8_t data4;  //0x05     REG_OVF_COUNTER
		struct{
			uint8_t	OVF_COUNTER        :5; // number of samples lost due to full FIFO, resets to zero with a new read
			uint8_t RESERVED           :3; // RESERVED
		}OVEF_COUNTER;
	};

	union{
		uint8_t data5;  //0x06    REG_FIFO_RD_PTR
		struct{
			uint8_t	FIFO_RD_PTR        :5; // 
			uint8_t RESERVED           :3; // RESERVED
		}FIFO_READ_PTR;
	};

	union{
		uint8_t data6;  //0x08   REG_FIFO_CONFIG
		struct{
			uint8_t	FIFO_A_FULL        :4; // FIFO almost full value, sets when to trigger the A_FULL interrupt
			uint8_t FIFO_ROLLOVER_EN   :1; // enable rewriting data in FIFO when full 
			uint8_t SMP_AVE            :3; // enable averaging of samples to deacrease data throughput
		}FIFO_CONF;
	};

	union{
		uint8_t data7; //0x09   REG_MODE_CONFIG
		struct{
			uint8_t MODE               :3; // mode of operation; 010 HR only, 011 SPO2, 111 Multi-LED mode
			uint8_t	RESERVED           :3; // RESERVED
			uint8_t RESET              :1; // reset control
			uint8_t SHDN               :1; // shutdown control, puts device in the power save mode
		}MODE_CONF;
	};

	union {
		uint8_t data8; //0x0A   REG_SPO2_CONFIG
		struct {
			uint8_t LED_PW             :2; // LED pulse width(R and IR have the same); 00=69us, 01=118us, 10=215us, 11=411us
			uint8_t	SPO2_SR            :3; // Sample rate control, defines the effective sampling rate
			uint8_t SPO2_ADC_RGE       :2; // ADC range control, defines the resolution of 1b and therefore maximum range of the ADC
			uint8_t RESERVED           :1; // RESERVED
		} SPO2_CONF;
	};

	union{
		uint8_t data9; //0X0C   REG_LED1_PA
		struct{
			uint8_t LED1_PA            :8; // LED1 pulse amplitute
		}LED1_PULSE_AMP;
	};

	union{
		uint8_t data10; //0X0D   REG_LED2_PA
		struct{
			uint8_t LED2_PA            :8; // LED2 pulse amplitute
		}LED2_PULSE_AMP;
	};
	union{
		uint8_t data12; //0X11 REG_MULTI_LED_CTRL1
		struct {
			uint8_t SLOT1                :3;
			uint8_t RESERVED2            :1;
			uint8_t SLOT2                :3;
			uint8_t RESERVED1            :1;
		} MULTI_LED_CONTROL1;
	};

	union{
		uint8_t data13; //0X12  REG_MULTI_LED_CTRL2
		struct {
			uint8_t SLOT3                :3;
			uint8_t RESERVED2            :1;
			uint8_t SLOT4                :3;
			uint8_t RESERVED1            :1;
		} MULTI_LED_CONTROL2;
	};

} max_config;


void max30102_init(max_config *configuration);
void write_max30102_reg(uint8_t command, uint8_t reg);
void read_max30102_fifo(int32_t *red_data, int32_t *ir_data);
float get_max30102_temp();
void read_max30102_reg(uint8_t reg_addr, uint8_t *data_reg, size_t bytes_to_read);

#endif
