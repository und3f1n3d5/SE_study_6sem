#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

#include <iostream>
#include <cassert>
#include <memory.h>

namespace lab618
{
    template <class T>
    class CMemoryManager
    {
    private:
        struct block
        {
            // Массив данных блока
            T* pdata;
            // Адрес следующего блока
            block *pnext;
            // Первая свободная ячейка
            int firstFreeIndex;
            // Число заполненных ячеек
            int usedCount;

        };
    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false):
                m_blkSize(_default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct),
                m_pBlocks(nullptr), m_pCurrentBlk(nullptr)
        {
        }

        virtual ~CMemoryManager()
        {
            clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject()
        {
            if (nullptr == m_pBlocks)
            {
                m_pCurrentBlk = newBlock();
                m_pBlocks = m_pCurrentBlk;
            }
            else if (-1 == m_pCurrentBlk->firstFreeIndex)
            {
                block* tblock = m_pBlocks;
                while (nullptr != tblock && -1 == tblock->firstFreeIndex) {
                    tblock = tblock->pnext;
                }
                if (nullptr == tblock) {
                    tblock = newBlock();
                }
                m_pCurrentBlk = tblock;
            }
            int ind = m_pCurrentBlk->firstFreeIndex;
            T* new_obj = reinterpret_cast<T*>(m_pCurrentBlk->pdata + ind);

            m_pCurrentBlk->firstFreeIndex = *(reinterpret_cast<int*>(m_pCurrentBlk->pdata + ind));
            ++m_pCurrentBlk->usedCount;
            return new_obj;
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            block* cur_block = m_pBlocks;
            int cur_ind;
            while (cur_block != nullptr)
            {
                if (p >= cur_block->pdata && p <= (cur_block->pdata + m_blkSize - 1)) {
                    cur_ind = p - cur_block->pdata;
                    break;
                }
                cur_block = cur_block->pnext;
            }
            if (nullptr == cur_block)
            {
                return false;
            }
            if (-1 != cur_block->firstFreeIndex && cur_ind >= cur_block->firstFreeIndex)
            {
                return false;
            }
            (cur_block->pdata + cur_ind)->~T();
            --cur_block->usedCount;
            return true;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            if (nullptr == m_pBlocks)
            {
                return;
            }

            block* cur_block = m_pBlocks;
            while (nullptr != cur_block && 0 == cur_block->usedCount)
            {
                cur_block = cur_block->pnext;
            }

            if (!m_isDeleteElementsOnDestruct && nullptr != cur_block)
            {
                throw CException();
            }
            if (cur_block == nullptr) {
                return;
            }
            m_pBlocks = nullptr;
            m_pCurrentBlk = nullptr;

            bool* pmask = new bool[m_blkSize];

            while (nullptr != cur_block)
            {
                block* pnext = cur_block->pnext;
                deleteBlock(cur_block, pmask);

                cur_block = pnext;
            }
            delete[] pmask;
        }
    private:

        // Создать новый блок данных. применяется в newObject
        block* newBlock()
        {
            block* pnew_block = new block;
            pnew_block->pdata = reinterpret_cast<T*>(new char[sizeof(T) * m_blkSize]);
            memset(reinterpret_cast<void*>(pnew_block->pdata), 0, sizeof(T) * m_blkSize);
            T* p = pnew_block->pdata;
            for (int i = 0; i < m_blkSize - 1; ++i, ++p)
            {
                *(reinterpret_cast<int*>(p)) = i + 1;
            }
            *(reinterpret_cast<int*>(p)) = -1;

            pnew_block->firstFreeIndex = 0;
            pnew_block->usedCount = 0;
            pnew_block->pnext = m_pBlocks;
            m_pBlocks = pnew_block;
            return pnew_block;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p, bool *pmask)
        {
            assert(nullptr != p);
            if (!m_isDeleteElementsOnDestruct) {
                delete[] reinterpret_cast<char*>(p->pdata);
                delete p;
                return;
            }

            for (int i = 0; i < m_blkSize; ++i)
            {
                pmask[i] = true;
            }

            T* pData = p->pdata;
            int freeIndex = p->firstFreeIndex;
            while (freeIndex != -1)
            {
                pmask[freeIndex] = false;
                freeIndex = *(reinterpret_cast<int*>(pData + freeIndex));
            }

            for (int  i = 0; i < m_blkSize; ++i)
            {
                if (pmask[i])
                {
                    (pData + i)->~T();
                }
            }

            delete[] reinterpret_cast<char*>(p->pdata);
            delete p;
        }

        // Размер блока
        int m_blkSize;
        // Начало списка блоков
        block* m_pBlocks;
        // Текущий блок
        block* m_pCurrentBlk;
        // Удалять ли элементы при освобождении
        bool m_isDeleteElementsOnDestruct;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2021_02_18