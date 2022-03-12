//
// Created by dmitrij on 2/19/22.
//

#include "m3i.h"

M3i::M3i(const int64_t w, const int64_t h, const int64_t d) {
    ptr_ = new base();
    std::lock_guard<std::mutex> guard(ptr_->mutex_);
    if (w <= 0 || h <= 0 || d <= 0) {
        throw std::runtime_error("Error: wrong sizes\n");
    }
    ptr_->width_ = w;
    ptr_->height_ = h;
    ptr_->depth_ = d;
    ptr_->data_ = static_cast<int64_t *>(malloc(sizeof(int64_t) * w * h * d));
    if (ptr_->data_ == nullptr) {
        throw std::runtime_error("Error while allocating memory\n");
    }
}

M3i::M3i(const M3i &right) {
    right.copy();
}

M3i &M3i::operator=(const M3i &right) {
    if (this == &right) {
        throw std::runtime_error("Error while copying: self-assignment\n");
    }
    this->ptr_ = right.ptr_;
    ptr_->number_of_copies_.fetch_add(1);
    return *this;
}

int64_t M3i::at(const int64_t x, const int64_t y, const int64_t z) const {
    if (x < 0 || y < 0 || z < 0) {
        throw std::runtime_error("Error: wrong indices\n");
    }
    if (ptr_->data_ == nullptr) {
        throw std::runtime_error("Data is not initialized\n");
    }
    if (x < ptr_->width_ && y < ptr_->height_ && z < ptr_->depth_) {
        std::lock_guard<std::mutex> guard(ptr_->mutex_);
        return ptr_->data_[x + y * ptr_->width_ + z * ptr_->width_ * ptr_->height_];
    }
    throw std::runtime_error("Error: wrong indices\n");
}

void M3i::SetElement(const int64_t element, const int64_t x, const int64_t y, const int64_t z) {
    if (ptr_->data_ == nullptr) {
        throw std::runtime_error("Data is not initialized\n");
    }
    if (x < 0 || y < 0 || z < 0) {
        throw std::runtime_error("Error: wrong indices\n");
    }
    if (x < ptr_->width_ && y < ptr_->height_ && z < ptr_->depth_) {
        std::lock_guard<std::mutex> guard(ptr_->mutex_);
        ptr_->data_[x + y * ptr_->width_ + z * ptr_->width_ * ptr_->height_] = element;
        return;
    }
    throw std::runtime_error("Error: wrong indices\n");
}

void M3i::Resize(const int64_t w, const int64_t h, const int64_t d) {
    if (w <= 0 || h <= 0 || d <= 0) {
        throw std::runtime_error("Error: wrong sizes\n");
    }
    auto old_data = ptr_->data_;
    ConstructFromPointer(old_data, ptr_->width_, ptr_->height_, ptr_->depth_);
    int i = ptr_->width_, j = ptr_->height_, k = ptr_->depth_;
    ptr_->width_ = w;
    ptr_->height_ = h;
    ptr_->depth_ = d;
    for (; i < w; ++i) {
        for (; j < h; ++j) {
            for (; k < d; ++k) {
                SetElement(ptr_->default_, i, j, k);
            }
        }
    }
    free(old_data);
}

M3i::~M3i() {
    if (ptr_ == nullptr) return;
    if (ptr_->number_of_copies_.load() > 0) {
        ptr_->number_of_copies_.fetch_sub(1);
        return;
    }
    free(ptr_->data_);
    delete ptr_;
}

void M3i::ConstructFromPointer(int64_t *ptr, const int64_t w, const int64_t h, const int64_t d) {
    ptr_->data_ = static_cast<int64_t *>(malloc(sizeof(int64_t) * w * h * d));
    if (ptr_->data_ == nullptr) {
        throw std::runtime_error("Error while allocating memory\n");
    }
    ptr_->width_ = w;
    ptr_->height_ = h;
    ptr_->depth_ = d;
    for (int i=0; i < ptr_->width_; ++i) {
        for (int j=0; j < ptr_->height_; ++j) {
            for (int k=0; k < ptr_->depth_; ++k) {
                this->SetElement(ptr[i * w + j * h + k * d], i, j, k);
            }
        }
    }
}

int64_t M3i::GetSize() const {
    return ptr_->width_ * ptr_->height_ * ptr_->depth_;
}

M3i M3i::copy() const {
    std::lock_guard<std::mutex> guard(ptr_->mutex_);
    M3i tmp;
    tmp.ptr_ = ptr_;
    ptr_->number_of_copies_.fetch_add(1);
    return tmp;
}

M3i M3i::clone() const{
    std::lock_guard<std::mutex> guard(ptr_->mutex_);
    M3i tmp;
    tmp.ptr_->depth_ = ptr_->depth_;
    tmp.ptr_->width_ = ptr_->width_;
    tmp.ptr_->height_ = ptr_->height_;
    tmp.ptr_->is_copy_ = false;
    tmp.ptr_->number_of_copies_.store(ptr_->number_of_copies_);
    tmp.ConstructFromPointer(ptr_->data_, ptr_->width_, ptr_->height_, ptr_->depth_);
    return tmp;
}

void M3i::Fill(const int64_t element) {
    for (int i=0; i < ptr_->width_; ++i) {
        for (int j=0; j < ptr_->height_; ++j) {
            for (int k=0; k < ptr_->depth_; ++k) {
                this->SetElement(element, i, j, k);
            }
        }
    }
}

void M3i::SetDefault(const int64_t element) {
    ptr_->default_ = element;
}

int64_t &M3i::at(int64_t x, int64_t y, int64_t z) {
    if (x < 0 || y < 0 || z < 0) {
        throw std::runtime_error("Error: wrong indices\n");
    }
    if (ptr_->data_ == nullptr) {
        throw std::runtime_error("Data is not initialized\n");
    }
    if (x < ptr_->width_ && y < ptr_->height_ && z < ptr_->depth_) {
        return ptr_->data_[x + y * ptr_->width_ + z * ptr_->width_ * ptr_->height_];
    }
    throw std::runtime_error("Error: wrong indices\n");
}

int64_t M3i::GetWidth() const {
    return ptr_->width_;
}

int64_t M3i::GetHeight() const {
    return ptr_->height_;
}

int64_t M3i::GetDepth() const {
    return ptr_->depth_;
}

int64_t M3i::Size(const int64_t dim) const {
    if (dim == 0)
        return GetWidth();
    else if (dim == 1)
        return GetHeight();
    return GetDepth();
}

M3i::M3i(M3i &&right) noexcept {
    ptr_ = right.ptr_;
    right.ptr_ = nullptr;
}

M3i &M3i::operator=(M3i &&right) noexcept {
    std::lock_guard<std::mutex> guard_1(ptr_->mutex_);
    clear();
    ptr_ = right.ptr_;
    ptr_->number_of_copies_.fetch_add(1);
    std::lock_guard<std::mutex> guard_2(ptr_->mutex_);
    right.clear();
    return *this;
}


std::istream& operator>>(std::istream &in, M3i &a)
{
    int64_t h, w, d, element;
    in >> w >> h >> d;
    a = M3i(w, h, d);
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

std::ostream& operator<<(std::ostream &os, const M3i &a)
{
    std::string res;
    res.resize(60);
    res = std::to_string(a.GetWidth()) + " " + std::to_string(a.GetHeight()) + " " + std::to_string(a.GetDepth()) + "\n";
    for (int i=0; i < a.GetWidth(); ++i) {
        for (int j=0; j < a.GetHeight(); ++j) {
            for (int k=0; k < a.GetDepth(); ++k) {
                res += std::to_string(a.at(i, j, k));
                res += " ";
            }
            res += "\n";
        }
        res += "\n";
    }
    return os << res;
}

void M3i::clear() {
    if (ptr_ == nullptr) {
        return;
    }

    ptr_->number_of_copies_;

    if (ptr_->number_of_copies_ == 0) {
        if (ptr_->data_ != nullptr) {
            free(ptr_->data_);
        }

        delete ptr_;
    }

    ptr_ = nullptr;
}
