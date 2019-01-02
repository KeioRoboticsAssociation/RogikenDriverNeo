#include "user_main.h"

#include <stdio.h>

#include "stm32f303x8.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_gpio.h"

void setup() {}

void loop() {
  printf("test\n");
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
  HAL_Delay(100);
}
