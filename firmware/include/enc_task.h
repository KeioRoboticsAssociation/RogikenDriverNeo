#if !defined(__ENC_TASK_H__)
#define __ENC_TASK_H__

#include <inttypes.h>
#include <string>

#include "stm32f3xx_hal.h"

#include "task.h"

class EncTask : public Task {
  private:
    static constexpr uint8_t PREV_CNT_CIRC_BUF_SIZE = 0x01 << 7;
    static constexpr uint8_t MAX_QUANTIZATION_ERROR_PERCENTAGE = 5;
    static auto clampCntBufCursor(const uint8_t) -> uint8_t;
    TIM_HandleTypeDef& htim;
    struct {
      uint16_t pulse = 0;
      uint32_t cycle = DWT->CYCCNT;
    } prev_cnt_circ_buf[PREV_CNT_CIRC_BUF_SIZE] = {};
    uint8_t cnt_buf_s_cur = 0;
    auto task() -> void override;
  public:
    EncTask(const std::string, const uint32_t, TIM_HandleTypeDef&);
    ~EncTask() override;
};

#endif // __ENC_TASK_H__
