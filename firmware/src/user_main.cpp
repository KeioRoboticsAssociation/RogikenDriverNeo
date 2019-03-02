#include "user_main.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#include "stm32f3xx_hal.h"

#include "simple_task.h"
#include "task.h"

extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

namespace {
  constexpr size_t UART_RX_DMA_CIRC_BUF_SIZE = 64;
  uint8_t uart_rx_dma_circ_buf[UART_RX_DMA_CIRC_BUF_SIZE];

  Task&& blink = SimpleTask([] {
    printf("%" PRIu32 "\n", DWT->CYCCNT / 72000000);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3); 
  }, 5);
}

void setup() {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_UART_Receive_DMA(&huart2, uart_rx_dma_circ_buf, UART_RX_DMA_CIRC_BUF_SIZE);
}

void loop() {
  blink.process();
}
