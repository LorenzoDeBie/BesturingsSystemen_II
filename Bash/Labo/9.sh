#!/bin/bash

while IFS=' ' read user pid ptime rest ; do
    printf "user: %s pid: %s ptime: %s\n" $user $pid $ptime
done < <(ps -aux)