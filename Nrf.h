#ifndef NRF_H
#define	NRF_H

#include "SchedulerConfig.h"
#include "Gpio.h"
#include "Spi.h"

/* Memory Map */
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

#define rf24_max( a, b ) ( a>b? a:b )
#define rf24_min( a, b ) ( a<b? a:b )

typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e;
typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;
typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;

typedef enum
{
	NRF1_ID
}NRF_t;

void Nrf_init( Id_t id, Id_t ceGpioId, uint8_t cePin, Id_t csnGpioId, uint8_t csnPin, Id_t comGpioId, uint8_t comPin, Id_t comSpiId );
void Nrf_reinit( Id_t id );
void Nrf_csn( Id_t id, uint8_t state );
void Nrf_ce( Id_t id, uint8_t state );
void Nrf_writeByte( Id_t id, uint8_t reg, uint8_t buff );
void Nrf_writePage( Id_t id, uint8_t reg, uint8_t *buff, uint8_t len );
uint8_t Nrf_readByte( Id_t id, uint8_t reg );
void Nrf_readPage( Id_t id, uint8_t reg, uint8_t *buff, uint8_t len );
void Nrf_flushTx( Id_t id );
void Nrf_flushRx( Id_t id );
void Nrf_startTxMode( Id_t id );
void Nrf_startRxMode( Id_t id );
void Nrf_stopRxMode( Id_t id );
uint8_t Nrf_checkTxFlag( Id_t id );
uint8_t Nrf_checkRxFlag( Id_t id );
void Nrf_transmit( Id_t id, uint8_t *buffer );
void Nrf_receive( Id_t id, uint8_t *buffer );
void Nrf_ack( Id_t id, uint8_t *buffer );
void Nrf_setChannel( Id_t id, uint8_t channel );
void Nrf_openWritingPipe( Id_t id, uint8_t *address );
void Nrf_openReadingPipe1( Id_t id, uint8_t *address );
void Nrf_openReadingPipe2( Id_t id, uint8_t *address );
void Nrf_openReadingPipe3( Id_t id, uint8_t *address );
void Nrf_openReadingPipe4( Id_t id, uint8_t *address );
void Nrf_openReadingPipe5( Id_t id, uint8_t *address );
void Nrf_openReadingPipe6( Id_t id, uint8_t *address );

#endif	/* NRF_H */
