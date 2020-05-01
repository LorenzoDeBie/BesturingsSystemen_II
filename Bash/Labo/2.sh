#!/bin/bash

if (( $# < 1 ))
then
	printf "%b" "Give one or more numbers to convert!\n"
	exit 1
fi

roman_rev=( [1000]=M [900]=CM [500]=D [400]=CD [100]=C [90]=XC [50]=L [40]=XL [10]=X [9]=IX [5]=V [4]=IV [1]=I )

while (( $# > 0 ))
do

	number=$1

	indices=( ${!roman_rev[@]} )
	for (( i=${#indices[@]}-1; i >= 0; i-- ))
	do
		while (( ${indices[i]} <= $number ))
		do
			printf "%b" ${roman_rev[indices[i]]}
		number=$(( $number - ${indices[i]} ))
		done
	done
	echo
	shift
done
