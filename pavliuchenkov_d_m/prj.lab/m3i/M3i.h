//
// Created by dmitrij on 2/19/22.
//

// check task

#ifndef SE_STUDY_PAVLIUCHENKOV_M3I_H
#define SE_STUDY_PAVLIUCHENKOV_M3I_H

#include <iostream>
#include <vector>
#include <string>
#include <atomic>

//template <typename T>
class M3i {
public:
    explicit M3i(int64_t w = 1, int64_t h = 1, int64_t d = 1);
    M3i(const M3i &right);
    M3i(M3i &&right) noexcept;

    M3i &operator=(const M3i& right);
    M3i &operator=(M3i&& right) noexcept;

    int64_t at(int64_t x = 0, int64_t y = 0, int64_t z = 0) const; // 0 <= x < width
    int64_t& at(int64_t x = 0, int64_t y = 0, int64_t z = 0); // check indexes
    int64_t GetWidth() const;
    int64_t GetHeight() const;
    int64_t GetDepth() const;
    int64_t Size(int64_t dim) const;
    void SetElement(int64_t element, int64_t x=0, int64_t y=0, int64_t z=0); // check indexes

    void Resize(int64_t w=1, int64_t h=1, int64_t d=1); // check indexes
    int64_t GetSize() const;

    M3i copy() const; // check dependent
    M3i clone() const; // check independent

    void Fill(int64_t element=0);
    void SetDefault(int64_t element=0);

    ~M3i();

private:

    struct base
    {
        std::mutex mutex_;
        int64_t height_ = 0;
        int64_t width_ = 0;
        int64_t depth_ = 0;
        int64_t default_ = 0;
        int64_t* data_ = nullptr;
        bool is_copy_ = false;
        std::atomic<int64_t> number_of_copies_{0};
    };

    base* ptr_ = nullptr;

    void Check();

    void ConstructFromPointer(int64_t* ptr, int64_t w = 1, int64_t h = 1, int64_t d = 1);
    void clear();
};
// todo
// неудачно - доступ
// творчески - как можно обеспечивать доступ потокобезопасно

std::ostream& operator<<(std::ostream& os, const M3i& a);
std::istream& operator>>(std::istream& in, M3i& a);
// todo check program for google codestyle
// make different repos for different courses

#endif //SE_STUDY_PAVLIUCHENKOV_M3I_H