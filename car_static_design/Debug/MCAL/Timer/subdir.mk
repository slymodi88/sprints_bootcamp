################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/Timer/timer.c 

OBJS += \
./MCAL/Timer/timer.o 

C_DEPS += \
./MCAL/Timer/timer.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/Timer/%.o: ../MCAL/Timer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\APP\Car_Sm" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\MCAL\SPI" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\Common" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\APP\Sterring" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\Common" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\ECUAL\ICU" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\ECUAL\Motors" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\ECUAL\UltraSonic" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\MCAL\DIO1" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\MCAL\ExternalInterrupt" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\MCAL\PWM" -I"C:\Users\memo1\OneDrive\Desktop\IMT_SDK_Win_64\DIO\MCAL\Timer" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


