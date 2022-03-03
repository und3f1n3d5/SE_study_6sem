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
    right.copy();
}

M3i &M3i::operator=(const M3i &right) {
    if (this == &right) {
        throw std::runtime_error("Error while copying: self-assignment\n");
    }
    *this = right.copy();
    return *this;
}

int64_t M3i::at(const uint64_t x, const uint64_t y, const uint64_t z) const {
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

M3i M3i::copy() const {
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

M3i M3i::clone() const{
    return M3i(*this);
}

void M3i::Fill(const int64_t element) {
    for (int i=0; i < width_; ++i) {
        for (int j=0; j < height_; ++j) {
            for (int k=0; k < depth_; ++k) {
                this->SetElement(element, i, j, k);
            }
        }
    }
}

void M3i::SetDefault(const int64_t element) {
    default_ = element;
}

int64_t &M3i::at(uint64_t x, uint64_t y, uint64_t z) {
    if (data_ == nullptr) {
        throw std::runtime_error("Data is not initialized\n");
    }
    if (x < width_ && y < height_ && z < depth_) {
        return data_[x + y * width_ + z * width_ * height_];
    }
    throw std::runtime_error("Error: wrong indices\n");
}

int64_t M3i::GetWidth() const {
    return width_;
}

int64_t M3i::GetHeight() const {
    return height_;
}

int64_t M3i::GetDepth() const {
    return depth_;
}

int64_t M3i::Size(const uint64_t dim) const {
    if (dim == 0)
        return GetWidth();
    else if (dim == 1)
        return GetHeight();
    return GetDepth();
}

std::istream& operator >> (std::istream &in, M3i &a)
{
    int64_t h, w, d, element;
    in >> h >> w >> d;
    a = M3i(h, w, d);
    for (int i=0; i < w; ++i) {
        for (int j=0; j < h; ++j) {
            for (int k=0; k < d; ++k) {
                in >> element;
                a.SetElement(element, i, j, k);
            }
        }
    }
    return in;
}

std::ostream& operator << (std::ostream &os, const M3i &a)
{
    std::string res;
    res = std::to_string(a.GetWidth()) + " " + std::to_string(a.GetHeight()) + " " + std::to_string(a.GetDepth()) + "\n";
    for (int i=0; i < a.GetWidth(); ++i) {
        for (int j=0; j < a.GetHeight(); ++j) {
            for (int k=0; k < a.GetDepth(); ++k) {
                res += std::to_string(a.at(i, j, k)) + " ";
            }
            res += "\n";
        }
        res += "\n";
    }
    return os << res;
}
