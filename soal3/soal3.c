#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define FOR(i, k) for(int i=0; i<k; i++)
#define T 2
pthread_t tid[T];
int WU_stats=0;
int S_stats=100;


int flag1=0;
int flag2=0;

void menu0()
{
	printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n",WU_stats,S_stats);
}

void menu1()
{
	WU_stats=WU_stats+15;
}

void menu2()
{
	S_stats=S_stats-10;
}

void update_flag1()
{
	flag1++;
}

void update_flag2()
{
	flag2++;
}

void *jalanmenu1(void *arg)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
        	if(flag2==3)
    		{
			flag1=0;
    			sleep(10);
			
    		}
    		else
    		{
    			update_flag1();
				printf("flag1=%d\n",flag1);  			
    		}
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
        	if (flag2==3)
        	{
        		printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
        		sleep(10);
        	}
        	else
        	{
				printf("masuk menu1\n");
                menu1();
        	}
        }
}

void *jalanmenu2(void *arg)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
        	if(flag1==3)
    		{
			flag1=0;
    			sleep(10);
    		}
    		else
    		{
    			update_flag2();
				printf("flag2=%d\n",flag2);
    		}
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
        	if (flag1==3)
        	{
        		printf("Agmal Ayo Bangun disabled 10 s\n");
        		sleep(10);
        	}
        	else
        	{
                menu2();
				printf("masuk menu 2\n");
        	}
        }
}
int main(void)
{
	char input[100];
	char inpmenu0[100]="All Status";
	char inpmenu1[100]="Agmal Ayo Bangun";
	char inpmenu2[100]="Iraj Ayo Tidur";
	flag1=0;
	flag2=0;
	int err;
	//printf("%d-%d",WU_stats,S_stats);
	while(1)
	{
		if(WU_stats>=100)
		{
			printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
			break;
		}
		else if (S_stats <=0)
		{
			printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
			break;
		}
		printf("Masukkan input\n");
		gets(input);
		if(strcmp(input,inpmenu0)==0)
		{
			menu0();
		}

		else if(strcmp(input,inpmenu1)==0)
		{
			printf("masuk menu1\n");
			FOR(i, T) 
			{
	 	    	err = pthread_create(&(tid[i]), NULL,&jalanmenu1, NULL);
           	 }
		pthread_join(tid[0],NULL);
		pthread_join(tid[1],NULL);            
        }
		

		else if(strcmp(input,inpmenu2)==0)
		{
			printf("masuk menu2\n");
			FOR(i, T) 
			{
	 	    	err = pthread_create(&(tid[i]), NULL,&jalanmenu2, NULL);
            	}
        	pthread_join(tid[0],NULL);
			pthread_join(tid[1],NULL);
		}
		else
		{
			printf("input salah\n");
		}

	}

}