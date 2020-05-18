#!/bin/bash

# use file descriptors so we can read one line at a time ez way
exec {callisto}<callisto.log
exec {ganymedes}<ganymedes.log

# read first line of callisto
read callistoLine <&${callisto}
IFS=' ' read -ra callistoArr <<< "$callistoLine"

# read first line of ganymedes
read ganymedesLine <&${ganymedes}
read -ra ganymedesArr <<< "$ganymedesLine"

# while one of the files still has text
while [[ (! -z "${callistoArr[3]}${callistoArr[4]}" ) || (! -z "${ganymedesArr[3]}${ganymedesArr[4]}") ]] ; do    
    # check which one is earlier
    if [[ "${callistoArr[3]} ${callistoArr[4]}" > "${ganymedesArr[3]} ${ganymedesArr[4]}" ]] ; then
        # callisto is earlier --> print callisto line
        echo callisto $callistoLine
        # read next line from callisto
        read callistoLine <&${callisto}
        IFS=' ' read -ra callistoArr <<< "$callistoLine"
    else
        # ganymedes is earlier
        echo ganymedes $ganymedesLine
        # read next line of ganymedes
        read ganymedesLine <&${ganymedes}
        IFS=' ' read -ra ganymedesArr <<< "$ganymedesLine"
    fi
done

# close the file descriptors
exec {callisto}<&-
exec {ganymedes}<&-