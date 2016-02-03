################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Utilities/Strings.cpp 

OBJS += \
./src/Utilities/Strings.o 

CPP_DEPS += \
./src/Utilities/Strings.d 


# Each subdirectory must supply rules for building sources it contributes
src/Utilities/%.o: ../src/Utilities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/user/Desktop/labs-code/Shared" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


