#ifndef AFIO_H
#define AFIO_H

#include "Config.h"

typedef enum
{
	AFIO_ID = 0,
	AFIO_ID_MAX,
}	AFIO_ID_t;

void Afio_disableJTAG( AFIO_ID_t id );

#endif /* AFIO_H */
