# ubuntu 20.04 环境设置问题

## ch341-uart ttyUSB0 不被识别
- ubuntu 20.04 中ch341.ko太老
  - [ch341x-下载链接](https://www.wch.cn/download/CH341SER_LINUX_ZIP.html)
  - 放入Ubuntu后，unzip解压，并执行sudo make install 
- brltty进程占用了串口
  - sudo apt autoremove brltty
- 拔插串口，重新识别设备

## lib
  - sudo apt-get install libncurses5-dev
  - sudo apt-get install lzop
  - sudo apt-get install build-essential

## yyloc

  - /usr/bin/ld: scripts/dtc/dtc-parser.tab.o:(.bss+0x50): multiple definition of `yylloc'; scripts/dtc/dtc-lexer.lex.o:(.bss+0x0): first defined here
  - 解决：修改scripts/dtc目录下的dtc-lexer.lex.c_shipped文件中找到YYLTYPE yyloc这一行，在640行，在之前面加上extern

# gun binutils
[链接](https://sourceware.org/binutils/)

# uboot

## 语法 & uboot
  [uboot-record](https://elinux.org/Boot_Loaders#U-Boot)

## 基础知识

  ### nand和esmc的区别

  ### norflash、nandflash 区别

  ### sram 和sdram区别

## linker script 

[参考链接](http://sourceware.org/binutils/docs/ld/Scripts.html#Scripts)