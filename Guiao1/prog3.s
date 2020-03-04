
.data
msg: .asciiz "a"
.text
.globl main
main:
  li $v0, 1
  syscall
  bne $v0, 0, endif
  li $v0, '0'
endif:
  sb $v0, msg
  la $a0, msg
  li $v0, 8
  syscall
  bne $a0, 10, main
  li $v0, 0
  j $ra
