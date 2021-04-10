#ifndef NRF_H
#define	NRF_H

#include "Config.h"
#include "Gpio.h"
#include "Spi.h"
#include "Led.h"

/* Memory Map */
/*
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define RPD         0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D
*/

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define COUNT_WAVE  7
#define RF_DR_LOW   5
#define PLL_LOCK    4
#define RF_DR_HIGH  3
#define RF_PWR      1
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5	    5
#define DPL_P4	    4
#define DPL_P3	    3
#define DPL_P2	    2
#define DPL_P1	    1
#define DPL_P0	    0
#define EN_DPL	    2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0

/* Instruction Mnemonics */
/*
#define REGISTER_MASK 0x1F
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define R_RX_PL_WID   0x60
#define W_ACK_PAYLOAD 0xA8
#define W_TX_PAYLOAD_NO_ACK  0xB0
#define NOP           0xFF
*/

#define NRF_MAX( a, b ) ( a>b? a:b )
#define BRF_MIN( a, b ) ( a<b? a:b )

typedef enum
{
	NRF_ID_1 = 0,
	NRF_ID_MAX,
}	NRF_ID_t;

typedef enum
{
	NRF_STATE_LOW = 0,
	NRF_STATE_HIGH,
}	NRF_STATE_t;

typedef enum
{
	R_REGISTER = 0x00,
	REGISTER_MASK = 0x1F,
	W_REGISTER = 0x20,
	R_RX_PL_WID = 0x60,
	R_RX_PAYLOAD = 0x61,
	W_TX_PAYLOAD = 0xA0,
	W_ACK_PAYLOAD = 0xA8,
	W_TX_PAYLOAD_NO_ACK = 0xB0,
	FLUSH_TX = 0xE1,
	FLUSH_RX = 0xE2,
	REUSE_TX_PL = 0xE3,
	NOP = 0xFF,
}	NRF_COMMAND_t;

typedef enum
{
	CONFIG = 0,
	EN_AA,
	EN_RXADDR,
	SETUP_AW,
	SETUP_RETR,
	RF_CH,
	RF_SETUP,
	STATUS,
	OBSERVE_TX,
	RPD,
	RX_ADDR_P0,
	RX_ADDR_P1,
	RX_ADDR_P2,
	RX_ADDR_P3,
	RX_ADDR_P4,
	RX_ADDR_P5,
	TX_ADDR,
	RX_PW_P0,
	RX_PW_P1,
	RX_PW_P2,
	RX_PW_P3,
	RX_PW_P4,
	RX_PW_P5,
	FIFO_STATUS,
	DYNPD = 0x1C,
	FEATURE = 0x1D,
}	NRF_REG_t;

typedef enum
{
	NRF_CRC_DISABLED = 0,
	NRF_CRC_8BITS,
	NRF_CRC_16BITS,
}	NRF_CRC_t;

typedef enum
{
	NRF_DR_1MBPS = 0,
	NRF_DR_2MBPS,
	NRF_DR_250KBPS,
}	NRF_DR_t;

typedef enum
{
	NRF_PA_MIN = 0,
	NRF_PA_LOW,
	NRF_PA_HIGH,
	NRF_PA_MAX,
	NRF_PA_ERROR,
}	NRF_PA_t;

typedef enum
{
	NRF_NODE_0,
	NRF_NODE_1,
	NRF_NODE_2,
	NRF_NODE_3,
	NRF_NODE_4,
	NRF_NODE_MAX,
}	NRF_NODE_t;

void Nrf_init( NRF_ID_t id, GPIO_ID_t gpioIdCe, GPIO_PIN_t gpioPinCe,
							 GPIO_ID_t gpioIdCsn, GPIO_PIN_t gpioPinCsn, GPIO_ID_t gpioIdCom, GPIO_PIN_t gpioPinCom, SPI_ID_t spiId );
void Nrf_reinit( NRF_ID_t id );
void Nrf_csn( NRF_ID_t id, NRF_STATE_t state );
void Nrf_ce( NRF_ID_t id, NRF_STATE_t state );
void Nrf_writeByte( NRF_ID_t id, NRF_REG_t reg, uint8_t buff );
void Nrf_writePage( NRF_ID_t id, NRF_REG_t reg, uint8_t *buff, uint8_t len );
uint8_t Nrf_readByte( NRF_ID_t id, NRF_REG_t reg );
void Nrf_readPage( NRF_ID_t id, NRF_REG_t reg, uint8_t *buff, uint8_t len );
void Nrf_flushTx( NRF_ID_t id );
void Nrf_flushRx( NRF_ID_t id );
void Nrf_startTxMode( NRF_ID_t id );
void Nrf_startRxMode( NRF_ID_t id );
void Nrf_stopRxMode( NRF_ID_t id );
uint8_t Nrf_checkTxFlag( NRF_ID_t id );
uint8_t Nrf_checkRxFlag( NRF_ID_t id );
void Nrf_transmit( NRF_ID_t id, uint8_t *buffer );
void Nrf_receive( NRF_ID_t id, uint8_t *buffer );
void Nrf_writeAck( NRF_ID_t id, uint8_t *buffer );
void Nrf_setChannel( NRF_ID_t id, uint8_t channel );
void Nrf_openWritingPipe( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe0( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe1( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe2( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe3( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe4( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe5( NRF_ID_t id, uint8_t *address );
void Nrf_openReadingPipe6( NRF_ID_t id, uint8_t *address );

void Nrf_setNodeNumber( NRF_ID_t id, NRF_NODE_t node );
NRF_NODE_t Nrf_getNodeNumber( NRF_ID_t id );
void Nrf_setTxBuffer( NRF_ID_t id, NRF_NODE_t node, uint8_t index, uint8_t buffer );
uint8_t Nrf_getRxBuffer( NRF_ID_t id, NRF_NODE_t node, uint8_t index );
void Nrf_clearTxBuffer( NRF_ID_t id );
void Nrf_clearRxBuffer( NRF_ID_t id );
void Nrf_updateMaster( void *paramter );
void Nrf_updateSlave( void *paramter );

#endif	/* NRF_H */
