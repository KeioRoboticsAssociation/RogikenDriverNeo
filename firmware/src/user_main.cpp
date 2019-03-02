#include "user_main.h"

#include <inttypes.h>
#include <stdio.h>

#include "stm32f3xx_hal.h"

#include "global.h"
#include "serial_task.h"
#include "simple_task.h"

extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

namespace {}

void setup() {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  task_list.emplace_back(new SimpleTask([] {
    printf("%" PRIu32 "\n", DWT->CYCCNT / 72000000);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3); 
  }, 5));
  task_list.emplace_back(new SerialTask(huart2, 60));
}

void loop() {
  for (auto const& task : task_list) {
    task->process();
  }
}
