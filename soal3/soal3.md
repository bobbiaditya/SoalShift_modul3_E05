## Nomor 3
Pada soal nomor 3 kita diberikan sebuah permasalahan tentang Agmal dan Iraj. Agmal memiliki `WakeUp_Status`yang awalnya 0 dan Iraj memiliki `Spirit_Status` yang awalnya 100. Agmal bisa menambah `WakeUp_Status` nya , Iraj bisa mengurangi `Spirit_Status` nya.

Fitur:
- All Status, yaitu menampilkan status kedua sahabat
Ex: Agmal WakeUp_Status = 75
Iraj Spirit_Status = 30
- “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar
15 point
- “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point

Kondisi:
- Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka
Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik
(Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled
10 s”)
- Jika Fitur “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur
“Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik
(Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled
10 s”)

Program akan berhenti jika Salah Satu :
- WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal
Terbangun,mereka bangun pagi dan berolahraga”)
- Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan
bangun kesiangan bersama Agmal”)

### Update WakeUp_Status Agmal
```c
void menu1()
{
	WU_stats=WU_stats+15;
	flag1++;
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
```
Potongan Code diatas berguna untuk menambah wakeup status agmal dan mendisable(`sleep(10)`) thread ini apabila fitur iraj dijalankan 3 kali.


### Update SpiritStatus Iraj
```c
void menu2()
{
	S_stats=S_stats-10;
	flag2++;
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
```
Potongan Code diatas berguna untuk mengurangi spirit status iraj dan mendisable(`sleep(10)`) thread ini apabila fitur agmal dijalankan 3 kali.

### Input
```c
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
```
Code diatas digunakan untuk menerima input user. Pilihan untuk menjalankan fitur all status juga ada pada potongan code di atas.

User akan tetap terus bisa melakukan input walaupun ada salah satu fitur yang di disable.

Pada saat ada kondisi terminasi yang terpenuhi, maka program otomatis akan berhenti. Untuk menghentikan program kami menggunakan `kill(0,SIGKILL);`

### Fungsi All Status
```c
void menu0()
{
	printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n",WU_stats,S_stats);
}
```
Potongan code diatas untuk memampilkan jumlah spirit_status dan wake_up status saat ini.

### Main Program
```c
int main(void)
{
	pthread_create(&(tid[1]), NULL,&jalanmenu1, NULL);
	pthread_create(&(tid[2]), NULL,&jalanmenu2, NULL);
	pthread_create(&(tid[0]), NULL,&jalaninput, NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[0],NULL);
		  
}
```
Potongan Code diatas adalah main program kami. Pada main program, kami membuat 3 buah thread dan menjoinkan semua thread agar thread-thread yang ada berjalan bersamaan.
