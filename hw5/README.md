# 說明
* 寫個簡單的 filesystem (沒 mount 到 device)
* 可以透過 echo 數字 > /input/a 和 echo 數字 >/input/b 來設定a和b的值，數值大小0~255之間
* 可以透過 cat /output/add取得a+b的值，透過cat /output/sub取得a-b的值
  
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
![image](https://github.com/yue0211/Linux-Operation-System-Kernel/assets/82385589/78d6ee8f-3c20-487a-9d67-196249a798d2)

