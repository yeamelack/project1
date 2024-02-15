#include <sys/types.h>
#include <signal.h>
#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128


int main(int argc, char *argv[]) {

    write_filepath_to_submissions("test_solutions", "submission.txt");
    FILE *file = fopen("submission.txt", "r");
      if(!file){
        return -1;
    }

    int total_executables = 0;
    char buff[128];
    while(fgets(buff, sizeof(buff), file) != NULL){
        total_executables++;
    }
    rewind(file);
    printf("student array: %d\n", total_executables);

    // char scores[total];
    char students[total_executables][WORD_LEN];
   
    int batch_size = atoi(argv[1]);
  
    int iter = 0;
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(students[iter], buff);
        iter++;
    }

    pid_t pids[batch_size];

    
    for(int i = 2; i < argc; i++){
        int done_executables = 0;
        while (done_executables < total_executables){
            for(int j = 0; j < batch_size; j++){
                pid_t pid = fork();
                    if (pid < 0) {
                        perror("fork failed");
                        return -1;
                    }
                    else if(pid == 0){
                        // printf("student: %s, answer%s, pid: %d\n", students[j], argv[i], getpid());
                        printf("get pid %d\n", getpid());
                        //start_timer(&start);
                        execl(students[done_executables], argv[1], argv[i], NULL);
                        perror("exec failed\n");
                        exit(1);
                    }
                    else {
                        pids[j] = pid;
                        printf("%s %d %d\n", "child ", j, pids[j]);
                    }      
                    done_executables++;   


            }
             printf("\n");

        //     int finished = 0;
        //     while(finished < batch_size){
        //         for(int k=0; k<batch_size; k++){


        //         }

        //     }
        //     done_executables += batch_size;
         }







    }
    fclose(file);
}

