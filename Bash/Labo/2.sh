#!/bin/bash

# check if we have arguments
if (( $# < 1 ))
then
	printf "%b" "Give one or more numbers to convert!\n"
	exit 1
fi

# array containing the numbers and their roman counterpart
roman_rev=( [1000]=M [900]=CM [500]=D [400]=CD [100]=C [90]=XC [50]=L [40]=XL [10]=X [9]=IX [5]=V [4]=IV [1]=I )
# array stores the indices of roman_rev in ascending order
indices=( ${!roman_rev[@]} )

# convert each number we got as arugment
while (( $# > 0 ))
do
	# var to store the number so we can do arithmetic calculations on it
	number=$1
	
	# loop over all the roman symbols in descending order by using the indices array
	for (( i=${#indices[@]}-1; i >= 0; i-- ))
	do
		# print the roman symbol as long as number is bigger than that symbol
		while (( ${indices[i]} <= $number ))
		do
			printf "%b" ${roman_rev[indices[i]]}
			# substract the roman value from this number
			number=$(( $number - ${indices[i]} ))
		done
	done
	# newline after each number
	echo
	# shift the number out of the parameter array
	# this also decreases $# by one so the while loop will end eventually
	shift
done
