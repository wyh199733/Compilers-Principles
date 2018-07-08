.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra


main:
  li $t0, 1
  li $t1, 2
  li $t2, 3
  addi $t3, $t0, 1
  move $t4, $t3
  addi $t5, 1, $t0
  move $t4, $t5
  addi $t6, 1, 2
  move $t4, $t6
  add $t7, $t0, $t1
  move $t4, $t7
  addi $s0, $t0, -1
  move $t4, $s0
  addi $s1, 1, -$t0
  move $t4, $s1
  addi $s2, 1, -2
  move $t4, $s2
  sub $s3, $t0, $t1
  move $t4, $s3
  mul $s4, $t0, 1
  move $t4, $s4
  mul $s5, 1, $t0
  move $t4, $s5
  mul $s6, 1, 3
  move $t4, $s6
  mul $s7, $t0, $t2
  move $t4, $s7
  div 1, $t0
  mflo $t8
  move $t4, $t8
  div $t0, 1
  mflo $t9
  move $t4, $t9
  div 2, 1
  mflo $t3
  move $t4, $t3
  div $t2, $t1
  mflo $t5
  move $t4, $t5
  add $t6, $t0, $t1
  move $t4, $t6
