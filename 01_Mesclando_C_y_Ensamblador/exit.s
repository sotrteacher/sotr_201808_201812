.extern idle_fn
.globl funcion
.globl funcion_exit

funcion_exit:
  pushl %ebp
  movl %esp,%ebp
  movl 8(%ebp),%ebx
  movl $1,%eax
  int $0x80

funcion:
  pushl %ebp
  movl %esp,%ebp

  pushl $2
  pushl $1
  call idle_fn

  movl %ebp,%esp
  popl %ebp
  ret

