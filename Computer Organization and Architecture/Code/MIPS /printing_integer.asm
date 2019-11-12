.data

    integer: .word 45    # word = 32 bits, so 4 bytes to store an integer

.text
.globl main

main:   li $v0, 1         # loading a 1 implies we will be printing an integer
        lw $a0, integer   # instead of loading an address, we are now loading a word
        syscall