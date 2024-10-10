
//PROBLEM 2b: 
//CODE: 
#include <iostream>
#include <string> //use for strings
#include <vector> //use for vectors
#include <unistd.h> // use for the pipe operations
#include <sys/wait.h>// use for wait (when parent waits)
#include <sstream> //use for string stream: manipulate strings as streams
#include <cstring> //need it for char* and strcpy()
#include <cstdlib>

/* Code written by James Levi, Florida Atlantic University.
This program executes a series of commands connected by pipes. It takes input from the user, parces the command line into individual commands separated by '|'. Create pipes to connect the commands. For each command, for and parce that command into arguments to then execute, and repeat for all children. Parent waits. 
*/

using namespace std;

const int NMAX = 1000; // Define a reasonable maximum number of commands

int main() {
    string user_input;
    cout << "Enter pipe command: " << endl;
    getline(cin, user_input);

    // Convert a standard input C++ string to a char array for strtok, char array is a C style string
    char* input_c_string = new char[user_input.length() + 1]; //create a dynamic array of characters using new operator 
    strcpy(input_c_string, user_input.c_str()); // copy content of user's C++ string to the input_c_string. We cannot directly use user_input.c_str() as a C style string because it is of type const, so we must make a copy.

    // Split input command by the pipe character '|', PARCING
    vector<string> coms;
    char* token = strtok(input_c_string, "|");
    while (token != nullptr) {
        coms.push_back(string(token));
        token = strtok(nullptr, "|");
    }

// this will store the individual coms that are separated by the '|', so, for example, for 
// cat James.text | ./ops + 10 | ./ops x 2, it would look like this:
                       /*
                       coms[0]: "cat numbers.txt"
                       coms[1]: "./ops + 10"
                       coms[2]: "./ops x 2"
                       */

    int numComs = coms.size(); // setting a variable for size of the vector of strings of commands. Indicates how man commands there are between the '|'s.

    int numPipes= numComs-1; // We use numComs-1 since, intuitively, if each pipe connects two commands to eachother, we need one pipe less than we do commands.

//error handling, making sure there are not too many commands (very  unlikely since NMAX is very large)
    if (numComs > NMAX) {
        cerr << "TOO MANY COMMANDS! CANNOT HAVE MORE THAN: " << NMAX << endl;
        delete[] input_c_string; //free the memory that was dynamically allocated using the new char above (C style Input), we do this to avoid potential memory leak. 
        exit(2); //end program if too big with return code 2
    }

    int pfd[NMAX][2]; // Array to store pipe file descriptors

    // Create pipes using a for loop. 

    
    for (int i = 0; i < numPipes; ++i) {
        if (pipe(pfd[i]) == -1) 
        //this creates a pipe using the pipe function which takes an array of two integers as a paramater.
        //pfd[i][0] is for reading 
        //pfd[i][1] is for writing
        //Visually: each row represent a pipe: pfd[0], pfd[1], pfd[2], and then each collum represents either the read or write end of that pipe. so pdf[0][0] and pdf[0][1]. 
        {
            cout << "PIPE CREATION HAS FAILED :(" << endl;//if it returns -1, fails
            delete[] input_c_string; //deallocate memory
            exit(1);
        }
    }

    // Fork and exec each command
        
    for (int i = 0; i < numComs; ++i) 
    {
        pid_t pid = fork(); //forking to create child processes

        if (pid == -1) { //error handling in case fork fails
            cout << "FORK HAS FAILED :(" << endl;
            delete[] input_c_string; //deallocate memory to prevent memory leak
            exit(1); //return with exit code

        } else if (pid == 0) { // Child process
            // Redirect input using dup2
            if (i != 0) //if i=0, then it is the first pipe, there is no previos pipe. so if i=0, no need to use dup2.
             {
                dup2(pfd[i - 1][0], 0);
            }

            // Redirect output using dup2
            if (i != numPipes) //similar to above, if this is the last pipe, there is no next pipe, so there is no need to use dup2.
             {
                dup2(pfd[i][1], 1);
            }

            // Close all pipe file descriptors
            for (int j = 0; j < numPipes; ++j) {
                //closes both the read (0) and write (1) of the pipes of all pipes of this child.
                close(pfd[j][0]);
                close(pfd[j][1]);
            }
            
            //Parse the each command into arguments
            //Before, we parsed the command line into individual commands, now, we are parsing each command into individual arguments, so that it can be used by execvp! This is in the for loop so this will happen for each child.
            vector<char*> args; //create a C-ctyle string of arguments, for exanple, this is how it would look for cat James.text | ./ops + 10, so two commands cat James.text and ./ops + 10. Then the arguments for the first child would be cat and James.text.

            char* command_c_string = new char[coms[i].length() + 1]; //declare command string into a C style string
            strcpy(command_c_string, coms[i].c_str()); //copies into the commsnf_c_string (same process that we did when parcing originally) 

            char* arg_token = strtok(command_c_string, " "); //tokenizes based on spaces between arguments. We declare a c style sting

            while (arg_token != nullptr)//strtok returns nullptr when it is done.
             { 
                //pushes the token onto the args vector.
                args.push_back(arg_token);
                arg_token = strtok(nullptr, " ");
            }
            args.push_back(nullptr); //pushes null at the end of it, execvp requires the vector to be null terminated

            execvp(args[0], args.data()); 
            cout<<"ERROR: execvp failed!";// this only runs if execvp fails, since it is below.
            exit(1); // Exit if execvp fails
        }
        // Parent does nothing 
    }

    // Close all the pipes in the parent process
    for (int i = 0; i < numPipes; ++i) {
        close(pfd[i][0]);
        close(pfd[i][1]);
    }

    // Parent waits for all child processes to finish
    for (int i = 0; i < numComs; ++i) {
        wait(NULL);
    }

    delete[] input_c_string; // Free the memory allocated for the input string

    return 0;
}