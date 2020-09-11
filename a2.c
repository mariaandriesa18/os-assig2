#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/sem.h>
#include<pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>
#include <semaphore.h>
#include "a2_helper.h"


pid_t  pid2, pid3, pid5, pid4, pid6, pid7, pid8, pid9;
sem_t *sem41, *sem63;
sem_t bar7, bar11, turnstile1, turnstile2, semk, sem44, sem42, sem1;

pthread_t t4[4], t6[5], t7[41];



void* funcForP4_T4(void* tid)
{
    const int thisId = *(int*)tid;
    info(BEGIN, 4, thisId);
    sem_post(&sem1);
    sem_wait(&sem44);
  
   info(END, 4, *(int*)tid);
   return NULL;
}

void* funcForP4_T2(void* tid)
{
    const int thisId = *(int*) tid;
    info(BEGIN, 4, thisId);
    info(END, 4, thisId);
    return NULL;
}

void* funcForP4(void *tid)
{
    info(BEGIN, 4, *(int*) tid);
    info(END, 4, *(int*) tid);
    return NULL;
}

void* funcForP6(void* tid)
{
    info(BEGIN, 6, *(int*)tid);

    info(END, 6, *(int*)tid);
    return NULL;
}


void* funcForP7(void* tid)
{
    int thisId = *(int*) tid;

    sem_wait(&semk);

    info(BEGIN, 7, thisId);

    info(END, 7, thisId);
    
    sem_post(&semk);

    return NULL;
}

void* funcForP7first3(void* tid)
{
    int thisId = *(int*) tid;

    info(BEGIN, 7, thisId);

    sem_wait(&bar7);

    info(END, 7, thisId);
    

    return NULL;
}

void* funcForT11(void* tid){
    int thisId = *(int*) tid;

    info(BEGIN, 7, thisId);

    info(END, 7, thisId);
    return NULL;
}

int main()

{
    init();
    int i, tids4[4], tids6[5], tids7[41];

    

    info(BEGIN, 1, 0);  
   
    if((pid2 = fork()) == 0)
    {
        info(BEGIN, 2, 0);

        info(END, 2, 0);

    }else

    {
        waitpid(pid2, NULL, 0);
        if((pid3 = fork()) == 0)

        {
            info(BEGIN, 3, 0);
            
            if((pid4 = fork()) == 0)
            {
               
                info(BEGIN , 4, 0);
                for( i = 1; i <= 4; i++)
                {
                    tids4[i] = i;
                }
                

                if (sem_init(&sem44, 1, 0) < 0) {
                        perror("Error creating the semaphore");
                        exit(2);
                    }

                if (sem_init(&sem1, 1, 0) < 0) {
                        perror("Error creating the semaphore");
                        exit(2);
                    }

                if (sem_init(&sem42, 1, 0) < 0) {
                        perror("Error creating the semaphore");
                        exit(2);
                    }
                if(pthread_create(&t4[4], NULL, funcForP4_T4, &tids4[4]) != 0)
                {
                    perror("Failed creating thread");
                    exit(2);
                 }
               
                sem_wait(&sem1);

                if(pthread_create(&t4[2], NULL, funcForP4_T2, &tids4[2]) != 0)
                {
                    perror("Failed creating thread");
                    exit(2);
                }

                pthread_join(t4[2], NULL);

                sem_post(&sem44);

                pthread_join(t4[4], NULL);

                if(pthread_create(&t4[3], NULL, funcForP4, &tids4[3]) != 0)
                {
                    perror("Failed creating thread");
                    exit(2);
                }
                pthread_join(t4[3], NULL);

                sem41 = sem_open("/sem41", O_CREAT, 0600, 0);
                if (sem41 == SEM_FAILED) {
                    perror("Error creating the semaphore set");
                    return 0;
                }

                sem_wait(sem41);

                if(pthread_create(&t4[1], NULL, funcForP4, &tids4[1]) != 0){
                    perror("Failed creating thread 4.1");
                    exit(2);
                }

                pthread_join(t4[1], NULL);

                sem63 = sem_open("/sem63", O_RDWR);
                sem_post(sem63);


                info(END, 4, 0);
            }else 

            {
                if((pid6 = fork()) == 0)
                {
                    info(BEGIN, 6, 0);   

                    for (i = 0; i < 3; i++) 
                        {
                            tids6[i] = i + 1;
                            
                            if(pthread_create(&t6[i], NULL, funcForP6, &tids6[i]) != 0){
                                perror("Failed creating thread");
                                exit(3);
                            }
                        }

                    for (i = 0; i < 3; i++) 
                    {
                        pthread_join(t6[i + 1], NULL);
                    }

                    sem41 = sem_open("/sem41", O_RDWR);
                    
                    sem_post(sem41);

                    sem63 = sem_open("/sem63", O_CREAT, 0600, 0);
                    if (sem63== SEM_FAILED) {
                        perror("Error creating the semaphore set");
                        return 0;
                    }

                    sem_wait(sem63);

                    for (i = 3; i < 5; i++) 
                        {
                            tids6[i] = i + 1;
                            
                            if(pthread_create(&t6[i], NULL, funcForP6, &tids6[i]) != 0){
                                perror("Failed creating thread");
                                exit(3);
                            }
                        }

                    for (i = 3; i < 5; i++) 
                    {
                        pthread_join(t6[i + 1], NULL);
                    }

                   
                    info(END, 6, 0);
                }else

                {
                    sem_close(sem41);
                    sem_close(sem63);
                    waitpid(pid4, NULL,0);
                    waitpid(pid6, NULL,0);
                    info(END, 3, 0);

                }
            }

        }else 

        {
            waitpid(pid3, NULL, 0);
            if((pid5 = fork()) == 0)
            {
                info(BEGIN, 5, 0);                
           
                if( (pid7 = fork()) == 0)
                {
                    

                    //wait
                    info(BEGIN, 7, 0);

                    if (sem_init(&semk, 1, 4) < 0) {
                        perror("Error creating the semaphore");
                        exit(2);
                    }

                    if (sem_init(&bar7, 1, 0) < 0) {
                        perror("Error creating the semaphore");
                        exit(2);
                    }
                    
                    for (i = 1; i <= 3; i++) 
                    {
                        tids7[i] = i ;
                        if(pthread_create(&t7[i], NULL, funcForP7first3, &tids7[i]) != 0){
                            perror("Failed creating thread");
                            exit(4);
                        }
                       
                    }

                    tids7[11] = 11;

                    if(pthread_create(&t7[11], NULL, funcForT11, &tids7[11]) != 0){
                        perror("Failed creating thread");
                        exit(4);
                    }
                    pthread_join(t7[11], NULL);

                    for(int a = 1; a <= 3; a++)
                    {
                        sem_post(&bar7);
                    }

                    for(i = 1 ; i<= 3; i++)
                    {
                        pthread_join(t7[i], NULL);    
                    }

                    for (i = 4; i <= 41; i++) 
                        {
                            if(i != 11)
                            {  
                                tids7[i] = i ;
                                if(pthread_create(&t7[i], NULL, funcForP7, &tids7[i]) != 0){
                                    perror("Failed creating thread");
                                    exit(4);
                                }
                            }
                            
                        }

                    for (i = 4; i <= 41; i++) 
                    {
                        if(i != 11)
                        {
                            pthread_join(t7[i], NULL);    
                        }
                        
                    }
                    sem_close(&semk);
                    sem_close(&bar7);
                    info(END, 7, 0);
                }else 

                {
                    waitpid(pid7, NULL, 0);
                   if((pid8 = fork()) == 0)
                    {
                       //wait
                       info(BEGIN, 8, 0);
                        if((pid9 = fork()) == 0) 
                       {
                            //wait
                            info(BEGIN, 9, 0);
                             
                            info(END, 9, 0);
                       }else 
                       {
                        
                        waitpid(pid9, NULL,0);
                        info(END, 8, 0);
                       }
                    
                    }else

                    {

                        waitpid(pid7, NULL,0);
                        waitpid(pid8, NULL, 0);
                        info(END, 5, 0);
                    }
                }
            
            }else 
            
            {
                waitpid(pid2, NULL,0);
                waitpid(pid3, NULL, 0);
                waitpid(pid5, NULL,0);
                info(END, 1, 0);
                
            }
        }
    }
        

    return 0;
}
