#!/bin/bash

FILE=$1
for word in `cat $FILE`
do
if [ ${#word} -lt 8];then
	echo ${#word} $word
 fi
done 

