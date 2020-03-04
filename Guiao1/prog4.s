.data
s1: .asciiz "\nIntroduza um numero (sinal e módulo): "
s2: .asciiz "\nValor lido, em base 2: "
s3: .asciiz "\nValor lido, em base 16: "
s4: .asciiz "\nValor lido, em base 10 (unsigned): "
s5: .asciiz "\nValor lido, em base 10 (signed): "
.text
.globl main
main:
  li $v0, 8
  la $a0, s1    # printStr(s1);
  syscall
  li $v0, 5
  syscall       # value = readInt10();
  move $t1, $v0

  
  li $v0, 8 
  la $a0, s2    # printStr(s2);
  syscall
  
  
  li $v0, 6
  move $a0, $t1
  li $a1, 2
  syscall # printInt(value, 2);
  
  
  
  li $v0, 8 
  la $a0, s3 # printStr(s3);
  syscall
  
  li $v0, 6
  move $a0, $t1
  li $a1, 16123
  syscall # printInt(value, 16);

  li $v0, 8 
  la $a0, s4 # printStr(s4);
  syscall

  
  li $v0, 6
  move $a0, $t1
  li $a1, 10
  syscall # printInt(value, 10);

  li $v0, 8 
  la $a0, s5 # printStr(s5);
  syscall

  li $v0, 6
  move $a0, $t1
  li $a1, 0
  syscall  # printInt10(value);
  j main
