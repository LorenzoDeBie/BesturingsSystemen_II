#!/bin/bash

# options indicating what results should be shown on screen
successFlag=0
rejectedFlag=0
nightFlag=0
# default file= dchp.log, can be overridden with -f filename
file="dhcp.log"

# see bash cookbook 13.1 && 13.2: Parsing arguments
while getopts ':srnf:' option ; do
    case "$option" in
        s)  successFlag=1
            ;;
        r)  rejectedFlag=1
            ;;
        n)  nightFlag=1
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
if (( !successFlag && !rejectedFlag && !nightFlag )) ; then successFlag=1 ; rejectedFlag=1 ; nightFlag=1; fi

echo success: $successFlag, rejected: $rejectedFlag, night: $nightFlag >&2
echo "Now processing $file" >&2

# 1) skip header lines
# 2) I only care about events 10, 11, 15:
#   10: New lease
#   11: Lease has been renewed
#   15: Lease has been denied

# key=datestring value=:lease1:lease2:lease3 all leases are in format: "time hostname mac"
declare -A success
declare -A renewed
denied=0

# open the file using file descriptors
exec {file}< $file

# skip all the header lines
for ((i=0;i<28;i++)) ; do read line <&$file; done

while IFS=',' read -a line <&$file ; do
    # lease is given
    if (( successFlag && line==10 )) ; then
        echo length: ${#line[@]}, first: ${line[0]} >&2
        # append this lease
        success["${line[1]}"]="${success[${line[1]}]},${line[2]} ${line[5]} ${line[6]}"
    # lease is rejected
    elif (( rejectedFlag && line==15 )) ; then
        echo length: ${#line[@]}, first: ${line[0]} >&2
        # increment the denied counter
        (( denied++ ))
    # lease is renewed
    # Wikipedia says nighttime is usually 9pm - 4am
    elif (( nightFlag && line==11)) && [[ "${line[2]}" > "21:00:00" || "${line[2]}" < "04:00:00" ]] ; then
        echo length: ${#line[@]}, first: ${line[0]}, time: ${line[2]} >&2
        # append this renew
        renewed["${line[1]}"]="${renewed[${line[1]}]},${line[2]} ${line[5]} ${line[6]}"
    fi
done

# now show output
if (( successFlag )); then
    echo "These hosts got a lease:"
    # loop over all the dates
    for date in "${!success[@]}" ; do
        # show the date
        echo "$date:"
        # split all the leases on the delimiter ',' and print each of them on a line and indented
        while read -d ',' lease ; do
            printf "\t%s\n" "$lease"
        done <<< ${success[$date]:1}
        # in the line above the ':1' in the var name skips the first char
        # the first char will always be a ',' because of we appended above
    done
fi
if (( rejectedFlag )); then
    # show how many hosts were denied
    printf "$denied hosts were denied an IP adress\n"
fi
if (( nightFlag )); then
    printf "These hosts renewed their lease between 9pm and 4 am"
    # loop over all the dates
    for date in "${!renewed[@]}" ; do
        # show the date
        echo "$date:"
        # split all the leases on the delimiter ',' and print each of them on a line and indented
        while read -d ',' lease ; do
            printf "\t%s\n" "$lease"
        done <<< ${renewed[$date]:1}
        # in the line above the ':1' in the var name skips the first char
        # the first char will always be a ',' because of we appended above
    done
fi

