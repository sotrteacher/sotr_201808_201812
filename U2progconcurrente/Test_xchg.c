/* Test_xchg.c */
#define FORCEINLINE  __inline__ __attribute__((__always_inline__))

//FORCEINLINE
void xchg(long *A,long *B)
{
//  __asm__ __volatile__("movl %0, %%eax":"=m"(*A));
//  __asm__ __volatile__("xchgl %%eax, %0":"=m"(*B));
//  __asm__ __volatile__("movl %%eax, %0":"=m"(*A));
  long X;
  __asm__ __volatile__("movl  %0, %%eax":"=m"(A));   // %eax <- A
  __asm__ __volatile__("movl (%eax), %eax");         // %eax <- *A
  __asm__ __volatile__("movl  %0, %%ebx":"=m"(B));   // %ebx <- B
  __asm__ __volatile__("movl (%ebx), %ebx");         // %ebx <- *B
  __asm__ __volatile__("movl %%ebx, %0":"=m"(X));    // X <- *B
  __asm__ __volatile__("xchgl %%eax, %0":"=m"(X));   // %eax <-> X
                                                     // %eax <- *B
                                                     // X <- *A      

  __asm__ __volatile__("movl  %0, %%ebx":"=m"(A));
  __asm__ __volatile__("movl  %eax, (%ebx)");        // *A <- *B, (X=*A) 
  __asm__ __volatile__("movl  %0, %%ebx":"=m"(B));
  __asm__ __volatile__("movl %0, %%eax":"=m"(X));
  __asm__ __volatile__("movl  %eax, (%ebx)");        // *B <- X
}

//FORCEINLINE
void xchg1(long *A,long *B)
{
  long TMP;
  __asm__ __volatile__("movl %0, %%eax":"=m"(A));
  __asm__ __volatile__("movl %0, %%ebx":"=m"(B));
  __asm__ __volatile__("movl (%eax), %eax");        // %eax <- *A
  __asm__ __volatile__("movl (%ebx), %ebx");        // %ebx <- *B
  __asm__ __volatile__("movl %%ebx, %0":"=m"(TMP)); // TMP <- *B
////  __asm__ __volatile__("xchgl %%ebx, %0":"=m"(TMP));
  __asm__ __volatile__("movl %0, %%ebx":"=m"(B));
  __asm__ __volatile__("movl %eax, (%ebx)");        // *B <- *A
  __asm__ __volatile__("movl %0, %%ebx":"=m"(TMP)); // %ebx <- TMP
  __asm__ __volatile__("movl %0, %%eax":"=m"(A));
  __asm__ __volatile__("movl %ebx, (%eax)");        // *A <- TMP
}

int main(int argc,char *argv[])
{
  long a=10,b=20;
  xchg1(&a,&b);
  return a;  /* se devuelve 20? si! */
  //return b;  /* se devuelve 10? si! */
}

/* gcc -g -c Test_xchg.c */
/* gcc -g -o Test_xchg Test_xchg.o */
/* ./Test_xchg */
/* echo $? */
