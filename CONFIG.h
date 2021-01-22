#ifndef CONFIG_H
#define CONFIG_H

//--------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------------------------------------------------------------------------------

#define CPU_F (72000000)
#define CPU_INSTRUCTION (1)

#define SCH_MAX_TASKS (5)
#define SCH_TICK_PERIOD_MS (1)
#define SYSTICK_MAX (16777216)

#ifndef TRUE
#define FALSE (0)
#define TRUE (!FALSE)
#endif

#define RETURN_ERROR (0)
#define RETURN_NORMAL (!RETURN_ERROR)

#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)

#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK (0xAA)
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (0xAA)

#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START (0xA0)
#define ERROR_SCH_LOST_SLAVE (0x80)

//--------------------------------------------------------------------------------------------------------------------

#define _BV(n) (1<<n)
#define SET_BIT(reg,bit) (reg |= (1<<bit))
#define CLEAR_BIT(reg,bit) (reg &= ~(1<<bit))
#define TOGGLE_BIT(reg,bit) (reg ^= (1<<bit))
#define ROR(reg,bit) (reg = (reg>>bit | reg<<8-bit))
#define ROL(reg,bit) (reg = (reg<<bit | reg>>8-bit))
#define BIT_IS_SET(reg,bit) (reg & (1<<bit))
#define BIT_IS_CLEAR(reg,bit) (!(reg & (1<<bit)))

#define Delay_ms(t) HW_Delay_ms(t)
#define Delay_us(t) SW_Delay_us(t)

//--------------------------------------------------------------------------------------------------------------------

#define		__I			volatile const       /*!< Defines 'read only' permissions */
#define		__O			volatile             /*!< Defines 'write only' permissions */
#define		__IO		volatile             /*!< Defines 'read / write' permissions */

#define		__IM		volatile const       /*! Defines 'read only' structure member permissions */
#define		__OM		volatile          	 /*! Defines 'write only' structure member permissions */
#define		__IOM		volatile             /*! Defines 'read / write' structure member permissions */

//--------------------------------------------------------------------------------------------------------------------

typedef signed char					int8_t;
typedef signed short				int16_t;
typedef signed long					int32_t;
typedef signed long long		int64_t;

typedef unsigned char				uint8_t;
typedef unsigned short			uint16_t;
typedef unsigned long				uint32_t;
typedef unsigned long long	uint64_t;

typedef union
{
    struct
    {
        uint8_t BIT0:1;
        uint8_t BIT1:1;
        uint8_t BIT2:1;
        uint8_t BIT3:1;
        uint8_t BIT4:1;
        uint8_t BIT5:1;
        uint8_t BIT6:1;
        uint8_t BIT7:1;
        uint8_t BIT8:1;
        uint8_t BIT9:1;
        uint8_t BIT10:1;
        uint8_t BIT11:1;
        uint8_t BIT12:1;
        uint8_t BIT13:1;
        uint8_t BIT14:1;
        uint8_t BIT15:1;
        uint8_t BIT16:1;
        uint8_t BIT17:1;
        uint8_t BIT18:1;
        uint8_t BIT19:1;
        uint8_t BIT20:1;
        uint8_t BIT21:1;
        uint8_t BIT22:1;
        uint8_t BIT23:1;
        uint8_t BIT24:1;
        uint8_t BIT25:1;
        uint8_t BIT26:1;
        uint8_t BIT27:1;
        uint8_t BIT28:1;
        uint8_t BIT29:1;
        uint8_t BIT30:1;
        uint8_t BIT31:1;
    }BITS_t;
    uint32_t BYTE:8;
}REG_t;

//--------------------------------------------------------------------------------------------------------------------

#endif /* CONFIG_H */
