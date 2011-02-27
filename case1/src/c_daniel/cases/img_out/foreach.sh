#!/bin/bash

if [ -z "$1" -o -z "$2" ]
then
	echo "Usage: $0 filetype 'command'"
	exit
fi

for i in `ls *$1`
do
	$2 $i
done

