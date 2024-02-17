How to Compile Program:

In the terminal type:
1. "make N=x" where x is the number of binaries you want to create, or you can also do "make" to create the default number
2. "./autograder <B> <pi>"   <B> = batch size, <pi> = test input parameters (can have multiple, each seperated with a space), they are integers
3. "make clean N=x" where x is the number of binaries to remove, or you can do "make clean" to remove the default number

Assumptions:
We were told by one of the ta's that the use of the timer functions were optional, so we just used a counter.


Team ID: Group 35
Member Names and x500's: Bhuvan Sakhamuru (sakha019), Van Le (lexxx668), Yeamelack Erist (erist003)

Contributions:

Bhuvan : Worked on printing to file and ctemplate.c and also having the outputs in the right order
Yeamelack: Worked on the logic and code to make sure we get the correct output
Van: Worked on the logic and code to make sure we get the correct output

Answer to question: with parallelism the benefit we saw was we could run multiple different problems at the same time, which was faster than running them one by one. but the proformance wouldnt improve in the same way for default soluion executable template because were not really computing anything within the code most of the time spent on it is the overhead of system calls, forking, waiting child processe, etc. since were arent computing anything or doing anything cpu intensive we see little to no benefits of running this in parallel with each other.



