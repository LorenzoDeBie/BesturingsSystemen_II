#!/bin/bash

exit_handler(){
	# now i can use the associative array
	declare -n results=$1

	# check if results exists || exit
	[[ -v results[@] ]] || echo "no results yet, let the script run longer you dummy" >&2

	# loop over the results we have until now
	for key in ${!results[@]} ; do
		# print "value : key"
		echo "${results[$key]}: $key"
		# sort output by first field numerically in descending order and only show first 10 lines
	done | sort -rn -k 1,1| head -n 10

	# end line with summarized data
	echo $(( (count*100) / total )).$(( ((count*1000) / total) % 10 ))% complete
	# clean up the tmp directory
	rm -rf $2

	# go back to original directory
	cd - &>/dev/null
	exit 0
}

# a: make temp directory
DIRECTORY=$( mktemp -d )

unzip iis-logs.zip -d $DIRECTORY &>/dev/null
cd $DIRECTORY

read total ee < <(wc -l *|grep total)
count=0
trap 'exit_handler requests "$DIRECTORY"' SIGINT SIGHUP

declare -A requests=()

# loop over all the files in the directory
for file in * ; do
	# read al the lines
	while read line ; do
		# beter met regex
		# this line is only a request when the first char is not '#'
		if [[ "${line::1}" != "#" ]] ; then
			# split request by spaces, url is at index 4
			lineArr=($line)
			# add 1 to the count of this request
			(( requests[${lineArr[4]}]+=1 ))
		fi
		(( count++ ))
	done < $file
done

exit_handler requests "$DIRECTORY"