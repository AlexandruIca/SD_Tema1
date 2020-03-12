#include <cstddef>

#include "sort_helper.hpp"

NEW_SORT("CountSort", input, output)
{
    std::vector<int> frecv(find_max(input) + 1, 0);
    output.reserve(input.size());

    for(auto const elem : input) {
        ++frecv[elem];
    }

    for(int i = 0; i < frecv.size(); ++i) {
        for(int j = 0; j < frecv[i]; ++j) {
            output.push_back(i);
        }
    }
}

NEW_SORT("BubbleSort", input, output)
{
    output =
        input; // array_size for bubble sort is not big, we can afford to copy
    bool sorted{ true };

    do {
        sorted = true;
        for(int i = 1; i < output.size(); ++i) {
            if(output[i] < output[i - 1]) {
                std::swap(output[i], output[i - 1]);
                sorted = false;
            }
        }
    } while(!sorted);
}

NEW_SORT("InsertionSort", input, output)
{
    if(input.empty()) {
        return;
    }

    output.reserve(input.size());
    output.push_back(input[0]);

    for(int i = 1; i < input.size(); ++i) {
        output.push_back(input[i]);

        int j = i - 1;
        while(j >= 0 && output[j] > input[i]) {
            std::swap(output[j], output[j + 1]);
            --j;
        }
    }
}
