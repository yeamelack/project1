#include "include/utility.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    
    write_filepath_to_submissions("/home/lexxx668/4061/p1/test", "/home/lexxx668/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
    if(!file){
        return -1;
    }
    int index = 0;
    pid_t pids[argc];
    // struct timeval timer[argc];
    char buff[128];
    int scores[argc];

    while(fgets(buff, sizeof(buff), file) != NULL){
        pid_t pid = fork();
        //parent process
        if(pid > 0){
            index++;
            // pid_t terminated_pid = wait(NULL);
            int status;
            waitpid(-1, &status, WNOHANG);

            // if the child process completed return the score and store it 
            if (WIFEXITED(status)){
                scores[index] = WEXITSTATUS(status);
                printf("score is: %d \n", scores[index]);
                printf("pid id: %d \n", pids[index]);
            }
        }
        //child process
        else if(pid == 0){
            //get the p id of the child and store it in the array and start the timer for the process
            pids[index] = getpid();
            // start_timer(&timer[index]);
            execl("/home/lexxx668/4061/p1/submission.txt", argv[index],(char *)NULL);
            // stop_timer(&timer[index]);
        }
    }
}     
