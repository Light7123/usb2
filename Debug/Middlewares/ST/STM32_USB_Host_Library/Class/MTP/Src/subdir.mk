################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.c \
../Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.c 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.d \
./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.d 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.o \
./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/%.o Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/%.su: ../Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/%.c Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F107xC -c -I../Core/Inc -I"/home/user/STM32CubeIDE/workspace_1.10.1/usb2/Core/Src/2can" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-MTP-2f-Src

clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-MTP-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.d ./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.o ./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.su ./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.d ./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.o ./Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-MTP-2f-Src

