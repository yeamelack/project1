#include "include/utility.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    
    write_filepath_to_submissions("/home/erist003/4061/p1/test", "/home/erist003/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
    if(!file){
        return -1;
    }
    int index = 0;
    pid_t pids[argc];
    struct timeval timer[argc];
    char buff[128];

    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        pid_t pid = fork();

        if(pid > 0){
            pid_t terminated_pid = wait(NULL);
        }
        else if(pid == 0){
            pids[index] =  getpid();
            start_timer(&timer[index]);

            int ans = execl(buff, buff, NULL);
            if(ans == -1){
                perror("execl failed, try again");
                stop_timer(&timer[index]);
            }
           
            
            //stop_timer(&timer[index]);
        }
    }
}     

