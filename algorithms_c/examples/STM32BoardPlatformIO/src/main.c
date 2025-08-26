#include "stm32f0xx_hal.h"                 
#include <stdio.h>
#include <string.h>
#include "switchingDetectionFixed.h"    // include the switchingDetectionFixed algorithm header from the main platformio project

static const float TIMESTEP_ms = 0.05;
uint32_t TIMESTEP_us;

void SystemClock_Config(void);              
void MX_GPIO_Init(void);
void MX_ADC_Init(void);
void MX_USART2_UART_Init(void);
void MX_TIM14_Init(void);

ADC_HandleTypeDef hadc;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim14;

int16_t current;
double integral;
char msg[64];
uint32_t adcValue;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC_Init();
    MX_USART2_UART_Init();
    MX_TIM14_Init();
    
    HAL_ADC_Start(&hadc);
    TIMESTEP_us = (uint32_t)(TIMESTEP_ms * 1000);

    while (true) {
        switchingDetectionFixed_Reset();  // reset the algorithm

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);  // valve off
        HAL_Delay(50);  // wait 50 ms
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);  // valve on

        for (int i=0; i<500; i++) {  // measure 500 values
            uint32_t startTime = __HAL_TIM_GET_COUNTER(&htim14);

            if (HAL_ADC_PollForConversion(&hadc, 10) == HAL_OK) {
                adcValue = HAL_ADC_GetValue(&hadc);  // read ADC value
            }
            current = (int16_t)(adcValue / 8);
            switchingDetectionFixed_StoreADC(current);  // store measured current in the algorithm
            
            while ((__HAL_TIM_GET_COUNTER(&htim14) - startTime) < TIMESTEP_us) { /* spin briefly */ }  // Wait until time complete
        }

        integral = switchingDetectionFixed_Calculate(TIMESTEP_ms);  // calculate the integral value
        snprintf(msg, sizeof(msg), "integral_value: %.6f \r\n", integral);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);  // send the integral value via UART
        
        HAL_Delay(50);  // wait 50 ms
    }
}



//*****************************************
// from here on only hardware definitions
//*****************************************

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);  // 1 ms Takt
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // D2 (PA10) als Ausgang
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void MX_ADC_Init(void) {
    __HAL_RCC_ADC1_CLK_ENABLE();

    hadc.Instance = ADC1;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = DISABLE;
    hadc.Init.ContinuousConvMode = ENABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    HAL_ADC_Init(&hadc);

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0; // A0 = PA0
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

void MX_USART2_UART_Init(void) {
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // PA2 = TX, PA3 = RX
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

void MX_TIM14_Init(void) {
    __HAL_RCC_TIM14_CLK_ENABLE();
    htim14.Instance = TIM14;
    htim14.Init.Prescaler = (HAL_RCC_GetHCLKFreq() / 1000000UL) - 1; // 1 MHz
    htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim14.Init.Period = 0xFFFF;
    htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim14);
    HAL_TIM_Base_Start(&htim14);
}

void ADC1_COMP_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&hadc);
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}