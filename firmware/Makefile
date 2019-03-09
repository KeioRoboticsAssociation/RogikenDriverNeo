rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

TARGET = firmware

DEBUG = 0

OPT = -Og

BUILD_DIR = .build

C_SOURCES = \
$(call rwildcard,src/,*.c) \
$(call rwildcard,lib/CMSIS_5/CMSIS/DSP/Source/,*.c) \
$(call rwildcard,lib/CubeMX/,*.c)
CXX_SOURCES = $(call rwildcard,src/,*.cpp)
ASM_SOURCES = lib/CubeMX/startup_stm32f303x8.s

PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS = 

CPP_DEFS = \
-DUSE_HAL_DRIVER \
-DSTM32F303x8

AS_INCLUDES = 

CPP_INCLUDES = \
-Iinclude

CPP_LIB_INCLUDES = \
-isystemlib/CMSIS_5/CMSIS/Core/Include \
-isystemlib/CMSIS_5/CMSIS/DSP/Include \
-isystemlib/CubeMX/Inc \
-isystemlib/CubeMX/Drivers/STM32F3xx_HAL_Driver/Inc \
-isystemlib/CubeMX/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy \
-isystemlib/CubeMX/Drivers/CMSIS/Device/ST/STM32F3xx/Include \
-isystemlib/CubeMX/Drivers/CMSIS/Include

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CPPFLAGS = $(MCU) $(CPP_DEFS) $(CPP_INCLUDES) $(CPP_LIB_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -fsingle-precision-constant
ifeq ($(DEBUG), 1)
CPPFLAGS += -g -gdwarf-2
endif
CPPFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

CXXFLAGS = -std=c++17 -fno-rtti -fno-exceptions -pedantic -Wextra -Wdisabled-optimization -Wdouble-promotion -Wfloat-equal -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wswitch-default -Wundef

LDSCRIPT = lib/CubeMX/STM32F303K8Tx_FLASH.ld

LIBS = -lc -lm -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -specs=nosys.specs -u_scanf_float -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

clean:
	-rm -fR $(BUILD_DIR)
  
-include $(wildcard $(BUILD_DIR)/*.d)
