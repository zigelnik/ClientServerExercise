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

#define SERVER_SIGNAL 5

char** getArguments(int* size);

int getAnswer(char* integer1, char* op, char* integer2);

void signal_handler(int sig)
{
    signal(SERVER_SIGNAL, signal_handler);
  printf("pid = %d(server) recieved a signal: %d\n",getpid(),sig);
  sleep(1);
  
  
   int fd,fdwrite,i,size, answer,x,fdsize,status,fdopen;
   char** args, cfile[20] = "toClient", *txt = ".txt", *fdanswer;
   pid_t pid;
   
   args = getArguments(&size);
   answer = getAnswer(args[1],args[2],args[3]);
     
    // execvp fork - removing toServer.txt
         if ((pid = fork()) < 0) 
         {
        perror(" fork error");
         }
         else{        /* start CHILD */
                  if (pid == 0)
                {
                fdopen = open("toServer.txt", O_RDWR , 777);
                 char *remove[] = {"rm","-f" ,"toServer.txt", NULL };
                  execvp(remove[0], remove);
                 }  
                          /* end CHILD */
            
                  else
                            {   /* start parent */
                              wait(&status);
                            }  /* end parent */
              }
           
   //
   
    // create client's answer file
    strcat(cfile,args[0]);
    strcat(cfile,txt);
    // answer from int to string
    
       fdsize = snprintf(NULL, 0, "%d", answer);
       fdanswer = malloc(fdsize+1);
       snprintf(fdanswer, fdsize + 1, "%d", answer);              
    // 
    
    fd = open(cfile, O_CREAT | O_WRONLY | O_TRUNC , 777);
    fdwrite = write(fd, fdanswer,strlen(fdanswer)+1);
    close(fd);
   
 
    kill(atoi(args[0]), SERVER_SIGNAL);
    
}




int main(int argc, char* argv[])
{
  
    signal(SERVER_SIGNAL,signal_handler);
    
      
     while(1)
     {
          pause();  
           printf("server pid: %d\n",getpid());
        }       
        
  exit(1);

}

char** getArguments(int* size)
{
    int nameCounter = 0;
    int fd, i = 0, k = 0, j = 0;
    char buffer[100];

    fd = open("toServer.txt", O_RDONLY, 777);
    if (fd == -1)
    {
        perror("error opening toserver.txt\n");
        exit(-1);
    }


    while (read(fd, &buffer[i], 1) != 0)
    {
        if (buffer[i] == '\n')
        {
            nameCounter++;
        }
        i++;
    }
    buffer[i+1] = '\0';

    close(fd);
    
    char** namesArr = (char**)malloc(nameCounter * sizeof(char*));
    for (i = 0; i < nameCounter; i++)
        namesArr[i] = (char*)malloc(20 * sizeof(char));

    i = 0;
    j = 0;
    k = 0;

    fd = open("toServer.txt", O_RDONLY, 777);
    while (read(fd, &buffer[i], 1) != 0)
    {

        if (buffer[i] != '\n')
        {
            namesArr[j][k] = buffer[i];
            k++;
            i++;
        }
        else {

            namesArr[j][k] = '\0';
            j++;
            i++;
            k = 0;
        }

    }
    close(fd);

    *size = nameCounter;
    return namesArr;

}


int getAnswer(char* integer1, char* op, char* integer2)
{
  int num1 = atoi(integer1) , num2 = atoi(integer2), x = atoi(op);
  
  	switch (x)
	{
	case 1:
		return num1 + num2;
		break;
	case 2:
		return num1 - num2;
		break;
	case 3:
		return num1 * num2;
		break;
	case 4:
		if (num2 == 0) {
			printf("Error -cannot divide by zero!\n");
			exit(-1);
		}
		return num1 / num2;
		break;
	}
 

}
