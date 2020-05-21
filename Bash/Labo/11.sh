#!/bin/bash

# options indicating what results should be shown on screen
success=0
rejected=0
night=0
# default file= dchp.log, can be overridden with -f filename
file="dhcp.log"

while getopts ':srnf:' option ; do
    case "$option" in
        s)  success=1
            ;;
        r)  rejected=1
            ;;
        n)  night=1
            ;;
        f)  file="$OPTARG"
            ;;
        :)  printf "Missing arugment for -$OPTARG\n" >&2 && exit 1;
            ;;
        ?)  printf "usage: %s [-s] [-r] [-n] [-f file]\n" "${0##/*}" >&2 && exit 1;
            ;;   
    esac
done

# check if file exists && is readable with passing error messages
[[ -f "$file" ]] && { [[ -r "$file" ]] || { echo "Cannot read from $file"; exit 1; } >&2 ;} || { echo "$file does not exist!"; exit 1 ;} >&2

# no options given -> print everything
if (( !success && !rejected && !night )) ; then success=1 ; rejected=1 ; night=1; fi

echo success: $success, rejected: $rejected, night: $night >&2
echo "Now processing $file" >&2

