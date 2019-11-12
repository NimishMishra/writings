.data
    character: .asciiz "n"  # a character needs only a byte
 
.text
.globl main

main:   li $v0, 4     # load immediate to v0
        la $a0, character
        syscall      