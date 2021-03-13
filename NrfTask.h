#ifndef SERVER_H
#define	SERVER_H

#include "SchedulerConfig.h"
#include "Nrf.h"
#include "LedTask.h"

void NrfTaskMaster_init( Id_t id, Id_t ceGpioId, uint8_t cePin, Id_t csnGpioId, uint8_t csnPin, Id_t comGpioId, uint8_t comPin, Id_t comSpiId );
void NrfTaskMaster_setNodeNumber( Id_t id, uint8_t nodeNumber );
void NrfTaskMaster_clearTxBuffer( Id_t id );
void NrfTaskMaster_clearRxBuffer( Id_t id );
void NrfTaskMaster_update( void *paramter );

void NrfTaskSlave_init( Id_t id, Id_t ceGpioId, uint8_t cePin, Id_t csnGpioId, uint8_t csnPin, Id_t comGpioId, uint8_t comPin, Id_t comSpiId );
void NrfTaskSlave_setNodeNumber( Id_t id, uint8_t nodeNumber );
void NrfTaskSlave_setBuffer( Id_t id, uint8_t index, uint8_t buffer );
uint8_t NrfTaskSlave_getBuffer( Id_t id, uint8_t Index );
void NrfTaskSlave_clearTxBuffer( Id_t id );
void NrfTaskSlave_clearRxBuffer( Id_t id );
void NrfTaskSlave_update( void *paramter );

#endif	/* SERVER_H */
