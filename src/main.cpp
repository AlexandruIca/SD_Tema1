#include <array>
#include <cstddef>
#include <queue>

#include "sort_helper.hpp"

NEW_SORT("CountSort", input, output)
{
    std::vector<int> frecv(find_max(input) + 1, 0);

    for(auto const elem : input) {
        ++frecv[elem];
    }

    int index{ 0 };
    for(int i = 0; i < frecv.size(); ++i) {
        for(int j = 0; j < frecv[i]; ++j) {
            output[index++] = i;
        }
    }
}

NEW_SORT("BubbleSort", input, output)
{
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

    for(int i = 1; i < output.size(); ++i) {
        int j = i - 1;
        while(j >= 0 && output[j] > output[j + 1]) {
            std::swap(output[j], output[j + 1]);
            --j;
        }
    }
}

NEW_SORT("InsertionSortSTL", input, output)
{
    for(auto it = output.begin(); it != output.end(); ++it) {
        std::rotate(std::upper_bound(output.begin(), it, *it), it, it + 1);
    }
}

template<int Base>
auto radix_sort(std::vector<int>& v) -> void
{
    std::array<std::queue<int>, Base> digits;
    int max = find_max(v);
    int pow{ 1 };

    while(max / pow > 0) {
        for(int const num : v) {
            digits[(num / pow) % Base].push(num);
        }

        pow *= Base;
        v.clear();

        for(int i = 0; i < Base; ++i) {
            while(!digits[i].empty()) {
                v.push_back(digits[i].front());
                digits[i].pop();
            }
        }
    }
}

NEW_SORT("RadixSort10", input, output)
{
    radix_sort<10>(output);
}

NEW_SORT("RadixSort2", input, output)
{
    radix_sort<2>(output);
}

auto merge(std::vector<int>& buf,
           std::vector<int>& v,
           int const left,
           int const mid,
           int const right) -> void
{
    int i{ left };
    int j{ mid + 1 };
    int index{ left };

    while(i <= mid && j <= right) {
        if(buf[j] < buf[i]) {
            v[index++] = buf[j++];
        }
        else {
            v[index++] = buf[i++];
        }
    }

    while(i <= mid) {
        v[index++] = buf[i++];
    }
    while(j <= right) {
        v[index++] = buf[j++];
    }

    for(int k = left; k < index; ++k) {
        buf[k] = v[k];
    }
}

auto merge_sort(std::vector<int>& buf,
                std::vector<int>& v,
                int const left,
                int const right) -> void
{
    if(right - left <= 0) {
        return;
    }
    if(right - left == 1) {
        if(buf[left] > buf[right]) {
            return std::swap(buf[left], buf[right]);
        }

        return;
    }

    int const mid = left + (right - left) / 2;
    merge_sort(buf, v, left, mid);
    merge_sort(buf, v, mid + 1, right);
    merge(buf, v, left, mid, right);
}

NEW_SORT("MergeSort", input, output)
{
    std::vector<int> buf = input;
    merge_sort(buf, output, 0, input.size() - 1);
}
