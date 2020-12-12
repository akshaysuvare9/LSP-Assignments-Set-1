#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>

typedef unsigned int u32;
typedef long unsigned int u64;


#define ADDR_FMT "%lx"
#if __x86_64__   /* 64-bit; __x86_64__ works for gcc */
 #define ADDR_TYPE u64
 static u64 invalid_kaddr = 0x456fff7ebcadedaaL;
#else
 #define ADDR_TYPE u32
 static u32 invalid_kaddr = 0x456fff7bL;
#endif


void segv();
char *str;
void my_segvhandler(int signum, siginfo_t * si, void *ucontext)
{
	int flag=0;
		printf("%s:received signal %d.errno=%d\n""Cause: (si_code=%d): ",__func__, signum, si->si_errno, si->si_code);

		/*Further various cases that generate segmentation faults can be added */
		switch (si->si_code) {
				    case SI_USER:
					printf("user\n");
					break;
					case SI_KERNEL:
					flag=1;
					printf("Here kernel\n");
					break;
			case SEGV_MAPERR:
						printf("\nSEGV_MAPERR: address not mapped to object\n");
						break;
				case SEGV_ACCERR:
						printf("\nSEGV_ACCERR: invalid permissions for mapped object\n");
						break;
				default:
						printf("-none-\n");
		}

		if(flag==1)
		{
		flag=0;
		printf(" Faulting addr=0x" ADDR_FMT "\n",
            (ADDR_TYPE) si->si_addr);

		}
		else
		{
		printf("Faulting addr=%p\n",&str);
		}
		if (signal(SIGSEGV, SIG_DFL) == SIG_ERR)
				printf("Setting back to default action- failed");
		if (raise(SIGSEGV))
				printf("raise SIGSEGV failed");

}
int main(int argc,char *argv[])
{
		struct sigaction act;

		if (argc != 3) {
            printf("Usage: %s u|k r|w\n"
        "u => user mode\n"
        "k => kernel mode\n"
        "r => read attempt\n" " w => write attempt\n", argv[0]);
        exit(1);
		 }


		memset(&act, 0, sizeof(act));
stack_t ss,old_ss;

           ss.ss_sp = malloc(SIGSTKSZ);
           if (ss.ss_sp == NULL) {
               perror("malloc");
               exit(EXIT_FAILURE);
           }

           ss.ss_size = SIGSTKSZ;
           ss.ss_flags = 0;
           if (sigaltstack(&ss,&old_ss) == -1) {
               perror("sigaltstack");
               exit(EXIT_FAILURE);
           }
printf("Regular stack uva=%p\n",old_ss.ss_sp);
printf("Alt signal stack size=%luBytes\n",ss.ss_size);
printf("Alt signal uva=%p\n",ss.ss_sp);
		act.sa_sigaction = my_segvhandler;
		act.sa_flags=SA_SIGINFO|SA_ONSTACK;
		sigemptyset(&act.sa_mask);

		if (sigaction(SIGSEGV, &act, 0) == -1)
				printf("sigaction SIGSEGV failed\n");

		 if ((tolower(argv[1][0]) == 'u') && tolower(argv[2][0] == 'r')) {
        printf("Attempting to read contents of arbitrary usermode uptr =%p\n",&str );
		segv();
		return 0;	
   } 
	else if ((tolower(argv[1][0]) == 'u') && tolower(argv[2][0] == 'w')) {
        printf("Attempting to write into arbitrary usermode uptr = %p\n",&str);
			 segv();
    }
		else if ((tolower(argv[1][0]) == 'k') && tolower(argv[2][0] == 'r')) {
        ADDR_TYPE *kptr = (ADDR_TYPE *) invalid_kaddr;
        printf
            ("Attempting to read contents of arbitrary kernel va kptr = 0x" ADDR_FMT ":\n",
             (ADDR_TYPE) kptr);
        printf("*kptr = 0x" ADDR_FMT "\n", *kptr);  // just reading
    }	else if ((tolower(argv[1][0]) == 'k') && tolower(argv[2][0] == 'w')) {
        ADDR_TYPE *kptr = (ADDR_TYPE *) invalid_kaddr;
        printf
            ("Attempting to write into arbitrary kernel va kptr = 0x" ADDR_FMT ":\n",
             (ADDR_TYPE) kptr);
        *kptr = 0x2A;

	}
     else
	 {
     printf("Usage: %s u|k r|w\n"
        "u => user mode\n"
        "k => kernel mode\n"
        " r => read attempt\n" " w => write attempt\n", argv[0]);
	 }	

return 0;
}

void segv()
{		
		/* Stored in read only part of data segment */
		str = "Segvfault";      

		/* Problem:  trying to modify read only memory */
		*(str+1) = 'n';  
}
