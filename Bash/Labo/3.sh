#!/bin/bash

frequencies=( )

while IFS=':' read -a lineArr ; do
	(( frequencies[lineArr[3]]+=1 )) 
done < /etc/passwd

for index in ${!frequencies[@]} ; do
	printf "%b:%b\n" $index ${frequencies[index]}
done
