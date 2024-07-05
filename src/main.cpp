#include "stats.hpp"


struct stats {
    int min{};
    int max{};
    double avg{};
    double median{};
    range ascSeq;
    range desSeq;

    stats(int min, int max, double avg, double median, range ascSeq, range desSeq) : min(min),
                                                                                     max(max),
                                                                                     avg(avg),
                                                                                     median(median),
                                                                                     ascSeq(ascSeq),
                                                                                     desSeq(desSeq) {};
};

std::ostream &operator<<(std::ostream &os, range &data) {
    for (auto iter = data.first; iter != data.second; iter++) {
        os << *iter;
        if (iter != data.second - 1) {
            os << ", ";
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, stats &value) {
    os << "Max: " << value.max << std::endl;
    os << "Min: " << value.min << std::endl;
    os << "Median: " << value.median << std::endl;
    os << "Average: " << std::setprecision(std::numeric_limits<double>::max_digits10) << value.avg << std::endl;
    os << "Ascending sequence: " << value.ascSeq << std::endl;
    os << "Descending sequence: " << value.desSeq;
    return os;
}


stats calcValues(const std::vector<int> &data) {
    st::comparableAccumulator<std::less<int>> minimum;
    st::comparableAccumulator<std::greater<int>> maximum;
    st::avgAccumulator average(data.size());
    for (const auto value: data) {
        minimum.accumulate(value);
        maximum.accumulate(value);
        average.accumulate(value);
    }
    range ascSeqMarker = st::sequenceFinder(data, std::less<int>());
    range desSeqMarker = st::sequenceFinder(data, std::greater<int>());
    auto median = st::calcMean(data);
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
