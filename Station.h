#ifndef STATION_TASK_H
#define	STATION_TASK_H

#include "Config.h"
#include "Nrf.h"
#include "Irsnsr.h"
#include "Lcd.h"

typedef enum
{
	STATION_ID_1,
	STATION_ID_MAX,
}	STATION_ID_t;

void Station_init( STATION_ID_t id, NRF_ID_t xNrfId, IRSNSR_ID_t eIrsnsrId, IRSNSR_ID_t pIrsnsrId, IRSNSR_ID_t wIrsnsrId, LCD_ID_t xLcdId );
void Station_startState( STATION_ID_t id );
void Station_getTrainAddress( STATION_ID_t id );
void Station_detectTrainEnter( STATION_ID_t id );
void Station_detectTrainReach( STATION_ID_t id );
void Station_serveTrain( STATION_ID_t id );
void Station_update( void *paramter );

#endif	/* STATION_TASK_H */
