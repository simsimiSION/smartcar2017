/**
  ******************************************************************************
  * @file    nrf24l01.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    ¡ä????t?aNRF24L01?T???¡ê?¨¦¦Ì??y?¡¥¡ê??¡ì3?¨¬¡§2¨²o¨ª?2¨ªt2¨²D???
  ******************************************************************************
  */
#ifndef __NRF2401_H
#define __NRF2401_H	 		  

#include <stdint.h>
#include "gpio.h"




static struct spi_device device;
//<! API functions
void nrf24l01_set_tx_mode(void);
void nrf24l01_set_rx_mode(void);
int nrf24l01_write_packet(uint8_t *buf, uint32_t len);
int nrf24l01_read_packet(uint8_t *buf, uint32_t *len);
int nrf24l01_probe(void);
int nrf24l01_init(spi_bus_t bus, uint32_t cs);


#endif











