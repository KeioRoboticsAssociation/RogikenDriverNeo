#include "user_main.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#include "stm32f303x8.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_gpio.h"

namespace {
  constexpr size_t UART_RX_DMA_CIRC_BUF_SIZE = 64;
  uint8_t uart_rx_dma_circ_buf[UART_RX_DMA_CIRC_BUF_SIZE];
}

extern UART_HandleTypeDef huart2;

void setup() {
  HAL_UART_Receive_DMA(&huart2, uart_rx_dma_circ_buf, UART_RX_DMA_CIRC_BUF_SIZE);
}

void loop() {
  printf("%s %" PRIu32 "\n", uart_rx_dma_circ_buf, huart2.hdmarx->Instance->CNDTR);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
  HAL_Delay(100);
}
