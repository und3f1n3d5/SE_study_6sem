//
// Created by dmitrij on 2/19/22.
//

// check task

#ifndef SE_STUDY_PAVLIUCHENKOV_M3I_H
#define SE_STUDY_PAVLIUCHENKOV_M3I_H

#include <iostream>
#include <vector>
#include <string>

//template <typename T>
class M3i {
public:
    explicit M3i(uint64_t w=1, uint64_t h=1, uint64_t d=1, int64_t* data=nullptr);
    M3i(const M3i &right);
    M3i(M3i &&right) noexcept = default;

    M3i &operator=(const M3i &right);
    M3i &operator=(M3i &&right) noexcept = default;

    int64_t at(uint64_t x=0, uint64_t y=0, uint64_t z=0) const;
    int64_t &at(uint64_t x=0, uint64_t y=0, uint64_t z=0);
    int64_t GetWidth() const;
    int64_t GetHeight() const;
    int64_t GetDepth() const;
    void SetElement(int64_t element, uint64_t x=0, uint64_t y=0, uint64_t z=0);

    void Resize(uint64_t w=1, uint64_t h=1, uint64_t d=1);
    uint64_t GetSize() const;

    M3i copy() const;
    M3i clone() const;

    void Fill(int64_t element=0);
    void SetDefault(int64_t element=0);

    ~M3i();

private:
    uint64_t height_ = 1, width_ = 1, depth_ = 1;
    int64_t default_ = 0;
    int64_t* data_ = nullptr;
    bool is_copy_ = false;
    int64_t* number_of_copies_ = new int64_t(0);

    static void Check(M3i &r);

    void ConstructFromPointer(int64_t* ptr, uint64_t w=1, uint64_t h=1, uint64_t d=1);
};

std::ostream& operator << (std::ostream &os, const M3i &a);
std::istream& operator >> (std::istream &in, M3i &a);

#endif //SE_STUDY_PAVLIUCHENKOV_M3I_H
