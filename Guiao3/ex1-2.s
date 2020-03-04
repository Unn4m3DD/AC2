.equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area

.equ TRISE, 0x6100 # TRISE address is 0xBF886100
.equ PORTE, 0x6110 # PORTE address is 0xBF886110
.equ LATE, 0x6120 # LATE address is 0xBF886120

.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ LATB, 0x6060 

.globl main

.text
main:
  lui $t2, SFR_BASE_HI

  lw $t0, TRISE($t2)
  and $t0, $t0, 0xFFF0  #setar out do E
  sw $t0, TRISE($t2)

  li $t3, 1
while:
  lw $t1, LATE($t2)
  
  beq $t3, 0, else
  or $t1, $t1, 0x000f
  
  j endif
else:
  and $t1, $t1, 0xFFF0
endif:
  sw $t1, LATE($t2)


  li $a0, 50
  jal delay
  xori $t3, $t3, 0x0001

  j while
  li $v0, 0
  jr $ra



delay:
  li $v0, 12
  syscall
while_delay:
  li $v0, 11
  syscall
  blt $v0, 20000, while_delay
  addi $a0, $a0, -1
  bge $a0, 0, delay
  jr $ra