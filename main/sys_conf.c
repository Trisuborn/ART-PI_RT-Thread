/************************************************
 * @file sys_conf.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-19
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "pro_conf.h"
#include "sys_conf.h"

#define LOG_TAG        "LCD_ST7735"

#if (USER_USE_RTTHREAD == 1)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_log.h"

#else
#define LOG_E(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#define LOG_D(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#endif

static void Error_Handler(void);

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct ={ 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct ={ 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ={ 0 };

    /** Supply configuration update enable
    */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while ( !__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY) ) {
    }
    /** Configure LSE Drive Capability
    */
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
    /** Macro to configure the PLL clock source
    */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE
        | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if ( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK ) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
        | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if ( HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK ) {
        Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_LPUART1
        | RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_SPI1
        | RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SDMMC
        | RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_I2C4
        | RCC_PERIPHCLK_USB | RCC_PERIPHCLK_QSPI
        | RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.PLL2.PLL2M = 5;
    PeriphClkInitStruct.PLL2.PLL2N = 192;
    PeriphClkInitStruct.PLL2.PLL2P = 4;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.PLL3.PLL3M = 5;
    PeriphClkInitStruct.PLL3.PLL3N = 192;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 20;
    PeriphClkInitStruct.PLL3.PLL3R = 4;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
    PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
    PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_D3PCLK1;
    PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if ( HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK ) {
        Error_Handler();
    }
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI48, RCC_MCODIV_4);
    /** Enable USB Voltage detector
    */
    HAL_PWREx_EnableUSBVoltageDetector();
}


void Error_Handler(void)
{

}

int clock_information(void)
{
    LOG_I("System Clock information");
    LOG_I("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_I("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_I("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_I("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());

    return 0;
}
INIT_BOARD_EXPORT(clock_information);


TIM_HandleTypeDef        htim5;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if ( htim->Instance == TIM5 ) {
        HAL_IncTick();
    }
}

void TIM5_IRQHandler(void)
{

    HAL_TIM_IRQHandler(&htim5);

}


HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock = 0;
    uint32_t              uwPrescalerValue = 0;
    uint32_t              pFLatency;
    /*Configure the TIM5 IRQ priority */
    HAL_NVIC_SetPriority(TIM5_IRQn, TickPriority, 0);

    /* Enable the TIM5 global Interrupt */
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
    /* Enable TIM5 clock */
    __HAL_RCC_TIM5_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Compute TIM5 clock */
    uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();

    /* Compute the prescaler value to have TIM5 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000) - 1);

    /* Initialize TIM5 */
    htim5.Instance = TIM5;

    /* Initialize TIMx peripheral as follow:
    + Period = [(TIM5CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim5.Init.Period = (1000000 / 1000) - 1;
    htim5.Init.Prescaler = uwPrescalerValue;
    htim5.Init.ClockDivision = 0;
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    if ( HAL_TIM_Base_Init(&htim5) == HAL_OK ) {
      /* Start the TIM time Base generation in interrupt mode */
        return HAL_TIM_Base_Start_IT(&htim5);
    }

    /* Return function status */
    return HAL_ERROR;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM5 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
  /* Disable TIM5 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim5, TIM_IT_UPDATE);
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM5 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
  /* Enable TIM5 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
}





void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (timHandle->Instance == TIM1) {

        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_TIM1_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PE10     ------> TIM1_CH2N
        */

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

    if (tim_baseHandle->Instance == TIM1) {

      /* TIM1 clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);

    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

    if (tim_baseHandle->Instance == TIM1) {
      /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();

        /* TIM1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
    }
}

