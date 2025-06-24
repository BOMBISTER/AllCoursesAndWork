#include "stm32f0xx.h"
#include <math.h>  

#define LED_PWM_MAX 1000    
#define SIN_RESOLUTION 100  
#define PI 3.14159265359f   

volatile uint16_t sin_index1 = 0;  
volatile uint16_t sin_index2 = 0;  

void pwm_tim3_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;       
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;      

    
    GPIOC->MODER |= GPIO_MODER_MODER6_1;     
    GPIOC->AFR[0] |= (0 << (4 * (6)));   

    
    GPIOC->MODER |= GPIO_MODER_MODER9_1;     
    GPIOC->AFR[1] |= (0 << (4 * (9 - 8)));   

    
    TIM3->PSC = 1;                           
    TIM3->ARR = LED_PWM_MAX;                 

    
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; 
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;          
    TIM3->CCER |= TIM_CCER_CC1E;             

    
    TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1; 
    TIM3->CCMR2 |= TIM_CCMR2_OC4PE;          
    TIM3->CCER |= TIM_CCER_CC4E;            
    
    TIM3->DIER |= TIM_DIER_UIE;

    
    TIM3->CR1 |= TIM_CR1_CEN;
		

    NVIC_EnableIRQ(TIM3_IRQn);               
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {             
        TIM3->SR &= ~TIM_SR_UIF;             

        
        float sin_value1 = sinf(3* (2 * PI * sin_index1) / SIN_RESOLUTION);  
        uint16_t duty1 = (uint16_t)((sin_value1 + 1.0f) / 2.0f * LED_PWM_MAX);  
        TIM3->CCR1 = duty1;                

       
        float sin_value2 = sinf(1 * (2 * PI * sin_index2) / SIN_RESOLUTION + PI / 2);  
        uint16_t duty2 = (uint16_t)((sin_value2 + 1.0f) / 2.0f * LED_PWM_MAX);  
        TIM3->CCR4 = duty2;                 

        
        sin_index1++;                       
        if (sin_index1 >= SIN_RESOLUTION) {
            sin_index1 = 0;
        }

        sin_index2 += 2;                    
        if (sin_index2 >= SIN_RESOLUTION) {
            sin_index2 = 0;
        }
    }
}

int main(void) {
    pwm_tim3_init();  

    while (1) {
        
    }
}
