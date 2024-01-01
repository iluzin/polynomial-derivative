#include "derivative.h"

int power(int x, unsigned int degree) {
    if (degree < 2) {
        return degree ? x : 1;
    }
    int y = power(x, degree >> 1);
    if (degree & 1) {
        return x * y * y;
    }
    return y * y;
}

void replace(std::string &str, const std::string &from, const std::string &to) {
    if (from.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end;
    std::string token;
    std::vector<std::string> result;
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delimiter.length();
        result.push_back(token);
    }
    result.push_back(s.substr(pos_start));
    return result;
}

std::string derivative(std::string polynomial) {
    replace(polynomial, "-x", "-1*x");
    replace(polynomial, "-", "+-");
    if (polynomial[0] == '+') {
        polynomial = polynomial.substr(1);
    }
    std::vector<int> coefficients;
    std::vector<std::string> monoms = split(polynomial, "+");
    for (int i = 0; i < monoms.size(); ++i) {
        int coefficient = 1, degree = 0;
        std::vector<std::string> multipliers = split(monoms[i], "*");
        for (int j = 0; j < multipliers.size(); ++j) {
            try {
                coefficient *= stoi(multipliers[j]);
            } catch (...) {
                int multiplier_degree = 1;
                std::vector<std::string> numbers = split(multipliers[j], "^");
                for (int k = numbers.size() - 1; k > 0; --k) {
                    multiplier_degree = power(stoi(numbers[k]), multiplier_degree);
                }
                try {
                    coefficient *= power(stoi(numbers[0]), multiplier_degree);
                } catch (...) {
                    degree += multiplier_degree;
                }
            }
        }
        if (coefficients.size() <= degree) {
            coefficients.resize(degree + 1);
        }
        coefficients[degree] += coefficient;
    }
    std::string result = "0";
    for (int i = coefficients.size() - 1; i > 0; --i) {
        if (coefficients[i] != 0) {
            if (result == "0") {
                result = "";
            } else {
                if (coefficients[i] > 0) {
                    result += '+';
                }
            }
            int coefficient = coefficients[i] * i;
            if (coefficient < 0) {
                coefficient = -coefficient;
                result += '-';
            }
            if (coefficient > 1 or i == 1) {
                result += std::to_string(coefficient);
                if (i > 1) {
                    result += '*';
                }
            }
            if (i > 1) {
                if (i > 2) {
                    result += "x^" + std::to_string(i - 1);
                } else {
                    result += 'x';
                }
            }
        }
    }
    return result;
}
