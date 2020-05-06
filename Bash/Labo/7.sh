#!/bin/bash

# a: make temp directory
DIRECTORY=$( mktemp -d )

unzip iis-logs.zip -d $DIRECTORY &>/dev/null
cd $DIRECTORY

declare -A requests=()

for file in $( ls $DIRECTORY ) ; do
	while read line ; do
		if [[ "${line::1}" != "#" ]] ; then
			lineArr=($line)
			#[[ -v "requests[${lineArr[4]}]" ]] && echo 1 || echo 0
			[[ -v "requests[${lineArr[4]}]" ]] && let requests[${lineArr[4]}]+=1 || requests[${lineArr[4]}]=1
			#echo "${requests[${lineArr[4]}]}"
		fi
	done < $file
done

for key in ${!requests[@]} ; do
	printf "%b: %b\n" $key ${requests[$key]}
done

# I clean up after myself
rm -rf $DIRECTORY
