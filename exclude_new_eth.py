""" Pre-build script: exclude new HAL ETH driver, use old one instead """
import os
Import("env")

piohome = env.subst("$PIOHOME_DIR")
fw_eth = os.path.join(piohome, "packages", "framework-stm32cubeh7",
    "Drivers", "STM32H7xx_HAL_Driver", "Src", "stm32h7xx_hal_eth.c")
fw_eth_ex = os.path.join(piohome, "packages", "framework-stm32cubeh7",
    "Drivers", "STM32H7xx_HAL_Driver", "Src", "stm32h7xx_hal_eth_ex.c")

for f in [fw_eth, fw_eth_ex]:
    bak = f + ".disabled"
    if os.path.exists(f):
        os.rename(f, bak)
        print(f"[pre-build] Disabled: {os.path.basename(f)}")
