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


