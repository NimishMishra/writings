.data

    PI: .float 3.14


.text
.globl main

main:   li $v0, 2     # load a 2 into $v0 to print a float. Co-processor 1 has the registers for floating points, so ...
        lwc1 $f12, PI # load the float PI into the register f12 into Co-processor 1 
        syscall


# so co-processor 1 has the registers to print the floating values