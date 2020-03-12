#ifndef SORT_HELPER_HPP
#define SORT_HELPER_HPP
#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

class sort;

auto get_tests() -> std::vector<sort*>&
{
    static std::vector<sort*> tests;
    return tests;
}

class sort
{
private:
    std::string m_name{};

protected:
    sort(std::string const name)
        : m_name{ std::move(name) }
    {
        get_tests().push_back(this);
    }

public:
    sort() = delete;
    virtual ~sort() noexcept = default;

    virtual auto do_sort(std::vector<int> const& input,
                         std::vector<int>& output) -> void = 0;

    auto benchmark() -> void
    {
    }

    [[nodiscard]] inline auto name() const noexcept -> std::string const&
    {
        return m_name;
    }
};

#define PASTE(a, b) a##b
#define PASTE_2(a, b) PASTE(a, b)
#define RAND(name) PASTE_2(name, __LINE__)

#define NEW_SORT(sort_name, input_name, output_name)                           \
    class RAND(sort__)                                                         \
        : public sort                                                          \
    {                                                                          \
    public:                                                                    \
        RAND(sort__)                                                           \
        ()                                                                     \
            : sort{ sort_name }                                                \
        {                                                                      \
        }                                                                      \
        virtual ~RAND(sort__)() noexcept override = default;                   \
                                                                               \
        auto do_sort(std::vector<int> const&, std::vector<int>&)               \
            -> void override;                                                  \
    };                                                                         \
    static RAND(sort__) RAND(sort_tmp_var__);                                  \
                                                                               \
    auto RAND(sort__)::do_sort(std::vector<int> const& input_name,             \
                               std::vector<int>& output_name)                  \
        ->void

// Use like this:
//
// NEW_SORT("Countsort", input, output) {
//      std::vector<int> frecv(find_max(input), 0);
//      output.reserve(input.size());
//
//      for(int const elem : input) {
//          ++frecv[elem];
//      }
//
//      for(int i = 0; i < static_cast<int>(input.size()); ++i) {
//          for(int j = 0; j < frecv[i]; ++j) {
//              output.push_back(i);
//          }
//      }
// }

auto find_max(std::vector<int> const& v) -> int
{
    int max{ std::numeric_limits<int>::min() };

    for(int const elem : v) {
        if(elem > max) {
            max = elem;
        }
    }

    return max;
}

auto main() noexcept -> int
{
    std::vector<int> out;
    for(auto& test : get_tests()) {
        test->do_sort({ 1, 2, 3, 4 }, out);
        std::cout << test->name() << std::endl;
        out.clear();
    }
}

#endif // !SORT_HELPER_HPP
