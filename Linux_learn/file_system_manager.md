## 分区管理

![Image](https://github.com/yangchao-nick-GitHub/learn_shell/blob/main/Linux_learn/Partition.png)

linux 磁盘设备名 ： /dev/sda-z
查看分区具体情况：先用df -h 查看磁盘挂载的文件系统
然后使用fdish -l /dev/sda5 看看磁盘分区信息
也可以使用fdisk -l 看有哪几块磁盘

#### 如何进行磁盘分区？
默认一个硬盘只能创建4个主分区，如果需要创建多个分区，需要用一个分区创建一个拓展分区，这个分区表用来记录更多的分区信息，使用在这个分区再创建多个分区，用拓展分区创建的分区叫做逻辑分区

#### 使用fdisk工具进行分区并挂载使用

##### 1.关机后在虚拟机上加一个40G硬盘
使用fdisk -l 查看，是一个没有被分区的磁盘
```
Disk /dev/sdb: 40 GiB, 42949672960 bytes, 83886080 sectors
Disk model: VMware Virtual S
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
```

##### 2.fdisk /dev/sdb 进入fdisk命令行进行分区

##### 3.创建一个10G的主分区
a.  n 创建分区
b.  p 创建主分区
c.  1 创建分区号为1的分区
d.  输入第一个扇区，这里直接回车即可，默认接上一块分区的扇区
e.  输入最后一个扇区，这里输入+10G
f.  查看创建的分区  ，输入p
g.  输入w保存并退出

```
Command (m for help): n
Partition type
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (1-4, default 1): 1
First sector (2048-83886079, default 2048):
Last sector, +/-sectors or +/-size{K,M,G,T,P} (2048-83886079, default 83886079): +10G

Created a new partition 1 of type 'Linux' and of size 10 GiB.

Command (m for help): p
Disk /dev/sdb: 40 GiB, 42949672960 bytes, 83886080 sectors
Disk model: VMware Virtual S
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x3818f521

Device     Boot Start      End  Sectors Size Id Type
/dev/sdb1        2048 20973567 20971520  10G 83 Linux

Command (m for help): w
The partition table has been altered.
Calling ioctl() to re-read partition table.
Syncing disks.
```

##### 4.创建逻辑分区
使用相同的步骤创建两个个主分区，一个拓展分区和三个个逻辑分区
主分区和扩展分区的磁盘号位1-4，也就是说最多有4个主分区或者扩展分区，逻辑分区开始的磁盘号为5


```
Command (m for help): p
Disk /dev/sdb: 40 GiB, 42949672960 bytes, 83886080 sectors
Disk model: VMware Virtual S
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x3818f521

Device     Boot    Start      End  Sectors Size Id Type
/dev/sdb1           2048 20973567 20971520  10G 83 Linux
/dev/sdb2       20973568 41945087 20971520  10G 83 Linux
/dev/sdb3       41945088 83886079 41940992  20G  5 Extended
/dev/sdb5       41947136 52432895 10485760   5G 83 Linux
/dev/sdb6       52434944 62920703 10485760   5G 83 Linux
/dev/sdb7       62922752 83886079 20963328  10G 83 Linux
```

现在虽然创建了分区，但是还不能使用，需要格式化分区，格式化成指定的文件系统

##### 5.格式化文件系统
使用ext4文件系统格式化分区
```
➜  Linux_learn git:(main) ✗ mkfs -t ext4 /dev/sdb1
mke2fs 1.45.5 (07-Jan-2020)
Creating filesystem with 2621440 4k blocks and 655360 inodes
Filesystem UUID: 53b8410a-6e88-4cae-89a7-442d12deec0b
Superblock backups stored on blocks:
        32768, 98304, 163840, 229376, 294912, 819200, 884736, 1605632

Allocating group tables: done
Writing inode tables: done
Creating journal (16384 blocks): done
Writing superblocks and filesystem accounting information: done
```

##### 6.挂载文件系统
mount /dev/sdb1 /yangchao
df -h 查看挂载成功

```
Filesystem      Size  Used Avail Use% Mounted on
udev            1.9G     0  1.9G   0% /dev
tmpfs           391M  2.0M  389M   1% /run
/dev/sda5        98G   17G   76G  18% /
tmpfs           2.0G     0  2.0G   0% /dev/shm
tmpfs           5.0M  4.0K  5.0M   1% /run/lock
tmpfs           2.0G     0  2.0G   0% /sys/fs/cgroup
/dev/loop0       56M   56M     0 100% /snap/core18/2066
/dev/loop1       33M   33M     0 100% /snap/snapd/12159
/dev/loop2      219M  219M     0 100% /snap/gnome-3-34-1804/66
/dev/loop3       56M   56M     0 100% /snap/core18/2074
/dev/loop4       66M   66M     0 100% /snap/gtk-common-themes/1515
/dev/loop5       65M   65M     0 100% /snap/gtk-common-themes/1514
/dev/loop6       51M   51M     0 100% /snap/snap-store/547
/dev/loop7       33M   33M     0 100% /snap/snapd/12398
/dev/loop8       51M   51M     0 100% /snap/snap-store/542
/dev/loop9      219M  219M     0 100% /snap/gnome-3-34-1804/72
/dev/sda1       511M  4.0K  511M   1% /boot/efi
tmpfs           391M   24K  391M   1% /run/user/1000
tmpfs           391M     0  391M   0% /run/user/0
/dev/sr0        2.7G  2.7G     0 100% /media/yangchao/Ubuntu 20.04.2.0 LTS amd64
/dev/sdb1       9.8G   37M  9.3G   1% /yangchao
```

取消挂载： umount /yangchao

挂载分区前最好使用fsck 强制修复磁盘，防止挂载失败

##### 创建交换分区
 创建交换分区可以解决物理内存不足导致程序无法运行的问题，也就是说，物理内存不够用时，可以用swap分区来顶替
1.修改分区类型
fdisk /dev/sdb
```
➜  ~ fdisk /dev/sdb

Welcome to fdisk (util-linux 2.34).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.


Command (m for help): p
Disk /dev/sdb: 40 GiB, 42949672960 bytes, 83886080 sectors
Disk model: VMware Virtual S
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x3818f521

Device     Boot    Start      End  Sectors Size Id Type
/dev/sdb1           2048 20973567 20971520  10G 83 Linux
/dev/sdb2       20973568 41945087 20971520  10G 83 Linux
/dev/sdb3       41945088 83886079 41940992  20G  5 Extended
/dev/sdb5       41947136 52432895 10485760   5G 83 Linux
/dev/sdb6       52434944 62920703 10485760   5G 83 Linux
/dev/sdb7       62922752 83886079 20963328  10G 83 Linux

Command (m for help): t
Partition number (1-3,5-7, default 7): 7
Hex code (type L to list all codes): 82

Changed type of partition 'Linux' to 'Linux swap / Solaris'.

Command (m for help):w
```
2.使用mkswap格式化swap分区
```
➜  ~ mkswap /dev/sdb7
Setting up swapspace version 1, size = 10 GiB (10733219840 bytes)
no label, UUID=6a7ea6d6-5780-4c1e-89ed-945b79b9f163
```
3.使用swapon /dev/sdb7 开启swap分区

##### 查看分区使用情况
free 命令主要是用来查看内存和 swap 分区的使用情况的
```
free
              total        used        free      shared  buff/cache   available
Mem:        4000692     1578368      896140        3828     1526184     2163396
Swap:       2097148           0     2097148
```
total：是指总数；
used：是指已经使用的；
free：是指空闲的；
shared：是指共享的；
buffers：是指缓冲内存数；
cached：是指缓存内存数，单位是KB








## 逻辑卷管理

使用逻辑卷管理的好处：可以动态分配挂载的文件系统的空间
