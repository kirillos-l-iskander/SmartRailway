#include "Afio.h"

typedef AFIO_TypeDef Afio_t;

static Afio_t *afio[ AFIO_ID_MAX ] = { AFIO };

void Afio_disableJTAG( AFIO_ID_t id )
{
		afio[ id ]->MAPR |= ( 0x4 << 24 );
}
