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

std::ostream &operator<<(std::ostream &os, range &data) {
    for (auto iter = data.first; iter != data.second; iter++) {
        os << *iter;
        if (iter != data.second - 1) {
            os << ", ";
        }
    }
    return os;
}

struct stats {
    int min{};
    int max{};
    double avg{};
    double median{};
    range ascSeq;
    range desSeq;

    friend std::ostream &operator<<(std::ostream &os, stats &value) {
        os << "Max: " << value.max << std::endl;
        os << "Min: " << value.min << std::endl;
        os << "Median: " << value.median << std::endl;
        os << "Average: " << std::setprecision(std::numeric_limits<double>::max_digits10) << value.avg << std::endl;
        os << "Ascending sequence: " << value.ascSeq << std::endl;
        os << "Descending sequence: " << value.desSeq;
        return os;
    }

    stats(int min, int max, double avg, double median, range ascSeq, range desSeq) : min(min),
                                                                                     max(max),
                                                                                     avg(avg),
                                                                                     median(median),
                                                                                     ascSeq(ascSeq),
                                                                                     desSeq(desSeq) {};
};

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

std::vector<int> loadData(std::istream &is) {
    std::vector<int> result;
    std::string strInput;
    int temp = 0;
    while (std::getline(is, strInput)) {
        try {
            temp = std::stoi(strInput);
            result.push_back(temp);
        }
        catch (const std::invalid_argument &e) {
            throw std::runtime_error("Invalid data: Non-integer value found - " + strInput);
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Invalid data: Value out of range - " + strInput);
        }
    }
    if (std::empty(result)) {
        throw std::runtime_error("Data file is empty!");
    }
    return result;
}

stats calcValues(const std::vector<int> &data) {
    comparableAccumulator<std::less<int>> minimum;
    comparableAccumulator<std::greater<int>> maximum;
    avgAccumulator average(data.size());
    for (const auto value: data) {
        minimum.accumulate(value);
        maximum.accumulate(value);
        average.accumulate(value);
    }
    range ascSeqMarker = sequenceFinder(data, std::less<int>());
    range desSeqMarker = sequenceFinder(data, std::greater<int>());
    auto median = calcMean(data);
    return {minimum.getResult().value(), maximum.getResult().value(), average.getResult().value(), median.value(),
            ascSeqMarker,
            desSeqMarker};
}

void validatePath(fs::path &filePath) {
    if (!fs::exists(filePath)) {
        throw std::runtime_error("File does not exist: " + filePath.string());
    }
    if (!fs::is_regular_file(filePath)) {
        throw std::runtime_error("Path is not a regular file: " + filePath.string());
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            throw std::runtime_error("No path provided");
        }
        std::vector<int> data;
        fs::path filePath = argv[1];
        validatePath(filePath);
        if (std::ifstream file(filePath); file.is_open()) {
            data = loadData(file);
            auto res = calcValues(data);
            std::cout << res << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
