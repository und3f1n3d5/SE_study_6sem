#ifndef SE_STUDY_PAVLIUCHENKOV_RATIONAL_H
#define SE_STUDY_PAVLIUCHENKOV_RATIONAL_H

#include <iostream>
#include <vector>
#include <string>


class Rational {
public:
    Rational() = default;
    explicit Rational(const int64_t &p, const int64_t &q);
    Rational(const int64_t &i);
    Rational(const int &i);
    Rational(const Rational &right) noexcept = default;
    Rational(Rational &&right) noexcept = default;

    Rational& operator=(const Rational &right) noexcept;
    Rational& operator=(Rational &&right) noexcept = default;

    ~Rational() = default;

    int64_t GetNominator() const noexcept;

    int64_t GetDenominator() const noexcept;

    void SetNominator(const int i) noexcept;

    void SetDenominator(const int i);

    Rational& operator+=(const Rational &right) noexcept;
    Rational& operator-=(const Rational &right) noexcept;
    Rational& operator*=(const Rational &right) noexcept;
    Rational& operator/=(const Rational &right);

    const Rational operator--(int) noexcept;
    Rational& operator--() noexcept;
    const Rational operator++(int) noexcept;
    Rational& operator++() noexcept;

    Rational operator-() noexcept;
    Rational operator+() const noexcept;

    std::string asDecimal(size_t precision = 0) const;
    explicit operator double () const;
    explicit operator bool () const;
    std::string toString() const;

private:

    int64_t nominator = 0, denominator = 1;

    class DivideByZero: std::exception {};

    static void reduce(Rational &r) noexcept;

    static void Check(Rational &r);
};

Rational operator+(const Rational &left, const Rational &right) noexcept;
Rational operator-(const Rational &left, const Rational &right) noexcept;
Rational operator*(const Rational &left, const Rational &right) noexcept;
Rational operator/(const Rational &left, const Rational &right);

bool operator==(const Rational &left, const Rational &right) noexcept;
bool operator>=(const Rational &left, const Rational &right) noexcept;
bool operator<=(const Rational &left, const Rational &right) noexcept;
bool operator<(const Rational &left, const Rational &right) noexcept;
bool operator>(const Rational &left, const Rational &right) noexcept;
bool operator!=(const Rational &left, const Rational &right) noexcept;

std::istream& operator >> (std::istream &in, Rational &a);
std::ostream& operator << (std::ostream &os, const Rational &a);

#endif
