#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<fcntl.h>
#include<sys/stat.h>
using namespace std;
void hdfn(int sig)
{
	cout<<"p1 got SIGUSR1 signal from p2\n";
}

int main()
{
	signal(SIGUSR1,hdfn);
	signal(SIGINT,SIG_DFL);
	int fd;
	key_t kx=ftok("x",65);
        key_t ky=ftok("y",70);
	//pid_t *p1,*p2;	
	int idx=shmget(kx,1024,0666|IPC_CREAT); 
	int idy=shmget(ky,1024,0666|IPC_CREAT);
	int *x=(int *) shmat(idx,(void*)0,0);   
        int *y=(int *)shmat(idy,(void *)0,0);
	*y=0;
	char myfifo[] = "myfifo"; 
	int k=mkfifo(myfifo, 0666);
	fd=open(myfifo,O_RDWR);	
	pid_t p1=getpid(),p2;
	cout<<"exchanging pid's \n";
		char buff[20];
		read(fd,buff,10);
		p2=atoi(buff);       
		 string s=to_string(p1);
		 char const* buf2=s.c_str();
		 write(fd,buf2,strlen(buf2));
	       sleep(2);
	cout<<"\nstart\n";
	while(1)
	{
	*x=(*y)+1;
	cout<<"reading y - "<<*y<<"\n";
	//raise(SIGUSR1);
	sleep(5);
	kill(p2,SIGUSR2);
	
	}
		close(fd);
	unlink(myfifo);
	shmdt(x);
	shmdt(y);
}	
