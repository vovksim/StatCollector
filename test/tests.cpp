//
// Created by ivan on 04.07.24.
//
#include "../src/stats.hpp"
#include <cassert>
#include <random>

void gen_random(std::vector<int> &r, std::size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    r.assign(length, 0);
    for (auto &value: r) {
        value = distrib(gen);
    }
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


int main() {
    emptyDataTest();
    oddDataSizeMedian();
    evenDataSizeMedian();
    overflowRiskMedian();

}