pcompile $1.c
ldpic32 $1.hex -p /dev/ttyS4
rm *.hex
rm *.elf
rm *.map
pterm -p /dev/ttyS4
