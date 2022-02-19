#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

// testing clear()
#define clear_without_funcs
// without: 0.002 vs 0.004 with


#include <utility>
#include <stdexcept>

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext = nullptr;
            leaf(T& _data, leaf * _pnext) : data(std::move(_data)), pnext(_pnext)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator() = default;

            explicit CIterator(leaf *p) : m_pCurrent(p)
            {
                p = nullptr;
            }

            CIterator(const CIterator &src) : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent)
            {
            }

            ~CIterator()
            {
                m_pCurrent = nullptr;
                m_pBegin = nullptr;
            }

            CIterator& operator=(const CIterator&  src)
            {
                if (*this != src) {
                    this->m_pCurrent = src.m_pCurrent;
                    this->m_pBegin = src.m_pBegin;
                }
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                if (it.m_pBegin != this->m_pBegin || it.m_pCurrent != this->m_pCurrent) {
                    return true;
                }
                return false;
            }

            void operator++()
            {
                if (m_pCurrent == nullptr) {
                    if (m_pBegin != nullptr) {
                        m_pCurrent = m_pBegin;
                        m_pBegin = nullptr;
                    }
                    return;
                }
                m_pCurrent = m_pCurrent->pnext;
            }

            T& getData()
            {
                if (m_pCurrent != nullptr) {
                    return m_pCurrent->data;
                }
                return T();
            }

            T& operator* ()
            {
                if (m_pCurrent == nullptr)
                    throw std::runtime_error("Error in getData.\nm_pCurrent is zero.\n");
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                if (m_pCurrent != nullptr) {
                    return m_pCurrent;
                }
                return nullptr;
            }

            void setLeaf(leaf* p)
            {
                m_pBegin = nullptr;
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pCurrent = nullptr;
                m_pBegin = p;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:

        CSingleLinkedList()
        {
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        }

        virtual ~CSingleLinkedList()
        {
            clear();
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        }

        void pushBack(T& data)
        {
            if ((m_pBegin == nullptr) && (m_pEnd == nullptr)) {
                m_pBegin = new leaf(data, nullptr);
                m_pEnd = m_pBegin;
                return;
            }
            leaf* p_tmpLeaf = m_pEnd;
            m_pEnd = new leaf(data, nullptr);
            p_tmpLeaf->pnext = m_pEnd;
        }

        void pushFront(T& data)
        {
            if ((m_pBegin == nullptr) && (m_pEnd == nullptr)) {
                m_pBegin = new leaf(data, m_pBegin);
                m_pEnd = m_pBegin;
                return;
            }
            m_pBegin = new leaf(data, m_pBegin);
        }

        T popFront()
        {
            T tmp{};
            if (m_pBegin != nullptr) {
                leaf* t = m_pBegin;
                tmp = m_pBegin->data;
                m_pBegin = m_pBegin->pnext;
                t->pnext = nullptr;
                delete t;
            }
            if (m_pBegin == nullptr) {
                m_pEnd = nullptr;
            }
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (!it.isValid()) return;
            leaf* p = it.getLeaf();

            leaf* pprev = nullptr;
            if (p == m_pBegin) {
                m_pBegin = p->pnext;
                it.setLeafPreBegin(m_pBegin);
            } else {
                // find prev element
                pprev = m_pBegin;
                while (pprev != nullptr) {
                    if (pprev->pnext == p) break;
                    pprev = pprev->pnext;
                }
                it.setLeaf(pprev);
            }
            if (pprev != nullptr) pprev->pnext = p->pnext;
            if (!p->pnext)  // in the end
                m_pEnd = pprev;
            p->pnext = nullptr;
            delete p;
            p = nullptr;
        }

        int getSize()
        {
            size_t size = 0;
            for (leaf* pCurrent = m_pBegin; pCurrent != nullptr; pCurrent = pCurrent->pnext)
                ++size;
            return size;
        }

        void clear()
        {
            while (m_pBegin != nullptr) {
                #ifndef clear_without_funcs
                popFront();
                #else
                if (m_pBegin != nullptr) {
                    leaf* t = m_pBegin;
                    m_pBegin = m_pBegin->pnext;
                    t->pnext = nullptr;
                    delete t;
                }
                if (m_pBegin == nullptr) {
                    m_pEnd = nullptr;
                }
                #endif
            }
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext)
            {
                data = _data;
                pprev = _pprev;
                pnext = _pnext;
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator() = default;

            CIterator(leaf *p) : m_pCurrent(p), m_pEnd(nullptr), m_pBegin(nullptr) {}

            CIterator(const CIterator &src) = default;

            ~CIterator()
            {
                m_pBegin = nullptr;
                m_pEnd = nullptr;
                m_pCurrent = nullptr;
            }

            CIterator& operator = (const CIterator&  src)
            {
                if (*this != src) {
                    m_pBegin = src.m_pBegin;
                    m_pCurrent = src.m_pCurrent;
                    m_pEnd = src.m_pEnd;
                }
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return (!((m_pCurrent == it.m_pCurrent) && (m_pBegin == it.m_pBegin) &&
                          (m_pEnd == it.m_pEnd)));
            }

            void operator++()
            {
                if (m_pBegin == nullptr) return;
                if (m_pCurrent == nullptr) {
                    m_pCurrent = m_pBegin;
                    m_pBegin = nullptr;
                    return;
                }
                if (m_pCurrent->pnext == nullptr)
                    m_pEnd = m_pCurrent;
                m_pCurrent = m_pCurrent->pnext;
            }

            void operator--()
            {
                if (m_pEnd == nullptr) return;

                if (m_pCurrent == nullptr) {
                    m_pCurrent = m_pEnd;
                    m_pEnd = nullptr;
                    return;
                }
                if (m_pCurrent->pprev == nullptr)
                    m_pBegin = m_pCurrent;
                m_pCurrent = m_pCurrent->pprev;
            }

            T& getData()
            {
                T tmp{};
                if (m_pCurrent)
                    return m_pCurrent->data;
                return tmp;
            }

            T& operator* ()
            {
                T tmp{};
                if (m_pCurrent != nullptr) return m_pCurrent->data;
                return tmp;
            }

            leaf* getLeaf()
            {
                if (m_pCurrent != nullptr) return m_pCurrent;
                return nullptr;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p)
            {
                m_pBegin = nullptr;
                m_pCurrent = p;
                m_pEnd = nullptr;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                m_pBegin = p;
                m_pCurrent = nullptr;
                m_pEnd = nullptr;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pBegin = nullptr;
                m_pCurrent = nullptr;
                m_pEnd = p;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList() = default;

        virtual ~CDualLinkedList()
        {
            clear();
            m_pEnd = nullptr;
            m_pBegin = nullptr;
        };

        void pushBack(T& data)
        {
            if (m_pBegin && m_pEnd) {
                m_pEnd->pnext = new leaf(data, m_pEnd, 0);
                m_pEnd = m_pEnd->pnext;
                return;
            }
            m_pBegin = new leaf(data, 0, 0);
            m_pEnd = m_pBegin;
        }

        T popBack()
        {
            if (m_pEnd == nullptr)
                throw std::runtime_error("Error in popBack().\nThe list is empty.\n");
            leaf* pTmp = m_pEnd;

            leaf* new_end = m_pEnd->pprev;
            m_pEnd = new_end;
            if (m_pEnd == 0) m_pBegin = 0;

            T tmp = std::move(pTmp->data);
            delete pTmp;
            pTmp = nullptr;
            return tmp;
        }

        void pushFront(T& data)
        {
            if ((m_pBegin == nullptr) && (m_pEnd == nullptr)) {
                m_pBegin = new leaf(data, nullptr, nullptr);
                m_pEnd = m_pBegin;
                return;
            }
            m_pBegin->pprev = new leaf(data, nullptr, m_pBegin);
            m_pBegin = m_pBegin->pprev;
        }

        T popFront()
        {
            if (m_pBegin == nullptr)
                throw std::runtime_error("Error in popFront.\nThe list is empty.\n");
            leaf* pTmp = m_pBegin;

            leaf* new_begin = m_pBegin->pnext;
            m_pBegin = new_begin;
            if (m_pBegin == nullptr) m_pEnd = nullptr;

            T tmp = std::move(pTmp->data);
            delete pTmp;
            pTmp = nullptr;

            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (!it.isValid()) return;
            leaf* p = it.getLeaf();

            leaf* pprev = nullptr;
            if (p == m_pBegin) {
                m_pBegin = p->pnext;
                it.setLeafPreBegin(m_pBegin);
            } else {
                // find previous element
                pprev = m_pBegin;
                while (pprev) {
                    if (pprev->pnext == p) break;
                    pprev = pprev->pnext;
                }
                it.setLeaf(pprev);
            }
            if (pprev) pprev->pnext = p->pnext;
            if (!p->pnext) {  // in the end
                m_pEnd = pprev;
            } else {
                p->pnext->pprev = pprev;
            }
            //p->pprev = nullptr;
            //p->pnext = nullptr;
            delete p;
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            if (!it.isValid()) return;
            leaf* p = it.getLeaf();

            leaf* pnext = nullptr;
            if (p == m_pEnd) {
                m_pEnd = p->pprev;
                it.setLeafPostEnd(m_pEnd);
            } else {
                // find next element
                pnext = m_pEnd;
                while (pnext) {
                    if (pnext->pprev == p) break;
                    pnext = pnext->pprev;
                }
                it.setLeaf(pnext);
            }
            if (pnext) pnext->pprev = p->pprev;
            if (p->pprev == nullptr) {  // in the begin
                m_pBegin = pnext;
            } else {
                p->pprev->pnext = pnext;
            }
            //p->pprev = nullptr;
            //p->pnext = nullptr;
            delete p;
        }

        int getSize()
        {
            size_t size = 0;
            for (leaf* pCurrent = m_pBegin; pCurrent != nullptr; pCurrent = pCurrent->pnext)
                ++size;
            return size;
        }

        void clear()
        {
            leaf* pCurrent = m_pBegin;

            m_pBegin = nullptr;
            m_pEnd = nullptr;

            while (pCurrent != nullptr) {
                leaf* pNext = pCurrent->pnext;
                delete pCurrent;
                pCurrent = pNext;
            }
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

        CIterator end()
        {
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
};
#endif //#ifndef TEMPLATES_LIST_2022_02_03