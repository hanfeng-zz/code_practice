# 用户态(mmap)访问CPU SPI寄存器,读写switch 芯片
    调试备忘录
## 环境

- CPU: 
  - AM6412 
  - 2 ARM Cortex-a53 
  - 64Bit
- Switch: VSC7512
- 环境:
  - Ubuntu20.04
  - PROCESSOR-SDK-LINUX-AM64X(TI 官方SDK, 6.1内核)
- 实现:直接控制CPU-SPI寄存器来实现和switch数据交互

## mmap

        内存映射接口,把设备地址映射到虚拟地址,通过操作虚拟地址直接访问设备,减少内核态到用户态的数据复制开销,提高访问性能速度比ioctl方式速度快(更多内容见链接博文)

## devmem

- 内核提供的访问器件的读写模块
- 要在内核中使能该模块  
    Device Drivers  --->  Character devices  --->  [*] /dev/mem virtual device support
- 读写规则  
    Usage: devmem ADDRESS [WIDTH [VALUE]]   
    Read/write from physical address    
    ADDRESS Address to act upon 
    WIDTH   Width (8/16/...)    
    VALUE   Data to be written  

## 代码实现(仅部分,不包括SPI控制)

### mmap
""" C

    -->确认CPU和SWITCH链接的SPI线编号,即SPI3   
        -->查找CPU手册,记录该SPI总线地址
            -->打开/dev/mmap设备
    
    page_size = sysconf(_SC_PAGESIZE);
    
    //打开/dev/mem设备
    spi3_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (spi3_fd < 0) {
        printf("%s open /dev/mem error\n", __func__);
        return -1;
    }

    //尽量使用volatile关键字,防止CPU优化
    //NULL表示内核选择(page-aligned)创建映射的地址(对使用者很好)
    //访问CPU的寄存器地址长度,为了省事直接使用页大小
    //推荐使用 PROT_READ | PROT_WRITE 参数组合
    //SPI3_BASE_ADDR是CPU SPI 第三根总线初始地址
    //共享这个MAP,其它进程可见
    //详细见 [kernel-mmap-desc] 链接
    spi_reg_base = (volatile unsigned char *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, spi3_fd, SPI3_BASE_ADDR);
    if (NULL == spi_reg_base) {
        printf("%s get spi 3 physical address error\n", __func__);
        return -1;
    }

"""

### SPI
    - SPI_CK, SPI_CLK, SPI_MISO, SPI_MOSI(读写数据线分开)
    - 需要启动内核,在dts中配置SPI的速率,模式等,用户态仅去读写操作
    - write操作后需要,立刻回读,防止没有写入
    - SPI WORD LEN = 8BIT; 由于CPU的SPI接受寄存器是32BIT, 每次仅读取低8BIT数据即可,然后拼接成32BIT数据
    - 由于SPI SPEED = 20MHZ, PAD = 3; SPI写入长度应该是 7 + PAD;写入地址开头应该是0x80 + switch偏移地址; 
    - 只读只写需要将设置相应模式 

## 参考

[kernel-mmap.rst](https://www.kernel.org/doc/html/v4.14/media/uapi/v4l/mmap.html?highlight=mmap)    
[mmap-1](https://www.cnblogs.com/wanghuaijun/p/7624564.html)    
[mmap-2](https://zhuanlan.zhihu.com/p/640169233)    
[kernel-mmap-desc](https://www.man7.org/linux/man-pages/man2/mmap.2.html)
[stackoverflow](https://stackoverflow.com/questions/45972/mmap-vs-reading-blocks)

