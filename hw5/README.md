# 說明
* 寫個簡單的 filesystem (沒 mount 到 device)

# 步驟 (先切到 root )
* 寫一個 myfs.c ，並修改 Makefile，執行 make，得到 myfs.ko。
* insmod myfs.ko
* lsmod | grep myfs
* mount -t myfs loop /mnt
* mount (需看到 /mnt 的掛載資訊)
* echo 3 >/mnt/input/a
* cat /mnt/input/a
* echo 2 > /mnt/input/b
* cat /mnt/input/b
* cat /mnt/output/add
* cat /mnt/output/sub
* cat /mnt/output/add
* cat /mnt/output/sub

# /mnt 下的檔案結構
---
/--+ input (dir)
   |   |
   |   +-- a (file)
   |   +-- b (file)
   |
   + output (dir)
       |
       +-- add (file)
       +-- sub (file)
