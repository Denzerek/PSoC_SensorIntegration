#pragma once

#include "common.h"


#ifdef I2C_HAL_DEBUG_ENABLE
#define I2CHAL_PRINT(...)	debugTransmit(I2CHAL_TASKMSG,__VA_ARGS__)
#else
#define I2CHAL_PRINT(...)
#endif


void i2c_scb3_master_clearInterrupt(uint32_t event);
cy_en_scb_i2c_status_t i2c_init(cy_cb_scb_i2c_handle_events_t cy_cb_scb_i2c_handle_events);
cy_en_scb_i2c_status_t i2c_scb3_master_write(cy_stc_scb_i2c_master_xfer_config_t *transfer);
uint32_t i2c_scb3_master_get_status();
cy_en_scb_i2c_status_t i2c_scb3_master_read(cy_stc_scb_i2c_master_xfer_config_t *transfer);
