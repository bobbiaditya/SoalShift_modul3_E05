#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define FOR(i, k) for(int i=0; i<k; i++)
#define T 3
pthread_t tid[T];
int WU_stats=0;
int S_stats=100;


int flag1=0;
int flag2=0;
int flagdone=0;
int status=0;
int inp=99;
int statusagmal=0;
int statusiraj=0;
void menu0()
{
	printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n",WU_stats,S_stats);
}

void menu1()
{
	WU_stats=WU_stats+15;
	flag1++;
}

void menu2()
{
	S_stats=S_stats-10;
	flag2++;
}

void input()
{
		if(WU_stats>=100)
			{
				printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
				kill(0,SIGKILL);
							
			}
		else if (S_stats <=0)
			{
				printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
				kill(0,SIGKILL);
			
			}
		else
			{
				printf("Masukkan input\n0=All Status\n1=Agmal Ayo Bangun\n2=Iraj Ayo Tidur\n");
				scanf("%d",&inp);
				if(inp==0)
				{
					menu0();
					inp=99;
					status=0;
				}
			}
}

void *jalaninput(void *arg)
{
	while(1)
	{
		if(!status || statusagmal || statusiraj)
		{
			sleep(1);
			//printf("%d-%d-%d\n",status,statusagmal,statusiraj);
			status=1;
			input();

		}

	}
	
}

void *jalanmenu1(void *arg)
{
	while(1)
	{
		while(!statusagmal)
		{
			if(flag2==3)
			{
				printf("Agmal Ayo Bangun disabled 10 s\n");				
				flag2=0;
				status=0;
				statusagmal=1;
				sleep(10);
				statusagmal=0;
			}
			else if(inp==1)
			{
					//printf("masuk menu 1 dong\n");
					menu1();
					inp=99;
					status=0;
			}
		}
	}
}

void *jalanmenu2(void *arg)
{
	while(1)
	{
		while(!statusiraj)
		{
			if(flag1==3)
			{
					printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
					flag1=0;
					status=0;
					statusiraj=1;
					sleep(10);
					statusiraj=0;
			}
			else if(inp==2)
			{

					//printf("masuk menu 2 dong\n");
					menu2();
					inp=99;
					status=0;
				
			}
		}
		
	}
}



int main(void)
{
	pthread_create(&(tid[1]), NULL,&jalanmenu1, NULL);
	pthread_create(&(tid[2]), NULL,&jalanmenu2, NULL);
	pthread_create(&(tid[0]), NULL,&jalaninput, NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[0],NULL);
		  
}
