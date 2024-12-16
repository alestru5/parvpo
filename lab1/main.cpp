#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <cmath>

#define START_RANGE 10
#define END_RANGE 300000000

std::vector<int> findPrimesInRange(int start, int end) {
    if (start > end) {
        std::cerr << "Invalid range" << std::endl;
        return {};
    }

    int maxNum = end;
    std::vector<bool> isPrime(maxNum + 1, true);
    isPrime[0] = isPrime[1] = false;

    for (int p = 2; p * p <= maxNum; ++p) {
        if (isPrime[p]) {
            for (int i = p * p; i <= maxNum; i += p) {
                isPrime[i] = false;
            }
        }
    }

    std::vector<int> primes;
    for (int i = std::max(start, 2); i <= end; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> primes = findPrimesInRange(START_RANGE, END_RANGE);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::ofstream outputFile("result.txt");
    if (!outputFile) {
        std::cerr << "Unable to open file for writing" << std::endl;
        return 1;
    }

    outputFile << std::endl;

    outputFile << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;

    outputFile.close();
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
