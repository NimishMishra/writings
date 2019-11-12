.data    # has all data/variables/information etc etc.

    message: .asciiz  "Hello world\n"               # asciiz : stores a string with a null terminator

.text   # instructions to manipulate that data
.globl main
    
    
main:   li $v0, 4       # getting the system to be ready for printing
        # currently my message is in memory. So I need to load it
        la $a0, message            # la = load address
        syscall            # execute the things I have told you to