#!/bin/bash

fact() {
	if (( $1 <= 1 )) ; then
		echo 1
	else
		echo $(( $1 * $( fact $(( $1 - 1 ))) ))
	fi
}

if (( $# != 1)) ; then
	echo "Script expects only one parameter!"
	exit 1
fi

fact $1
