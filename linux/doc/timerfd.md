# timerfd 
    创建一个定时器，该定时器通过文件描述符来传递定时器过期通知，可以用select、poll、epoll来监听。

## 接口
``````C
#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);
int timerfd_settime(int fd, int flags,
                    const struct itimerspec *new_value,
                    struct itimerspec *_Nullable old_value);
int timerfd_gettime(int fd, struct itimerspec *curr_value);
``````

- timerfd_create
  - 参数
    - clockid:只能用一下的参数
      - CLOCK_REALTIME: 非单调递增时钟，会被系统时间调整(NTP & adjtime)影响(一般不用)
      - CLOCK_MONOTONIC:单调递增时钟，不可修改，但是会被adjtime和NTP影响，频率影响时间(一般使用)，对系统挂起不敏感
      - CLOCK_BOOTTIME:单调递增时钟,系统挂起也一直计时，但会被settimeofday活着类似函数修改BOOT TIME影响
      - CLOCK_REALTIME_ALARM & CLOCK_BOOTTIME_ALARM: 没用过，应用场景目前不存在系统挂起状态
    - flags:
      - linux 2.6.26之前只能设置为0
      - O_NONBLOCK:设置对select、poll、epoll没有影响，仅告诉调用者文件描述符是否就绪
      - TFD_CLOEXEC:多线程使用
- timerfd_settime
  - new_value.it_value非0启动定时器，0关闭定时器；new_value.it_interval超时周期
  - flags:


[open](https://man7.org/linux/man-pages/man2/open.2.html)
[time_create](https://man7.org/linux/man-pages/man2/timerfd_create.2.html)