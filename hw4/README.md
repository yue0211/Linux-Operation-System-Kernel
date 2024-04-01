# 說明
* 做一個自己的 syscall
* 使用 mysyscalltest 測試 自己的syscall。
  * 測試範例 : ./mysyscalltest [學號]

# 步驟
* 在 kernel 的目錄下 (ex: /usr/src/linux-3.19/)，新增名稱為 mysyscall 的目錄
* 將 Makefile、mysyscall.c ，放到 mysyscall 目錄內。
* 修改 /usr/src/linux-3.19/Makefile 的內容:
  * 修改此行: core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ mysyscall/ 
* 修改 /usr/src/linux-3.19/arch/x86/syscalls/syscall_32.tbl 的內容。 (註冊自己的syscall)
  * 新增此行: 400    i386    mysyscall    sys_mysyscall  
* 修改 /usr/src/linux-3.19/arch/x86/syscalls/syscall_64.tbl 的內容。(註冊自己的syscall)
  * 新增此行: 400    common    mysyscall    sys_mysyscall
* rebuild kernel :
  * sudo make -j8
  * sudo make modules install
  * sudo make install
