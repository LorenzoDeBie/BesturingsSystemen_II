#!/bin/bash

frequencies=( )

groups=( )

while IFS=':' read -a lineArr ; do
	(( frequencies[lineArr[3]]+=1 )) 
done < /etc/passwd

while IFS=':' read -a lineArr ; do
	groups[lineArr[2]]=${lineArr[0]}
done < /etc/group

for index in ${!frequencies[@]} ; do
	printf "%b:%b\n" ${groups[index]} ${frequencies[index]}
done
