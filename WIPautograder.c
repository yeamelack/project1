#include <sys/types.h>
#include <signal.h>
#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128



int casecheck(pid_t pid) {
    char ans[255];
    char path[255];

    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(ans, sizeof(ans), file) != NULL) {
        if (strncmp(ans, "State:", 6) == 0) {
            fclose(file);
            printf("State: %s\n", ans);
            if (ans[7] == 'R' || ans[7] == 'r') {
                return 1;
            } else {
                return 0;
            }
        }
    }

    fclose(file);
    return 0;  // If state not found, consider it as not running
}

int main(int argc, char *argv[]) {

    write_filepath_to_submissions("test_solutions2", "submission.txt");
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
    for(int i =0 ;i<total_executables; i++){
    printf("array stu %s\n", students[i] );
    }

    pid_t pids[batch_size];
    int count = 0;
    
    for(int i = 2; i < argc; i++){
        int done_executables = 0;

        while (done_executables < total_executables){
            for(int j = 0; j < batch_size; j++){
                pid_t pid = fork();
                    if (pid < 0) {
                        perror("fork failed");
                        return 1;
                    }
                    else if(pid == 0){
                        printf("student: %s, answer%s, pid: %d\n", students[count], argv[i], getpid());
                        printf("get pid %d\n", getpid());
                        //start_timer(&start);
                        execl(students[count], argv[1], argv[i], NULL);
                        perror("exec failed\n");
                        exit(1);
                    }
                    else {
                        pids[j] = pid;
                        printf("%s %d %d\n", "child ", j, pids[j]);
                        
                    }       
                count++;
                
            }

            // for(int i=0; i<2; i++){
            //         printf("pids array at %d is %d:\n", i, pids[i]);
            //     }
            

            int finished = 0;
            // while(finished < batch_size){
                for(int i=0; i<2; i++){
                    printf("pids array at %d is %d:\n", i, pids[i]);
                }
                for(int k=0; k < batch_size; k++){
                    int status = 0;
                    printf("child getting checked %d pids at %d\n",k, pids[k]);

                    int pidStatus = waitpid(pids[k], &status, WNOHANG);
                    printf("pidstat getting checked %d\n", pidStatus);
                        if(pidStatus == -1){
                            printf("error in process\n");
                            pids[0] = 0;
                            finished++;             
                        }

                        if (pidStatus == 0){
                            printf("child processes still running\n");
                        }

                         // if pidstatus is greater than 0 it means child terminated 
                if (pidStatus > 0){//BRANCH GOOD
                    // if what WIFEXITED returns is > 0 it means the child succesfully terminated
                    if(WIFEXITED(status) > 0){ 
                        //checking the return status of the succesfully terminated child
                        int ret = WEXITSTATUS(status);
                        if(ret == 0){ // CORRECT
                            printf("%s", "correct \n");

                            pids[i] = 0;
                            finished++;
                        }
                        else if(ret == 1){ //INCORRECT
                            printf("%s", "incorrect\n");

                            finished++; 
                            pids[i] = 0;
                        }
                    }




                }

            }
            
            printf("done exe %d\n", done_executables);
            printf("total exe %d\n", total_executables);
         }
         done_executables += batch_size;




        


    }
    fclose(file);
}

