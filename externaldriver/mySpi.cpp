#include "mySpi.h"
#include "spi.h"
#include "common.h"
#include "gpio.h"
#include "spi.h"

/**
 * @brief  configure spi bus attr
 * @param  device           spi device struct
 * @param  configuration    spi configuration
 * @retval 0:succ other:error
 */
static int kinetis_spi_configure(spi_device_t device, struct spi_config *configuration)
{
    SPI_FrameFormat_Type mode;
    switch(configuration->mode & (SPI_CPHA | SPI_CPHA))
    {
        case SPI_MODE_0:
            mode = kSPI_CPOL0_CPHA0;
            break;
        case SPI_MODE_1:
            mode = kSPI_CPOL0_CPHA1;
            break;
        case SPI_MODE_2:
            mode = kSPI_CPOL1_CPHA0;
            break;
        case SPI_MODE_3:
            mode = kSPI_CPOL1_CPHA1;
            break;
        default:
            break;
    }
    SPI_CTARConfig(device->bus->instance, HW_CTAR0, mode, configuration->data_width, 0x00, configuration->baudrate);
    return 0;
}

/**
 * @brief  read data for spi bus
 * @param  device           spi device struct
 * @param  buf              pointer of data to be readed
 * @param  len              read size
 * @param  cs_return_inactive 
 *         @arg true : after data sended, cs return inactive state(normally high)
 *         @arg false :fater data sended, cs still in acvtive state(normally low)
 * @retval 0:succ other:error
 */
static int kinetis_spi_read(spi_device_t device, uint8_t *buf, uint32_t len, SPI_PCS_Type cs_return_inactive)
{
    uint16_t dummy = 0xFFFF;
    len--;
    while(len--)
    {
        *buf++ = SPI_ReadWriteByte(device->bus->instance ,HW_CTAR0, dummy, device->csn, kSPI_PCS_KeepAsserted);
    }
    if(cs_return_inactive == kSPI_PCS_KeepAsserted)
    {
        *buf++ = SPI_ReadWriteByte(device->bus->instance, HW_CTAR0, dummy, device->csn, kSPI_PCS_KeepAsserted);
    }
    else
    {
        *buf++ = SPI_ReadWriteByte(device->bus->instance, HW_CTAR0, dummy, device->csn, kSPI_PCS_ReturnInactive);
    }
    return SPI_EOK;
}

/**
 * @brief  write data for spi bus
 * @param  device           spi device struct
 * @param  buf              pointer of data to be written
 * @param  len              read size
 * @param  cs_return_inactive 
 *         @arg true : after data sended, cs return inactive state(normally high)
 *         @arg false :fater data sended, cs still in acvtive state(normally low)
 * @retval 0:succ other:error
 */
static int kinetis_spi_write(spi_device_t device, uint8_t *buf, uint32_t len, SPI_PCS_Type cs_return_inactive)
{
    len--;
    while(len--)
    {
        SPI_ReadWriteByte(device->bus->instance, HW_CTAR0, *buf++, device->csn, kSPI_PCS_KeepAsserted);
    }
    if(cs_return_inactive == kSPI_PCS_KeepAsserted)
    {
        SPI_ReadWriteByte(device->bus->instance, HW_CTAR0, *buf++, device->csn, kSPI_PCS_KeepAsserted);
    }
    else
    {
        SPI_ReadWriteByte(device->bus->instance, HW_CTAR0, *buf++, device->csn, kSPI_PCS_ReturnInactive);
    }
    return SPI_EOK;
}

/* spi bus layer register struct */
const static struct spi_ops kinetis_spi_ops = 
{
    kinetis_spi_configure,
    kinetis_spi_read,
    kinetis_spi_write,
};

/**
 * @brief  initalize an spi bus
 * @param  bus              pointer of a bus struct
 * @param  instance         instance, used by hardware driver
 * @retval 0:succ other:error
 */
int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance)
{
    
    /* init hardware with defalt settings */
    SPI_InitTypeDef SPI_InitStruct1;
    SPI_InitStruct1.baudrate = 1000*1000;
    SPI_InitStruct1.frameFormat = kSPI_CPOL0_CPHA1;
    SPI_InitStruct1.dataSize = 8;
    SPI_InitStruct1.instance = instance;
    SPI_InitStruct1.mode = kSPI_Master;
    SPI_InitStruct1.bitOrder = kSPI_MSB;
    SPI_InitStruct1.ctar = HW_CTAR0;
    SPI_Init(&SPI_InitStruct1);
    
    /* register bus */
    bus->instance = instance;
    return spi_bus_register(bus, &kinetis_spi_ops);
}

