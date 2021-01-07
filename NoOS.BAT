SET PATH=F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin;F:\SoftWare\Keil_v5\ARM\ARMCC\Bin;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;F:\SoftWare\Git\cmd;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;F:\SoftWare\CMake\bin;C:\Program Files\dotnet\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\170\Tools\Binn\;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Users\SU\AppData\Local\Programs\Python\Python38\Scripts\;C:\Users\SU\AppData\Local\Programs\Python\Python38\;C:\Users\SU\AppData\Local\Microsoft\WindowsApps;F:\SoftWare\VSCode\bin;C:\Program Files\Bandizip\;C:\Users\SU\.dotnet\tools
SET CPU_TYPE=STM32H743VIHx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=NoOS
SET CPU_CLOCK=0x017D7840
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\stm32h7xx_hal_msp.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\stm32h7xx_it.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\system_stm32h7xx.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\startup_stm32h743xx._ac"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\main.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\sys_conf.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\stm32h7xx_hal.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\stm32h7xx_hal_adc.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\project\obj\stm32h7xx_hal_adc_ex.__i"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\project\obj\stm32h743vih6.lnp"
"F:\SoftWare\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\project\obj\stm32h743vih6.axf" --i32combined --output ".\project\obj\stm32h743vih6.hex"
