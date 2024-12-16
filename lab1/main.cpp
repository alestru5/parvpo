#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#define START_RANGE 10
#define END_RANGE 100000000

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

    std::cout << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
