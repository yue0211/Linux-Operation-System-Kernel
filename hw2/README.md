# 說明
* 使用 kernel 版本 3.19.0，分別編譯並載入 hello 與 hellop 這兩個 kernel module。
* 另外撰寫一個 hellop3，可傳入整數 a 與整數 b 和字串 c ，在載入時透過 printk 印出 a*b 次的 c 字串。
* 以下列指令，依序執行 :
  * uname -a
  * insmod hello.ko
  * insmod hellop.ko howmany=2 whom="IIS"
  * insmod hellop3.ko a=3 b=2 c=mytext
  * lsmod | grep hello
  * rmmod hellop
  * rmmod hello
  * rmmod hellop3
  * lsmod | grep hello 

# 補充 
* 以下命令，需要切成 root
  * insmod (載入 kernel module)
  * rmmod (移除 kernel module )
  * lsmod (列出所有的 kernel module)
* cscope 指令，可用來快速查找**設定的目錄下，所有檔案的關鍵字**。
  * find . -type f -name "*.[chxsS]" > cscope.files
  * cscope -b -q -k
  * cscope -p4 (每次要找關鍵字時，在設定的目錄下打此指令)
  * ^d -- 退出介面。
  * ^f -- 繼續找關鍵字。
  * 空白鍵 -- 下一頁。
