/*
 * uart.h
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */

#ifndef SOURCE_HAL_UART_H_
#define SOURCE_HAL_UART_H_




void UART_Init(cy_cb_scb_uart_handle_events_t callback);
uint32_t UART_Receive();
void UART_Transmit(char* transmitData);

#endif /* SOURCE_HAL_UART_H_ */
