.orig x3000
;;halt
fibonacci
; set up book-keeping
ADD R6, R6, -1 ;allocate space for return value
ADD R6, R6, -1
STR R7, R6, 0 ;save old return address
ADD R6, R6, -1
STR R5, R6, 1 ;save old frame pointer
ADD R5, R6, -1 ;set current frame pointer to point at first local variable
; set up local variables
ADD R6, R6, -1 ;allocate enough space on stack to hold all local variables (just 1 in this case)
; start the behavior of fibonacci
LDR R0, R5, 4 ;R0 <= n (4 is always the offset from R5 of the first arg)
;base case
ADD R2, R0, -1
BRnz BASE
;recursive case
ADD R6, R6, -1 ;R6 will now point to one above R5
ADD R1, R0, -1 ;getting n-1
STR R1, R6, 0 ;store (n-1) at first parameter
JSR fibonacci ;fib(n-1)
LDR R1, R6, 0 ;when the subroutine ends, R6 will point to the return value
ADD R6, R6, 1 ;pop return value into R1
ADD R6, R6, 1 ;pop arg off stack
ADD R2, R0, -2 ;getting n-2
ADD R6, R6, -1 ;
STR R2, R6, 0 ;store (n-2) at first parameter
JSR fibonacci ;fib(n-2)

LDR R2, R6, 0 ;when the subroutine ends, R6 will point to the return value
ADD R6, R6, 1 ;pop return value into R2
ADD R6, R6, 1 ;pop arg off stack
ADD R3, R1, R2 ;have r3 hold the values of fib(n-1) + fib(n-2)
STR R3, R5, 3 ;store the return value in the return value slot
BR STACK_BREAKDOWN
BASE
STR R0, R5, 3 ;store the return value in the return value slot
BR STACK_BREAKDOWN
;;end the behavior of fibonacci
;break down the stack
STACK_BREAKDOWN
ADD R6, R5, 1 ;pop locals (disregard their values)
LDR R5, R6, 0
ADD R6, R6, 1 ;pop old frame pointer (R5)
LDR R7, R6, 0
ADD R6, R6, 1 ;restore return address (R7)
RET
;leave stack pointer pointing at return value
STACK .fill xF000
.end