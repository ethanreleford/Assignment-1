#Assignment-1

First I wanted to make a program that could get the prime numbers in a given range of n. 
Next, I would get the sum of all the prime numbers and the count of all the prime numbers.
I used the Sieve of Eratosthenes to find prime numbers since I've heard talk about it being efficient 
but I think my program is still too slow(could just be my machine)
After getting that to work I split the work up between the threads(8) evenly and had them calculate 
their own given range of numbers from the original n kinda like a piece of the pie.
I checked to see the correct calculations of what the outputs should be and they came out correct but the company will go bankrupt with 
the amount of time it takes to run the code.

All in all, the program runs to check to find the execution time of the program, 
the count of prime numbers there are in the range n, and the sum of all the prime numbers in the given range n, and have an array that holds the top ten greatest prime numbers