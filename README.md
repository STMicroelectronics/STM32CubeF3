# STM32CubeF3 MCU Firmware Package

**STM32Cube** is an STMicroelectronics original initiative to ease the developers life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform (this repo), delivered for each series (such as the STM32CubeF3 for the STM32F3 series).
   * The CMSIS modules (core and device) corresponding to the ARM-tm core implemented in this STM32 product
   * The STM32 HAL-LL drivers : an abstraction drivers layer, the API ensuring maximized portability across the STM32 portfolio
   * The BSP Drivers of each evaluation or demonstration board provided by this STM32 series
   * A consistent set of middlewares components such as RTOS, USB, FatFS, LwIP, Graphics ...
   * A full set of software projects (basic examples, applications or demonstrations) for each board provided by this STM32 series

The **STM32CubeF3 MCU Package** projects are directly running on the STM32F3 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples)

In this FW Package, the module **Middlewares/ST/STemWin** is not directly accessible. It must be downloaded from a ST server. The URL is available in a readme.txt file inside the module.

## Boards available
  * STM32F3
    * [STM32303E-EVAL](https://www.st.com/en/product/stm32303e-eval.html)
    * [STM32373C-EVAL](https://www.st.com/en/product/stm32373c-eval.html)
    * [32F3348DISCOVERY](https://www.st.com/en/evaluation-tools/32f3348discovery.html)
    * [STM32F3DISCOVERY](https://www.st.com/en/evaluation-tools/stm32f3discovery.html)
    * [NUCLEO-F301K8](https://www.st.com/en/evaluation-tools/nucleo-f301k8.html)
    * [NUCLEO-F302R8](https://www.st.com/en/product/nucleo-f302r8.html)
    * [NUCLEO-F303K8](https://www.st.com/en/product/nucleo-f303k8.html)
    * [NUCLEO-F303RE](https://www.st.com/en/evaluation-tools/nucleo-f303re.html)
    * [NUCLEO-F303ZE](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)
    * [NUCLEO-F334R8](https://www.st.com/en/evaluation-tools/nucleo-f334r8.html)

## Troubleshooting

**Caution** : The **Issues** requests are strictly limited to submit problems or suggestions related to the software delivered in this repo 

**For any question** related to the STM32F3 product, the hardware performance, the hardware characteristics, the tools, the environment, you can submit a topic on the [ST Community/STM32 MCUs forum](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus)