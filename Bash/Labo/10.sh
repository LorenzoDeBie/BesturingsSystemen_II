#!/bin/bash

echo "Redirect STDERR to /dev/null to disable DEBUG output" >&2

exec {input}< <(ps -ef)
read header <&${input}

# array: key=pid value=chilpid1:chilpdi2:...
tree=()
cmds=()

while read uid pid ppid c stime tty time cmd <&${input}; do
    cmds[$pid]="$cmd"
    tree[$ppid]="${tree[ppid]}:$pid"
    echo $pid $cmd >&2
done

print_all_children() {
    # $1 = ppid
    # $2 = start level of indentation
    printf "f with ppid= $1 and indent= $2\n" >&2
    for ((i=0;i<$2;i++)) ; do
        printf "    "
    done
    echo "${cmds[$1]}"
    IFS=':' read -ra cpids <<< ${tree[$1]:1}
    (( ${#cpids[@]} == 0 )) && echo "no children!" >&2 && return 0;
    echo "${#cpids[@]} children: ${cpids[@]}" >&2
    for cpid in "${cpids[@]}" ; do
        echo $cpid >&2
        print_all_children "$cpid" "$(( $2 + 1))"
    done
}

echo nu output >&2

print_all_children 1 0