#!/bin/bash

gameboard=( NTOKEZ CDEPAM LUNPET MOARSI DONSTE HIEFES RESHCA BQMJOD TIALNB GYNJEU ZAEDNV AOIEAW HSIREN GENVIT WEGLUR RIAFXK )

# Number of collumn to print
COLS=4

# COUNT checks when to insert a newline 
COUNT=0

# loop over all the indices of gameboard in random order
for index in $( shuf -i 0-$((${#gameboard[@]}-1)) )
do
	# Prints substring of length 1 using parameter expansion
	# the start of the substring is calculated with shuf
	# since the length of all strings is 5 we can hardcode it
	echo -n "${gameboard[$index]:$( shuf -i 0-5 -n1 ):1}"
	# use printf to print tab because echo -e is not POSIX compliant
	printf "\t"
	# print newline every 4 words, so we have a nice table
	if (( ($COUNT+1)%$COLS==0 ))
	then
		echo ""
	fi

	COUNT=$(( COUNT+1 )) 
done
