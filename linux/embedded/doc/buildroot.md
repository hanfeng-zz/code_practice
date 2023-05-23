# buildroot

## 交叉编译工具链

### 介绍

+ 编译器（gcc工具等）
+ 二进制工具（汇编器、链接器、binutils）
+ C标准库（GUN LIBC、uGlibc-ng）
  
### 工具

+ 外部工具链：
  + 支持CodeSourcery 和 Linaro 的工具链、由crosstool-NG生成的工具链以及由Buildroot本身生成的工具链
  + 不支持由 OpenEmbedded 或 Yocto 生成的工具链或 SDK，因为它们不是纯粹的工具链（即仅编译器、 binutils、 C 和 C++库）
+ 内部狗工具链：build root自己内嵌的

# bootloader 、kernel、 file system

## boot loader

### 作用

>在嵌入式操作系统中，BootLoader是在操作系统内核运行之前运行。可以初始化硬件设备、建立内存空间映射图，从而将系统的软硬件环境带到一个合适状态，以便为最终调用操作系统内核准备好正确的环境。在嵌入式系统中，通常并没有像BIOS那样的固件程序（注，有的嵌入式CPU也会内嵌一段短小的启动程序），因此整个系统的加载启动任务就完全由BootLoader来完成。在一个基于ARM7TDMI core的嵌入式系统中，系统在上电或复位时通常都从地址0x00000000处开始执行，而在这个地址处安排的通常就是系统的BootLoader程序

### 种类

+ red boot
+ arm boot
+ u-boot
+ 等等

### 参考

[百度-bootloader](https://baike.baidu.com/item/BootLoader/8733520)
[bootloader-kernel-fileSystem关系](https://www.cnblogs.com/schips/p/13129047.html)
