# SBL
    
- seconder bootloader, 辅助引导加载程序
  - Bootstrap: Initial software launched by the ROM code during the memory booting phase
  - Downloaded software: Initial software downloaded into on-chip RAM by the ROM code during the peripheral booting phase
  - Initial software: Software executed by any of the ROM code mechanisms (memory booting or peripheral booting). Initial software is a generic term for bootstrap and downloaded software. This can be the SBL (secondary bootloader) responsible for loading an OS

# SPL OR MLO
- 二级加载程序,被ROM中的一级引导程序加载到外部内存(RAM==内存==DDR),用于初始化内存和配置UBOOT初始引导程序  
- The second stage bootloader is known as the SPL (Secondary Program Loader), but is sometimes referred to as the MLO (MMC Card Loader). The SPL is the first stage of U-Boot, and must be loaded from one of the boot sources into internal RAM. The SPL has very limited configuration or user interaction, and mainly serves to initialize the external DDR memory and set-up the boot process for the next bootloader stage: U-Boot
- 

# 参考
[SBL](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-jacinto7/07_00_00_11/exports/docs/pdk_jacinto_07_00_00/docs/userguide/boot/boot.html)
[SPL](https://software-dl.ti.com/jacinto7/esd/processor-sdk-linux-jacinto7/07_01_00_10/exports/docs/linux/Foundational_Components_U-Boot.html#general-information)
