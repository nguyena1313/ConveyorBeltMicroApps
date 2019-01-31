
#include "main.h" //clock speed at 100MHz

/* Prototypes */
/* Prototypes */
void TIM5_Config(void);
void TIM4_Config(void);
void TIM3_Config(void);

static __IO uint32_t TimingDelay;

void getDutyCycle();
//void delay1(int time);
static void Delay(__IO uint32_t nTime); 
void TimingDelay_Decrement(void);

/*Typedefs and structs */
GPIO_InitTypeDef        GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_ICInitTypeDef       TIM_ICInitStructure;


/* Global Variables */
//float frequency = 0;
int duty_cycle;        //value of duty cycle (integers)
uint16_t outSignalPeriod = 1666;//outSignalPeriod of 25us signal
int Infreq = 0;               //input frequency
__IO uint16_t high=830;
unsigned int START = 0x0001;
  unsigned int STOP = 0x0001;
  __IO uint16_t uhCCR2_H;


/* Main Program */
int main(void)
{

  SysTick_Config(168000);
  
  //Initialize the dip switch inputs
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //START (PD0) and STOP (PD1)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //Initialize the dip switch inputs
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // STOP (PC0)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //initialize the LED outputs
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_5; // LED Indicator (PD3) and LED IR Indicator (PD4)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //initialize the LED outputs
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;// LED Indicator (PD3) and LED IR Indicator (PD4)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  

    /* TIM Configuration */
  TIM5_Config();
  TIM4_Config();
    TIM3_Config();

    /* Time3 base configuration */
TIM_TimeBaseStructure.TIM_Period = 65535;
TIM_TimeBaseStructure.TIM_Prescaler = 0; 
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

/* Output Compare Toggle Mode configuration:Timer3 Channel3 */
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle; //output compare function used for TIM3
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_Pulse = uhCCR2_H;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OC3Init(TIM3, &TIM_OCInitStructure);
TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

/* TIM3 enable counter */
//TIM_Cmd(TIM3, ENABLE);

/* TIM3 IT enable */
TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);

int first = 1; 
int second =1;
  while (1){
    START = (GPIOD->IDR & 0x0001); 
    STOP = ((GPIOD->IDR)>>1 & 0x0001);
    GPIOD->BSRRH = GPIO_Pin_4; 
    GPIOD->BSRRH = GPIO_Pin_5; 
    GPIOD->BSRRH = GPIO_Pin_7; 
    getDutyCycle(); 
    
    
 if(!START){       
   if(first){
     first =0;
      uhCCR2_H=7636;
              
      for(int i=0; i<6; i++){
             TIM_Cmd(TIM3, ENABLE);
             GPIOD->BSRRL = GPIO_Pin_5;  
             Delay(500);
             GPIOD->BSRRH = GPIO_Pin_5;
              Delay(500);
             }
              TIM_Cmd(TIM3, DISABLE);
             Delay(500); 
            }
 }
    //motor on
    GPIOD->BSRRL = GPIO_Pin_4; 
    TIM_Cmd(TIM4, ENABLE);                 
 
    if(!STOP){
      //if(second){
        second =0;
       GPIOD->BSRRL = GPIO_Pin_7;
       
      //}
      
      
      
      
    }
    
  
 
 
    //end position uhCCR_H = 12000;
    //blink 10 times 

    
          }

    }
    
  





//returns proper duty cycle depending on the input frequency
void getDutyCycle(){
  
  if(Infreq < 4700){
    duty_cycle = 90;
  }
  if(4900 <= Infreq && Infreq <= 5000){
    duty_cycle =  80;
  }
  if(5100 <= Infreq && Infreq <= 5200){
    duty_cycle =  70;
  }
  if(5300 <= Infreq && Infreq <= 5400){
    duty_cycle =  60;
  }
  if(5500 <= Infreq && Infreq <= 5600){
    duty_cycle =  50;
  }
  if(5700 <= Infreq && Infreq <= 5800){
    duty_cycle =  40;
  }
  if(5900 <= Infreq && Infreq <= 6000){
    duty_cycle =  30;
  } 
  if(6100 <= Infreq && Infreq <= 6200){
    duty_cycle =  20;
  }
  
  if(6300 <= Infreq){
    duty_cycle =  10;
  }
  
}


void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
  { 
    TimingDelay--;
  }
}




static void TIM3_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
/* TIM3 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
/* TIM4 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
/* GPIOB clock enable */
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
/* GPIOB Configuration: TIM3 CH3 (PB0) and TIM4 CH3 (PB8) */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8; //setting PB0 and PB1 as AF for TIM3 and TIM4 respectivly.
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //set then as alternating function
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
GPIO_Init(GPIOB, &GPIO_InitStructure);
/* Connect TIM3 Channels to AF2 */
GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); //connect TIM3 to PB0

/* Enable the TIM3 global Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

}



//motor driver through h bridge

static void TIM4_Config(void)
{
  
  NVIC_InitTypeDef        NVIC_InitStructure;
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* GPIOB Configuration:  TIM4 CH2 PB7 is output for TIM4 Channel2, CH3 PB10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Connect TIM4 pins to AF1 */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = outSignalPeriod;			//outSignalPeriod of signal is set as calculated above
  TIM_TimeBaseStructure.TIM_Prescaler = 0;			//prescaler is not used
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);		
  
  /* PWM1 Mode configuration: Channe2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//TIM4 Channel2 is used in PWM mode
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = outSignalPeriod/2;			//Pulse is set to the outSignalPeriod over 2 (high for half time and low for half time)
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  
  
  
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
}

//input for function generator
static void TIM5_Config(void)
{
  
  NVIC_InitTypeDef        NVIC_InitStructure;
  
  /* TIM5 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  /* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* TIM5 channel 2 pin (PA3) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Connect TIM5 pins to AF1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
  
  /* Enable the TIM5 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  /* TIM5 configuration: Input Capture mode ---------------------*/
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  
    /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);
  
  
  /* TIM5 enable counter */
  TIM_Cmd(TIM5, ENABLE);
  

}












