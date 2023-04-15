#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/random.h>

#define CLIENT_SIGNAL 5

void writeToServer(char *argv[]);
void readFromClient();


void signal_handler(int sig)
{

  signal(CLIENT_SIGNAL, signal_handler);
  printf("pid = %d(client) recieved a signal: %d\n",getpid(),sig);
  sleep(1);
  
   readFromClient();
 
}



int main(int argc, char* argv[])
{

signal(CLIENT_SIGNAL,signal_handler);
  
  if (argc != 5)
      {
      printf("wrong number of inputs, exiting..\n");
      exit(-1);
      }
  
  writeToServer(argv);
  
  kill(atoi(argv[1]), CLIENT_SIGNAL);
  printf("client pid: %d\n",getpid());

  pause();
 
 return 1;
   
}

void readFromClient()
{
 char cfile[40] = "toClient", *txt = ".txt", *fdpid, buffer[60];
  int fdsize, fdwrite, fd, fdread;
  
      fdsize = snprintf(NULL, 0, "%d", getpid());
       fdpid = malloc(fdsize+strlen(txt) + 1);
       snprintf(fdpid, fdsize + 1, "%d", getpid());  
       
  
  strcat(fdpid,txt);
  strcat(cfile,fdpid);
  
  fd = open(cfile, O_RDONLY , 777);
  if (fd<0)
  {
      perror("cant open file\n");
      exit(-1);
   }
      
    if (read(fd, buffer, 256) >0 )
          printf("\nThe solution is %s.\n" , buffer);
  

  close (fd);
}

void writeToServer(char *argv[])
{
  int fd,fdsize, fdwrite, i,ret,x;
  char *fdstr;
  unsigned char rand[10];
  
   ret = getrandom(rand,sizeof(rand),0);
   if(ret<0)
     {
     perror("getrandom\n");
     exit(-1);
     }
     
for(i=0;i<10;i++) 
{
  x = rand[i] %6;
  
  fd = open("toServer.txt" , O_CREAT | O_RDWR , 777);
     if (fd < 0)
     {
       printf("random = %d\n", x);
     sleep(x);
     }
     else
     {
         break;
     }
     
    if(i==11)
      {
        perror("cannot connect to server.exe, bye bye\n");
        exit(-1);
      }
      
      }
      pid_t pid = getpid();

   fdsize = snprintf(NULL, 0, "%d", pid);
   fdstr = malloc(fdsize+1);
   snprintf(fdstr, fdsize + 1, "%d", pid);
 
 fdwrite = write(fd,fdstr,fdsize);
 fdwrite = write(fd,"\n",1);
 
 

 for(i=2;i<5;i++)
 {
   fdwrite = write(fd,argv[i],strlen(argv[i]));
    fdwrite = write(fd,"\n",1);
 }
 
 
    close(fd);
    
}
