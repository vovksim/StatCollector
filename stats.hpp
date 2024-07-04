//
// Created by ivan on 04.07.24.
//

#ifndef STATCOLLECTOR_STATS_HPP
#define STATCOLLECTOR_STATS_HPP

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <optional>
#include <iomanip>

namespace fs = std::filesystem;
using range = std::pair<std::vector<int>::const_iterator, std::vector<int>::const_iterator>;

namespace st {
    std::optional<double> calcMean(std::vector<int> data) {
        if (data.begin() == data.end()) {
            return {};
        }

        auto mid = data.begin() + data.size() / 2;


        if (data.size() % 2 == 1) {
            std::nth_element(data.begin(), mid, data.end());
            return {*mid};
        } else {
            auto midNeighbour = data.begin() + data.size() / 2 - 1;
            std::nth_element(data.begin(), mid, data.end());
            std::nth_element(data.begin(), midNeighbour, data.end());
            return {static_cast<double>(*mid + *midNeighbour) / 2.0};
        }
    }

    template<typename Comparator>
    range sequenceFinder(const std::vector<int> &data, Comparator comp) {
        if (data.begin() == data.end() || data.size() == 1) {
            return std::make_pair(data.begin(), data.end());
        }

        auto start = data.begin();
        auto end = data.begin();

        auto bestStart = data.begin();
        auto bestEnd = data.begin();

        for (auto iter = data.begin() + 1; iter != data.end(); iter++) {
            if (comp(*end, *iter)) {
                end++;
            } else {
                if (std::distance(start, end) > std::distance(bestStart, bestEnd)) {
                    bestStart = start;
                    bestEnd = end;
                }
                start = iter;
                end = iter;
            }
        }
        return std::make_pair(bestStart, bestEnd + 1);
    }

    class avgAccumulator {
        std::optional<double> result;
        std::size_t length;
    public:

        void accumulate(int value) {
            result = result.value() + value / static_cast<double>(length);
        }

        std::optional<double> getResult() const {
            return result;
        }

        explicit avgAccumulator(std::size_t size) {
            length = size;
            result = 0;
        };
    };

    template<typename Comparator>
    class comparableAccumulator {
        std::optional<int> result;
    public:

        void accumulate(int value) {
            if (result.has_value()) {
                if (Comparator comp; comp(value, result.value())) {
                    result = value;
                }
            } else {
                result = value;
            }
        }

        std::optional<int> getResult() const {
            return result;
        }
    };
}

#endif //STATCOLLECTOR_STATS_HPP
