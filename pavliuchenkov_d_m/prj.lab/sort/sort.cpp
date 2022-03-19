//
// Created by dmitrij on 3/17/22.
//
#include "sort.h"

namespace templates {

    void merge(void **ppArray, int left, int mid, int right, CompareSortType pCompareFunc) {
        void *result[right - left];
        int iterator1 = 0, iterator2 = 0;
        while (mid + iterator2 < right && iterator1 + left < mid) {
            if (pCompareFunc(ppArray[iterator1 + left], ppArray[mid + iterator2]) <= 0) {
                ++iterator2;
                result[iterator1 + iterator2 - 1] = ppArray[mid + iterator2 - 1];
            }
            else {
                ++iterator1;
                result[iterator1 + iterator2 - 1] = ppArray[iterator1 + left - 1];
            }
        }
        for (; left + iterator1 < mid; ++iterator1) {
            result[iterator1 + iterator2] = ppArray[left + iterator1];
        }
        for (; mid + iterator2 < right; ++iterator2) {
            result[iterator1 + iterator2] = ppArray[mid + iterator2];
        }
        for (int i = 0; i < right - left; ++i) {
            ppArray[left + i] = result[i];
        }
    }

    void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc, int left, int right) {
        if (length == 1) {
            return;
        }
        if (right == 0) {
            right = length;
        }
        int mid = (left + right) / 2;
        mergeSort(ppArray, mid - left, pCompareFunc, left, mid);
        mergeSort(ppArray, right - mid, pCompareFunc, mid, right);
        merge(ppArray, left, mid, right, pCompareFunc);
    }

}; // namespace templates
