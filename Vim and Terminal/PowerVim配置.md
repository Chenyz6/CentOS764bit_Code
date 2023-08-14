## PowerVim安装步骤

GitHub链接：[youngyangyang04/PowerVim: Make your vim more power and much easer. (github.com)](https://github.com/youngyangyang04/PowerVim)

1. git clone https://github.com/youngyangyang04/PowerVim.git

2. cd PowerVim

3. sh install.sh

直接执行 sh install.sh会报install.sh: 8: install.sh: Syntax error: "(" unexpected

4. sudo dpkg-reconfigure dash   					  	需将sh指向bash可正常运行脚本    选择no



执行完脚本后使用vim会报如下错误：
处理 /home/wsy/.vimrc 时发生错误:
第 42 行:
E197: 不能设定语言为 "zh_CN.gb2312"



ubuntu中文支持，及中文乱码问题

状况：所用的ubuntu系统不支持中文，遇见中文就????。ORZ…

目标：使系统/服务器支持中文，能够正常显示。

首先，安装中文支持包language-pack-zh-hans：

5. sudo apt-get install language-pack-zh-hans
   然后，修改/etc/environment（在文件的末尾追加）：

LANG="zh_CN.UTF-8"
LANGUAGE="zh_CN:zh:en_US:en"

6. 再修改/var/lib/locales/supported.d/local(没有这个文件就新建，同样在末尾追加)：

en_US.UTF-8 UTF-8
zh_CN.UTF-8 UTF-8
zh_CN.GBK GBK
zh_CN GB2312
最后，执行命令：

7. sudo locale-gen
   对于中文乱码是空格的情况，安装中文字体解决。

8. sudo apt-get install fonts-droid-fallback ttf-wqy-zenhei ttf-wqy-microhei fonts-arphic-ukai fonts-arphic-uming