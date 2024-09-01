#!/bin/bash

mkdir d > /dev/null 2>&1
for i in {0..255}
do
    hex=$(printf "%02X" "$i")
    printf "\x$hex" > d/"data_${hex}.dat"
done
