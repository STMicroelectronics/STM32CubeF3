@echo off
echo AXF to BIN file generation ...

"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" --bin --output ..\MDK-ARM\STM32303E_EVAL\STM32303E_EVAL_SysTick.bin ..\MDK-ARM\STM32303E_EVAL\STM32303E_EVAL.axf 

echo AXF to BIN file generation completed.

