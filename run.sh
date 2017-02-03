#!/bin/sh

cat <<-EOF

Vous pouvez lancer tout module dont vous avez precise le nom (sans le .o en parametre
Type <return> to install modules, then <return> again when you want to stop it
EOF

read junk
sudo insmod /usr/lib/realtime/modules/rtai_hal.ko
sudo insmod /usr/lib/realtime/modules/rtai_sched.ko
sudo insmod /usr/lib/realtime/modules/rtai_sem.ko

sudo insmod $1.ko

echo "Program is running"
echo "Type <return> to remove modules"
read junk

sudo rmmod $1
sudo rmmod rtai_sem
sudo rmmod rtai_sched
sudo rmmod rtai_hal

