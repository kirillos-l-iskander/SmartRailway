#include "Afio.h"

typedef AFIO_TypeDef Afio_t;

static Afio_t *afio[ 1 ] = { AFIO };

void Afio_disableJTAG( Id_t id )
{
		afio[ id ]->MAPR |= ( 0x4 << 24 );
}
