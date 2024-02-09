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
        for (int j = 0; j < numStudents; j++){
            pid_t pid = fork();
            if(pid == 0){
                pids[index] = getpid();
                execl(students[j], argv[1], argv[k], NULL); 
                index++ 

                    
            } 
            else if(pid < 0){
                perror("failed");
                exit(EXIT_FAILURE);
                    
            }
        }
        
    }
    printf("%sc\n", students);

    while(1){
        int status = 0;
        int finished = waitpid(-1, &status, WNOHANG);
        if(finished > 0){
            if(WIFEXITED(status)){
                int ret = WEXITSTATUS(status);
                if(ret == 0){
                    printf("%s", "correct");
                }
                else if(ret == 1){
                    printf("%s", "incorrect");
                }
                else if(ret == 3){
                    printf("%s", "seg fault");
                }
            }
        }
        else if(finished == 0){
            break;
        }
        else if(finished == -1){
            perror('waitpid() failed');
            exit(EXIT_FAILURE)
        }
    }
    fclose(file);
    return 0;
}  
