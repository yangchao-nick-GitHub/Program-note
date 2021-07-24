## 分区管理

![Image](https://github.com/yangchao-nick-GitHub/learn_shell/blob/main/Linux_learn/Partition.png)

linux 磁盘设备名 ： /dev/sda-z
查看分区具体情况：先用df -h 查看磁盘挂载的文件系统
然后使用fdish -l /dev/sda5 看看磁盘分区信息
也可以使用fdisk -l 看有哪几块磁盘

#### 如何进行磁盘分区？
使用fdisk工具进行分区，步骤如下
##### 1.关机后在虚拟机上加一个硬盘
使用fdisk -l 查看
>Disk /dev/sdb: 40 GiB, 42949672960 bytes, 83886080 sectors
Disk model: VMware Virtual S
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes




2.fdisk /dev/sdb 使用fdisk工具分区





## 逻辑卷管理

使用逻辑卷管理的好处：可以动态分配挂载的文件系统的空间
