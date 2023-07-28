[TOC]

# 环境
    - ubuntu 20.04 
    - crosstools : arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf
    - kernel:6.4
    - ubuntu:
    - board environment:am335x(arm、cortex-A8、 32bit)

# uboot

## set kgdboc
    "mmcargs=setenv bootargs root=${mmcroot} rootfstype=ext4 rootwait rw console=${console} blkdevparts=${blkdevparts}  kgdbwait\0"

    Note: 理论上应该写为``kgdboc console=/dev/ttyS0,115200n8``,但是这样会导致内核起不来，不清楚什么原因

# kernel

## set config

### kgdb
    CONFIG_STRICT_KERNEL_RWX is not set
    CONFIG_FRAME_POINTER=y
    CONFIG_KGDB=y
    CONFIG_KGDB_SERIAL_CONSOLE=y

### kdb
    CONFIG_STRICT_KERNEL_RWX is not set
    CONFIG_FRAME_POINTER=y
    CONFIG_KGDB=y
    CONFIG_KGDB_SERIAL_CONSOLE=y
    CONFIG_KGDB_KDB=y
    CONFIG_KDB_KEYBOARD=y

# ubuntu 20.04 问题与解决
1、sudo apt install libncursesw5
> error while loading shared libraries: libncursesw.so.5: cannot open shared object file: No such file or directory
>

[参考链接](https://github.com/CleverRaven/Cataclysm-DDA/issues/51775)

2、由于交叉编译工具链的中gdb工具是由python 3.8 编译的，所以需要下载指定的python3.8版本(错误如下)

> 
> Fatal Python error: init_sys_streams: can't initialize sys standard streams
> Python runtime state: core initialized Traceback (most recent call last): File "/usr/lib/python3.10/io.py",
> line 54, in <module> ImportError: cannot import name 'text_encoding' from 'io' (unknown location)
>

[python3.8下载-参考链接](https://towardsdatascience.com/installing-multiple-alternative-versions-of-python-on-ubuntu-20-04-237be5177474)

3、下载 agent-proxy
    单板只有一个串口，但Linux console和kgdb公用，会输出紊乱信息

[agent-proxy 下载链接](https://git.kernel.org/cgit/utils/kernel/kgdb/agent-proxy.git/)

# 调试

1、启动单板

    (1) 使能 kgdboc on ttyS0，命令如下，成功会提示【KGDB: Registered I/O driver kgdboc】
        - echo ttyS0 > /sys/module/kgdboc/parameters/kgdboc
    (2) 使能kgdb，命令如下，成功提示【KGDB: KGDB or $3#33 for KDB】
        - echo g > /proc/sysrq-trigger

2、crosstools gdb连接
    
    需要使用交叉编译工具链的gdb工具，和编译kernel的vmlinux可执行程序
    - sudo PYTHONHOME=/usr/lib/python3.8/ PYTHONPATH=/usr/lib/python3.8/ ~/linux/shifei/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gdb ./vmlinux
    - set serial baud 115200
    - target remote /dev/ttyUSB0

# 参考
[kernel-tools](https://www.kernel.org/doc/html/v5.2/dev-tools/kgdb.html)

[kernel-debugging-by-gdb](https://www.kernel.org/doc/html/v4.10/dev-tools/gdb-kernel-debugging.html#:~:text=The%20kernel%20debugger%20kgdb%2C%20hypervisors%20like%20QEMU%20or,scripts%20that%20can%20simplify%20typical%20kernel%20debugging%20steps.)

[kernel-mirrors](https://mirrors.edge.kernel.org/pub/linux/kernel/people/jwessel/kdb/kgdbKernelArgs.html)  

[linux-link](https://linuxlink.timesys.com/docs/how_to_use_kgdb)

[csdn-kgdb-imx6ull](https://blog.csdn.net/weixin_38832162/article/details/115347640)


