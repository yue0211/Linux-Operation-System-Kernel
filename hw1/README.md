# 說明
* 安裝任一個 Linux 發行版本。
* 根據所安裝的 Linux 系統中的 kernel 版本，下載相同 major 版本的 source code 並編譯安裝。
* 此作業 kernel 版本，從 3.13 升級成 3.19。

# Operating System
* https://old-releases.ubuntu.com/releases/14.04.0/ubuntu-14.04-desktop-amd64.iso

# Command
* 下載所有版本的 kernel source code
  * git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
  * git reset --hard v3.19
* 下載對應版本的 kernel source code
  * [連結](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/refs/?h=v3.19)
* make config
* make menuconfig
* make oldconfig
* make -j $(nproc)
* make modules_install
* make install

# Update
![image](https://github.com/yue0211/Linux-Operation-System-Kernel/assets/82385589/4c3b3340-69a4-4f67-bce6-25d423b51432)

將 **3.19 的 kernel source code**，裝在**虛擬機的 /usr/src**。
