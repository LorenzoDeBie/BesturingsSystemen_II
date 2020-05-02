#!/bin/bash

if (( $# < 1 )) ; then
	echo "Please supply one or more strings to reverse!"
	exit 1
fi

reverseWords() {	
	while IFS=' ' read -a lineArr ; do
		for (( i=( ${#lineArr[@]} - 1 ) ; i >= 0 ; i-- )) ; do
			printf "%b " ${lineArr[i]}
		done
		echo
	done < <( echo $1 )
}

while (( $# >= 1 )) ; do
	reverseWords "$1"
	shift
done
