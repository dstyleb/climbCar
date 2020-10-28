################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_20.2.2.LTS/bin/cl430" -vmsp --opt_for_speed=2 --use_hw_mpy=16 --include_path="C:/ti/ccs1011/ccs/ccs_base/msp430/include" --include_path="C:/Users/dxd19/workspace_v10/climbCar" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_20.2.2.LTS/include" --advice:power=all --define=__MSP430F149__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


