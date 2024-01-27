#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <algorithm> 

//global variables 
const int threadNumber = 8;
const int starting = 0;
const int ending = 100000000;
const int topTenSize = 10;

//output values needed
std::mutex mtx;
int primeCount = 0;
long long sumOfPrimes = 0;
int topTenPrimes[topTenSize]; 

//check to see if the num is prime; return true if it is, false otherwise
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

// dd to the output global variables and top ten primes in each of the threads from the given range n
void countPrimesInRange(int start, int end)
{
    int localCount = 0;
    long long localSum = 0;
    int localTopTenPrimes[topTenSize] = {0};

    for (int i = start; i <= end; i++)
    {
        if (isPrime(i))
        {
            ++localCount;
            localSum += i;

            // Update local top ten primes
            for (int j = 0; j < topTenSize; ++j)
            {
                if (localTopTenPrimes[j] == 0 || i > localTopTenPrimes[j])
                {
                    for (int k = topTenSize - 1; k > j; --k)
                    {
                        localTopTenPrimes[k] = localTopTenPrimes[k - 1];
                    }
                    localTopTenPrimes[j] = i;
                    break;
                }
            }
        }
    }

    std::lock_guard<std::mutex> lock(mtx);
    primeCount += localCount;
    sumOfPrimes += localSum;

    //merge top ten primes into the global top ten primes
    for (int i = topTenSize - 1; i >= 0; --i)
    {
        if (localTopTenPrimes[i] == 0)
        {
            continue;
        }
        for (int j = 0; j < topTenSize; ++j)
        {
            if (topTenPrimes[j] == 0 || localTopTenPrimes[i] > topTenPrimes[j])
            {
                for (int k = topTenSize - 1; k > j; --k)
                {
                    topTenPrimes[k] = topTenPrimes[k - 1];
                }
                topTenPrimes[j] = localTopTenPrimes[i];
                break;
            }
        }
    }
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

    //open file
    std::ofstream outputFile("primes.txt");

    if (outputFile.is_open())
    {
        //write to file
        outputFile << std::fixed << std::setprecision(2) << executionTime << " seconds "
                   << primeCount << " " << sumOfPrimes << std::endl;

        //write top ten primes to file
        for (int i = topTenSize - 1; i >= 0; --i)
        {
            if (topTenPrimes[i] != 0)
            {
                outputFile << topTenPrimes[i] << " ";
            }
        }
        outputFile << std::endl;

        //close file
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file for writing." << std::endl;
        return 1; //error
    }

    return 0;
}
