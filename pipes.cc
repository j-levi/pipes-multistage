#include <stdio.h>
#include <stdlib.h>// for exit
#include <unistd.h>//for pipe(),fork(),dup2(),execlp(),close(),read().
#include <sys/wait.h>//for wait().




/*
Written by James Levi, Florida Atlantic University
This program creates a pipe between two childs and a parent,


Child 1 redirects stdout (using dup2) to the write end of the pipe and then executes with execlp() the "ps -aux". This will replace the current process image with a new process image running the ps -aux command, which lists all running processes on the system wwith information such as  memory usage, user, CPU usage, PID,etc. Child 1 writes.


Child 2 redirects its input from stdin (using dup 2) to the read end of the pipe, then it executes the "sort -r -n -k 5" command. This command sorts lines of input in reverse order (hense the r) in the fifth collumn (hence the 5). The n signifies it is numerical and the k is for key. Child 2 reads.
*/


int main() {
   int pipes[2]; //0 is for reading and 1 is for writing
   int r;
   r = pipe(pipes);
   if (r < 0) {
       fprintf(stderr, "PIPE HAS FAILED\n\n");
       //stderr is a FILE* variable for the standard error file (terminal)
       exit(2);}
      
  
   int pid2 = fork(); //fork for child 2
   if(pid2<0) { //error handling for child 2 fork
       fprintf(stderr, "FORK HAS FAILED\n\n");
       exit(3);
   }
  
   else if (pid2 == 0) { // Process for child 2
   // Child 2 redirects its input from stdin to the read end of the pipes, then it executes the "sort -r -n -k 5" command.
   dup2(pipes[0], 0); // Redirects stdin to pipe's read end
   close(pipes[0]); // now close both pipe's ends
   close(pipes[1]);
   if (execlp("sort", "sort", "-r", "-n", "-k", "5", (char *)NULL) == -1) { //call execlp(), if returns -1, then exit. If it does not return a value, then it means it was successful.
       fprintf(stderr, "EXECLP HAS FAILED\n");
       exit(4);  // If execlp() fails
   }
}
  
   //parent waits for both children to terminate, so, next step is to fork the other child.
   int pid1=fork();
   if(pid1<0) { //error handling
       fprintf(stderr, "FORK HAS FAILED\n\n");
       exit(3);
   }
  
if (pid1 == 0) { // Process for child 1
   // Child 1 redirects stdout to the write end of the pipes, then it executes the "ps -aux" command.
   dup2(pipes[1], 1); // Redirects stdout to pipe's write end
   close(pipes[0]); // now close both pipe's ends
   close(pipes[1]);
   if (execlp("ps", "ps", "-aux", (char *)NULL) == -1) { // Execute ps -aux command, if returns -1 then error
       fprintf(stderr, "execlp HAS FAILED\n");
       exit(4);  // If execlp fails
   }
}
  


   else {  // PARENT  (pid > 0)
   close(pipes[0]);  // Close both ends of the pipe in the parent
   close(pipes[1]);
   wait(NULL);  // Wait for any child process to finish
   wait(NULL);  // Wait for the other child process to finish


 
   return 0;
}


return(0);
}
