#if !defined(__SERIAL_TASK_H__)
#define __SERIAL_TASK_H__

#include <inttypes.h>
#include <string>

#include "stm32f3xx_hal.h"

#include "task.h"

class SerialTask : public Task {
  private:
    enum struct State {
      READY,
    };
    static constexpr uint16_t RX_DMA_CIRC_BUF_SIZE = 0x01 << 10;
    static auto clampRxBufCursor(const uint16_t) -> uint16_t;
    UART_HandleTypeDef& huart;
    uint16_t rx_buf_r_cur = 0;
    uint8_t rx_dma_circ_buf[RX_DMA_CIRC_BUF_SIZE];
    struct {
      uint16_t s_cur = 0;
      uint16_t e_cur;
      std::string data;
    } rx_frame = {};
    auto task() -> void override;
  public:
    SerialTask(const std::string, const uint32_t, UART_HandleTypeDef&);
    ~SerialTask() override;
};

#endif // __SERIAL_TASK_H__
