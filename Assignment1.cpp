#include <iostream>
#include <fstream> 
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#include <chrono>
#include <iomanip> 

//global variables that would be established
const int threadNumber = 8;
const int starting = 0;
const int ending = 100000000;

//track the output values needed
std::mutex mtx;
int primeCount = 0;
long long sumOfPrimes = 0;

//check to see if the num is prime return true if it is, false otherwise
bool isPrime(int num)
{

    if (num < 2)
    {
        return false;
    }
    for (int i = 2; i <= sqrt(num); ++i)
    {
        if (num % i == 0)
        {
            return false;
        }
    }
    return true;
}

//add to the output global variables in each of the threads from the given range
void countPrimesInRange(int start, int end)
{
  
    int localCount = 0;
    long long localSum = 0;

    for (int i = start; i <= end; i ++)
    {

        if (isPrime(i))
        {
            ++localCount;
            localSum += i;
        }
    }

    std::lock_guard<std::mutex> lock(mtx);
    primeCount += localCount;
    sumOfPrimes += localSum;
}

//main 
int main()
{
    std::vector<std::thread> threads;
    
    auto startTime = std::chrono::high_resolution_clock::now();

    //split the work between the threads
    int rangeSize = (ending - starting + 1) / threadNumber;
    for (int i = 0; i < threadNumber; ++i)
    {
        int start = starting + i * rangeSize;
        int end = (i == threadNumber - 1) ? ending : start + rangeSize - 1;
        threads.push_back(std::thread(countPrimesInRange, start, end));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto executionTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();

    // open a file
    std::ofstream outputFile("primes.txt"); 

    if (outputFile.is_open())
    {
        //write to the file
        outputFile << std::fixed << std::setprecision(2) << executionTime << " seconds "
                   << primeCount << " " << sumOfPrimes << std::endl;

        //close the file
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file for writing." << std::endl;
        return 1; //error
    }

    return 0;
}
