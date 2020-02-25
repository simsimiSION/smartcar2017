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
  
#include "nrf2401.h"
#include "spi_abstraction.h"



//?¨¢??¡ä??¡Â
static uint8_t read_reg(uint8_t addr)
{
    uint8_t temp = READ_REG + addr;
    spi_write(&device, &temp, 1, false);
    spi_read(&device, &temp, 1, true);
    return temp;
}
//D¡ä??¡ä??¡Â
static void write_reg(uint8_t addr, uint8_t val)
{
    uint8_t temp = WRITE_REG + addr;
    spi_write(&device, &temp, 1, false);
    spi_write(&device, &val, 1, true);
}
//D¡ä¨ºy?Y
static void write_buffer(uint8_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t temp = WRITE_REG + addr;
    spi_write(&device, &temp, 1, false);
    spi_write(&device, buf, len, true);
}
//?¨¢¨ºy?Y
static void read_buffer(uint8_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t temp = READ_REG + addr;
    spi_write(&device, &temp, 1, false);
    spi_read(&device, buf, len, true);
}
//NRF¨¦¨¨¡À??¨¬2a¡ê?2¡é?????¨®¨º?o¨ª¡¤¡é?¨ª¦Ì??¡¤
int nrf24l01_probe(void)
{
    uint8_t i;
    uint8_t buf[5];
    read_buffer(TX_ADDR, buf, 5);
    for(i = 0; i < 5; i++)
    {
        if((buf[i]!= 0) && (buf[i] != 0xFF))
        {
            NRF24L01_CE_LOW();
            /* init sequence */
            write_reg(CONFIG, 0x0F); /* config */
            write_reg(EN_AA, 0x03);/* aa */
            write_reg(EN_RXADDR, 0x03); /* available pipe */
            write_reg(SETUP_AW, 0x03);  /* setup_aw */
            write_reg(SETUP_RETR, 0x07);/* setup_retr */
            write_reg(RF_CH,40);/* RF freq */
            write_reg(RF_SETUP, 0X04);
            write_reg(RX_PW_P0, 0X20);
            write_reg(RX_PW_P1, 0X20);
            write_reg(RX_PW_P2, 0X20);
            write_reg(RX_PW_P3, 0X20);
            write_reg(RX_PW_P4, 0X20);
            write_reg(RX_PW_P5, 0X20);
            write_reg(PYNPD, 0x3F); /* enable dynmic playload whhich means packet len is varible */
            write_reg(FEATURE, 0x07); /* enable dynmic playload whhich means packet len is varible */
            write_buffer(RX_ADDR_P0, (uint8_t*)RX_ADDRESS, 5);
            write_buffer(TX_ADDR, (uint8_t*)TX_ADDRESS, 5);
            return 0;
        }
    }
    return 1;
}
//NRF?¡ê?¨¦3?¨º??¡¥
int nrf24l01_init(spi_bus_t bus, uint32_t cs)
{
    uint32_t ret;
    device.csn = cs;
    device.config.baudrate = 1*1000*1000;
    device.config.data_width = 8;
    device.config.mode = SPI_MODE_0 | SPI_MASTER | SPI_MSB;
    ret = spi_bus_attach_device(bus, &device);
    if(ret)
    {
        return ret;
    }
    else
    {
        ret = spi_config(&device);
    }
    return ret;
}


//??o¡¥¨ºy3?¨º??¡¥NRF24L01¦Ì?TX?¡ê¨º?
//¨¦¨¨??TX¦Ì??¡¤,D¡äTX¨ºy?Y?¨ª?¨¨,¨¦¨¨??RX¡Á??¡¥¨®|¡äe¦Ì?¦Ì??¡¤,¨¬?3?TX¡¤¡é?¨ª¨ºy?Y,????RF?¦Ì¦Ì¨¤,2¡§¨¬??¨ºo¨ªLNA HCURR
//PWR_UP,CRC¨º1?¨¹
//¦Ì¡ÀCE¡À???o¨®,?¡ä??¨¨?RX?¡ê¨º?,2¡é?¨¦¨°??¨®¨º?¨ºy?Y¨¢?		   
//CE?a??¡ä¨®¨®¨²10us,?¨°???¡¥¡¤¡é?¨ª.

void nrf24l01_set_tx_mode(void)
{
    uint8_t value;
    value = FLUSH_TX;
    spi_write(&device, &value, 1, true);
    NRF24L01_CE_LOW();
    value = read_reg(CONFIG);
    value &= ~CONFIG_PRIM_RX_MASK;
    write_reg(CONFIG, value); /* Set PWR_UP bit, enable CRC(2 length) & Prim:RX. RX_DR enabled.. */
    NRF24L01_CE_HIGH();
}

//??o¡¥¨ºy3?¨º??¡¥NRF24L01¦Ì?RX?¡ê¨º?
//¨¦¨¨??RX¦Ì??¡¤,D¡äRX¨ºy?Y?¨ª?¨¨,????RF?¦Ì¦Ì¨¤,2¡§¨¬??¨ºo¨ªLNA HCURR
//¦Ì¡ÀCE¡À???o¨®,?¡ä??¨¨?RX?¡ê¨º?,2¡é?¨¦¨°??¨®¨º?¨ºy?Y¨¢?	
void nrf24l01_set_rx_mode(void)
{
	uint8_t value;
    /* reflash data */
    value = FLUSH_RX;
    spi_write(&device, &value, 1, true);
	NRF24L01_CE_LOW();
    /* set CONFIG_PRIM_RX_MASK to enable Rx */
    value = read_reg(CONFIG);
    value |= CONFIG_PRIM_RX_MASK;
    write_reg(CONFIG, value);
	NRF24L01_CE_HIGH();
}

//???¡¥NRF24L01¡¤¡é?¨ª¨°?¡ä?¨ºy?Y
//txbuf:¡äy¡¤¡é?¨ª¨ºy?Y¨º¡Á¦Ì??¡¤
//¡¤¦Ì???¦Ì:¡¤¡é?¨ª¨ª¨º3¨¦¡Á¡ä??
int nrf24l01_write_packet(uint8_t *buf, uint32_t len)
{
    uint8_t status;
    uint8_t plos;
    /* clear bits */
    status = read_reg(STATUS);
    status |= STATUS_TX_DS_MASK | STATUS_MAX_RT_MASK;
    write_reg(STATUS, status);
    NRF24L01_CE_LOW();
    /* clear PLOS */
    write_reg(RF_CH, 40); 
    /* write data */
    uint8_t value = WR_TX_PLOAD;
    spi_write(&device, &value, 1, false);
    spi_write(&device, buf, len, true);
    NRF24L01_CE_HIGH();
    while(1)
    {
        status = read_reg(STATUS);
        if(status & STATUS_TX_DS_MASK)
        {
            /* send complete */
            status |= STATUS_TX_DS_MASK | STATUS_MAX_RT_MASK;
            write_reg(STATUS, status);
            return 0;
        }
        status = read_reg(OBSERVE_TX);
        plos = (status & OBSERVE_TX_PLOS_CNT_MASK) >> OBSERVE_TX_PLOS_CNT_SHIFT;
        
        /* if it reach max re send count */
        if(plos && (status & STATUS_MAX_RT_MASK))
        {
            status = FLUSH_TX; /* clear TX FIFO */
            spi_write(&device, &status, 1, true);
            return 1;
        }
    }
}

//NRF?¨¢¨°???¨ºy?Y
int nrf24l01_read_packet(uint8_t *buf, uint32_t *len)
{
	uint8_t sta,rev_len;
    sta = read_reg(STATUS);
	if(sta & STATUS_RX_DR_MASK)
	{
        /* clear pendign bit */
        sta |= STATUS_RX_DR_MASK;
        write_reg(STATUS, sta);
        /* read len and data */
        rev_len = read_reg(R_RX_PL_WID);
        read_buffer(RD_RX_PLOAD, buf, rev_len);
        *len = rev_len;
        /* if rev_len > 32 which means a error occur, usr FLUSH_RX to clear */
        if(rev_len > 32)
        {
            NRF24L01_TRACE("rev len > 32, error!\r\n");
            sta = FLUSH_RX;
            spi_write(&device, &sta, 1, true);
            *len = 32;
            return 2;
        }
        return 0;
	}
    *len = 0;
    return 1;
}

