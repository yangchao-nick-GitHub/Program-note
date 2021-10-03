# Shell程序设计

### 变量

##### 变量的使用

定义变量：

```shell
variable=value
variable='value'
variable="value"
variable=`ls -l`		#不支持嵌套
variable=$(ls -l)		#支持嵌套ret=$(echo $(ls -l) | wc -L)
	
```

引用变量：

```shell
echo $variable
echo "I can $skill"		#会解析skill
echo 'I can $skill'		#不会会解析skill
```

删除变量：

```shell
unset variable
```



##### 变量作用域

1.变量默认是全局作用域

2.可以使用local命令在函数中定义局部变量

3.全局变量的作用域在当前进程，对其他进程和它的子进程无效

4.可以使用export命令定义全局变量，使它在子进程中依然有效



##### 特殊变量

```shell
$0		#当前文件名
$n		#位置参数
$#		#参数个数
$*		#所有参数	"$*"被整体作为多份参数
$@		#所有参数	"$@"每个参数彼此独立
$?		#上次命令的退出状态，或者函数返回值
$$		#当前进程pid

```



##### 字符串操作

```shell
 url="https://www.baidu.com"
 echo ${#url}			#获取字符串长度
 echo ${url:3:5}		#从第3个字符串开始截取5个字符
 echo ${url:3}			#从第3个字符串开始截取到末尾
 echo ${url#*//}		#截取字符串中//右边的所有字符
 echo ${url%//*}		#截取字符串中//左边的所有字符


```





### 内置命令

```shell
echo $val		#默认换行输出
echo -n $val	#不换行输出
echo -e $val	#能识别转义字符
```

```shell
read 			#用来从标准输入中读取数据并赋值给变量


#5秒内输入两次密码校验 
read -t 5 -sp "Please enter passwd in 5 seconds(once)   > " passwd1  && printf "\n" &&
read -t 5 -sp "Please enter passwd in 5 seconds(again)  > " passwd2  && printf "\n" &&
  
if [ "$passwd1" == "$passwd2" ]; then
    echo "passwd success"
 else
    echo "passwd Invalid"
 fi
```





### 数学运算符

```shell
#常见用法
echo $((1+1))
echo $((a+10))
echo $((a++))
let a+=6
let a++
```



### 分支语句

##### if else

```shell
#文件判断
-f filename		#判断文件是否存在，井且是否为普通文件
-d filename		#判断文件是否存在，并且是否为目录文件
-r/w/x filename #判断文件是否存在，并且是否有读写执行权限
```

```shell
#数值比较
num1 -eq num2	#判断 num1 是否和 num2 相等(equal)
num1 -ne num2	#判断 num1 是否和 num2 不相等
num1 -gt num2	#判断 num1 是否大于 num2
num1 -lt num2	#判断 num1 是否小于 num2
num1 -ge num2	#判断 num1 是否大于等于 num2
num1 -le num2	#判断 num1 是否小于等于 num2
```

```shell
#字符串比较
-z str			#判断字符串 str 是否为空	
str1 == str2	#=和==是等价的，都用来判断 str1 是否和 str2 相等
```

```shell
#逻辑运算
expression1 -a expression		#逻辑与
expression1 -o expression2		#逻辑或
```

注意：

1.强烈建议if [ -z "$file" ]  而不是没有“”的 if [ -z $file"]  防止出现空字符串报错

2.强烈建议使用[[]]

```shell
#[[]]的好处
[[ -z $str1 || -z $str2 ]]
[[ -z $str1 && -z $str2 ]]
```



##### case in

```shell
read num
case $num in
    1)
        echo "Monday"
        ;;
    2)
        echo "Tuesday"
        ;;
    3)
        echo "Wednesday"
        ;;
    *)
        echo "error"
esac
```

case in与正则表达式的结合：

| 格式  | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| *     | 表示任意字符串。                                             |
| [abc] | 表示 a、b、c 三个字符中的任意一个。比如，[15ZH] 表示 1、5、Z、H 四个字符中的任意一个。 |
| [m-n] | 表示从 m 到 n 的任意一个字符。比如，[0-9] 表示任意一个数字，[0-9a-zA-Z] 表示字母或数字。 |
| \|    | 表示多重选择，类似逻辑运算中的或运算。比如，abc \| xyz 表示匹配字符串 "abc" 或者 "xyz"。 |



##### for in

```shell
#常见for循环用法
for num in nums
do
	...
done

for num in $(seq 1 100)
```



##### select in 

终端交互选择语句

```shell
select name in "Linux" "Windows" "Mac OS" "UNIX" "Android"
do
    case $name in
        "Linux")
            echo "Linux是一个类UNIX操作系统，它开源免费，运行在各种服务器设备和嵌入式设备。"
            break
            ;;
        "Windows")
            echo "Windows是微软开发的个人电脑操作系统，它是闭源收费的。"
            break
            ;;
        *)
            echo "输入错误，请重新输入"
    esac
done
```



### Shell高级功能

##### shell重定向

1.打开文件的流程：文件描述符(进程pcb中)  --->文件指针---->打开文件表(系统全局维护)--->i-node表(系统全局维护)

2.重定向的实质是修改文件描述符对应的文件指针的指向，将其指向需要重定向的另一个文件

3.重定向命令从左向右依次执行

```shell
#默认文件描述符1和stdout绑定，默认文件描述符2和stderr绑定
cat test.txt  1>out.txt		#文件描述符1和out.txt文件绑定
cat test.txt  2>out.txt		#文件描述符2和out.txt文件绑定
cat test.txt  >/dev/null	#文件描述符1和黑洞绑定
cat test.txt  >/dev/null 2>&1	#文件描述符1和黑洞绑定，再将文件描述符1的文件指针赋值给文件描述符2的文件指针，使文件描述符1/2和同一个文件绑定，这里也就是黑洞
```

4.使用exec >log.txt可以将stdout永久重定向到log.txt

5.输入重定向（常见用法使将文本作为while done的输入）

6.内嵌文档重定向here Decument

```shell
command <<END
    document
END

cat <<END
    usage:1...
    	  2...
END

```



shell过滤器命令

```shell
cut -d ":" -f 1 /etc/passwd

grep -q		#禁止输出
grep -n		#输出匹配的行号
grep -v		#输出匹配的行不包括不匹配的行
```

| 命令    | 说明                                                         |
| ------- | ------------------------------------------------------------ |
| awk     | 用于文本处理的解释性程序设计语言，通常被作为数据提取和报告的工具。 |
| cut     | 用于将每个输入文件（如果没有指定文件则为标准输入）的每行的指定部分输出到标准输出。 |
| grep    | 用于搜索一个或多个文件中匹配指定模式的行。                   |
| tar     | 用于归档文件的应用程序。                                     |
| head    | 用于读取文件的开头部分（默认是 10 行）。如果没有指定文件，则从标准输入读取。 |
| paste   | 用于合并文件的行。                                           |
| sed     | 用于过滤和转换文本的流编辑器。                               |
| sort    | 用于对文本文件的行进行排序。                                 |
| split   | 用于将文件分割成块。                                         |
| strings | 用于打印文件中可打印的字符串。                               |
| tac     | 与 cat 命令的功能相反，用于倒序地显示文件或连接文件。        |
| tail    | 用于显示文件的结尾部分。                                     |
| tee     | 用于从标准输入读取内容并写入到标准输出和文件。               |
| tr      | 用于转换或删除字符。                                         |
| uniq    | 用于报告或忽略重复的行。                                     |
| wc      | 用于打印文件中的总行数、单词数或字节数。                     |