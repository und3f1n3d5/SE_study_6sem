//
// Created by dmitrij on 3/24/22.
//

#ifndef SE_STUDY_PAVLIUCHENKOV_AVLTREE_H
#define SE_STUDY_PAVLIUCHENKOV_AVLTREE_H

#include "../mm/mm.h"

namespace lab618
{
    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
            T* pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;
        };

    public:
        class CException
        {
        public:
            CException() = default;
        };

    public:
        CAVLTree() : m_pRoot(nullptr), m_Memory(10000, true){}

        virtual ~CAVLTree()
        {
            clear();
        }

        bool add(T* element_ptr) {
            leaf* found_leaf = findLeaf(element_ptr);
            if (found_leaf) {
                return false;
            }
            leaf* new_leaf = m_Memory.newObject();
            if (nullptr == new_leaf) {
                throw CException();
            }
            new_leaf->pData = element_ptr;
            new_leaf->balanceFactor = 0;
            new_leaf->pLeft = nullptr;
            new_leaf->pRight = nullptr;
            bool balanced = false;
            m_pRoot = Insert(m_pRoot, new_leaf, balanced);
            return true;
        }

        bool update(T* element_ptr) {
            leaf* element_leaf = findLeaf(element_ptr);
            if (nullptr != element_leaf) {
                element_leaf->pData = element_ptr;
                return true;
            }
            leaf* new_leaf = m_Memory.newObject();
            if (nullptr == new_leaf) {
                throw CException();
            }
            new_leaf->pData = element_ptr;
            new_leaf->balanceFactor = 0;
            new_leaf->pLeft = nullptr;
            new_leaf->pRight = nullptr;
            bool balanced = false;
            m_pRoot = Insert(m_pRoot, new_leaf, balanced);
            return false;
        }

        T* find(const T& element_ptr){
            leaf* element_leaf = findLeaf(&element_ptr);
            if (element_leaf) {
                return element_leaf->pData;
            }
            return nullptr;
        }

        bool remove(const T& element) {
            bool removed = false;
            bool balanced = false;
            m_pRoot = findAndDelete(m_pRoot, element, balanced, removed);
            return removed;
        }

        void clear() {
            m_pRoot = nullptr;
            m_Memory.clear();
        }

    private:
        leaf *findLeaf(const T* element_ptr) {
            if (m_pRoot == nullptr) {
                return nullptr;
            }
            leaf* current_leaf = m_pRoot;
            int comp = Compare(current_leaf->pData, element_ptr);
            while(comp != 0) {
                if (comp > 0 && current_leaf->pRight != nullptr) {
                    current_leaf = current_leaf->pRight;
                    comp = Compare(current_leaf->pData, element_ptr);
                }
                else if (comp < 0 && current_leaf->pLeft != nullptr) {
                    current_leaf = current_leaf->pLeft;
                    comp = Compare(current_leaf->pData, element_ptr);
                }
                else {
                    return nullptr;
                }
            }
            return current_leaf;
        }

        leaf* findMinLeaf(leaf* current_leaf) {
            while (nullptr != current_leaf->pLeft) {
                current_leaf = current_leaf->pLeft;
            }
            return current_leaf;
        }

        leaf* deleteMinLeaf(leaf* parent_leaf, bool &balanced) {
            if (nullptr == parent_leaf->pLeft) {
                return parent_leaf->pRight;
            }
            parent_leaf->pLeft = deleteMinLeaf(parent_leaf->pLeft, balanced);
            if (!balanced) {
                --parent_leaf->balanceFactor;
            }
            if (std::abs(parent_leaf->balanceFactor) == 1 || balanced) {
                balanced = true;
                return parent_leaf;
            }
            if (std::abs(parent_leaf->balanceFactor) == 2) {
                leaf* new_parent_leaf = balance(parent_leaf);
                if (std::abs(new_parent_leaf->balanceFactor) == 1) {
                    balanced = true;
                }
                return new_parent_leaf;
            }
            return parent_leaf;
        }

        leaf* balance(leaf* current_leaf) {
            if (2 == current_leaf->balanceFactor) {
                if (current_leaf->pLeft->balanceFactor < 0) {
                    if (current_leaf->pLeft->pRight->balanceFactor == 1) {
                        current_leaf->balanceFactor = -1;
                        current_leaf->pLeft->balanceFactor = 0;
                    }
                    else if (current_leaf->pLeft->pRight->balanceFactor == 0) {
                        current_leaf->balanceFactor = 0;
                        current_leaf->pLeft->balanceFactor = 0;
                    }
                    else {
                        current_leaf->balanceFactor = 0;
                        current_leaf->pLeft->balanceFactor = 1;
                    }
                    current_leaf->pLeft->pRight->balanceFactor = 0;
                    return BigRight(current_leaf);
                }
                if (current_leaf->pLeft->balanceFactor == 1) {
                    current_leaf->balanceFactor = 0;
                    current_leaf->pLeft->balanceFactor = 0;
                }
                else {
                    current_leaf->balanceFactor = 1;
                    current_leaf->pLeft->balanceFactor = -1;
                }
                return LittleRight(current_leaf);
            }
            else if (current_leaf->balanceFactor == -2) {
                if (current_leaf->pRight->balanceFactor > 0) {
                    if (current_leaf->pRight->pLeft->balanceFactor == 1) {
                        current_leaf->balanceFactor = 0;
                        current_leaf->pRight->balanceFactor = -1;
                    }
                    else if (current_leaf->pRight->pLeft->balanceFactor == 0) {
                        current_leaf->balanceFactor = 0;
                        current_leaf->pRight->balanceFactor = 0;
                    }
                    else {
                        current_leaf->balanceFactor = 1;
                        current_leaf->pRight->balanceFactor = 0;
                    }
                    current_leaf->pRight->pLeft->balanceFactor = 0;
                    return BigLeft(current_leaf);
                }
                if (current_leaf->pRight->balanceFactor == -1) {
                    current_leaf->balanceFactor = 0;
                    current_leaf->pRight->balanceFactor = 0;
                }
                else {
                    current_leaf->balanceFactor = -1;
                    current_leaf->pRight->balanceFactor = 1;
                }
                return LittleLeft(current_leaf);
            }
            return current_leaf;
        }

        leaf* findAndDelete(leaf* parent_leaf, const T& element, bool &balanced, bool& removed) {
            if (!parent_leaf) {
                balanced = true;
                return parent_leaf;
            }
            if (Compare(parent_leaf->pData, &element) != 0) {
                if (Compare(parent_leaf->pData, &element) < 0) {
                    parent_leaf->pLeft = findAndDelete(parent_leaf->pLeft, element, balanced, removed);
                    if (removed && !balanced) {
                        --parent_leaf->balanceFactor;
                    }
                }
                else if (Compare(parent_leaf->pData, &element) > 0) {
                    parent_leaf->pRight = findAndDelete(parent_leaf->pRight, element, balanced, removed);
                    if (removed && !balanced) {
                        ++parent_leaf->balanceFactor;
                    }
                }
                if (std::abs(parent_leaf->balanceFactor) == 1 || balanced || !removed) {
                    balanced = true;
                    return parent_leaf;
                }
                if (std::abs(parent_leaf->balanceFactor) == 2) {
                    leaf* newParentLeaf = balance(parent_leaf);
                    if (newParentLeaf->balanceFactor == 1 || newParentLeaf->balanceFactor == -1) {
                        balanced = true;
                    }
                    return newParentLeaf;
                }
                return parent_leaf;
            }
            else {
                removed = true;
                leaf* left_child = parent_leaf->pLeft;
                leaf* right_child = parent_leaf->pRight;
                size_t b = parent_leaf->balanceFactor;
                m_Memory.deleteObject(parent_leaf);
                if (!right_child) {
                    return left_child;
                }
                leaf* minLeaf = findMinLeaf(right_child);
                minLeaf->pRight = deleteMinLeaf(right_child, balanced);
                minLeaf->pLeft = left_child;
                minLeaf->balanceFactor = b;
                if (!balanced) {
                    ++minLeaf->balanceFactor;
                }
                if (std::abs(minLeaf->balanceFactor) == 1 || balanced) {
                    balanced = true;
                    return minLeaf;
                }
                if (std::abs(minLeaf->balanceFactor) == 2) {
                    leaf* newParentLeaf = balance(minLeaf);
                    if (std::abs(newParentLeaf->balanceFactor) == 1) {
                        balanced = true;
                    }
                    return newParentLeaf;
                }
                return minLeaf;
            }
        }

        leaf* Insert(leaf* parent_leaf, leaf* current_leaf, bool &balanced) {
            if (!parent_leaf) {
                return current_leaf;
            }
            if (Compare(parent_leaf->pData, current_leaf->pData) < 0) {
                parent_leaf->pLeft = Insert(parent_leaf->pLeft, current_leaf, balanced);
                if (!balanced) {
                    ++parent_leaf->balanceFactor;
                }
            }
            else {
                parent_leaf->pRight = Insert(parent_leaf->pRight, current_leaf, balanced);
                if (!balanced) {
                    --parent_leaf->balanceFactor;
                }
            }
            if (parent_leaf->balanceFactor == 0 || balanced) {
                balanced = true;
                return parent_leaf;
            }
            if (std::abs(parent_leaf->balanceFactor) == 2) {
                leaf* new_parent_leaf = balance(parent_leaf);
                if (new_parent_leaf->balanceFactor == 0) {
                    balanced = true;
                }
                return new_parent_leaf;
            }
            return parent_leaf;
        }

        leaf* BigRight(leaf* current_leaf) {
            current_leaf->pLeft = LittleLeft(current_leaf->pLeft);
            return LittleRight(current_leaf);
        }

        leaf* BigLeft(leaf* current_leaf) {
            current_leaf->pRight = LittleRight(current_leaf->pRight);
            return LittleLeft(current_leaf);
        }

        leaf* LittleRight(leaf* current_leaf) {
            leaf* p = current_leaf->pLeft;
            current_leaf->pLeft = p->pRight;
            p->pRight = current_leaf;
            return p;
        }

        leaf* LittleLeft(leaf* current_leaf) {
            leaf* p = current_leaf->pRight;
            current_leaf->pRight = p->pLeft;
            p->pLeft = current_leaf;
            return p;
        }

        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;
    };
} // namespace templates


#endif //SE_STUDY_PAVLIUCHENKOV_AVLTREE_H
