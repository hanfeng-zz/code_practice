# 简介
> 
    LinuxPPS提供了一个编程接口(API)来在系统中定义几个PPS源。
    PPS的意思是“每秒脉冲数”，PPS源就是一种每秒提供高精度信号的设备，这样应用程序就可以用它来调整系统时钟时间。
    PPS源可以连接到串行端口(通常连接到数据载波检测引脚)或并行端口(ack引脚)或特定CPU的gpio(这是嵌入式系统中的常见情况)，但在每种情况下，当一个新的脉冲到达时，系统必须对其应用时间戳并为用户区记录它。
    常见的用途是结合NTPD作为用户程序，与GPS接收器作为PPS源，以获得与UTC同步的亚毫秒时钟时间。
>
# 目前使用场景

> 打开/dev/pps1设备，该设备是外部PPS触发，软件收到pps信号，来写NTP Share Memory

# 待研究
> 根据PPS信号来进行时间同步，计算时间偏差，频率偏差，来调整CPU或者其它的设备进行时间同步等
> 未完全理解Linux PTP、chrony、GPSD、NTP几者之间的关联

# 头文件

``````C
#include <linux/pps.h>
#include <sys/timepps.h>
``````


# RFC 介绍

1. RFC 2783: PPS POSIX API
    1. 两种时间格式
      - NTP：64BIT, 32BIT表示SEC，32BIT表示NSEC
      - struct timespec
    2. PPS_CANPOLL:目前不能使用
    3. PPS_CANWAIT:
       >该BIT只读，使用前需time_pps_getcap()判断当前Linux系统是否支持；若支持，配合time_pps_fetch()使用
    4. 如果需要设置PPS FD，open时需要设置成O_RDWR
    5. pps handle不支持 select()或者poll()来等待PPS EVENT，即PPS FD(handle)只能使用time_pps_fetch()来监听，该函数第4个参数可以设置等待时延，如果设置为NULL，则有PPS EVENT才会执行，使用前需要检查PPS_CANWAIT功能是否存在
    6. 
+ RFC 1589:精确时间同步模型`待研究`



# Example

```C++
#include <iostream>
#include <sys/timepps.h>
#include <linux/pps.h>
#include <spdlog/spdlog.h>
#define DP4000_PPS_DEVICE "/dev/pps1"

struct pps_irq {
    pps_handle_t handle;
    int avail_mode;
    pps_params_t params;
};

static struct timespec offset_assert = {0, 0};

int main() {

    struct pps_irq _pps {};
    
    int ret  = open(DP4000_PPS_DEVICE, O_RDWR);
    if (ret < 0) {
        spdlog::error("[{}][{}] open /dev/pps1 failed,errno:{}", __FUNCTION__, __LINE__, errno);
        return -1;
    }
    spdlog::info("[{}][{}] {} open success,fd:{}", __FUNCTION__, __LINE__, DP4000_PPS_DEVICE, ret);

    ret = time_pps_create(ret, &_pps.handle);
    if (ret < 0) {
        spdlog::error("[{}][{}] cannot create a PPS source from device:{}", __FUNCTION__, __LINE__, DP4000_PPS_DEVICE);
        return -1;
    }
    spdlog::info("[{}][{}] found PPS source:{}", __FUNCTION__, __LINE__, DP4000_PPS_DEVICE);

    ret = time_pps_getcap(_pps.handle, &_pps.avail_mode);
    if (ret < 0) {
        spdlog::error("[{}][{}] cannot get capabilities", __FUNCTION__, __LINE__);
        return -1;
    }

    if ((_pps.avail_mode & PPS_CAPTUREASSERT) == 0) {
        spdlog::error("[{}][{}] cannot CAPTUREASSERT", __FUNCTION__, __LINE__);
        return -1;
    }

    if ((_pps.avail_mode & PPS_CANWAIT) == 0) {
        spdlog::error("[{}][{}] cannot CANWAIT", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = time_pps_getparams(_pps.handle, &_pps.params);
    if (ret < 0) {
        spdlog::error("[{}][{}] cannot get parameters", __FUNCTION__, __LINE__);
        return -1;
    }
    _pps.params.mode |= PPS_CAPTUREASSERT;
    /* Override any previous offset if possible */
    if ((_pps.avail_mode & PPS_OFFSETASSERT) != 0) {
        _pps.params.mode |= PPS_OFFSETASSERT;
        _pps.params.assert_offset = offset_assert;
    }

    ret = time_pps_setparams(_pps.handle, &_pps.params);
    if (ret < 0) {
        spdlog::error("[{}][{}] cannot set parameters", __FUNCTION__, __LINE__);
        return -1;
    }
    return 0;
}
```

# 链接

- [linuxPPS/description](https://manpages.org/pps-tools/8)
- [参考代码/pps-test](https://github.com/redlab-i/pps-tools.git)
- [pps/ntpd](https://www.crc.id.au/2016/09/24/adding-a-pps-source-to-ntpd/)
- [linux_kernel/PPS](https://www.kernel.org/doc/html/latest/driver-api/pps.html)
- [一般](http://paul.chavent.free.fr/pps.html)