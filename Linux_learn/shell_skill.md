循环读取每一行
while read line
do
    echo "$line"
done < $FILE

循环读每一行的单词
for word in `cat $FILE`
do
echo "$word"
done

得到字符串长度
${#word}

累加进程内存之和
while read line
do
        size=`echo $line | awk '{print $6}'`
        echo $((num=num+size))
done < $1

echo "toalt size $num"


统计单词出现的次数并排序
cat $1 | tr -s ' ' '\n' | sort | uniq -c | sort | awk '{print $2 " " $1}'
tr -s ' ' '\n'  将空格替换成回车
uniq -c     统计重复出现的行的次数
sort  排序


第二列是否有重复,打印重复出现的行
cat 456.txt  | awk '{print $2}' | sort | uniq -c | grep -v 1


删除包含go字符串的行
sed "/go/d" 456.txt
