#!/bin/bash
tput clear
read -p "enter a filename:" filename
if [ -f $filename ]
then
	filesize=`ls -l $filename | awk '{print $5}'`
	echo $filesize
else
	echo "Error!"
	exit 0
fi
filesizepre=`ls -l $filename | awk '{print $5}'`
filesizenow=`ls -l $filename | awk '{print $5}'`
change=0
unchange=0
while [ $change -le 1 -a $unchange -le 9 ]
do
	filesizenow=`ls -l $filename | awk '{print $5}'`
	if [ $filesizepre == $filesizenow ]
	then
		unchange=$[$unchange+1]
		echo "unchange=$unchange"
	else
		filesizepre=$filesizenow
		change=$[$change+1]
		echo "file $filename size changed"
		echo "change=$change"
	fi
	sleep 5
done;
tput clear
exit 0


