#ifndef SORT_HELPER_HPP
#define SORT_HELPER_HPP
#pragma once

#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "table.hpp"

template<typename T>
auto operator<<(std::ostream& os, std::vector<T> const& v) -> std::ostream&
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

enum class sort_type
{
    empty = 0,
    sorted,
    sorted_repeat,
    reversed,
    reversed_repeat,
    shuffled,
    shuffled_repeat,
    random
};

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
    fort::utf8_table m_table{};

protected:
    sort(std::string const name)
        : m_name{ std::move(name) }
    {
        get_tests().push_back(this);

        m_table.set_border_style(FT_NICE_STYLE);

        m_table << fort::header << m_name << ""
                << ""
                << ""
                << ""
                << "";
        m_table << fort::endr;
        m_table << "TYPE"
                << "SUCCESSFUL?"
                << "INPUT_SIZE"
                << "MAX_INPUT"
                << "DURATION(ms)"
                << "DURATION(std::sort)" << fort::endr;
    }

public:
    sort() = delete;
    sort(sort const&) = default;
    virtual ~sort() noexcept = default;

    virtual auto do_sort(std::vector<int> const& input,
                         std::vector<int>& output) -> void = 0;

    auto benchmark(std::vector<int>& input,
                   std::vector<int>& output,
                   std::string const& type,
                   int const array_size,
                   int const biggest_num) -> void
    {
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        this->do_sort(input, output);
        auto end = high_resolution_clock::now();

        auto sort_duration = duration_cast<milliseconds>(end - start).count();

        auto start_std = high_resolution_clock::now();
        std::sort(input.begin(), input.end());
        auto end_std = high_resolution_clock::now();

        auto std_sort_duration =
            duration_cast<milliseconds>(end_std - start_std).count();

        bool successful{ true };
        if(input != output) {
            successful = false;
        }

        m_table << type << (successful ? "YES" : "NO") << array_size
                << biggest_num << sort_duration << std_sort_duration
                << fort::endr;
    }

    [[nodiscard]] auto format() -> std::string
    {
        m_table.row(0)[0].set_cell_text_align(fort::text_align::center);
        m_table.row(0)[0].set_cell_bg_color(fort::color::red);
        m_table.row(0)[0].set_cell_span(6);

        m_table.row(1)[0].set_cell_content_fg_color(fort::color::yellow);
        m_table.row(1)[1].set_cell_content_fg_color(fort::color::red);
        m_table.row(1)[2].set_cell_content_fg_color(fort::color::light_blue);
        m_table.row(1)[3].set_cell_content_fg_color(fort::color::light_blue);
        m_table.row(1)[4].set_cell_content_fg_color(fort::color::green);
        m_table.row(1)[5].set_cell_content_fg_color(fort::color::green);

        for(int i = 2; i < m_table.row_count(); ++i) {
            m_table[i][0].set_cell_text_align(fort::text_align::left);
            m_table[i][1].set_cell_text_align(fort::text_align::center);
            m_table[i][2].set_cell_text_align(fort::text_align::center);
            m_table[i][3].set_cell_text_align(fort::text_align::center);
            m_table[i][4].set_cell_text_align(fort::text_align::right);
            m_table[i][4].set_cell_content_fg_color(fort::color::magenta);
            m_table[i][5].set_cell_text_align(fort::text_align::right);
            m_table[i][5].set_cell_content_fg_color(fort::color::magenta);
        }

        return m_table.to_string();
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
    auto RAND(sort__)::do_sort(                                                \
        [[maybe_unused]] std::vector<int> const& input_name,                   \
        std::vector<int>& output_name)                                         \
        ->void

// Use like this:
//
// NEW_SORT("Countsort", input, output) {
//      std::vector<int> frecv(find_max(input), 0);
//      output.reserve(input.size());
//      // sort...
// }

auto find_max(std::vector<int> const& v) -> int
{
    int max{ 0 };

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
        , m_biggest_number{ max_biggest_number - 1 }
    {
        m_rng.seed(m_rd());
    }

    auto rnd()
    {
        return m_dist(m_rng) % m_biggest_number;
    }

public:
    virtual ~sort_generator() noexcept = default;

    virtual auto generate(std::vector<int>&) -> void = 0;
};

std::mt19937_64 sort_generator::m_rng{};
std::random_device sort_generator::m_rd{};
std::uniform_int_distribution<int> sort_generator::m_dist{
    0, std::numeric_limits<int>::max()
};

class empty_generator final : public sort_generator
{
public:
    empty_generator()
        : sort_generator{ 0, 0 }
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
            std::generate(v.begin(), v.end(), [this, x = 0]() mutable {
                return (this->rnd() % 2 == 0) ? x++ : x;
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
        std::generate(v.begin(), v.end(), [this]() { return this->rnd(); });
        std::shuffle(v.begin(), v.end(), m_rng);
    }
};

auto to_lower(std::string const& str) -> std::string
{
    std::string result;

    for(char const ch : str) {
        result.push_back(std::tolower(ch));
    }

    return result;
}

auto to_sort_type(std::string const& str) -> sort_type
{
    if(to_lower(str) == "empty") {
        return sort_type::empty;
    }
    else if(to_lower(str) == "sorted") {
        return sort_type::sorted;
    }
    else if(to_lower(str) == "sorted_rep") {
        return sort_type::sorted_repeat;
    }
    else if(to_lower(str) == "reversed") {
        return sort_type::reversed;
    }
    else if(to_lower(str) == "reversed_rep") {
        return sort_type::reversed_repeat;
    }
    else if(to_lower(str) == "shuffled") {
        return sort_type::shuffled;
    }
    else if(to_lower(str) == "shuffled_rep") {
        return sort_type::shuffled_repeat;
    }
    else if(to_lower(str) == "random") {
        return sort_type::random;
    }
    else {
        throw "Unknown sort type!";
    }
}

auto make_generator(sort_type const type,
                    int const array_size,
                    int const biggest_num) -> std::unique_ptr<sort_generator>
{
    switch(type) {
    case sort_type::empty:
        return std::make_unique<empty_generator>();
    case sort_type::sorted:
        return std::make_unique<sorted_generator>(array_size);
    case sort_type::sorted_repeat:
        return std::make_unique<sorted_generator>(array_size, true);
    case sort_type::reversed:
        return std::make_unique<reversed_generator>(array_size);
    case sort_type::reversed_repeat:
        return std::make_unique<reversed_generator>(array_size, true);
    case sort_type::shuffled:
        return std::make_unique<shuffled_generator>(array_size);
    case sort_type::shuffled_repeat:
        return std::make_unique<shuffled_generator>(array_size, true);
    case sort_type::random:
        return std::make_unique<shuffled_generator>(array_size, biggest_num);
    default:
        throw "Unknown sort type given!";
    }
}

auto split(std::string const& str, std::string const& pattern)
    -> std::vector<std::string>
{
    std::size_t pos_start = 0;
    std::size_t pos_end;
    std::size_t delim_len = pattern.length();
    std::string token;
    std::vector<std::string> res;

    while((pos_end = str.find(pattern, pos_start)) != std::string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if(!token.empty()) {
            res.push_back(token);
        }
    }

    auto s = str.substr(pos_start);

    if(!s.empty()) {
        res.push_back(s);
    }
    return res;
}

[[nodiscard]] auto check_input(std::string const& config_path,
                               std::map<std::string, config> const& config)
    -> std::vector<std::tuple<sort_type, int, int>>
{
    std::vector<std::tuple<sort_type, int, int>> params;
    std::ifstream f{ config_path };
    std::string line{};

    while(std::getline(f, line)) {
        sort_type type;
        int array_size{ 0 };
        int biggest_num{ 0 };
        std::stringstream ss;

        auto words = split(line, " ");
        auto [sort_name, cfg] = *config.find(
            std::string{ config_path.begin(), config_path.end() - 4 });
        type = to_sort_type(words[0]);

        if(words.size() > 1) {
            array_size = std::stoi(words[1]);

            if(array_size > cfg.max_array_size()) {
                std::cout << sort_name
                          << " array size too large: " << array_size
                          << std::endl;
                std::cout << "Max: " << cfg.max_array_size() << std::endl;
                throw "array_size too large";
            }
        }

        if(words.size() > 2) {
            biggest_num = std::stoi(words[2]);

            if(biggest_num > cfg.max_biggest_number()) {
                std::cout << sort_name
                          << " does not accept numbers larger than "
                          << cfg.max_biggest_number() << std::endl;
                std::cout << "Given: " << biggest_num << std::endl;
                throw "biggest_num too large";
            }
        }
        else {
            biggest_num = array_size;
        }

        params.emplace_back(type, array_size, biggest_num);
    }

    return params;
}

auto to_str(sort_type const type) -> std::string
{
    switch(type) {
    case sort_type::empty:
        return "empty";
    case sort_type::random:
        return "random";
    case sort_type::reversed:
        return "reversed";
    case sort_type::reversed_repeat:
        return "reversed_repeat";
    case sort_type::shuffled:
        return "shuffled";
    case sort_type::shuffled_repeat:
        return "shuffled_repeat";
    case sort_type::sorted:
        return "sorted";
    case sort_type::sorted_repeat:
        return "sorted_repeat";
    default:
        return "";
    }
}

auto main() noexcept -> int
{
    std::map<std::string, config> sort_config{};

    sort_config.emplace(
        std::make_pair("CountSort", config{ 10'000'000, 1'000'000 }));
    sort_config.emplace(std::make_pair(
        "BubbleSort", config{ 7'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(std::make_pair(
        "InsertionSort", config{ 20'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(
        std::make_pair("InsertionSortSTL",
                       config{ 100'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(std::make_pair("RadixSort10", config{}));
    sort_config.emplace(std::make_pair("MergeSort", config{}));
    sort_config.emplace(std::make_pair("MergeSort2", config{}));
    sort_config.emplace(std::make_pair("QuickSort", config{}));
    sort_config.emplace(std::make_pair("QuickSort2", config{}));

    for(auto& test : get_tests()) {
        std::vector<std::tuple<sort_type, int, int>> params{};

        try {
            using namespace std::string_literals;
            params = check_input(test->name() + ".txt"s, sort_config);
        }
        catch(char const* text) {
            std::cout << "Error: " << text << std::endl;
        }

        for(auto const [type, array_size, biggest_num] : params) {
            std::vector<int> input;
            std::vector<int> output;

            make_generator(type, array_size, biggest_num)->generate(input);
            output = input;
            test->benchmark(
                input, output, to_str(type), array_size, biggest_num);
        }

        std::cout << std::endl << test->format() << std::endl << std::endl;
    }
}

#endif // !SORT_HELPER_HPP
