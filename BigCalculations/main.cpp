#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstring>

class BigInteger {
    //������ �������
    static const int BASE_POW = 9;
    static const int BASE = 1e9;

    //�������
    std::vector<int> digits;

    //���� ���������������
    bool isNegative;

    //�������� ���������� �����
    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

public:
    //������� �����������
    BigInteger() {
        isNegative = false;
    }

    //����������� �� ������
    BigInteger(std::string str) {        
        if (str.length() == 0) { //���� ������ ������, �� ������ ������
            isNegative = false;
        }
        else { //����� ��������� ������ �� �������

            isNegative = (str[0] == '-'); //�������� �� ������������� �����
            if (isNegative) str = str.substr(1);

            for (long long i = str.length(); i > 0; i -= BASE_POW) { //���������� �������� � �����
                
                std::string stringDigit = i < BASE_POW 
                    ? str.substr(0, i) 
                    : str.substr(i - BASE_POW, BASE_POW);

                int convertedDigit = std::stoi(stringDigit);

                digits.push_back(convertedDigit);
            }

            //�������� ������� �����
            removeLeadingZeros();
        }
    }

    const BigInteger operator *(const BigInteger& right) {
        //���������
        BigInteger result;
        result.digits.resize(digits.size() + right.digits.size());

        //������������
        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0; //������������ �������
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

        //����
        result.isNegative = isNegative != right.isNegative;
        //�������� ���������� �����
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
    if (bigInt.digits.empty()) {
        os << 0;
    }
    else {
        if (bigInt.isNegative) {
            os << '-';
        }
        os << bigInt.digits.back();

        char old_fill = os.fill('0');

        for (long long i = (bigInt.digits.size() - 2); i >= 0; --i) {
            os << std::setw(9) << bigInt.digits[i];
        }

        os.fill(old_fill);
    }

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

void handleFile(char* argv[]) {
    throw NotImplementedException();
}

void handleConsole() {
    std::string line;

    std::cout << "Enter first number: "

    while (std::getline(std::cin, line)) {
        if (line.empty()) break;

        std::cout << "You've entered: " << line << std::endl;
    }
}

void handleArgs(char* argv[]) {
    throw NotImplementedException();
}

//void 

void processArguments(InputType type, int argc, char* argv[]) {
    switch (type) {
        case WithConsole:
            handleConsole();
            break;
        case WithArgs:
            handleArgs(argv);
            break;
        case WithFile:
            handleFile(argv);
            break;
        }
}


class Calculator {
    BigInteger result;

public:
    Calculator(BigInteger bigInt) {
        result = bigInt;
    }

    void multiply(const BigInteger& bigInt) {
        result = result * bigInt;
    }

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


int main(int argc, char* argv[]) {
    try {
        //BigInteger a("1234567890123456789012345678901234567890");
        //BigInteger b("2345678901234567890123456789012345678901");
        //Calculator calc(a);
        //calc.multiply(b);
        //BigInteger c = calc.getResult();
        //c = c * b;
        //std::cout << c << std::endl;
        //std::cout << calc.getResult() << std::endl;
        processArguments(getInputType(argc, argv), argc, argv);
    }
    catch (std::exception err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    return 0;
}
