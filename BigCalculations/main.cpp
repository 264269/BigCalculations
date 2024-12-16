#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <fstream>

class BigInteger {
    //digit size
    static const int BASE_POW = 9;
    static const int BASE = 1e9;

    //digits
    std::vector<int> digits;

    //negativity flag
    bool isNegative;

    //removing leading zeros (was in the algorithm but it feels like not necessary)
    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

public:
    //base constructor
    BigInteger() {
        isNegative = false;
    }

    //constructor from str
    BigInteger(std::string str) {        
        if (str.length() == 0 || (str.length() == 1 && str[0] == '-')) { //if line empty then empty vector
            isNegative = false;
            return;
        }
        else { //else split str by digits

            isNegative = (str[0] == '-'); //проверка на отрицательное число
            if (isNegative) str = str.substr(1);

            for (long long i = str.length(); i > 0; i -= BASE_POW) { //fill from the end
                
                std::string stringDigit = i < BASE_POW 
                    ? str.substr(0, i) 
                    : str.substr(i - BASE_POW, BASE_POW);

                int convertedDigit = std::stoi(stringDigit);

                digits.push_back(convertedDigit);
            }

            removeLeadingZeros();
        }
    }

    const BigInteger operator *(const BigInteger& right) {
        //result
        BigInteger result;
        result.digits.resize(digits.size() + right.digits.size());

        //multiplying
        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0; //overflow
            for (size_t j = 0; j < right.digits.size() || carry != 0; ++j) {
                int leftDigit = digits[i];
                int rightDigit = j < right.digits.size() ? right.digits[j] : 0;
                long long cur =
                    result.digits[i + j] 
                    + leftDigit * 1ll * rightDigit
                    + carry;
                result.digits[i + j] = int(cur % BASE);
                carry = int(cur / BASE);
            }
        }

        //neg.flag
        result.isNegative = isNegative != right.isNegative;

        result.removeLeadingZeros();
        
        return result;
    }

    friend std::ostream& operator <<(std::ostream& os, const BigInteger& bigInt);

    operator std::string() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
};

std::ostream& operator <<(std::ostream& os, const BigInteger& bigInt) {
    std::string result;
    if (bigInt.digits.empty()) {
        result = "0";
    }
    else {
        //to avoid velosipeding, use std::scientific, write it to stringstream, modify exp as digits.size * BASE_POW
        std::stringstream ss;

        if (bigInt.isNegative) {
            ss << '-';
        }

        ss << std::scientific << std::setprecision(9);
        
        //scientific form if number is greater than 1e9
        if (bigInt.digits.size() > 1) {
            long long firstNumbers = 1LL * bigInt.digits.back() * bigInt.BASE;
            long long secondDigit = bigInt.digits.size() - 2;
            firstNumbers += bigInt.digits[secondDigit];
            double firstNumbersDouble = static_cast<double>(firstNumbers);
            ss << firstNumbersDouble;

            result = ss.str();

            size_t ePos = result.find_first_of("eE");

            std::string eStr = result.substr(ePos + 1);

            int e = std::stoll(eStr);

            long long digitsSize = bigInt.digits.size() - 2;
            digitsSize = digitsSize > 0 ? digitsSize : 0;
            e += digitsSize * bigInt.BASE_POW;

            std::string eNew = std::to_string(e);

            result.replace(ePos + 2, eStr.length(), eNew);
        }
        else {
            ss << bigInt.digits.back();
            result = ss.str();
        }
    }

    os << result;

    return os;
}

enum InputType {
    WithFile,
    WithConsole,
    WithArgs
};

InputType getInputType(int argc, char* argv[]) {
    InputType result = WithConsole;
    if (argc > 1) {
        if (strcmp(argv[1], "args") == 0) {
            result = WithArgs;
        }
        else if (strcmp(argv[1], "file") == 0) {
            result = WithFile;
        }
        else if (strcmp(argv[1], "console") == 0) {
            result = WithConsole;
        }
        else {
            std::cout << "Value " << argv[1] << " doesn't match any input type." << std::endl;
        }
        std::cout << argv[1] << " input will be used." << std::endl;
    }
    return result;
}

class NotImplementedException : public std::logic_error {
public:
    NotImplementedException() : std::logic_error("Function not yet implemented") {};
};

class Calculator {
    BigInteger result;

public:
    Calculator(BigInteger bigInt) {
        result = bigInt;
    }

    void multiply(const BigInteger& bigInt) {
        result = result * bigInt;
    }
    
    //TODO
    void add(const BigInteger& bigInt) {
        throw NotImplementedException();
    }
    void subtract(const BigInteger& bigInt) {
        throw NotImplementedException();
    }
    void divide(const BigInteger& bigInt) {
        throw NotImplementedException();
    }

    BigInteger getResult() {
        return result;
    }
};

//BigInteger handleFile(int argc, char* argv[]) {
//    //throw NotImplementedException();
//    std::ifstream file(argv[2]);
//
//    std::string firstLine;
//    std::getline(file, firstLine);
//
//    std::string secondLine;
//    std::getline(file, secondLine);
//
//    file.close();
//
//    BigInteger x(firstLine);
//    BigInteger y(secondLine);
//
//    Calculator calc(x);
//    calc.multiply(y);
//    return calc.getResult();
//}
//
//BigInteger handleConsole() {
//    //throw NotImplementedException();
//    std::string firstLine;
//    std::cout << "Enter first number:" << std::endl;
//    std::getline(std::cin, firstLine);
//
//    std::string secondLine;
//    std::cout << "Enter second number:" << std::endl;
//    std::getline(std::cin, secondLine);
//
//    BigInteger x(firstLine);
//    BigInteger y(secondLine);
//
//    Calculator calc(x);
//    calc.multiply(y);
//    return calc.getResult();
//}
//
//BigInteger handleArgs(int argc, char* argv[]) {
//    std::string firstLine = argv[2];
//    std::string secondLine = argv[3];
//
//    BigInteger x(firstLine);
//    BigInteger y(secondLine);
//
//    Calculator calc(x);
//    calc.multiply(y);
//    return calc.getResult();
//}

class InputProcessor {
public:
    virtual ~InputProcessor() {};
    virtual BigInteger getNext() = 0;
};

class FromConsoleProcessor : public InputProcessor {
public:
    BigInteger getNext() {
        std::cout << "Enter your number: " << std::endl;
        std::string line;
        if (std::getline(std::cin, line)) {
            return BigInteger(line);
        }
        else {
            throw std::invalid_argument("Can't read line from console.");
        }
    }
};

class FromFileProcessor : public InputProcessor {
private:
    std::ifstream file;
public:
    FromFileProcessor(std::string fileName) {
        file.open(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Can't open file.");
        }
    };

    ~FromFileProcessor() {
        if (file.is_open()) {
            file.close();
        }
    }

    BigInteger getNext() {
        std::string line;
        if (std::getline(file, line)) {
            return BigInteger(line);

        }
        else {
            throw std::invalid_argument("Can't read line from file.");
        }
    }
};

class FromArgsProcessor : public InputProcessor {
private:
    char** argv;
    int i = 2;
public:
    FromArgsProcessor(char* argv_[]) : argv(argv_) {};

    BigInteger getNext() {
        if (argv[i]) {
            char* str = argv[i++];
            return BigInteger(str);
        }
        else {
            throw std::invalid_argument("No more args left");
        }
    }
};

InputProcessor* getInputProcessor(InputType type, int argc, char* argv[]) {
    switch (type) {
        case InputType::WithConsole:
            return new FromConsoleProcessor();
        case InputType::WithFile:
            return new FromFileProcessor(argv[2]);
        case InputType::WithArgs:
            return new FromArgsProcessor(argv);
        default:
            throw std::runtime_error("Couldn't find InputProcessor for chosen input type");
    }
}

//BigInteger processArguments(InputType type, int argc, char* argv[]) {
//    BigInteger result;
//    switch (type) {
//        case WithConsole:
//            result = handleConsole();
//            break;
//        case WithFile:
//            result = handleFile(argc, argv);
//            break;
//        case WithArgs:
//            result = handleArgs(argc, argv);
//            break;
//    }
//    return result;
//}

int main(int argc, char* argv[]) {
    try {
        //std::cout << processArguments(getInputType(argc, argv), argc, argv) << std::endl;
        InputProcessor* inputProcessor = getInputProcessor(getInputType(argc, argv), argc, argv);
        BigInteger x = inputProcessor->getNext();
        BigInteger y = inputProcessor->getNext();
        Calculator calc(x);
        calc.multiply(y);
        std::cout << "Result of multiplying " << x << " and " << y << " is " << calc.getResult() << std::endl;
    }
    catch (std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    return 0;
}
