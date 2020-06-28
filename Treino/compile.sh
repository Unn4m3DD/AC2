pcompile $1.c 
ldpic32 -p /dev/ttyS4 $1.hex
rm *.hex;
rm *.elf;
rm *.map;
rm *.o;
pterm -p /dev/ttyS4