#!/bin/bash

gameboard=( NTOKEZ CDEPAM LUNPET MOARSI DONSTE HIEFES RESHCA BQMJOD TIALNB GYNJEU ZAEDNV AOIEAW HSIREN GENVIT WEGLUR RIAFXK )

# loop over all words
for index in ${!gameboard[@]}
do
	# Prints substring of length 1 using parameter expansion
	# the start of the substring is calculated with shuf
	# since the length of all strings is 5 we can hardcode it
	echo -n "${gameboard[$index]:$( shuf -i 0-5 -n1 ):1}"
	# use printf to print tab because echo -e is not POSIX compliant
	printf "\t"
	# print newline every 4 words, so we have a nice table
	if (( (index+1)%4==0 ))
	then
		echo ""
	fi
done

