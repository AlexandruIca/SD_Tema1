#include <cstddef>

#include "sort_helper.hpp"

NEW_SORT("CountSort", input, output)
{
    std::vector<int> frecv(find_max(input), 0);
    output.reserve(input.size());

    for(auto const elem : input) {
        ++frecv[elem];
    }

    for(int i = 0; i < input.size(); ++i) {
        for(int j = 0; j < frecv[i]; ++j) {
            output.push_back(i);
        }
    }
}
