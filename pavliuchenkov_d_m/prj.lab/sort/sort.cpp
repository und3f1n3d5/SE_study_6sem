//
// Created by dmitrij on 3/17/22.
//
#include "sort.h"

namespace templates {

    void merge(void **ppArray, int left, int mid, int right, CompareSortType pCompareFunc) {
        void *result[right - left];
        int it1 = 0, it2 = 0;
        while (mid + it2 < right && it1 + left < mid) {
            if (pCompareFunc(ppArray[it1 + left], ppArray[mid + it2]) <= 0) {
                ++it2;
                result[it1 + it2 - 1] = ppArray[mid + it2 - 1];
            }
            else {
                ++it1;
                result[it1 + it2 - 1] = ppArray[it1 + left - 1];
            }
        }
        for (; left + it1 < mid; ++it1) {
            result[it1 + it2] = ppArray[left + it1];
        }
        for (; mid + it2 < right; ++it2) {
            result[it1 + it2] = ppArray[mid + it2];
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
