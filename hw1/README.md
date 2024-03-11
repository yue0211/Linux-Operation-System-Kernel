# 說明
* 安裝任一個 Linux 發行版本。
* 根據所安裝的 Linux 系統中的 kernel 版本，下載相同 major 版本的 source code 並編譯安裝。
* 此作業 kernel 版本，從 3.13 升級成 3.19。

# Operating System
* https://old-releases.ubuntu.com/releases/14.04.0/ubuntu-14.04-desktop-amd64.iso

# Command
* git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
* git reset --hard v3.19
* make config
* make menuconfig
* make oldconfig
* make -j $(nproc)
* make -j32
* make modules_install
* make install
