#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class DecimalNumber {
private:
    const int integer_size_;
    const int fractional_size_;
    vector<unsigned char> integer_part_;
    vector<unsigned char> fractional_part_;

public:
    DecimalNumber(const int& integer_size, const int& fractional_size) :
        integer_size_{ integer_size },
        fractional_size_{ fractional_size }
    {
        integer_part_.resize(integer_size_);
        fractional_part_.resize(fractional_size_);
        //cout << this->to_string() << endl;
    }

    void set_both_parts(unsigned char integer_part, unsigned char fractional_part)
    {
        //integer_part_.clear();
        int digit = 0;
        for (size_t i = 0; i < integer_size_; i++)
        {
            digit = integer_part % 10;
            integer_part /= 10;
            integer_part_[i] = digit;
        }
        //reverse(integer_part_.begin(), integer_part_.end());

        //fractional_part_.clear();
        digit = 0;
        for (size_t i = 0; i < fractional_size_; i++)
        {
            digit = fractional_part % 10;
            fractional_part /= 10;
            fractional_part_[i] = digit;
        }
        reverse(fractional_part_.begin(), fractional_part_.end());

        //cout << this->to_string() << endl;
    }
    

    string to_string() {
        string str = "";
        for (int i = integer_size_ - 1; i >= 0; i--) {
            str += std::to_string(integer_part_[i]);
        }
        str += ".";
        for (int i = 0; i < fractional_size_; i++) {
            str += std::to_string(fractional_part_[i]);
        }
        return str;
    }

    DecimalNumber operator+(const DecimalNumber& other) {
        DecimalNumber result(integer_size_, fractional_size_);
        int carry = 0;
        for (int i = 0; i < fractional_size_; i++) {
            int sum = fractional_part_[i] + other.fractional_part_[i] + carry;
            result.fractional_part_[i] = sum % 10;
            carry = sum / 10;
        }
        for (int i = 0; i < integer_size_; i++) {
            int sum = integer_part_[i] + other.integer_part_[i] + carry;
            result.integer_part_[i] = sum % 10;
            carry = sum / 10;
        }
        return result;
    }

    DecimalNumber operator-(const DecimalNumber& other) {
        DecimalNumber result(integer_size_, fractional_size_);
        int borrow = 0;
        for (int i = 0; i < fractional_size_; i++) {
            int diff = fractional_part_[i] - other.fractional_part_[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            result.fractional_part_[i] = diff;
        }
        for (int i = 0; i < integer_size_; i++) {
            int diff = integer_part_[i] - other.integer_part_[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            result.integer_part_[i] = diff;
        }
        return result;
    }

    DecimalNumber operator*(const DecimalNumber& other) {
        DecimalNumber result(integer_size_ + other.integer_size_,
            fractional_size_ + other.fractional_size_);

        for (int i = 0; i < fractional_size_; i++) {
            int carry = 0;
            for (int j = 0; j < other.fractional_size_; j++) {
                int product = fractional_part_[i] * other.fractional_part_[j] + carry + result.fractional_part_[i + j];
                result.fractional_part_[i + j] = product % 10;
                carry = product / 10;
            }
            for (int j = other.fractional_size_; j < result.fractional_size_ - i; j++) {
                int product = carry + result.fractional_part_[i + j];
                result.fractional_part_[i + j] = product % 10;
                carry = product / 10;
            }
        }

        for (int i = 0; i < integer_size_; i++) {
            int carry = 0;
            for (int j = 0; j < other.integer_size_; j++) {
                int product = integer_part_[i] * other.integer_part_[j] + carry + result.integer_part_[i + j];
                result.integer_part_[i + j] = product % 10;
                carry = product / 10;
            }
            for (int j = other.integer_size_; j < result.integer_size_ - i; j++) {
                int product = carry + result.integer_part_[i + j];
                result.integer_part_[i + j] = product % 10;
                carry = product / 10;
            }
        }

        // Remove leading zeros from fractional part
        while (!result.fractional_part_.empty() && result.fractional_part_.back() == 0) {
            result.fractional_part_.pop_back();
        }

        // Remove leading zeros from integer part
        while (!result.integer_part_.empty() && result.integer_part_.back() == 0) {
            result.integer_part_.pop_back();
        }

        return result;
    }

    bool operator==(const DecimalNumber& other) {
        if (integer_part_.size() != other.integer_part_.size() ||
            fractional_part_.size() != other.fractional_part_.size()) {
            return false;
        }
        for (int i = 0; i < integer_part_.size(); i++) {
            if (integer_part_[i] != other.integer_part_[i]) {
                return false;
            }
        }
        for (int i = 0; i < fractional_part_.size(); i++) {
            if (fractional_part_[i] != other.fractional_part_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const DecimalNumber& other)  {
        return !(*this == other);
    }

    bool operator<(const DecimalNumber& other) const {
        if (integer_part_.size() < other.integer_part_.size()) {
            return true;
        }
        if (integer_part_.size() > other.integer_part_.size()) {
            return false;
        }
        for (int i = integer_part_.size() - 1; i >= 0; i--) {
            if (integer_part_[i] < other.integer_part_[i]) {
                return true;
            }
            if (integer_part_[i] > other.integer_part_[i]) {
                return false;
            }
        }
        for (int i = 0; i < fractional_part_.size(); i++) {
            if (fractional_part_[i] < other.fractional_part_[i]) {
                return true;
            }
            if (fractional_part_[i] > other.fractional_part_[i]) {
                return false;
            }
        }
        return false;
    }

    bool operator>(const DecimalNumber& other)  {
        return other < *this;
    }

    bool operator<=(const DecimalNumber& other)  {
        return !(other < *this);
    }

    bool operator>=(const DecimalNumber& other)  {
        return !(*this < other);
    }
};

int main()
{
    setlocale(LC_ALL, "Rus");
    DecimalNumber a(2, 2);
    a.set_both_parts(1, 2);
    cout << a.to_string() << endl;
    //cout << a.to_string() << endl; // Output: 123.45

    //DecimalNumber b(2, 2);
    ////b = DecimalNumber({ 9, 8, 7 }, { 6, 5 });
    //cout << b.to_string() << endl; // Output: 987.65

    //DecimalNumber c = a + b;
    //cout << c.to_string() << endl; // Output
}