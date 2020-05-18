#!/bin/bash

# this version is with external commands but I think it is the easiest to make

# a: make temp directory
DIRECTORY=$( mktemp -d )

# unzip in that tmp dir
unzip iis-logs.zip -d $DIRECTORY &>/dev/null

# go to that dir
cd $DIRECTORY

# loop over all the files
for file in $( ls $DIRECTORY ) ; do
    # read all the lines
	while read line ; do
    # we have a request when the first char is not '#'
		if [[ "${line::1}" != "#" ]] ; then
			lineArr=($line)
            # print the field which contains the url
			echo ${lineArr[4]}
		fi
	done < $file
    # sort all the requests | print count of each request | sort in reverse and numerical | show first 10 lines
done | sort | uniq -c | sort -rn | head -n 10

# I clean up after myself
rm -rf $DIRECTORY
