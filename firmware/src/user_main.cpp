#include "user_main.h"

#include <cmath>
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

  task_list.emplace_back("debug", new SimpleTask(5, [] {
    // static int cnt = 0;
    // if (!(cnt++ % 5)) {
    //   state_mgr.state.target_vel *= -1;
    // }
    if (state_mgr.state.debug) {
      printf(
        "%d, %d, %d\n",
        static_cast<int>(state_mgr.state.motor_power),
        static_cast<int>(state_mgr.state.target_vel),
        static_cast<int>(state_mgr.state.enc_vel)
      );
    }
  }));
  task_list.emplace_back("encoder", new EncTask(1000, htim3));
  task_list.emplace_back("pid_vel", new PidTask(0));
  task_list.emplace_back("motor", new SimpleTask(1000, [] {
    if (!state_mgr.state.motor_power) {
      // htim1.Instance->CCER |=  TIM_CCxN_DISABLE << TIM_CHANNEL_2;
      htim1.Instance->CCR2 = 0; // pwm_h
      // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    } else {
      const auto prev = state_mgr.state.enc_vel;
      if (prev < 2000 && prev > -2000) {
        htim1.Instance->PSC = 2;
        htim1.Instance->ARR = 47999;
        htim1.Instance->CCR2 = 47999 * std::abs(state_mgr.state.motor_power) * 0.01; // pwm_h // aho
        htim1.Instance->CCR3 = state_mgr.state.motor_power > 0 ? 47999 : 0; // phase
      } else if (prev < 4000 && prev > -4000) {
        htim1.Instance->PSC = 1;
        htim1.Instance->ARR = 35999;
        htim1.Instance->CCR2 = 35999; // pwm_h
        htim1.Instance->CCR3 = 17999 * state_mgr.state.motor_power * 0.01 + 18000; // phase
      } else {
        htim1.Instance->PSC = 0;
        htim1.Instance->ARR = 7199;
        htim1.Instance->CCR2 = 7199; // pwm_h
        htim1.Instance->CCR3 = 3599 * state_mgr.state.motor_power * 0.01 + 3600; // phase
      }
      // // htim1.Instance->CCER |=  TIM_CCxN_ENABLE << TIM_CHANNEL_2;
      // htim1.Instance->CCR2 = 35999; // pwm_h
      // htim1.Instance->CCR3 = 17999 * state_mgr.state.motor_power * 0.01 + 18000; // phase
    }
    // prev = state_mgr.state.motor_power;
  }));
  task_list.emplace_back("blink", new SimpleTask(5, [] {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
  }));
  task_list.emplace_back("serial", new SerialTask(120, huart2));
}

void loop() {
  for (const auto& task : task_list) {
    task->process();
  }
}
