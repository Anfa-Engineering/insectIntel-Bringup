#include "main.h"
#include  <stdio.h>

#ifdef PSRAM_TEST
#define BUFFERSIZE                              10240
#define KByte                                   1024

uint8_t aTxBuffer[BUFFERSIZE];
__IO uint8_t *mem_addr;

void psram_test(void){
	uint16_t errorBuffer = 0;
	uint32_t index, index_K;

	/*fill aTxBuffer */
	for (index_K = 0; index_K < 10; index_K++)
	{
	 for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
	 {
	   aTxBuffer[index]=index + index_K;
	 }
	}

	printf("\r\n /* Writing Sequence ----------------------------------------------- */\r\n");

	/* Writing Sequence ----------------------------------------------- */
	index_K=0;
	for (index_K = 0; index_K < 10; index_K++)
	{
	printf("\r\n \r\n");

//	 mem_addr = (uint8_t *)(XSPI1_BASE + (index_K * KByte));
	 mem_addr = (uint8_t *)(0x34200000+ (index_K * KByte));
	 for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
	 {
	   *mem_addr = aTxBuffer[index];

	   printf("%u, ",aTxBuffer[index]);

	   mem_addr++;
	 }

	 /* In memory-mapped mode, not possible to check if the memory is ready
	 after the programming. So a delay corresponding to max page programming
	 time is added */
	 HAL_Delay(1);
	}

	printf("\r\n\r\n\r\n/* Reading Sequence ----------------------------------------------- */\r\n");
	/* Reading Sequence ----------------------------------------------- */
	index_K=0;
	for (index_K = 0; index_K < 10; index_K++)
	{
	 printf("\r\n \r\n");

//	 mem_addr = (uint8_t *)(XSPI1_BASE + (index_K * KByte));
	 mem_addr = (uint8_t *)(0x34200000 + (index_K * KByte));

	 for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
	 {
	   printf("%u, ",*mem_addr);

	   if (*mem_addr != aTxBuffer[index])
	   {
		printf("\r\nError\r\n");
		Error_Handler();
		 errorBuffer++;
	   }

	   mem_addr++;
	 }

	  /* In memory-mapped mode, not possible to check if the memory is ready
	 after the programming. So a delay corresponding to max page programming
	 time is added */
	 HAL_Delay(1);
	}
	if (errorBuffer == 0)
	{
	 /* Turn GREEN on */
	}
	printf("\r\n\r\n\r\nSuccess\r\n");

}

#endif
