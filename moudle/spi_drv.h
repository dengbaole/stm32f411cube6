
#ifndef _SPI_DRV_H
#define _SPI_DRV_H

#include "platform.h"

extern SPI_HandleTypeDef hspi1;

void spi_init(void);
void spi_send(uint8_t *data, uint16_t size);

#endif /*__ pinoutConfig_H */


