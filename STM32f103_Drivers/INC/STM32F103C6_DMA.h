/*
 * STM32F103C6_DMA.H
 *
 *  Created on: Dec 15, 2023
 *      Author: Mostafa Elshiekh
 */

#ifndef INC_STM32F103C6_DMA_H_
#define INC_STM32F103C6_DMA_H_



/*==========================================================*/
//includes
/*==========================================================*/
#include "STM32_F103C6.h"
#include "STM32F103C6_GPIO_DRIVERS.h"
#include "STM32F103C6_RCC_DRIVERS.h"



/*
 * Channel configuration procedure
The following sequence should be followed to configure a DMA channelx (where x is the
channel number).
	1. Set the peripheral register address in the DMA_CPARx register. The data will be
	moved from/ to this address to/ from the memory after the peripheral event.

	2. Set the memory address in the DMA_CMARx register. The data will be written to or
	read from this memory after the peripheral event.

	3. Configure the total number of data to be transferred in the DMA_CNDTRx register.
	After each peripheral event, this value will be decremented.

	4. Configure the channel priority using the PL[1:0] bits in the DMA_CCRx register

	5. Configure data transfer direction, circular mode, peripheral & memory incremented
	mode, peripheral & memory data size, and interrupt after half and/or full transfer in the
	DMA_CCRx register

	6. Activate the channel by setting the ENABLE bit in the DMA_CCRx register.
	As soon as the channel is enabled, it can serve any DMA request from the peripheral
	connected on the channel.

Once half of the bytes are transferred, the half-transfer flag (HTIF) is set and an interrupt is
generated if the Half-Transfer Interrupt Enable bit (HTIE) is set. At the end of the transfer,
the Transfer Complete Flag (TCIF) is set and an interrupt is generated if the Transfer
Complete Interrupt Enable bit (TCIE) is set.
 */



#define dma_baseAddress  0x40020000

/*========================== global registers =========================*/
#define DMA_ISR         *( volatile uint32_t *)(dma_baseAddress+0x00)
#define DMA_IFCR        *( volatile uint32_t *)(dma_baseAddress+0x04)
/*=====================================================================*/

#define DMA_CCR_ch1     *( volatile uint32_t *)(dma_baseAddress +0x8)
#define DMA_CCR_ch2     *( volatile uint32_t *)(dma_baseAddress +0x1c)
#define DMA_CCR_ch3     *( volatile uint32_t *)(dma_baseAddress +0x30)
#define DMA_CCR_ch4     *( volatile uint32_t *)(dma_baseAddress +0x44)
#define DMA_CCR_ch5     *( volatile uint32_t *)(dma_baseAddress +0x58)
#define DMA_CCR_ch6     *( volatile uint32_t *)(dma_baseAddress +0x6c)
#define DMA_CCR_ch7     *( volatile uint32_t *)(dma_baseAddress +0x80)

/*=====================================================================*/

#define DMA_CNDTR_ch1   *( volatile uint32_t *)(dma_baseAddress +0xc)
#define DMA_CNDTR_ch2   *( volatile uint32_t *)(dma_baseAddress +0x20)
#define DMA_CNDTR_ch3   *( volatile uint32_t *)(dma_baseAddress +0x34)
#define DMA_CNDTR_ch4   *( volatile uint32_t *)(dma_baseAddress +0x48)
#define DMA_CNDTR_ch5   *( volatile uint32_t *)(dma_baseAddress +0x5c)
#define DMA_CNDTR_ch6   *( volatile uint32_t *)(dma_baseAddress +0x70)
#define DMA_CNDTR_ch7   *( volatile uint32_t *)(dma_baseAddress +0x84)

/*=====================================================================*/

#define DMA_CPAR_ch1   *( volatile uint32_t *)(dma_baseAddress +0x10)
#define DMA_CPAR_ch2   *( volatile uint32_t *)(dma_baseAddress +0x24)
#define DMA_CPAR_ch3   *( volatile uint32_t *)(dma_baseAddress +0x38)
#define DMA_CPAR_ch4   *( volatile uint32_t *)(dma_baseAddress +0x4c)
#define DMA_CPAR_ch5   *( volatile uint32_t *)(dma_baseAddress +0x60)
#define DMA_CPAR_ch6   *( volatile uint32_t *)(dma_baseAddress +0x74)
#define DMA_CPAR_ch7   *( volatile uint32_t *)(dma_baseAddress +0x88)

/*=====================================================================*/

#define DMA_CMAR_ch1   *( volatile uint32_t *)(dma_baseAddress +0x14)
#define DMA_CMAR_ch2   *( volatile uint32_t *)(dma_baseAddress +0x28)
#define DMA_CMAR_ch3   *( volatile uint32_t *)(dma_baseAddress +0x3c)
#define DMA_CMAR_ch4   *( volatile uint32_t *)(dma_baseAddress +0x50)
#define DMA_CMAR_ch5   *( volatile uint32_t *)(dma_baseAddress +0x64)
#define DMA_CMAR_ch6   *( volatile uint32_t *)(dma_baseAddress +0x78)
#define DMA_CMAR_ch7   *( volatile uint32_t *)(dma_baseAddress +0x8c)

/*=====================================================================*/


//configuration structure
typedef struct
{
	uint32_t          DMA_NUMx;

	uint32_t          DMA_MODE ;   //Specifies TX/RX Enable/Disable
	//This parameter must be set based on @Ref DMA_MODE_define

	uint32_t          MEMORY_increment_MODE; //Specifies communication MEMORY_increment_MODE
	//This parameter must be set based on @Ref MEMORY_increment

	uint32_t          PERIPHERAL_increment_MODE; //Specifies communication Pri_increment_MODE
	//This parameter must be set based on @Ref Pri_increment

	uint32_t          MEMORY_SIZE;     //Specifies communication MEMORY_SIZE
	//This parameter must be set based on @Ref MEMORY_SIZE

	uint32_t          PERIPHERAL_SIZE;     //Specifies communication PERIPHERAL_SIZE
	//This parameter must be set based on @Ref PERIPHERAL_SIZE


	uint32_t          DATA_TRANSFER_DIR;     //Specifies communication DATA_TRANSFER_DIR
	//This parameter must be set based on @Ref DATA_TRANSFER_DIR

	uint8_t           CHANNELx ;//Specifies communication CHANNELx
	//This parameter must be set based on //@Ref Channel number

	uint8_t           Transfer_complete_interrupt_EnORdis;	//Specifies communication Transfer_complete_interrupt_EnORdis
	//This parameter must be set based on //@Ref Transfer_complete_interrupt_EnORdis

	//void (* P_IRQ_Call_Back)(void);  //Set C Function() Which You Need Call it When IRQ is happened
}DMA_Config;







//===========================
//Macros Configuration References
//===========================

/*@Ref DMA_MODE_define */

/*
 * Bit 14 MEM2MEM: Memory to memory mode
This bit is set and cleared by software.
0: Memory to memory mode disabled
1: Memory to memory mode enabled
 */
#define DMA_MODE_MEMORY2MEMOEY_EN         (uint32_t)(1<<14)   //Bit 14 MEM2MEM: Memory to memory mode
#define DMA_MODE_MEMORY2MEMOEY_DIS        (uint32_t)(0<<14)   //Bit 14 MEM2MEM: Memory to memory mode

/*Bit 7 MINC: Memory increment mode
This bit is set and cleared by software.
0: Memory increment mode disabled
1: Memory increment mode enabled*/
#define MEMORY_increment_Enable       (uint32_t)(1<<7)    //Bit 7 MINC: Memory increment mode

#define PERIPHERAL_increment_Enable   (uint32_t)(1<<6)    //PINC: Peripheral increment mode
#define PERIPHERAL_increment_Disable   (uint32_t)(0<<6)    //PINC: Peripheral increment mode

//@Ref Channel number
#define CHANNEL_1     1
#define CHANNEL_2     2
#define CHANNEL_3     3
#define CHANNEL_4     4
#define CHANNEL_5     5
#define CHANNEL_6     6
#define CHANNEL_7     7

//@Ref Transfer_complete_interrupt_EnORdis
#define Transfer_complete_interrupt_disable    0
#define Transfer_complete_interrupt_Enable     1




//Memory size
#define MEMORY_SIZE_32                (uint32_t)(0b10<<10)//Bits 11:10 MSIZE[1:0]: Memory size

//Peripheral size
#define PERIPHERAL_SIZE_32             (uint32_t)(0b10<<8)//Bits 9:8 PSIZE[1:0]: Peripheral size

//Data transfer direction
/*Bit 4 DIR: Data transfer direction
This bit is set and cleared by software.
0: Read from peripheral
1: Read from memory*/
#define DATA_TRANSFER_DIR_MEM2MEM      (uint32_t)(1<<4)//Bit 4 DIR: Data transfer direction
#define DATA_TRANSFER_DIR_PRI2MEM      (uint32_t)(0<<4)//Bit 4 DIR: Data transfer direction


#define DMA_NUM_1                      (uint8_t)(1<<0)

#define CHANNEL_1_ENABLE               (uint32_t)(1<<0);
#define CHANNEL_1_DISABLE              (uint32_t)(0<<0)

/*
 * ========================================================================================
 *                         APIs Supported by "MCAL DMA  DRIVER"
 * ========================================================================================
 *
 */


void MCAL_DMA_INIT(DMA_Config * dma_Config);
void MCAL_DMA_COPY_MEMORY2MEMORY(uint8_t DMA_NUMx,uint8_t CHANNELx,uint32_t *PtrSource,uint32_t *PtrDestination,uint32_t size);
void MCAL_DMA_TRIGGER_CH_EN(uint8_t CHANNELx);
void MCAL_DMA_CH_DIS(uint8_t CHANNELx);


#endif /* INC_STM32F103C6_DMA_H_ */









