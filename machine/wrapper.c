#include <msp430.h>

#ifdef __cplusplus
extern "C"
#endif /* __cplusplus */
void guardian (unsigned int slot);


#define WRAPPER(name, slot) __attribute__((interrupt(name))) __attribute((wakeup)) \
void wrapper_##name(void)	\
{			\
  guardian(slot);	\
}


WRAPPER(AES_VECTOR, 		 0)
WRAPPER(RTC_VECTOR, 		 1)
WRAPPER(LCD_B_VECTOR, 		 2)
WRAPPER(PORT2_VECTOR,		 3)
WRAPPER(PORT1_VECTOR,		 4)
WRAPPER(TIMER1_A1_VECTOR,	 5)
WRAPPER(TIMER1_A0_VECTOR,	 6)
WRAPPER(DMA_VECTOR,		     7)
WRAPPER(CC1101_VECTOR,		 8)
WRAPPER(TIMER0_A1_VECTOR,	 9)
WRAPPER(TIMER0_A0_VECTOR,	10)
WRAPPER(ADC12_VECTOR,		11)
WRAPPER(USCI_B0_VECTOR,		12)
WRAPPER(USCI_A0_VECTOR,		13)
WRAPPER(WDT_VECTOR,		    14)
WRAPPER(COMP_B_VECTOR,		15)
WRAPPER(UNMI_VECTOR,		16)
WRAPPER(SYSNMI_VECTOR,		17)
