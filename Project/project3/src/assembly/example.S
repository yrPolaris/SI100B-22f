.section .data
message1:
	.asciz "Flappy"
message2:
	.asciz "Bird"
message3:
	.asciz "Welcome"

.section .text
.globl welcome
.type welcome, @function
welcome:
	li a0, 15
	li a1, 40
	la a2, message1
	li a3, 65535
	addi sp,sp,-4
	sw ra,0(sp)
	jal LCD_ShowString 	
	lw ra,0(sp)
	addi sp,sp,4

	li a0, 25
	li a1, 60
	la a2, message2
	li a3, 65535
	addi sp,sp,-4
	sw ra,0(sp)
	jal LCD_ShowString 	
	lw ra,0(sp)
	addi sp,sp,4

	li a0, 10
	li a1, 100
	la a2, message3
	li a3, 65535
	addi sp,sp,-4
	sw ra,0(sp)
	jal LCD_ShowString 	
	lw ra,0(sp)
	addi sp,sp,4

	addi sp,sp,-4
	sw ra,0(sp)
	jal key_check	
	lw ra,0(sp)
	addi sp,sp,4
	ret



