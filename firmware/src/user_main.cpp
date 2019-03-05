#include "user_main.h"

#include <inttypes.h>
#include <stdio.h>

#include "stm32f3xx_hal.h"

#include "global.h"
#include "serial_task.h"
#include "simple_task.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

namespace {}

void setup() {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

  task_list.emplace_back(new SimpleTask("motor", 1000, [] {
    if (!motor_power) {
      // htim1.Instance->CCER |=  TIM_CCxN_DISABLE << TIM_CHANNEL_2;
      htim1.Instance->CCR2 = 0; // pwm_h
      // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    } else {
      // htim1.Instance->CCER |=  TIM_CCxN_ENABLE << TIM_CHANNEL_2;
      htim1.Instance->CCR2 = 35999; // pwm_h
      htim1.Instance->CCR3 = 17999 * motor_power + 18000; // phase
    }
  }));
  task_list.emplace_back(new SimpleTask("blink", 5, [] {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
  }));
  task_list.emplace_back(new SerialTask("serial", 120, huart2));
}

void loop() {
  for (const auto& task : task_list) {
    task->process();
  }
}
