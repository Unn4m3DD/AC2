
.data
msg: .asciiz "a"
.text
.globl main
main:
  li $v0, 2
  syscall
  sb $v0, msg
  la $a0, msg
  li $v0, 8
  syscall
  bne $a0, 10, main
  li $v0, 0
  j $ra
