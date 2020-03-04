.equ inKey, 1
.equ putChar, 3
.equ printInt, 6

.globl main
main:
  li $s1, 0 # int cnt = 0;
  li $s2, 0 # char c;
do:
  li $v0, putChar
  li $a0, '\r'
  syscall    # putChar('\r'); // Carriage return character

  li $v0, printInt
  move $a0, $s1
  li $a1, 0x0003000A
  syscall  # printInt( cnt, 10 | 3 << 16 ); // 0x0003000A: decimal w/ 3 digits

  
  li $v0, putChar
  li $a0, '\t'
  syscall    # putChar('\t'); // Tab character

  
  li $v0, printInt
  move $a0, $s1
  li $a1, 0x00080002
  syscall  # printInt( cnt, 2 | 8 << 16 ); // 0x00080002: binary w/ 8 bits

  li $a0, 5
  jal wait # wait(5);

  li $v0, inKey
  syscall
  move $s2, $v0  # c = inkey();

  bne $s2, '+', else  # if( c == '+' )
  addi $s1, $s1, 1 # (cnt + 1)
else:
  bne $s2, '-', endIf # if( c == '-' )
  addi $s1, $s1, -1 # (cnt - 1)
endIf:
  andi $s1, $s1, 0xFF

j do 

wait: 
  li $t0, 0       #int i;
  li $t1, 515000
  mul $t1, $t1, $a0 
for:
  bge $t0, $t1, endFor
  addi $t0, $t0, 1
  j for
endFor:
  jr $ra
# void wait(int ts)
# {
# 
# for( i=0; i < 515000 * ts; i++ );
# } 
