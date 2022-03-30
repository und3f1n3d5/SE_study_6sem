#pragma once
#ifndef M3i_HEAD_H_2022_03_03
#define M3i_HEAD_H_2022_03_03

#include <iostream>
#include <memory>
#include <initializer_list>
#include <mutex>
#include <atomic>
typedef std::initializer_list<std::initializer_list<std::initializer_list<int>>> il3int;

namespace details {
    class M3iImpl {
    public:
        M3iImpl(); // done
        M3iImpl(int d1, int d2, int d3); // done
        // мой вариант конструктора от std::Initializer_list
        M3iImpl(const il3int& list); // done
        // легкая копия(подсчет ссылок)
        M3iImpl(const M3iImpl& other); // done
        // легкая копия(подсчет ссылок)
        M3iImpl& operator=(const M3iImpl& other); // done

        M3iImpl(M3iImpl&& other); // done
        M3iImpl& operator=(M3iImpl&& other); // done

        ~M3iImpl(); // done

        // полная копия данных
        M3iImpl Clone() const; // done

        M3iImpl& Resize(int d1, int d2, int d3); // done

        // доступ к элементу
        int& At(int i, int j, int k); // done
        int At(int i, int j, int k) const; // done

        // получение размера по измерению 0, 1, 2
        int Size(int dim) const; // done
        // заполнение значениями
        void Fill(int val); // done
    private:
        void Initialize(int***& arr, int d1, int d2, int d3); // заполнение arr значениями по умолчанию
        void Clear(); // очистка данных
        struct SharedPtr {
            int*** arr; // буфер
            int d1; // измерение 1
            int d2; // измерение 2
            int d3; // измерение 3
            std::atomic<int> count; // кол-во ссылок
            SharedPtr(int*** arr, int d1, int d2, int d3, int count):
                    arr(arr), d1(d1), d2(d2), d3(d3), count(count)
            {}
        };
        SharedPtr* ptr_;
    };
}

std::istream& operator>>(std::istream& istrm, details::M3iImpl& m); // done
std::ostream& operator<<(std::ostream& ostrm, const details::M3iImpl& m); // done

class M3i {
private:
    // using namespace details;
public:
    M3i() = default; // done
    M3i(int d1, int d2, int d3); // done
    // мой вариант конструктора от std::Initializer_list
    M3i(const il3int& list); // done
    // легкая копия(подсчет ссылок)
    M3i(const M3i& other); // done
    // легкая копия(подсчет ссылок)
    M3i& operator=(const M3i& other); // done

    M3i(M3i&& other); // done
    M3i& operator=(M3i&& other); // done

    ~M3i() = default; // done

    // полная копия данных
    M3i Clone() const; // done

    M3i& Resize(int d1, int d2, int d3); // done

    // доступ к элементу
    int& At(int i, int j, int k); // done
    int At(int i, int j, int k) const; // done

    // получение размера по измерению 0, 1, 2
    int Size(int dim) const; // done
    // заполнение значениями
    void Fill(int val); // done

    std::ostream& WriteTo(std::ostream& ostrm) const; // done
    std::istream& ReadFrom(std::istream& istrm); // done
private:
    explicit M3i(details::M3iImpl impl);
    details::M3iImpl impl_;
    std::shared_ptr<std::mutex> mutex_ = std::make_shared<std::mutex>();
};

std::istream& operator>>(std::istream& istrm, M3i& m); // done
std::ostream& operator<<(std::ostream& ostrm, const M3i& m); // done

#endif // #define M3i_HEAD_H_2022_03_03