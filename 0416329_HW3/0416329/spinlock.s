
.text
.global spin_lock
.global spin_unlock

spin_lock:
	movl $1,%ebx
	loop:
		mov $0,%rax
		lock cmpxchgl %ebx,(%rdi)
		jnz loop
	ret

spin_unlock:
	movl $0,(%rdi)
	ret
