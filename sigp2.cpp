#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
void hdfn(int sig)
{
	cout<<"p2 got SIGUSR2 signal from p1\n";
}

int main()
{
	signal(SIGUSR2,hdfn);
	signal(SIGINT,SIG_DFL);
	int fd1;
	key_t kx=ftok("x",65);
	key_t ky=ftok("y",70);
	//pid_t *p1,*p2;
	int idx=shmget(kx,1024,0666|IPC_CREAT);
	int idy=shmget(ky,1024,0666|IPC_CREAT);
	int *x=(int *) shmat(idx,(void*)0,0);
	int *y=(int *)shmat(idy,(void *)0,0);
	
	char myfifo[]="myfifo";
	mkfifo(myfifo,0666);
	
	fd1=open(myfifo,O_RDWR);
		pid_t p2=getpid(),p1;
		string s=to_string(p2);
		char const* buff=s.c_str();
		cout<<"exchanging pid's\n";
		write(fd1,buff,strlen(buff));
		sleep(2);
		char buf2[20];	
		read(fd1,buf2,10);
		p1=atoi(buf2); 
		sleep(2);
	while(1)
	{

		*y=(*x)+1;
		cout<<"reading x - "<<*x<<"\n";
		sleep(5);
		kill(p1,SIGUSR1);
	}
		close(fd1);
		unlink(myfifo);
	shmdt(x);
	shmdt(y);
	
	shmctl(idx,IPC_RMID,0);
	shmctl(idy,IPC_RMID,0);
}
