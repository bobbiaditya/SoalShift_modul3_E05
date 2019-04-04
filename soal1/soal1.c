#include <fcntl.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

#define FOR(i, k) for(int i=0; i<k; i++)
#define T 1000

pthread_t tid[T];
int temp[1000];
int hasil[1000];
int jumlah;
int fakto(int n)
{
	int hasil=1;
	int i;

	if(n==0) return 0;
	for(int i=1;i<=n;i++)
	{
		hasil=hasil*i;
	}

	return hasil;
}

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void bubbleSort(int arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
}

void *jalanin(void *arg)
{
    pthread_t id=pthread_self();
	FOR(i,jumlah)
	{
	    if(pthread_equal(id, tid[i])) 
	    {
	    	hasil[i]=fakto(temp[i]);
	    }
	}
}


int main(int argc, char *argv[])
{
	int i=0;
	int err;
	jumlah=argc;
	for(i=0;i<jumlah;i++)
	{
		temp[i]=atoi(argv[i]);
		//printf("%d\t",temp[i]);
	}
	for(i=0;i<jumlah;i++)
	{
		//temp[i]=atoi(argv[i]);
		//printf("%d\t",temp[i]);
	}    
FOR(i, jumlah) 
    {
    	err = pthread_create(&(tid[i]), NULL, &jalanin, NULL);
    
	    if(err != 0) 
	    {
	        //printf("\n Can't create thread : [%s]", strerror(err));
	    } else 
	    {
	        //printf("\n Created thread successfully\n");
	    }
    }
    FOR(i, jumlah)
    {
		pthread_join(tid[i],NULL);
    }
    bubbleSort(temp,jumlah);
    bubbleSort(hasil,jumlah);
    for(int i=1;i<jumlah;i++)
    {
    	printf("%d!=%d\n",temp[i],hasil[i]);
    }

    return 0;
}