#include <cassert>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Usage: ./day1 <input file>" << std::endl;
        return 1;
    }
    std::ifstream input_file(argv[1]);
    std::vector<int> list1;
    std::vector<int> list2;
    while(input_file.good()) {
        int a, b;
        input_file >> a >> b;
        list1.push_back(a);
        list2.push_back(b);
    }
    // Sizes
    std::cout << "List 1 size: " << list1.size() << std::endl;
    std::cout << "List 2 size: " << list2.size() << std::endl;
    assert(list1.size() == list2.size());
    // First 6
    std::cout << "List 1 first 6: " << list1[0] << " " << list1[1] << " " << list1[2]
        << " " << list1[3] << " " << list1[4] << " " << list1[5] << std::endl;
    std::cout << "List 2 first 6: " << list2[0] << " " << list2[1] << " " << list2[2]
        << " " << list2[3] << " " << list2[4] << " " << list2[5] << std::endl;
    // Last 6
    std::cout << "List 1 last 6: " << list1[list1.size() - 1] << " "
        << list1[list1.size() - 2] << " " << list1[list1.size() - 3]
        << " " << list1[list1.size() - 4] << " " << list1[list1.size() - 5]
        << " " << list1[list1.size() - 6] << std::endl;
    std::cout << "List 2 last 6: " << list2[list2.size() - 1] << " "
        << list2[list2.size() - 2] << " " << list2[list2.size() - 3]
        << " " << list2[list2.size() - 4] << " " << list2[list2.size() - 5]
        << " " << list2[list2.size() - 6] << std::endl;
    // Sort
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());
    // Compute the distance
    const uint64_t N = list1.size();
    uint64_t distance = 0;
    for(uint64_t i = 0; i < N; ++i) {
        distance += std::abs(list2[i] - list1[i]);
    }
    std::cout << "The total distance is " << distance << std::endl;
    // Compute the similarity
    std::unordered_map<int, int> list2_counts;
    list2_counts.reserve(N);
    for(const auto value : list2) {
        auto itr = list2_counts.find(value);
        if(itr == list2_counts.end()) {
            list2_counts.insert({value, 1});
        }
        else {
            itr->second++;
        }
    }
    uint64_t similarity = 0;
    for(const auto value : list1) {
        auto itr = list2_counts.find(value);
        if(itr != list2_counts.end()) {
            similarity += value * itr->second;
        }
    }
    std::cout << "The total similarity score is " << similarity << std::endl;
    return 0;
}
