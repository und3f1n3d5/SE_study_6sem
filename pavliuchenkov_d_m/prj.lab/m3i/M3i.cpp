//
// Created by dmitrij on 2/19/22.
//

#include "M3i.h"

M3i::M3i(const uint64_t w, const uint64_t h, const uint64_t d, int64_t *data)
        : width_(w), height_(h), depth_(d) {
    if (data == nullptr) {
        data_ = static_cast<int64_t *>(malloc(sizeof(int64_t) * w * h * d));
        if (data_ == nullptr) {
            throw std::runtime_error("Error while allocating memory\n");
        }
        return;
    }
    ConstructFromPointer(data, w, h, d);
}

M3i::M3i(const M3i &right) {
    ConstructFromPointer(right.data_, width_, height_, depth_);
    width_ = right.width_;
    height_ = right.height_;
    depth_ = right.depth_;
}

M3i &M3i::operator=(const M3i &right) {
    if (this->data_ == right.data_) {
        if (width_ == right.width_ && height_ == right.height_ && depth_ == right.depth_) {
            return *this;
        }
        throw std::runtime_error("Error while copying: same pointers\n");
    }
    ConstructFromPointer(right.data_, width_, height_, depth_);
    width_ = right.width_;
    height_ = right.height_;
    depth_ = right.depth_;
    number_of_copies_ = right.number_of_copies_;
    is_copy_ = right.is_copy_;
    return *this;
}

int64_t M3i::GetElement(const uint64_t x, const uint64_t y, const uint64_t z) {
    if (data_ == nullptr) {
        throw std::runtime_error("Data is not initialized\n");
    }
    if (x < width_ && y < height_ && z < depth_) {
        return data_[x + y * width_ + z * width_ * height_];
    }
    throw std::runtime_error("Error: wrong indices\n");
}

void M3i::SetElement(const int64_t element, const uint64_t x, const uint64_t y, const uint64_t z) {
    if (data_ == nullptr) {
        throw std::runtime_error("Data is not initialized\n");
    }
    if (x < width_ && y < height_ && z < depth_) {
        data_[x + y * width_ + z * width_ * height_] = element;
        return;
    }
    throw std::runtime_error("Error: wrong indices\n");
}

void M3i::Resize(const uint64_t w, const uint64_t h, const uint64_t d) {
    auto old_data = data_;
    ConstructFromPointer(old_data, width_, height_, depth_);
    int i = width_, j = height_, k = depth_;
    width_ = w;
    height_ = h;
    depth_ = d;
    for (; i < w; ++i) {
        for (; j < h; ++j) {
            for (; k < d; ++k) {
                SetElement(default_, i, j, k);
            }
        }
    }
    free(old_data);
}

M3i::~M3i() {
    if ((*number_of_copies_) > 0) {
        (*number_of_copies_)--;
        return;
    }
    free(data_);
    delete number_of_copies_;
}

void M3i::ConstructFromPointer(int64_t *ptr, const uint64_t w, const uint64_t h, const uint64_t d) {
    data_ = static_cast<int64_t *>(malloc(sizeof(int64_t) * w * h * d));
    if (data_ == nullptr) {
        throw std::runtime_error("Error while allocating memory\n");
    }
    width_ = w;
    height_ = h;
    depth_ = d;
    for (int i=0; i < width_; ++i) {
        for (int j=0; j < height_; ++j) {
            for (int k=0; k < depth_; ++k) {
                this->SetElement(ptr[i * w + j * h + k * d], i, j, k);
            }
        }
    }
}

uint64_t M3i::GetSize() const {
    return width_ * height_ * depth_;
}

M3i M3i::copy() {
    M3i tmp;
    tmp.depth_ = depth_;
    tmp.width_ = width_;
    tmp.height_ = height_;
    tmp.data_ = data_;
    tmp.is_copy_ = true;
    (*number_of_copies_)++;
    tmp.number_of_copies_ = number_of_copies_;
    return tmp;
}

M3i M3i::clone() {
    return M3i(*this);
}

