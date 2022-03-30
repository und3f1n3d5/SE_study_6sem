#include <m3i***/m3i_by_t_o.h>
#include <stdexcept>
#include <exception>

using namespace details;

typedef std::lock_guard<std::mutex> LockGuardMutex;

int min(int a, int b)
{
    return a < b ? a : b;
}

M3iImpl::M3iImpl() : ptr_(new SharedPtr(nullptr, 0, 0, 0, 0))
{
}

void M3iImpl::Initialize(int ***&arr, int d1, int d2, int d3)
{
    if (d1 <= 0 || d2 <= 0 || d3 <= 0)
    {
        throw std::invalid_argument("dimensions must be positive");
    }
    arr = new int **[d1];
    for (int i = 0; i < d1; ++i)
    {
        arr[i] = new int *[d2];
        for (int j = 0; j < d2; ++j)
        {
            arr[i][j] = new int[d3];
            for (int k = 0; k < d3; ++k)
            {
                arr[i][j][k] = int();
            }
        }
    }
}

M3iImpl::M3iImpl(int d1, int d2, int d3) : ptr_(new SharedPtr(nullptr, d1, d2, d3, 1))
{
    Initialize(ptr_->arr, ptr_->d1, ptr_->d2, ptr_->d3);
}

M3iImpl::M3iImpl(const il3int &list) : ptr_(new SharedPtr(nullptr, 0, 0, 0, 0))
{
    ptr_->d1 = list.size();
    ptr_->d2 = list.begin()->size();
    ptr_->d3 = list.begin()->begin()->size();
    ptr_->count = 1;
    Initialize(ptr_->arr, ptr_->d1, ptr_->d2, ptr_->d3);
    int i = 0;
    int j = 0;
    int k = 0;
    for (const auto &sublist : list)
    {
        j = 0;
        for (const auto &subsublist : sublist)
        {
            k = 0;
            for (const auto &elem : subsublist)
            {
                ptr_->arr[i][j][k] = elem;
                ++k;
            }
            ++j;
        }
        ++i;
    }
}

void M3iImpl::Clear()
{
    for (int i = 0; i < ptr_->d1; ++i)
    {
        for (int j = 0; j < ptr_->d2; ++j)
        {
            delete[] ptr_->arr[i][j];
        }
        delete[] ptr_->arr[i];
    }
    delete[] ptr_->arr;
}

M3iImpl::M3iImpl(const M3iImpl &other) : ptr_(other.ptr_)
{
    ptr_->count.fetch_add(1);
}

M3iImpl &M3iImpl::operator=(const M3iImpl &other)
{
    if (this == &other)
    {
        return *this;
    }
    if (ptr_->count.fetch_sub(1) == 1)
    {
        Clear();
    }
    ptr_ = other.ptr_;
    ptr_->count.fetch_add(1);
    return *this;
}

M3iImpl::M3iImpl(M3iImpl &&other) : ptr_(other.ptr_)
{
    other.ptr_ = nullptr;
}

M3iImpl &M3iImpl::operator=(M3iImpl &&other)
{
    if (this == &other)
    {
        return *this;
    }
    if (ptr_->count.fetch_sub(1) == 1)
    {
        Clear();
    }
    ptr_ = other.ptr_;
    ptr_->count.fetch_add(1);
    other.ptr_ = nullptr;
    return *this;
}

M3iImpl::~M3iImpl()
{
    if (ptr_)
    {
        if (ptr_->count.fetch_sub(1) == 1)
        {
            Clear();
        }
        ptr_ = nullptr;
    }
}

int &M3iImpl::At(int i, int j, int k)
{
    int arr[3] = {i, j, k};
    for (int i = 0; i < 3; ++i)
    {
        if (arr[i] < 0 || arr[i] >= Size(i))
        {
            throw std::out_of_range("out of dim");
        }
    }
    return ptr_->arr[i][j][k];
}

int M3iImpl::At(int i, int j, int k) const
{
    int arr[3] = {i, j, k};
    for (int i = 0; i < 3; ++i)
    {
        if (arr[i] < 0 || arr[i] >= Size(i))
        {
            throw std::out_of_range("out of dim");
        }
    }
    return ptr_->arr[i][j][k];
}

M3iImpl M3iImpl::Clone() const
{
    M3iImpl copy(ptr_->d1, ptr_->d2, ptr_->d3);
    for (int i = 0; i < ptr_->d1; ++i)
    {
        for (int j = 0; j < ptr_->d2; ++j)
        {
            for (int k = 0; k < ptr_->d3; ++k)
            {
                copy.At(i, j, k) = ptr_->arr[i][j][k];
            }
        }
    }
    return copy;
}

M3iImpl &M3iImpl::Resize(int d1, int d2, int d3)
{
    int ***new_arr;
    Initialize(new_arr, d1, d2, d3);
    for (int i = 0; i < min(ptr_->d1, d1); ++i)
    {
        for (int j = 0; j < min(ptr_->d2, d2); ++j)
        {
            for (int k = 0; k < min(ptr_->d3, d3); ++k)
            {
                new_arr[i][j][k] = ptr_->arr[i][j][k];
            }
        }
    }
    Clear();
    Initialize(ptr_->arr, d1, d2, d3);
    for (int i = 0; i < d1; ++i)
    {
        for (int j = 0; j < d2; ++j)
        {
            for (int k = 0; k < d3; ++k)
            {
                ptr_->arr[i][j][k] = new_arr[i][j][k];
            }
        }
    }
    ptr_->d1 = d1;
    ptr_->d2 = d2;
    ptr_->d3 = d3;
    return *this;
}

int M3iImpl::Size(int dim) const
{
    if (dim == 0)
    {
        return ptr_->d1;
    }
    else if (dim == 1)
    {
        return ptr_->d2;
    }
    else if (dim == 2)
    {
        return ptr_->d3;
    }
    throw std::out_of_range("dim should be 0, 1 or 2");
}

void M3iImpl::Fill(int val)
{
    for (int i = 0; i < ptr_->d1; ++i)
    {
        for (int j = 0; j < ptr_->d2; ++j)
        {
            for (int k = 0; k < ptr_->d3; ++k)
            {
                ptr_->arr[i][j][k] = val;
            }
        }
    }
}

std::ostream &operator<<(std::ostream &ostrm, const M3iImpl &m)
{
    ostrm << "size: " << m.Size(0) << " " << m.Size(1) << " " << m.Size(2) << std::endl;
    for (int i = 0; i < m.Size(0); ++i)
    {
        for (int j = 0; j < m.Size(1); ++j)
        {
            for (int k = 0; k < m.Size(2); ++k)
            {
                ostrm << m.At(i, j, k) << " ";
            }
            ostrm << std::endl;
        }
        ostrm << std::endl;
    }
    return ostrm;
}

std::istream &operator>>(std::istream &istrm, details::M3iImpl &m)
{
    int d1;
    int d2;
    int d3;
    istrm >> d1 >> d2 >> d3;
    m = M3iImpl(d1, d2, d3);
    for (int i = 0; i < d1; ++i)
    {
        for (int j = 0; j < d2; ++j)
        {
            for (int k = 0; k < d3; ++k)
            {
                int number;
                istrm >> number;
                m.At(i, j, k) = number;
            }
        }
    }
    return istrm;
}

M3i::M3i(int d1, int d2, int d3) : impl_(d1, d2, d3) {}

M3i::M3i(const il3int &list) : impl_(list) {}

M3i::M3i(const M3i &other)
{
    mutex_ = other.mutex_;
    impl_ = other.impl_;
}

M3i &M3i::operator=(const M3i &other)
{
    mutex_ = other.mutex_;
    impl_ = other.impl_;
    return *this;
}

M3i::M3i(M3i &&other)
{
    mutex_ = std::move(other.mutex_);
    impl_ = std::move(other.impl_);
}

M3i &M3i::operator=(M3i &&other)
{
    mutex_ = std::move(other.mutex_);
    impl_ = std::move(other.impl_);
    return *this;
}

M3i::M3i(M3iImpl impl) : impl_(std::move(impl)) {}

M3i M3i::Clone() const
{
    LockGuardMutex lock(*mutex_);
    return M3i(std::move(impl_.Clone()));
}

M3i &M3i::Resize(int d1, int d2, int d3)
{
    LockGuardMutex lock(*mutex_);
    impl_.Resize(d1, d2, d3);
    return *this;
}

int &M3i::At(int i, int j, int k)
{
    // пользователь сам должен отвечать за безопасность здесь
    return impl_.At(i, j, k);
}
int M3i::At(int i, int j, int k) const
{
    return impl_.At(i, j, k);
}

int M3i::Size(int dim) const
{
    LockGuardMutex lock(*mutex_);
    return impl_.Size(dim);
}

void M3i::Fill(int val)
{
    LockGuardMutex lock(*mutex_);
    impl_.Fill(val);
}

std::ostream &M3i::WriteTo(std::ostream &ostrm) const
{
    LockGuardMutex lock(*mutex_);
    ostrm << impl_;
    return ostrm;
}

std::ostream &operator<<(std::ostream &ostrm, const M3i &m)
{
    return m.WriteTo(ostrm);
}

std::istream &M3i::ReadFrom(std::istream &istrm)
{
    LockGuardMutex lock(*mutex_);
    istrm >> impl_;
    return istrm;
}

std::istream &operator>>(std::istream &istrm, M3i &m)
{
    return m.ReadFrom(istrm);
}