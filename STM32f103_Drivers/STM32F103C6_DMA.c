/*
 * STM32F103C6_DMA.C
 *
 *  Created on: Dec 15, 2023
 *      Author: musta
 */

#include <STM32F103C6_DMA.h>




uint8_t Transfer_complete_interrupt_En_disable_flg =0;



//private functions
void DMA_Wait_transmite_done(uint8_t CHANNELx)
{
	// Wait for DMA transfer completion
	while (!((DMA_ISR>>CHANNEL_1)));
}



void MCAL_DMA_INIT(DMA_Config * dma_Config)
{

	if(dma_Config->DMA_NUMx == 1)
	{
		RCC_DMA1_CLOCK_EN();
		NVIC_IRQ11_DMA_CH1_ENABLE();

		if(dma_Config->CHANNELx== CHANNEL_1)
		{
			if(dma_Config->DMA_MODE == DMA_MODE_MEMORY2MEMOEY_EN)
			{
				DMA_CCR_ch1 |=DMA_MODE_MEMORY2MEMOEY_EN; //mem2mem
			}

			if(dma_Config->DATA_TRANSFER_DIR == DATA_TRANSFER_DIR_MEM2MEM)
			{
				DMA_CCR_ch1 |=DATA_TRANSFER_DIR_MEM2MEM; //read from mem
			}
			if(dma_Config->MEMORY_SIZE == MEMORY_SIZE_32)
			{
				DMA_CCR_ch1 |=MEMORY_SIZE_32;
			}
			if(dma_Config->PERIPHERAL_SIZE == PERIPHERAL_SIZE_32)
			{
				DMA_CCR_ch1 |=PERIPHERAL_SIZE_32;
			}

			if(dma_Config->MEMORY_increment_MODE == MEMORY_increment_Enable)
			{
				DMA_CCR_ch1 |=MEMORY_increment_Enable;
			}
			if(dma_Config->PERIPHERAL_increment_MODE == PERIPHERAL_increment_Enable)
			{
				DMA_CCR_ch1 |=PERIPHERAL_increment_Enable;
			}
			if(dma_Config->Transfer_complete_interrupt_EnORdis == Transfer_complete_interrupt_Enable)
			{
				DMA_CCR_ch1 |=(1<<1);
				Transfer_complete_interrupt_En_disable_flg = 1;
			}

			DMA_CCR_ch1 |=(1<<5);


		}
	}
}



void MCAL_DMA_COPY_MEMORY2MEMORY(uint8_t DMA_NUMx , uint8_t CHANNELx,uint32_t *PtrSource , uint32_t *PtrDestination , uint32_t size)
{
	if(DMA_NUMx == 1)
	{
		if(CHANNELx == CHANNEL_1)
		{
			/*1. Set the peripheral register address in the DMA_CPARx register. The data will be
				 moved from/ to this address to/ from the memory after the peripheral event.*/
			DMA_CPAR_ch1 =(uint32_t)PtrDestination ;

			/*2. Set the memory address in the DMA_CMARx register. The data will be written to or
				read from this memory after the peripheral event.*/
			DMA_CMAR_ch1 =(uint32_t)PtrSource ;

			/*  3. Configure the total number of data to be transferred in the DMA_CNDTRx register.
			    After each peripheral event, this value will be decremented.*/
			DMA_CNDTR_ch1 = size; // Number of data items to transfer
		}
		MCAL_DMA_TRIGGER_CH_EN(CHANNELx);
		if(Transfer_complete_interrupt_En_disable_flg==0)
		{
			DMA_Wait_transmite_done(CHANNELx);
			MCAL_DMA_CH_DIS(CHANNELx);
		}

	}
}


void MCAL_DMA_TRIGGER_CH_EN(uint8_t CHANNELx)
{
	/*Bit 0 EN: Channel enable
	This bit is set and cleared by software.
	0: Channel disabled
	1: Channel enabled*/
	if(CHANNELx == CHANNEL_1)
	{
		DMA_CCR_ch1 |=CHANNEL_1_ENABLE;
	}
}

void MCAL_DMA_CH_DIS(uint8_t CHANNELx)
{
	/*Bit 0 EN: Channel enable
	This bit is set and cleared by software.
	0: Channel disabled
	1: Channel enabled*/
	if(CHANNELx == CHANNEL_1)
	{
		DMA_CCR_ch1 |=CHANNEL_1_DISABLE;
	}
}

