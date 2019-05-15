@echo off
echo AXF to BIN file generation ...

"C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\STM32373C_EVAL\STM32373C_EVAL_SysTick.bin ..\MDK-ARM\STM32373C_EVAL\STM32373C_EVAL.axf 

echo AXF to BIN file generation completed.
