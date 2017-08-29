################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ..\makefile.init

RM := del 

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries\async-mqtt-client\src\AsyncMqttClient\Packets\subdir.mk
-include libraries\async-mqtt-client\src\subdir.mk
-include libraries\Timezone\subdir.mk
-include libraries\Time\subdir.mk
-include libraries\Ticker\subdir.mk
-include libraries\Hash\src\sha1\subdir.mk
-include libraries\Hash\src\subdir.mk
-include libraries\ESPAsyncWebServer\src\subdir.mk
-include libraries\ESPAsyncUDP\src\subdir.mk
-include libraries\ESPAsyncTCP\src\subdir.mk
-include libraries\ESP8266httpUpdate\src\subdir.mk
-include libraries\ESP8266WiFi\src\subdir.mk
-include libraries\ESP8266HTTPClient\src\subdir.mk
-include libraries\EEPROM\subdir.mk
-include libraries\DNSServer\src\subdir.mk
-include libraries\DHT\subdir.mk
-include core\core\umm_malloc\subdir.mk
-include core\core\spiffs\subdir.mk
-include core\core\libb64\subdir.mk
-include core\core\subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ..\makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
F:\zzzBinh\Arduino\IoTFirmware\Release\arduino.ar \

ELF += \
IoTFirmware.elf \

HEX += \
IoTFirmware.hex \


# All Target
all: IoTFirmware

# Tool invocations
IoTFirmware: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"F:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/xtensa-lx106-elf/bin/xtensa-lx106-elf-size" -A "F:/zzzBinh/Arduino/IoTFirmware/Release/IoTFirmware.elf"
	@echo 'Finished building target: $@'
	@echo ' '

F:\zzzBinh\Arduino\IoTFirmware\Release\arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"F:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/xtensa-lx106-elf/bin/xtensa-lx106-elf-ar" cru "F:\zzzBinh\Arduino\IoTFirmware/Release/arduino.ar"    F:/zzzBinh/Arduino/IoTFirmware/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

IoTFirmware.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"F:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc" -g -Wall -Wextra -Os -nostdlib -Wl,--no-check-sections -u call_user_start -u _printf_float -u _scanf_float -Wl,-static "-LF:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/sdk/lib" "-LF:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/sdk/ld" "-LF:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/sdk/libc/xtensa-lx106-elf/lib" "-Teagle.flash.1m0.ld" -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read -o "F:\zzzBinh\Arduino\IoTFirmware/Release/IoTFirmware.elf" -Wl,--start-group   $(LINK_OBJ) $(AR)  "F:\zzzBinh\Arduino\IoTFirmware/Release/arduino.ar" -lhal -lphy -lpp -lnet80211 -llwip_gcc -lwpa -lcrypto -lmain -lwps -laxtls -lespnow -lsmartconfig -lmesh -lwpa2 -lstdc++ -lm -lc -lgcc -Wl,--end-group "-LF:\zzzBinh\Arduino\IoTFirmware/Release" F:/zzzBinh/Arduino/IoTFirmware/Release/arduino.ar
	@echo 'Finished building: $@'
	@echo ' '

IoTFirmware.hex: $(ELF)
	@echo 'Create eeprom image'
	"F:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/tools/esptool/esptool.exe" -eo "F:/zzzBinh/arduino-1.8.4/hardware/esp8266com/esp8266/bootloaders/eboot/eboot.elf" -bo "F:/zzzBinh/Arduino/IoTFirmware/Release/IoTFirmware.bin" -bm dio -bf 40 -bz 1M -bs .text -bp 4096 -ec -eo "F:/zzzBinh/Arduino/IoTFirmware/Release/IoTFirmware.elf" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ..\makefile.targets
