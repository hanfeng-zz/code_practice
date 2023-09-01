Author: zheng zhao  
Data: 2023-09-01
Note: 理解不深入,欢迎交流

# 问题 :使用initramfs的方式加载文件系统
    遗留:
    1. rootfs.cpio压缩文件(gz,xz等)为什么未生效?本身不支持?
    2. 自定义初始化脚本(todo)
# 环境
    - ubuntu20.04
    - buildroot 2022-08 (ti官方)
    - kernel/uboot 基于ti arm6412 官方sdk
    将build root 编译的rootfs内嵌到ti 官方sdk-kernel中,使用initramfs方式, init system 是busybox

# 解决

## buildroot config 文件配置
    
### 指定 CPU, external toolchain 
  
### initramfs 相关配置
-  BR2_ROOTFS_DEVICE_CREATION_DYNAMIC_DEVTMPFS = y
-  BR2_TARGET_ROOTFS_CPIO=y
-  BR2_TARGET_ROOTFS_CPIO_GZIP=y (可选)

## kernel config 文件配置

CONFIG_BLK_DEV_INITRD=y
CONFIG_INITRAMFS_SOURCE=" "buildroot-path"/rootfs.cpio"
CONFIG_INITRAMFS_ROOT_UID=0
CONFIG_INITRAMFS_ROOT_GID=0
CONFIG_RD_GZIP=y
CONFIG_RD_BZIP2=y
CONFIG_RD_LZMA=y
CONFIG_RD_XZ=y
CONFIG_RD_LZO=y
CONFIG_RD_LZ4=y
CONFIG_RD_ZSTD=y
CONFIG_INITRAMFS_COMPRESSION_GZIP=y


# 调试备忘录
    1.实际调试kernel过程中,发现需要init 的引导文件
    2.查找验证,得出buildroot 指定BR2_ROOTFS_DEVICE_CREATION_DYNAMIC_DEVTMPFS=y,编译后得到rootfs.cpio文件,解压发现内容init文件
    3.确认后,尝试kernel指定initramfs文件为rootfs.cpio, rootfs.cpio解压文件,gz压缩文件,只有rootfs.cpio文件有效

# 参考 & 简述 
- [Early userspace support](https://github.com/torvalds/linux/blob/master/Documentation/driver-api/early-userspace/early_userspace_support.rst)
    - 简述: 当前内核有3中挂在文件系统方式,initramfs是其中一种,文件格式是cpio压缩格式,通过/init文件引导
- [主要参考连接-1](http://trac.gateworks.com/wiki/buildroot)
- [主要参考链接-2](http://trac.gateworks.com/wiki/linux/initramfs)
- [次要参考连接](https://blog.51cto.com/u_15127585/4384962)
