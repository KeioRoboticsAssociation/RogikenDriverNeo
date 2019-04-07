#include "serial_task.h"

#include <inttypes.h>

#include "stm32f3xx_hal.h"

#include "global.h"
#include "task.h"

auto SerialTask::clampRxBufCursor(const uint16_t cur) -> uint16_t {
  return cur & (RX_DMA_CIRC_BUF_SIZE - 1);
}

SerialTask::SerialTask(const uint32_t hertz, UART_HandleTypeDef& huart)
  : Task(hertz), huart(huart) {
  HAL_UART_Receive_DMA(&huart, rx_dma_circ_buf, RX_DMA_CIRC_BUF_SIZE);
}

SerialTask::~SerialTask() {
  HAL_UART_DMAStop(&huart);
}

auto SerialTask::task() -> void {
  auto rx_buf_w_next_cur = RX_DMA_CIRC_BUF_SIZE - huart.hdmarx->Instance->CNDTR; // SIZE >= CNDTR >= 1
  auto rx_buf_w_last_cur = clampRxBufCursor(rx_buf_w_next_cur - 1);
  if (rx_buf_r_cur != rx_buf_w_last_cur) {
    do {
      rx_buf_r_cur = clampRxBufCursor(rx_buf_r_cur + 1);
    } while (rx_buf_r_cur != rx_buf_w_last_cur && rx_dma_circ_buf[rx_buf_r_cur] != '\n');
    if (rx_dma_circ_buf[rx_buf_r_cur] == '\n') {
      rx_frame.e_cur = rx_buf_r_cur;
      if (rx_frame.s_cur < rx_frame.e_cur) {
        rx_frame.data.assign(
          reinterpret_cast<char*>(rx_dma_circ_buf + rx_frame.s_cur),
          rx_frame.e_cur + 1 - rx_frame.s_cur
        );
      } else {
        rx_frame.data.assign(
          reinterpret_cast<char*>(rx_dma_circ_buf + rx_frame.s_cur),
          RX_DMA_CIRC_BUF_SIZE - rx_frame.s_cur
        );
        rx_frame.data.append(
          reinterpret_cast<char*>(rx_dma_circ_buf),
          rx_frame.e_cur + 1
        );
      }
      state_mgr.executeTextCommand(rx_frame.data);
      rx_frame.s_cur = clampRxBufCursor(rx_frame.e_cur + 1);
    }
  }
}
