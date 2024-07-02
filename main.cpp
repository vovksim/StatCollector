#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;


double calcMean(std::vector<int> data) {
    auto mid = data.begin() + data.size() / 2;
    if (data.size() % 2 == 1) {
        std::nth_element(data.begin(), mid, data.end());
        return *mid;
    } else {
        auto mid_prev = data.begin() + data.size() / 2 - 1;
        std::nth_element(data.begin(), mid, data.end());
        std::nth_element(data.begin(), mid_prev, data.end());
        return (*mid + *mid_prev) / 2.0;
    }
}

template<typename Comparator>
std::pair<std::vector<int>::iterator, std::vector<int>::iterator> sequenceFinder(std::vector<int> &data, Comparator comp) {
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
    return std::make_pair(bestStart, bestEnd+1);
}

class avgAccumulator {
    double result;
    std::size_t length;
public:
    void accumulate(int value) {
        this->result += value / static_cast<double>(length);
    }

    double getResult() const {
        return this->result;
    }

    explicit avgAccumulator(std::size_t length) : result(double(0)) {
        this->length = length;
    };
};

class minAccumulator {
    bool isInit;
    int result;
public:
    void accumulate(int value) {
        if (!(this->isInit)) {
            this->result = value;
            this->isInit = true;
        } else if (value < this->result) {
            this->result = value;
        }
    }

    int getResult() const {
        if (!isInit) {
            throw std::runtime_error("No values accumulated!");
        } else {
            return this->result;
        }
    }

    minAccumulator() : isInit(false), result(int()) {};
};

class maxAccumulator {
    bool isInit;
    int result;
public:
    void accumulate(int value) {
        if (!(this->isInit)) {
            this->result = value;
            this->isInit = true;
        } else if (value > this->result) {
            this->result = value;
        }
    }

    int getResult() const {
        if (!isInit) {
            throw std::runtime_error("No values accumulated!");
        } else {
            return this->result;
        }
    }

    maxAccumulator() : isInit(false), result(int()) {};
};


bool inputPath(fs::path &filePath) {
    std::string pathString;
    std::cin >> pathString;
    filePath = fs::path(pathString);
    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
        return true;
    }
    return false;
}

void loadData(fs::path &filePath, std::vector<int> &result) {
    std::ifstream fileReader(filePath);
    if (fileReader.is_open()) {
        std::string strInput;
        int temp = 0;
        while (std::getline(fileReader, strInput)) {
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
    } else {
        throw std::runtime_error("File is not opened!");
    }
}

class statCollector {
    std::vector<int> data;
    minAccumulator minimum;
    maxAccumulator maximum;
    avgAccumulator average;
    double mean;
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> ascSeq;
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> desSeq;

public:
    void calcValues() {
        for (auto iter: data) {
            minimum.accumulate(iter);
            maximum.accumulate(iter);
            average.accumulate(iter);
        }
        ascSeq = sequenceFinder(data, std::greater<int>());
        desSeq = sequenceFinder(data, std::less<int>());
        mean = calcMean(data);

    }


    void displayValues(std::ostream &os) {
        os << "Minimum: " << minimum.getResult() << std::endl;
        os << "Maximum: " << maximum.getResult() << std::endl;
        os << "Average: " << average.getResult() << std::endl;
        os << "Mean: " << mean << std::endl;
        os << "Ascending Sequence: ";
        for (auto iter = ascSeq.first; iter != ascSeq.second; ++iter) {
            os << *iter;
            if (iter != ascSeq.second - 1) {
                os << ", ";
            }
        }
        os << std::endl;
        os << "Descending Sequence: ";
        for (auto iter = desSeq.first; iter != desSeq.second; ++iter) {
            os << *iter;
            if (iter != desSeq.second - 1) {
                os << ", ";
            }
        }
        os << std::endl;
    }

    statCollector(std::vector<int> &data) : average(data.size()), minimum(), maximum() {
        mean = 0;
        this->data = data;
    }

    ~statCollector() = default;
};

int main() {
    try {
        std::vector<int> data;
        fs::path filePath;
        inputPath(filePath);
        loadData(filePath, data);
        auto *coll = new statCollector(data);
        coll->calcValues();
        coll->displayValues(std::cout);
        delete (coll);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
