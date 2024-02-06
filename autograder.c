#include "include/utility.h"
#include <stdio.h>
#define WORD_LEN 128


int main(int argc, char *argv[]) {
    
    write_filepath_to_submissions("/home/lexxx668/4061/p1/test", "/home/lexxx668/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
    if(!file){
        return -1;
    }
    int i = 0;
    int index = 2;
    pid_t pids[argc];
    struct timeval timer[argc];
    char buff[128];
    int numStudents = atoi(argv[1]);
    char students[numStudents][WORD_LEN];
    
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(students[i], buff);
        i++;
    }
    for (int k = 2; k < argc; k++){
        printf("\n");
        printf("Argument at k is: %d\n", k);
        for (int j = 0; j < numStudents; j++){
            printf("\n");
            printf("j is: %d\n", j);
            pid_t pid = fork();
            if(pid > 0){
                pid_t terminated_pid = wait(NULL);
                index++;            
            }
            else if(pid == 0){
                pids[index] = getpid();
                printf("Pid id is: %d\n", pids[index]);
                // printf("\n");
                // printf("index is: %d\n", j);
                // start_timer(&timer[index]);

                printf("%s\n", students[j]);
                printf("argument: %s\n", argv[k]);
                execl(students[j], argv[1], argv[k], NULL);
                    
            }
        }
        
    }
    
    fclose(file);
    return 0;
}     
