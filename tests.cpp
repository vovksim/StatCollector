//
// Created by ivan on 04.07.24.
//
#include "stats.hpp"
#include <cassert>
#include <random>

std::vector<int> genDataVector() {
    std::vector<int> r(1000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    std::generate(r.begin(), r.end(), [&]() { return distrib(gen); });
    return r;
}

void emptyDataTest() {
    std::vector<int> emptyVector;
    statistics::comparableAccumulator<std::less<int>> minAccumulator;
    statistics::comparableAccumulator<std::greater<int>> maxAccumulator;
    statistics::avgAccumulator averageAccumulator(emptyVector.size());
    auto ascSeq = statistics::sequenceFinder(emptyVector, std::greater<>());
    auto desSeq = statistics::sequenceFinder(emptyVector, std::less<>());
    auto median = statistics::calcMean(emptyVector);
    std::optional<double> integralExpected = std::nullopt;
    assert(minAccumulator.getResult() == integralExpected);
    assert(maxAccumulator.getResult() == integralExpected);
    assert(averageAccumulator.getResult() == integralExpected);
    assert(median == integralExpected);
    assert(ascSeq.first == ascSeq.second);
    assert(desSeq.first == desSeq.second);
}

void oddDataSizeMedianTest() {
    std::vector<int> oddDataSize{234, 6, 45, 2, 12, 31};
    auto median = statistics::calcMean(oddDataSize);
    auto expected = 21.5;
    assert(median == expected);
}

void evenDataSizeMedianTest() {
    std::vector<int> oddDataSize{8, 6, 3, 21, 243, 64, 6};
    auto median = statistics::calcMean(oddDataSize);
    auto expected = 8;
    assert(median == expected);
}

void overflowRiskMedianTest() {
    std::vector<int> oddDataSize;
    oddDataSize.assign(10, std::numeric_limits<int>::max());
    auto median = statistics::calcMean(oddDataSize);
    auto expected = std::numeric_limits<int>::max();
    assert(median.value() == expected);
}

void comparableAccumulatorRandomTest() {
    const int TEST_QUANTITY = 10;
    for (int i = 0; i < TEST_QUANTITY; i++) {
        auto randData = genDataVector();
        statistics::comparableAccumulator<std::less<>> min;
        statistics::comparableAccumulator<std::greater<>> max;
        for (auto value: randData) {
            min.accumulate(value);
            max.accumulate(value);
        }
        auto expected = *std::min_element(randData.begin(), randData.end());
        assert(min.getResult() == expected);
    }
}

void increasingSequenceFindTest() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    auto result = statistics::sequenceFinder(data, std::less<>());
    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    assert(std::vector<int>(result.first, result.second) == expected);
}

void decreasingSequenceFindTest() {
    std::vector<int> data = {5, 4, 3, 2, 1};
    auto result = statistics::sequenceFinder(data, std::greater<>());
    std::vector<int> expected = {5, 4, 3, 2, 1};
    assert(std::vector<int>(result.first, result.second) == expected);
}

void decreasingSubSequenceFindTest() {
    std::vector<int> data = {1, 2, 3, 2, 1, 4, 5, 6, 7, 8};
    auto result = statistics::sequenceFinder(data, std::greater<>());
    std::vector<int> expected = {3, 2, 1};
    assert(std::vector<int>(result.first, result.second) == expected);
}

void increasingSubSequenceFindTest() {
    std::vector<int> data = {1, 2, 3, 2, 1, 4, 5, 6, 7, 8};
    auto result = statistics::sequenceFinder(data, std::less<>());
    std::vector<int> expected = {1, 4, 5, 6, 7, 8};
    assert(std::vector<int>(result.first, result.second) == expected);
}

void uniformSubSequenceFindTest() {
    std::vector<int> data = {1, 1, 1, 1, 1, 1, 1, 1};
    auto result = statistics::sequenceFinder(data, std::less<>());
    std::vector<int> expected = {1};
    assert(std::vector<int>(result.first, result.second) == expected);
}

int main() {
    emptyDataTest();
    oddDataSizeMedianTest();
    evenDataSizeMedianTest();
    overflowRiskMedianTest();
    comparableAccumulatorRandomTest();
    increasingSequenceFindTest();
    decreasingSequenceFindTest();
    increasingSubSequenceFindTest();
    decreasingSubSequenceFindTest();
    uniformSubSequenceFindTest();
}