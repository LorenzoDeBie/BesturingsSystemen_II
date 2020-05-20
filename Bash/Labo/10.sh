#!/bin/bash

echo "Redirect STDERR to /dev/null to disable DEBUG output" >&2

exec {input}< <(ps -ef)
# skip header line
read header <&${input}

# array: key=pid value=chilpid1:chilpdi2:...
tree=()
# array: key=pid value=command
cmds=()

# parse output of ps -ef
while read uid pid ppid c stime tty time cmd <&${input}; do
    cmds[$pid]="$cmd"
    tree[$ppid]="${tree[ppid]}:$pid"
    echo $pid $cmd >&2
done

print_all_children() {
    # $1 = ppid
    # $2 = start level of indentation
    printf "f with ppid= $1 and indent= $2\n" >&2
    # print indentation
    for ((i=0;i<$2;i++)) ; do
        printf "    "
    done
    # print out the command
    echo "${cmds[$1]}"
    # read all the child commands (skip the first ':')
    IFS=':' read -ra cpids <<< ${tree[$1]:1}
    # stopcondition for recursion: no children
    (( ${#cpids[@]} == 0 )) && echo "no children!" >&2 && return 0;
    echo "${#cpids[@]} children: ${cpids[@]}" >&2
    # loop over all the children
    for cpid in "${cpids[@]}" ; do
        echo $cpid >&2
        # print this child and its children
        print_all_children "$cpid" "$(( $2 + 1))"
    done
}

echo nu output >&2

# start with pid 1, recursion does the rest
print_all_children 1 0