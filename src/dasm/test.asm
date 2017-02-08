;simple tests
push 6
pop r0

;Test mov to
mov r0, 0
mov [r1], 2
mov [0], 3

;Test mov from
mov r2, [r3]
mov sp, [0]

;End it
hlt
