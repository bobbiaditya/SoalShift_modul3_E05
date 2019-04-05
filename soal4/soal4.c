
void bikin1()#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FOR(i, k) for(int i=0; i<k; i++)
#define T 2
pthread_t tid[T];

void bikin1()
{
	system("ps -aux|head -11 | tail -10 > $HOME/Documents/FolderProses1/SimpanProses1.txt");
}

void bikin2()
{
	system("ps -aux|head -11 | tail -10 > $HOME/Documents/FolderProses2/SimpanProses2.txt");	
}

void zip1()
{

	system("zip -rmj $HOME/Documents/FolderProses1/KompresProses1.zip $HOME/Documents/FolderProses1/SimpanProses1.txt");
	//printf("Menunggu 15 detik untuk mengekstrak kembali SimpanProses1\n");
}

void zip2()
{
	system("zip -rmj $HOME/Documents/FolderProses2/KompresProses2.zip $HOME/Documents/FolderProses2/SimpanProses2.txt");
	//printf("Menunggu 15 detik untuk mengekstrak kembali SimpanProses2\n");
}

void unzip1()
{
	system("unzip $HOME/Documents/FolderProses1/KompresProses1.zip -d $HOME/Documents/FolderProses1/");
}

void unzip2()
{
	system("unzip $HOME/Documents/FolderProses2/KompresProses2.zip -d $HOME/Documents/FolderProses2/");
}

void *lsproses(void *argv)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
                bikin1();
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
                bikin2();
        }
}

void *zipfile(void *argv)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
                zip1();
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
                zip2();
        }
}

void *unzipfile(void *argv)
{
	pthread_t id=pthread_self();
        if(pthread_equal(id,tid[0]))
        {
                unzip1();
        }
        else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
        {
                unzip2();
        }
}



int main(void)
{
	int i,err;
	i=0;
	//lsproses
	while(i<2)
	{
		err=pthread_create(&(tid[i]),NULL,&lsproses,NULL); //membuat thread
		if(err!=0) //cek error
		{
			//printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			//printf("\n create thread success\n");
		}
		i++;
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	//zipfile
	i=0;
	while(i<2)
	{
		err=pthread_create(&(tid[i]),NULL,&zipfile,NULL); //membuat thread
		if(err!=0) //cek error
		{
			//printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			//printf("\n create thread success\n");
		}
		i++;
	}	
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
	sleep(15);

	//unzipfile
	i=0;
	while(i<2)
	{
		err=pthread_create(&(tid[i]),NULL,&unzipfile,NULL); //membuat thread
		if(err!=0) //cek error
		{
			//printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			//printf("\n create thread success\n");
		}
		i++;
	}	
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

}