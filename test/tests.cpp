//
// Created by ivan on 04.07.24.
//
#include "../src/stats.hpp"
#include <cassert>
#include <random>

std::vector<int> genDataVector() {
    const int MIN_VECTOR_SIZE = 1;
    const int MAX_VECTOR_SIZE = 1000;
    std::vector<int> r;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    std::uniform_int_distribution<int> distribLength(MIN_VECTOR_SIZE, MAX_VECTOR_SIZE);
    auto length = static_cast<std::size_t>(distribLength(gen));
    r.assign(length, 0);
    for (auto &value: r) {
        value = distrib(gen);
    }
    return r;
}

void emptyDataTest() {
    std::vector<int> emptyVector;
    st::comparableAccumulator<std::less<int>> minAccumulator;
    st::comparableAccumulator<std::greater<int>> maxAccumulator;
    st::avgAccumulator averageAccumulator(emptyVector.size());
    auto ascSeq = st::sequenceFinder(emptyVector, std::greater<>());
    auto desSeq = st::sequenceFinder(emptyVector, std::less<>());
    auto median = st::calcMean(emptyVector);

    assert(minAccumulator.getResult() == std::nullopt);
    assert(maxAccumulator.getResult() == std::nullopt);
    assert(averageAccumulator.getResult() == std::nullopt);
    assert(ascSeq.first == ascSeq.second);
    assert(desSeq.first == desSeq.second);
    assert(median == std::nullopt);
}

void oddDataSizeMedian() {
    std::vector<int> oddDataSize{234, 6, 45, 2, 12, 31};
    auto median = st::calcMean(oddDataSize);
    assert(median == 21.5);
}

void evenDataSizeMedian() {
    std::vector<int> oddDataSize{8, 6, 3, 21, 243, 64, 6};
    auto median = st::calcMean(oddDataSize);
    assert(median == 8);
}

void overflowRiskMedian() {
    std::vector<int> oddDataSize;
    oddDataSize.assign(10, std::numeric_limits<int>::max());
    auto median = st::calcMean(oddDataSize);
    assert(median.value() == std::numeric_limits<int>::max());
}

void comparableAccumulatorRandomTest() {
    const int TEST_QUANTITY = 10;
    for (int i = 0; i < TEST_QUANTITY; i++) {
        auto randData = genDataVector();
        st::comparableAccumulator<std::less<>> min;
        st::comparableAccumulator<std::greater<>> max;
        for (auto value: randData) {
            min.accumulate(value);
            max.accumulate(value);
        }
        assert(min.getResult() == *std::min_element(randData.begin(), randData.end()));
    }
}

int main() {
    emptyDataTest();
    oddDataSizeMedian();
    evenDataSizeMedian();
    overflowRiskMedian();
    comparableAccumulatorRandomTest();
}