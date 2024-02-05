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

            for(int i = 0; i < argc; i++){
                char string[20];
                sprintf(string, "%d", i);
                memset(string, 0, sizeof(string));
                int ans = execl(buff, buff, string, NULL);


            if(ans == -1){
                perror("execl failed, try again");
                stop_timer(&timer[index]);
            }


            }
            
            
            // if(ans == -1){
            //     perror("execl failed, try again");
            //     stop_timer(&timer[index]);
            // }
           
            
            //stop_timer(&timer[index]);
        }
    }
    fclose(file);
    return 0;
}     

