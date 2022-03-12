//
// Created by dmitrij on 3/12/22.
//

#ifndef SE_STUDY_PAVLIUCHENKOV_HASH_H
#define SE_STUDY_PAVLIUCHENKOV_HASH_H

#include "mm/mm.h"

namespace lab618
{
    /**
    Шаблонный класс CHash.
    В качестве параметров шаблона применяется:
    тип - T;
    функция вычисления хеша - unsigned int HashFunc(const T* pElement);
    функция сравнения - int Compare(const T *pElement, const T* pElement2).

     Класс реализует алгоритм контейнера Хеш-таблица (ассоциативный массив). Данный алгоритм базируется на идее формирования линейной адресации
    произвольных элементов в зависимости от специально вычисляемого целого значения, уникального для каждого элемента (хеш).
     Формирование линейной адресации позволяет получать и сохранять элементы в контейнер за время не зависящее от числа элементов в контейнере — O(1).
     В идеальном случае каждому хешу соответствует ячейка в линейном массиве данных. На самом деле подобрать функцию вычисления хеша, таким образом,
    чтобы соблюдалась уникальность сложно, или получаются числа очень большой размерности. Для преодоления данной проблемы, в Хеш-таблице допускаются
    коллизии — одинаковые значения, уникальность элементов определяется их полями (функциями сравнения).
     Существует два способа разрешения коллизий — разреженная Хеш-таблица и Хеш-таблица со списками. Разреженные Хеш-таблицы при возникновении коллизии
    начинают искать первую свободную ячейку в самой таблице, поиск аналогично происходит от ячейки с номером равным значению хеша, пока не найдется нужный
    элемент. Хеш-таблица со списками, в каждой ячейке Хеш-таблицы хранит начало списка элементов с одинаковыми значениями хеша. Соответственно,
    вставка производится по алгоритму: вычисляем значение хеша для элемента, проверяем есть ли в списке с данным индексом (значение хеш функции)
    данный элемент с помощью функции сравнения, если нет до добавляем его в список. Поиск производится по алгоритму: вычисляем значение хеша для элемента,
    проверяем есть ли в списке с данным индексом искомый элемент с помощью функции сравнения.
     Размер Хеш-таблицы, как правило, меньше размерности функции вычисления хеша, поэтому вычисленное значение хеша ограничивают по размеру таблицы
    с помощью операции вычисления остатка от деления.
     Функция вычисления хеша и операция сравнения задаются как параметры шаблона, что позволяет построить разные Хеш-таблицы для одного набора элементов.
     Основные недостатки Хеш-таблиц:
      1. Сложность подбора хорошей хеш функции (функции дающей наименьшее число коллизий);
      2. Отсутствие упорядоченности элементов;
      3. Вычислительная сложность хеш функции может добавить большую константу в вычислительную сложность алгоритма.

    Данный класс реализует алгоритм Хеш-таблицы со списками
    Данный класс не хранит данные — хранит, только указатели на них.
    Хранение данных производится вне рамок данного класса!
    */
    template <class T, unsigned int(*HashFunc)(const T* pElement), int(*Compare)(const T *pElement, const T* pElement2) >
    class CHash {
    private:
        /**
        Лист списка элементов. Обратите внимание, что хранится указатель на данные.
        Данный класс не хранит данные — хранит, только указатели на них.
        Хранение данных производится вне рамок данного класса!
        */
        struct leaf {
            T* pData;
            leaf *pnext;
        };

    public:
        /**
        Исключение, которое применяется при нехватке памяти на работу алгоритма
        */
        class CMemoryException {
        public:
            CMemoryException() = default;
        };

    public:
        /**
        Конструктор с параметрами: размер Хеш-таблицы, и размер блока памяти для менеджера памяти.
        Размер Хеш таблицы реализуем жестко — изменение размера таблицы в зависимости от числа элементов в контейнере не требуется.
        Все создаваемые листики списков разрешения коллизий храним в менеджере памяти.
        */
        CHash(int hashTableSize, int default_block_size) :  m_tableSize(hashTableSize),
                                                            m_pTable(new leaf*[hashTableSize]),
                                                            m_Memory(default_block_size, true) {
            for (int i = 0; i < m_tableSize; ++i) {
                m_pTable[i] = nullptr;
            }
        }
        /**
        Деструктор. Должен освобождать всю выделенную память
        */
        virtual ~CHash() {
            clear();
            delete[] m_pTable;
            m_pTable = nullptr;
        }

        /**
        Функция добавления элемента в Хеш-таблицу. Возвращает false, если элемент уже есть и true, если элемент добавлен.
        */
        bool add(T* pElement) {
            uint index = 0;
            leaf* element_l = findLeaf(pElement, index);
            if (element_l) {
                return false;
            }
            leaf* current_l = m_Memory.newObject();
            if (!current_l) {
                throw CMemoryException();
            }
            current_l->pData = pElement;
            current_l->pnext = m_pTable[index];
            m_pTable[index] = current_l;
            return true;
        }
        /**
        Функция обновления элемента в Хеш-таблице. Обновляет, если элемент уже есть добавляет, если элемента еще нет.
        Возвращает false, если был добавлен новый элемент, true если элемент обновлен.
        */
        bool update(T* pElement) {
            uint index = 0;
            leaf* element_l = findLeaf(pElement, index);
            if (element_l) {
                element_l->pData = pElement;
                return true;
            }
            leaf* current_l = m_Memory.newObject();
            if (!current_l) {
                throw CMemoryException();
            }
            current_l->pData = pElement;
            current_l->pnext = m_pTable[index];
            m_pTable[index] = current_l;
            return false;
        }

        /**
        Функция поиска элемента в Хеш-таблице. Возвращает указатель на данные. Если элемента не нашлось, то null.
        Обратите внимание, что для поиска используется частично заполненный объект, т.е. В нем должны быть заполнены поля на основе которых рассчитывается хеш.*/
        T* find(const T& pElement) {
            uint index = 0;
            leaf* element_l = findLeaf(&pElement, index);
            if (element_l) {
                return element_l->pData;
            }
            return nullptr;
        }

        /**
        Функция удаления элемента из Хеш-таблицы. Возвращает false, если не нашлось элемента, true если элемент был удален.
        */
        bool remove(const T& element)
        {
            uint element_h = HashFunc(&element);
            uint index = element_h % m_tableSize;
            leaf* current_l = m_pTable[index];
            if (!current_l) {
                return false;
            }
            if (!Compare(current_l->pData, &element)) {
                m_pTable[index] = current_l->pnext;
                m_Memory.deleteObject(current_l);
                return true;
            }
            while (current_l->pnext && Compare(current_l->pnext->pData, &element)) {
                current_l = current_l->pnext;
            }
            if (!current_l->pnext) {
                return false;
            }
            leaf* new_next_leaf = current_l->pnext->pnext;
            m_Memory.deleteObject(current_l->pnext);
            current_l->pnext = new_next_leaf;
            return true;
        }

        /**
        Удаление всех элементов. Можно вызвать в деструкторе
        */
        void clear()
        {
            m_Memory.clear();
            for (int i = 0; i < m_tableSize; ++i) {
                m_pTable[i] = nullptr;
            }
        }
    private:
        /**

        Элементарная функция поиска узла в Хеш-таблицу. Возвращает найденный узел и в переменную index выставляет актуальный индекс хеш-таблицы.
        Данную функцию следует использовать в функциях add, update, find.
        Алгоритм функции:
         1. вычисляем хеш функцию
         2. вычисляем индекс в Хеш-таблице (приводим вычисленное значение хеш функции к размеру массива)
         3. Перебираем список значений и если нашли, то возвращаем его.
         4. Если ничего не нашли, то возвращаем null
        */
        leaf *findLeaf(const T* pElement, unsigned int & index)
        {
            uint element_h = HashFunc(pElement);
            index = element_h % m_tableSize;
            leaf* current_l = m_pTable[index];
            while (current_l) {
                if (!Compare(current_l->pData, pElement)){
                    return current_l;
                }
                current_l = current_l->pnext;
            }
            return nullptr;
        }

        /**
        Размер Хеш-таблицы
        */
        int m_tableSize;
        /**
        Хеш-таблица
        */
        leaf** m_pTable;
        /**
        Менеджер памяти, предназначен для хранение листов списков разрешения коллизий
        */
        CMemoryManager<leaf> m_Memory;
    };
}; // namespace templates


#endif //SE_STUDY_PAVLIUCHENKOV_HASH_H
