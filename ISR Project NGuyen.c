#include "stm32f4xx_it.h"

/* global/ external variables */
uint32_t uhIC3ReadValue1 = 0;
uint32_t uhIC3ReadValue2 = 0;
uint8_t uhCaptureNumber = 0;
uint32_t InSignalPeriod = 0;
extern int Infreq;
extern int duty_cycle;                       //value of duty cycle (integers)                                 
extern uint16_t outSignalPeriod;
extern int buzzerPeriod;
extern __IO uint16_t high;

//buzz
extern __IO uint16_t uhCCR2_H;
uint16_t uhCapture = 0;

extern void TimingDelay_Decrement(void);



void TIM3_IRQHandler(void)
{
/* TIM3_CH3 toggling */
if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
{
TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
uhCapture = TIM_GetCapture3(TIM3);

TIM_SetCompare3(TIM3, uhCapture + uhCCR2_H);



}
}


// PWM signal to drive the motor 
void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    high = outSignalPeriod*duty_cycle/100;
    TIM4->CCR2 = high;                   //sets the CCR2 value according to the duty cycle percentage multiplied by clock
  }
 
}
  


/* Timer 5 interrupt handler
 *
 * captures the time on each rising edge to caputure the input frequency
*/
void TIM5_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET) 
  {
    /* Clear TIM5 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
   
    if(uhCaptureNumber == 0)
    {
      /* Get the Input Capture value */
      uhIC3ReadValue1 = TIM_GetCapture4(TIM5);
      uhCaptureNumber =  0x1;
    }

    else
    {
      /* Get the Input Capture value */
      uhIC3ReadValue2 = TIM_GetCapture4(TIM5); 
      
      /* Capture computation */
      if (uhIC3ReadValue2 > uhIC3ReadValue1)
      {

        InSignalPeriod = (uhIC3ReadValue2 - uhIC3ReadValue1);

        Infreq = 84000000/InSignalPeriod;
     
      }
      else
      {
        InSignalPeriod = ((0xFFFFFFFF - uhIC3ReadValue1) + uhIC3ReadValue2);

        Infreq = 84000000/InSignalPeriod;
      }

      uhCaptureNumber = 0x0;
    }
  }
}





void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

