#include "user_main.h"

#include <inttypes.h>
#include <stdio.h>

#include "stm32f3xx_hal.h"

#include "enc_task.h"
#include "global.h"
#include "pid_task.h"
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

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

  task_list.emplace_back("encoder", new EncTask(1000, htim3));
  task_list.emplace_back("pid_vel", new PidTask(0)); // kp = ki = 0.0000001 (* 100)
  task_list.emplace_back("motor", new SimpleTask(1000, [] {
    if (!state_mgr.state.motor_power) {
      // htim1.Instance->CCER |=  TIM_CCxN_DISABLE << TIM_CHANNEL_2;
      htim1.Instance->CCR2 = 0; // pwm_h
      // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    } else {
      // htim1.Instance->CCER |=  TIM_CCxN_ENABLE << TIM_CHANNEL_2;
      htim1.Instance->CCR2 = 2399; // pwm_h
      htim1.Instance->CCR3 = 1199 * state_mgr.state.motor_power * 0.01 + 1200; // phase
    }
  }));
  task_list.emplace_back("blink", new SimpleTask(5, [] {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    printf(
      "%d, %d, %d\n",
      static_cast<int>(state_mgr.state.motor_power),
      static_cast<int>(state_mgr.state.target_vel),
      static_cast<int>(state_mgr.state.enc_vel)
    );
  }));
  task_list.emplace_back("serial", new SerialTask(120, huart2));
}

void loop() {
  for (const auto& task : task_list) {
    task->process();
  }
}
