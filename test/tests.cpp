//
// Created by ivan on 04.07.24.
//
#include "../src/stats.hpp"
#include <cassert>

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

int main() {
    emptyDataTest();
}