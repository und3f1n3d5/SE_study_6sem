

#include "rational.h"



/*/////////////////////////////////////////////////////////////////////////////////////

                                 Rational

/////////////////////////////////////////////////////////////////////////////////////*/

Rational::Rational(const int64_t &p, const int64_t &q) : nominator(p), denominator(q) {
    if (denominator == 0)
        throw DivideByZero();
    Check(*this);
}

Rational::Rational(const int64_t &i) : Rational(i, 1) {}
Rational::Rational(const int &i) : Rational(i, 1) {}

void Rational::reduce(Rational &r) noexcept {
    if (r.nominator != 0) {
        int64_t num_first;
        num_first = std::abs(r.nominator);
        int64_t num_second = r.denominator;
        if (num_second > num_first) {
            std::swap(num_first, num_second);
        }
        while (num_first % num_second) {
            num_first = num_first % num_second;
            std::swap(num_first, num_second);
        }
        r.nominator = r.nominator / num_second;
        r.denominator = r.denominator / num_second;
    } else {
        r.denominator = 1;
    }
}


void Rational::Check(Rational &r) {
    if (r.denominator < 0){
        r.nominator *= -1;
        r.denominator *= -1;
    }
    if (r.denominator == 0) {
        throw DivideByZero();
    }
    if (r.nominator == 0) r.denominator = 1;
    reduce(r);
}


Rational::operator bool () const {
    return this->nominator != 0;
}

std::string Rational::toString() const {
    if (this->denominator == 1)
        return std::to_string(this->nominator);
    std::string s =  std::to_string(this->nominator) + '/' + std::to_string(this->denominator);
    return s;
}

std::string Rational::asDecimal(size_t precision) const {
    std::string s;
    int64_t q = 10;
    for (size_t i = 0; i < precision; ++i){
        q *= 10;
    }
    Rational r(5, q);
    if (0 > *this) r = -r;
    r += *this;
    s = std::to_string(r.nominator / r.denominator) + '.';
    int64_t left = std::abs(r.nominator) % r.denominator;
    for (size_t i = 0; i <= precision; ++i){
        s += std::to_string((left * 10) / r.denominator);
        left *= 10;
        left %= r.denominator;
    }
    s.erase(s.size() - 1, 1);
    if (s[s.size() - 1] == '.') {
        s.erase(s.size() - 1, 1);
    }
    if (this->nominator < 0 && s[0] != '-') s = '-' + s;
    return s;
}

Rational::operator double () const {
    std::string s = this->asDecimal(7);
    double res = 0;
    size_t i = 0;
    if (s[0] == '-') ++i;
    for (; i < s.size() && s[i] != ','; ++i){
        res = res*10 + (int) s[i] - '0';
    }
    double k = 10;
    if (s[i] == ',') ++i;
    for (; i < s.size(); ++i) {
        res += ((int) s[i] - '0') / k;
        k *= 10;
    }
    if (s[0] == '-') res = -res;
    return res;
}

Rational operator+(const Rational &left, const Rational &r) noexcept {
    Rational result = left;
    result += r;
    return result;
}


std::istream& operator >> (std::istream &in, Rational &a)
{
    std::string s;
    in >> s;
    int64_t p = 0, q = 0;
    size_t i;
    for (i = (s[0] == '-'); i < s.size() && s[i] != '/'; ++i) {
        if (0 > (int) s[i] - '0' || (int) s[i] - '0' >= 10) {
            throw std::runtime_error("Error in input format.\n");
        }
        p = p*10 + (int) s[i] - '0';
    }
    if (s[0] == '-') p = -p;
    if (i == s.size()) q = 1;
    ++i;
    int j = i;
    for (i = i + (s[i] == '-'); i < s.size(); ++i){
        q = q * 10 + (int) s[i] - '0';
    }
    if (s[j] == '-')
        throw std::runtime_error("Error in input format.\n");
    a = Rational(p, q);
    return in;
}

std::ostream& operator << (std::ostream &os, const Rational &a)
{
    return os << a.GetNominator() << "/" << a.GetDenominator();
    //return os << a.toString();
}

Rational Rational::operator+() const noexcept{
    return *this;
}


Rational operator-(const Rational &left, const Rational &r) noexcept {
    Rational result = left;
    result -= r;
    return result;
}

Rational &Rational::operator+=(const Rational &right) noexcept {
    *this = Rational(this->nominator * right.denominator + this->denominator * right.nominator,
                     this->denominator * right.denominator);
    Check(*this);
    return *this;
}

Rational& Rational::operator-=(const Rational &right) noexcept {
    *this = Rational(this->nominator * right.denominator - this->denominator * right.nominator,
                     this->denominator * right.denominator);
    Check(*this);
    return *this;
}

Rational& Rational::operator=(const Rational &right) noexcept {
    if (this == &right) {
        return *this;
    }
    this->nominator = right.nominator;
    this->denominator = right.denominator;
    Check(*this);
    return *this;
}

Rational Rational::operator-() noexcept {
    Rational res = *this;
    res.nominator = -this->nominator;
    Check(res);
    return res;
}

bool operator==(const Rational &left, const Rational &right) noexcept {
    return !(left > right) && !(left < right);
}

bool operator>(const Rational &left, const Rational &right) noexcept {
    return left.GetNominator() * right.GetDenominator() > left.GetDenominator() * right.GetNominator();
}

bool operator<(const Rational &left, const Rational &right) noexcept {
    return right > left;
}

bool operator<=(const Rational &left, const Rational &right) noexcept {
    return left < right || left == right;
}

bool operator>=(const Rational &left, const Rational &right) noexcept {
    return left == right || left > right;
}

Rational& Rational::operator++() noexcept {
    return *this += 1;
}

const Rational Rational::operator++(int) noexcept{
    Rational old_value = *this;
    *this += 1;
    Check(*this);
    return old_value;
}

Rational& Rational::operator--() noexcept {
    return *this -= 1;
}

const Rational Rational::operator--(int) noexcept {
    Rational old_value = *this;
    *this -= 1;
    Check(*this);
    return old_value;
}

Rational operator/(const Rational &left, const Rational &right) {
    Rational res = left;
    res /= right;
    return res;
}

Rational operator*(const Rational &left, const Rational &right) noexcept {
    Rational res = left;
    res *= right;
    return res;
}

Rational& Rational::operator*=(const Rational &right) noexcept {
    *this = Rational(this->nominator * right.nominator, this->denominator * right.denominator);
    Check(*this);
    return *this;
}

Rational& Rational::operator/=(const Rational &right) {
    if (right == 0) {
        throw Rational::DivideByZero();
    }
    *this = Rational(this->nominator * right.denominator, this->denominator * right.nominator);
    Check(*this);
    return *this;
}

bool operator!=(const Rational &left, const Rational &right) noexcept {
    return !(left == right);
}

int64_t Rational::GetNominator() const noexcept {
    return nominator;
}

int64_t Rational::GetDenominator() const noexcept {
    return denominator;
}

void Rational::SetNominator(const int i) noexcept {
    nominator = i;
    Check(*this);
}

void Rational::SetDenominator(const int i) {
    denominator = i;
    Check(*this);
}
