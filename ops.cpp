#include <iostream>
#include <vector> //to use vectors
#include <string> // to use strings
#include <sstream> // to use string stream
#include <cstdlib> // to use for exit


/* Code Written by James Levi, Florida Atlantic University*/


using namespace std;
/*
   These parameters for the main allows the program to get input from the command line.
   This means the program is runnanble different commands and values without changing the code, which is what we need for this to work, since there will always be different commands coming in.
   */
int main(int argc, char* argv[]) {


   //error handling, for if there are not 3 things inside the argument vector (for command line). It must be equal to 3 because, for example ./ops + 10 has 3, argv[0] has the file name, argv[1] has the operator, and argv[2] has the p (number we want to use with the operator)
   if (argc != 3) {
       cout << "ERROR: MUST BE IN THE FORMAT: cat filename.text | ./ops op(operator) p(number)" << endl;
       exit(1);
   }


   //stringstream argv[][2] to convert this command line argument to a double, which can then be used to do math


   string op = argv[1];
   double value;
   stringstream(argv[2]) >> value;


//using a vector as a list because it is dynamic, the size depends on the amount of numbers in the text file
   vector<double> list;
   double number;


   // Read the numbers from the list using cin, and then push it on the vector.
   while (cin >> number) {
       list.push_back(number);
   }


   // if else statements to determine which operator was used. It also prints the values after it is modified (arithmetic) or after it was compared (relational)


   //arithmetic operators
   for (int i = 0; i < list.size(); i++) {
       if (op == "+") {
           list[i] += value;
           cout << list[i] << endl;
       } else if (op == "-") {
           list[i] -= value;
           cout << list[i] << endl;
       } else if (op == "x" || op == "*") {
           list[i] *= value;
           cout << list[i] << endl;
       } else if (op == "/") {
           //check to make sure there is no division by zero
           if (value) {
               list[i] /= value;
               cout << list[i] << endl;
           } else {
               cout << "ERROR: do not divide by zero please! Goodbye" << endl;
               exit(2);
           }
       } else if (op == "%") {
           //modulus only works with integers, so this will static cast them to integers, in order to avoid errors.
           list[i] = static_cast<int>(list[i]) % static_cast<int>(value);
           cout << list[i] << endl;
       }
       //relational operators
       else if (op == "==") {
           if (list[i] == value) {
               cout << list[i] << endl;
           }
       } else if (op == "!=") {
           if (list[i] != value) {
               cout << list[i] << endl;
           }
       } else if (op == "le") {
           if (list[i] <= value) {
               cout << list[i] << endl;
           }
       } else if (op == "lt") {
           if (list[i] < value) {
               cout << list[i] << endl;
           }
       } else if (op == "ge") {
           if (list[i] >= value) {
               cout << list[i] << endl;
           }
       } else if (op == "gt") {
           if (list[i] > value) {
               cout << list[i] << endl;
           }
       } else {
           //if none of these were inputed as op, error.
           cout << "ERROR: INVALID OPERATION" << endl;
       }
   }


    return 0;
}
