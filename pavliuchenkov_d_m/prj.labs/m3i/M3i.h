//
// Created by dmitrij on 2/19/22.
//

#ifndef SE_STUDY_PAVLIUCHENKOV_M3I_H
#define SE_STUDY_PAVLIUCHENKOV_M3I_H

#include <iostream>
#include <vector>
#include <string>

//template <typename T>
class M3i {
public:
    explicit M3i(const uint64_t w=1, const uint64_t h=1, const uint64_t d=1, int64_t* data=nullptr);
    M3i(const M3i &right);
    M3i(M3i &&right) noexcept = default;

    M3i &operator=(const M3i &right);
    M3i &operator=(M3i &&right) noexcept = default;

    int64_t GetElement(const uint64_t x=0, const uint64_t y=0, const uint64_t z=0);
    void SetElement(const int64_t element, const uint64_t x=0, const uint64_t y=0, const uint64_t z=0);

    void Resize(const uint64_t w=1, const uint64_t h=1, const uint64_t d=1);
    uint64_t GetSize() const;

    ~M3i();

private:
    uint64_t height_ = 1, width_ = 1, depth_ = 1;
    int64_t default_ = 0;
    int64_t* data_ = nullptr;

    static void Check(M3i &r);

    void ConstructFromPointer(int64_t* ptr, const uint64_t w=1, const uint64_t h=1, const uint64_t d=1);
};


#endif //SE_STUDY_PAVLIUCHENKOV_M3I_H
