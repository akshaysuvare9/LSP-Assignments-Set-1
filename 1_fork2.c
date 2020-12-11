#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>

int main()
{
		int ret;
		struct sigaction v;
		v.sa_handler=SIG_IGN;
		sigemptyset(&v.sa_mask);
		v.sa_flags=0;//SA_NOCLDWAIT etc../

		if((ret=fork())==0){
				printf("In child process..%d\n",getpid());
				sleep(5);
                                printf("child %d exit\n",getpid());
		}
		else if(ret==-1){
				perror("Fork failed\n");
				exit(1);
		}
		else{//parent
		if((ret=fork())==0)
				{//child2
						printf("In child process..%d\n",getpid());
						sleep(3);
						printf("child %d exit\n",getpid());
				}
				else if(ret==-1)
				{
						perror("Error in fork\n");
						exit(1);
				}

				else
				{//parent

						sigaction(SIGCHLD,&v,0);		
						while(waitpid(-1,0,0)!=-1);
						printf("Parent %d exit\n",getpid());
				}
		}
}



