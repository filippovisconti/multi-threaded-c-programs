Write a C function with the following interface: 

int above_threshold(char *filename, int n, int threshold).

This function must launch a number of threads / processes equal 
to n to read a sequence of ints from the binary file filename and 
count how many of these have a value greater than threshold. 
Finally, the function prints to standard output and returns the 
total number of int greater than threshold value contained in the 
file.

