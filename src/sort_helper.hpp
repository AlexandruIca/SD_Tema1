#ifndef SORT_HELPER_HPP
#define SORT_HELPER_HPP
#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <random>
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

class config
{
private:
    int m_max_array_size{ 10'000'000 };
    int m_max_biggest_number{ std::numeric_limits<int>::max() };

public:
    constexpr config() noexcept = default;
    constexpr config(int const max_arr_size, int const max_big_num)
        : m_max_array_size{ max_arr_size }
        , m_max_biggest_number{ max_big_num }
    {
    }
    ~config() noexcept = default;

    [[nodiscard]] constexpr auto max_array_size() const noexcept -> int
    {
        return m_max_array_size;
    }

    [[nodiscard]] constexpr auto max_biggest_number() const noexcept -> int
    {
        return m_max_biggest_number;
    }
};

class sort_generator
{
protected:
    static std::mt19937_64 m_rng;
    static std::random_device m_rd;
    static std::uniform_int_distribution<int> m_dist;

    int m_array_size{ 0 };
    int m_biggest_number{ 0 };

    sort_generator() noexcept = default;
    sort_generator(int const array_size, int const max_biggest_number) noexcept
        : m_array_size{ array_size }
        , m_biggest_number{ max_biggest_number }
    {
        m_rng.seed(m_rd());
        m_dist.param(std::uniform_int_distribution<int>::param_type{
            0, m_biggest_number - 1 });
    }

    static auto rnd()
    {
        return m_dist(m_rng);
    }

public:
    virtual ~sort_generator() noexcept = default;

    virtual auto generate(std::vector<int>&) -> void = 0;
};

std::mt19937_64 sort_generator::m_rng{};
std::random_device sort_generator::m_rd{};
std::uniform_int_distribution<int> sort_generator::m_dist{};

class empty_generator final : public sort_generator
{
public:
    empty_generator()
        : sort_generator{}
    {
    }
    ~empty_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        v.clear();
    }
};

class sorted_generator final : public sort_generator
{
private:
    bool m_repeat{ false };

public:
    sorted_generator() = delete;
    sorted_generator(int const array_size, bool repeat = false)
        : sort_generator{ array_size, array_size }
        , m_repeat{ repeat }
    {
    }
    ~sorted_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        v.resize(m_array_size);

        if(m_repeat) {
            std::generate(v.begin(), v.end(), [x = 0]() mutable {
                return (rnd() % 2 == 0) ? x++ : x;
            });
        }
        else {
            std::generate(
                v.begin(), v.end(), [x = 0]() mutable { return x++; });
        }
    }
};

class reversed_generator final : public sort_generator
{
private:
    bool m_repeat{ false };

public:
    reversed_generator() = delete;
    reversed_generator(int const array_size, bool repeat = false)
        : sort_generator{ array_size, array_size }
        , m_repeat{ repeat }
    {
    }
    ~reversed_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        sorted_generator g{ m_array_size, m_repeat };
        g.generate(v);
        std::reverse(v.begin(), v.end());
    }
};

class shuffled_generator final : public sort_generator
{
private:
    bool m_repeat{ false };

public:
    shuffled_generator() = delete;
    shuffled_generator(int const array_size, bool repeat = false)
        : sort_generator{ array_size, array_size }
        , m_repeat{ repeat }
    {
    }
    ~shuffled_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        sorted_generator g{ m_array_size, m_repeat };
        g.generate(v);
        std::shuffle(v.begin(), v.end(), m_rng);
    }
};

class random_generator final : public sort_generator
{
public:
    random_generator() = delete;
    random_generator(int const array_size, int const biggest_num)
        : sort_generator{ array_size, biggest_num }
    {
    }
    ~random_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        v.resize(m_array_size);
        std::generate(v.begin(), v.end(), []() { return rnd(); });
        std::shuffle(v.begin(), v.end(), m_rng);
    }
};

auto operator<<(std::ostream& os, std::vector<int> const& v) -> std::ostream&
{
    os << '[';
    if(v.size() >= 1) {
        os << v.front();

        if(v.size() >= 2) {
            for(int i = 1; i < v.size(); ++i) {
                os << ", " << v[i];
            }
        }
    }
    os << ']';
    return os;
}

auto main() noexcept -> int
{
    std::vector<int> v;
    {
        empty_generator g{};
        g.generate(v);
        std::cout << "Empty: " << v << std::endl;
    }
    v.clear();
    {
        sorted_generator g{ 100 };
        g.generate(v);
        std::cout << "Sorted:\n" << v << std::endl;
    }
    v.clear();
    {
        sorted_generator g{ 100, true };
        g.generate(v);
        std::cout << "Sorted(repeat):\n" << v << std::endl;
    }
    v.clear();
    {
        reversed_generator g{ 100 };
        g.generate(v);
        std::cout << "Reversed:\n" << v << std::endl;
    }
    v.clear();
    {
        reversed_generator g{ 100, true };
        g.generate(v);
        std::cout << "Reversed(repeat):\n" << v << std::endl;
    }
    v.clear();
    {
        shuffled_generator g{ 100 };
        g.generate(v);
        std::cout << "Shuffled:\n" << v << std::endl;
    }
    v.clear();
    {
        shuffled_generator g{ 100, true };
        g.generate(v);
        std::cout << "Shuffled(repeat):\n" << v << std::endl;
    }
    v.clear();
    {
        random_generator g{ 100, 100000 };
        g.generate(v);
        std::cout << "Random:\n" << v << std::endl;
    }
    /*
    std::map<std::string, config> sort_config{};

    sort_config.emplace(
        std::make_pair("CountSort", config{ 10'000'00, 1'000'000 }));

    std::ifstream input_config{ "sort_config.txt" };

    std::vector<int> out;

    for(auto& test : get_tests()) {
        test->do_sort({ 1, 2, 3, 4 }, out);
        std::cout << test->name() << std::endl;
        out.clear();
    }*/
}

#endif // !SORT_HELPER_HPP
