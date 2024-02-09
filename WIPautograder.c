#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128


int main(int argc, char *argv[]) {

    write_filepath_to_submissions("/home/erist003/4061/p1/test", "/home/erist003/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
      if(!file){
        return -1;
    }

    int k = 0;
    struct timeval timer[argc];
    char buff[128];
    int numStudents = atoi(argv[1]);
    char students[numStudents][WORD_LEN];
    
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(students[k], buff);
        k++;
    }
    fclose(file);

    pid_t pids[numStudents];
    
   
    int finishedpid = 0;

    for(int i=0; i < argc; i++){//running through arguments 
        for(int j=0 ; j < numStudents; j++){
            pid_t pid = fork();
            if(pid > 0){
                
                  pid_t terminated_pid = wait(NULL);
                }
                else if(pid == 0){
                    //printf("%d", pids[j]);
                    pids[j] = getpid();
                    //printf("%d%s", pids[j], "\n");
                    //execl(students[i], argv[1], argv[j+2], NULL);
                    //printf("%s%s", students[j],"\n\n");
                    //execl(students[j], argv[1], argv[i], NULL);
                  
                }
                    for(int h=0; h<6; h++){
                        printf("%d\n", pids[h]);

                    }
            //     while(finishedpid < numStudents){
            //         int status = 0;
            //         //printf("%s%d", "\n", pids[j]);
            //         int finished = waitpid(pids[j], &status, WNOHANG);
            //         //printf("%s%s", students[i], "\n");
            //         //printf("%d%s", pids[j],"\n\n");
            //         break;
                    

            //         if(finished > 0){
            //             if(WIFEXITED(status)){
            //                 int ret = WEXITSTATUS(status);
            //                 if(ret == 0){
            //                     printf("%s", "correct \n");
            //                     finishedpid++;
            //                     break;
            //                 }
            //                 else if(ret == 1){
            //                     printf("%s", "incorrect\n");
            //                     finishedpid++;   
            //                 }
            //                 else if(ret == 3){
            //                     printf("%s", "seg fault\n");
            //                     finishedpid++;
            //                 }
            //                 else{
            //                     break;
            //                 }
            //             }
            //         }
            //     }
            // }
            
        }
    }
                
    
    return 0;
}  
