MCU := STM32F103RE
PRODUCT_ID := 0003
ERROR_LED_PORT := GPIOA
ERROR_LED_PIN := 5
DENSITY := STM32_HIGH_DENSITY
TARGET_FLAGS += -D$(DENSITY)
MCU_SERIES := stm32f1
