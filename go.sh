#!/bin/sh

make clean
echo "[MAKE]"
make
echo "\n=== Done : press RETURN to run or Ctrl-C to abort ! ===\n"
read junk
./run task && dmesg

