/******************************************************************************
 * @file           : main.c
 * @author         : Eng : Mostafa Elshiekh -- Learn In Depth
 * @brief          : DMA
 ******************************************************************************/
#include "STM32_F103C6.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "STM32F103C6_DMA.h"
#include "RSA_Algorithm.h"

#define ARRAY_SIZE 5


/*Global Variables*/
// Source and destination arrays
uint32_t OriginalData[5];
uint32_t EcrypteddData[5];



/*=== RSA Algorithm global Variables ==*/
double euler= 0;
double n=0;
double e=0;
double d = 0;
int Encryption_Message=0;
double Decryption_Message=0;
/*=====================================*/


/*================TASKS HANDELS=================*/
TaskHandle_t xDMA_COPY_MEMORY2MEMORY_Handle = NULL;
TaskHandle_t xEncryptionTask2_Handle = NULL;
TaskHandle_t xDeEncryptionTask3_Handle = NULL;


/*================Semaphores HANDELS===============*/
SemaphoreHandle_t xSmphor_EncryptionIsDone = NULL;
SemaphoreHandle_t xSmphor_Start_Encryption = NULL;
/*==================================================*/


/*============Tasks Prototype===========*/
void DMA_COPY_MEMORY2MEMORY(void* params);
void EncryptionTask2(void* params);


QueueHandle_t xQueueHandel = NULL;



void DMA1_Channel1_IRQHandler(void)
{
	if((DMA_ISR &(1<<1))>>1)
	{
		DMA_IFCR |=(1<<0);//Clears the corresponding TCIF flag in the DMA_ISR register
		DMA_IFCR |=(1<<1);//Clears the corresponding TCIF flag in the DMA_ISR register
		DMA_CCR_ch1 &=~(1<<0);
		MCAL_DMA_CH_DIS(CHANNEL_1);
	}
}


void RSA_Calc_Init(void)
{
	euler=RSA_Calc_Euler(p,q);
	n=RSA_Calc_N(p,q);
	e=RSA_Generate_e(euler);
	d =RSA_Decryption_Calc_d(euler,e);
}

int main(void)
{
	DMA_Config dma_cfg;
	dma_cfg.DMA_NUMx = DMA_NUM_1;
	dma_cfg.CHANNELx = CHANNEL_1;
	dma_cfg.DMA_MODE = DMA_MODE_MEMORY2MEMOEY_EN;
	dma_cfg.DATA_TRANSFER_DIR = DATA_TRANSFER_DIR_MEM2MEM;
	dma_cfg.MEMORY_increment_MODE = MEMORY_increment_Enable;
	dma_cfg.PERIPHERAL_increment_MODE = PERIPHERAL_increment_Enable;
	dma_cfg.MEMORY_SIZE = MEMORY_SIZE_32;
	dma_cfg.PERIPHERAL_SIZE = PERIPHERAL_SIZE_32;
	dma_cfg.Transfer_complete_interrupt_EnORdis = Transfer_complete_interrupt_Enable;

	RSA_Calc_Init();
	MCAL_DMA_INIT(&dma_cfg);


	// Initialize source array with some data
	for (uint32_t i = 0; i < ARRAY_SIZE; i++)
	{
		OriginalData[i] = i;
	}


	//Semaphores Creation

	xSmphor_EncryptionIsDone = xSemaphoreCreateBinary();
	xSmphor_Start_Encryption = xSemaphoreCreateBinary();

	/*===============Tasks Creation===============*/
	xTaskCreate(DMA_COPY_MEMORY2MEMORY, "DMA_COPY_MEMORY2MEMORY", 128, NULL, 3, xDMA_COPY_MEMORY2MEMORY_Handle);
	xTaskCreate(EncryptionTask2, "EncryptionTask2", 128, NULL, 2, xEncryptionTask2_Handle);



	//Scheduler Start
	vTaskStartScheduler();

	while(1)
	{
	}

	return 0;
}



/*==============System Tasks==========*/
void DMA_COPY_MEMORY2MEMORY(void* params)
{
	xSemaphoreGive(xSmphor_EncryptionIsDone);
	while(1)
	{
		if(xSemaphoreTake(xSmphor_EncryptionIsDone , (TickType_t)5)==pdTRUE)
		{
			MCAL_DMA_COPY_MEMORY2MEMORY(DMA_NUM_1,CHANNEL_1,OriginalData, EcrypteddData, ARRAY_SIZE);
			xSemaphoreGive(xSmphor_Start_Encryption);
		}
	}
}


void EncryptionTask2(void* params)
{

	while(1)
	{
		if(xSemaphoreTake(xSmphor_Start_Encryption , (TickType_t)5)==pdTRUE)
		{
			for (int i = 0; i < ARRAY_SIZE; i++)
			{
				EcrypteddData[i] = RSA_Encryption(EcrypteddData[i], n, e);
			}
			//Give the semaphore when end the Encryption
			xSemaphoreGive(xSmphor_EncryptionIsDone);

		}
	}
}

