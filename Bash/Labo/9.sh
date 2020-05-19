#!/bin/bash

exec {input}< <(ps -aux)

# skip header line
read header <&${input}

ptimes=( )
pids=()

while IFS=' ' read user pid ptime rest <&${input} ; do
    [[ "$$" == $pid ]] && continue
    #echo $pid takes $ptime% of processor time
    # loop over the top five processes (can be empty)
    for((i=0;i<5;i++)); do
        # if ptimes[i] == empty || ptime bigger than ptimes[i]
        if [[ -z "${ptimes[i]}" || "$ptime" > "${ptimes[i]}" ]] ; then
            # insert current pid at index $i
            ptimes=( "${ptimes[@]:0:i}" "$ptime"  "${ptimes[@]:i}" )
            pids=( "${pids[@]:0:i}" "$pid"  "${pids[@]:i}" )
            # we found the spot --> go to next line
            break
        fi
    done
done

#echo "${ptimes[@]:0:5}"
echo "${pids[@]:0:5}"