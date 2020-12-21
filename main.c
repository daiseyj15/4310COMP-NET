#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>


#define PORT 8080

 struct ActiveAgents{

        char agentIPP[17];
        double timeInMili;
    };

struct ActiveAgents actagents[10];
void join(char agentip[17],FILE *thelog,int new_s);
void leave(char agentip2[17],FILE *thelog2,int new_s2);
void listt(char agentip3[17],FILE *thelog3,int new_s3);
void logg(FILE *thelog4,int new_s4,char agentip4[17]);

int main(int argc, char* arvg[])
{
    //things to pass to the agent.c the C2C server IP, server port and the action string.
    //All actions start with a ‘#’ and all responses start with ‘$’ followed by the data/message.

    //to keep log
    FILE *theLog;
    theLog = fopen("log.txt","w+");

    if(theLog == NULL)
   {
      printf("Error!");
      exit(1);
   }
    clock_t startTime;
    startTime = clock();
    double dummyTime = 3.0;

    int new_socket, serverFD, valueRead;
    struct sockaddr_in address;
    int opt = 1;
    int addressLength = sizeof(address);



      // create a new socket to use for communication
    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("ERROR: socket() failed\n");
            exit (-1);
        }

    printf("Server Socket Created\n");

    //forcefully attaching socket to the port 8080
    if(setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("setsockopt");
        exit(-1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
                //inet_addr("147.26.231.156");
    address.sin_port = htons(PORT);

    //Forcefully attaching socket to the port 8080
    if(bind(serverFD,(struct sockaddr *)&address, sizeof(address))<0)
    {
        printf("bind failed");
        exit(-1);
    }



    while(1)
    {
        char buffe[1024];
        //listen
        if(listen(serverFD, 5) < 0)
        {
            printf(" no listen");
            exit(-1);
        }

        //accept
        if((new_socket = accept(serverFD, (struct sockaddr *)&address, (socklen_t*)&addressLength))<0)
        {
            printf(" no accept");
            exit(-1);
        }

        //recieve
        valueRead = read(new_socket,buffe,1024);

        char agentIP[17];
        strcpy(agentIP,inet_ntoa(address.sin_addr));


            if(buffe[0+1]=='j' || buffe[0+1]=='J')
            {
                 clock_t t;
                  t = clock();
                  t= t - startTime;
                  double tim = ((double)t)/CLOCKS_PER_SEC;//in secs
                    tim *= 1000; //milisec
                fprintf(theLog,"%f: Received a #JOIN action from agent %s\n",tim, agentIP);
                fflush(theLog);
                join(agentIP, theLog, new_socket);
                fflush(stdout);
                printf("works\n\n");
                close(new_socket);
            }
            if(buffe[0+1]=='l'||buffe[0+1]=='L')
            {
                if(buffe[0+2]=='e'||buffe[0+2]=='E')
                {
                    /*clock_t t;
                  t = clock();
                  t= t - startTime;
                  double tim = t;*/
                  fprintf(theLog,"%f: Received a #LEAVE action from agent %s\n",dummyTime, agentIP);
                  fflush(theLog);
                leave(agentIP, theLog, new_socket);
                close(new_socket);

                }
                if(buffe[0+2]=='i'||buffe[0+2]=='I')
                {
                    /*startTime = clock();*/
                    fprintf(theLog,"%f: Received a #LIST action from agent %s\n",dummyTime, agentIP);
                    fflush(theLog);
                    listt(agentIP, theLog, new_socket);
                    close(new_socket);
                }
                if(buffe[0+2]=='o'||buffe[0+2]=='O')
                {
                    /*startTime = clock();*/
                    fprintf(theLog,"%f: Received a #LOG action from agent %s\n",dummyTime, agentIP);
                    fflush(theLog);
                    logg(theLog,new_socket,agentIP);
                    close(new_socket);
                }
            }




    }
    close(new_socket);
    fclose(theLog);
    return 0;
}

void join(char agentip[17],FILE *thelog,int new_s)
{
    char res[5] = "$OK";
    char res2[17] = "$ALREADY MEMBER";
    clock_t milisecond;

    int i=0;
        while(i<10)
        {
            if(strcmp(actagents[i].agentIPP,agentip)==0)
            {
                 milisecond = clock();
                  double time_taken = ((double)milisecond)/CLOCKS_PER_SEC; // in seconds



                fprintf(thelog,"%f: Responded to agent %s with $ALREADY MEMBER\n",time_taken,agentip);
                fflush(thelog);

                //return(res2);
               write(new_s,res2,strlen(res2));
                return;
            }

            i++;
        }

        static int numOfJoined = 0;
        milisecond = clock();
         double time_taken = ((double)milisecond)/CLOCKS_PER_SEC; // in seconds
        fprintf(thelog,"%f: Responded to agent %s with $OK\n",time_taken,agentip);
        fflush(thelog);

        if(numOfJoined==0)
        {
            strcpy(actagents[0].agentIPP,agentip);
            actagents[0].timeInMili = time_taken;
        }
        else
        {
            strcpy(actagents[numOfJoined].agentIPP,agentip);
            actagents[numOfJoined].timeInMili = time_taken;
        }

        numOfJoined++;
        //return(res);
        write(new_s,res,strlen(res));

        return;
}


void leave(char agentip2[17],FILE *thelog2,int new_s2)
{
    char resl[5] = "$OK";
    char res2l[17] = "$NOT MEMEBER";
    clock_t milisecond;
     double time_taken = ((double)milisecond)/CLOCKS_PER_SEC; // in seconds




    int i=0;
        while(i<10)
        {
            if(strcmp(actagents[i].agentIPP,agentip2)==0)
            {
                strcpy(actagents[i].agentIPP,"000.000.000.000");
                actagents[i].timeInMili=0.0;
                //milisecond =clock();
                fprintf(thelog2,"%f: Responded to agent %s with $OK\n",time_taken,agentip2);
                fflush(thelog2);
               // return(res);
               send(new_s2,resl,strlen(resl),0);
               return;
            }

            i++;
        }

      //milisecond = clock();
      fprintf(thelog2,"%f: Responded to agent %s with $NOT MEMEBER\n",time_taken,agentip2);
      fflush(thelog2);
      //return(res2);
      send(new_s2,res2l,strlen(res2l),0);
      return;

}

void listt(char agentip3[17],FILE *thelog3,int new_s3)
{

       clock_t milisec;
       milisec = clock();
        double time_taken = ((double)milisec)/CLOCKS_PER_SEC; // in seconds

     int i=0;
        while(i<10)
        {
            if(strcmp(actagents[i].agentIPP,agentip3)==0)
            {
                //milisec = clock();
               int x=0;
               while(x<10)
               {
                   if(strcmp(actagents[x].agentIPP,"000.000.000.000")==0)
                   {
                       x++;
                       continue;
                   }

                   double duration = time_taken - actagents[x].timeInMili;
                   char hey[1024];
                   sprintf(hey,"<%s,%f>\n",actagents[x].agentIPP,duration);/////////////////////////////////////
                   send(new_s3,hey,strlen(hey),0);
                   x++;
               }

               return;
               fflush(thelog3);
            }

            i++;
        }

    //milisec = clock();
    fprintf(thelog3,"%f: No response is supplied to agent %s (agent not active)\n",time_taken,agentip3);
    fflush(thelog3);
}

void logg(FILE *thelog4,int new_s4,char agentip4[17])
{

    clock_t milsec = clock();
     double time_taken = ((double)milsec)/CLOCKS_PER_SEC; // in seconds


    rewind(thelog4);
    char buff[1024];

    while(fgets(buff, sizeof buff, thelog4) != NULL)
    {
       send(new_s4,buff,strlen(buff),0);
    }
}

