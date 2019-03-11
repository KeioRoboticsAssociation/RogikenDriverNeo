#include "enc_task.h"

#include <inttypes.h>
#include <string>

#include "stm32f3xx_hal.h"

#include "global.h"
#include "task.h"

auto EncTask::clampCntBufCursor(const uint8_t cur) -> uint8_t {
  return cur & (PREV_CNT_CIRC_BUF_SIZE - 1);
}

EncTask::EncTask(const uint32_t hertz, TIM_HandleTypeDef& htim) : Task(hertz), htim(htim) {
  HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_ALL);
}

EncTask::~EncTask() {
  HAL_TIM_Encoder_Stop(&htim, TIM_CHANNEL_ALL);
}

auto EncTask::task() -> void {
  uint16_t cur_pulse_cnt = htim.Instance->CNT;
  auto cur_measure_cycle_cnt = DWT->CYCCNT;
  int16_t pulse;
  uint32_t cycle;
  auto i = cnt_buf_s_cur;
  do {
    pulse = cur_pulse_cnt - prev_cnt_circ_buf[i].pulse;
    cycle = cur_measure_cycle_cnt - prev_cnt_circ_buf[i].cycle;
    i = clampCntBufCursor(i + 1);
  } while (i != cnt_buf_s_cur && pulse < 100 / MAX_QUANTIZATION_ERROR_PERCENTAGE);
  enc_speed = static_cast<float>(pulse) / cycle * FREQ;
  cnt_buf_s_cur = clampCntBufCursor(cnt_buf_s_cur - 1);
  prev_cnt_circ_buf[cnt_buf_s_cur] = {cur_pulse_cnt, cur_measure_cycle_cnt};
}
