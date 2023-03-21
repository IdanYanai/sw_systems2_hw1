#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdptArray.h"

typedef struct AdptArray_ {
    PElement* arr;
    int size;
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
} AdptArray, *PAdptArray;


PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc,PRINT_FUNC printFunc) {
    PAdptArray pArr = (PAdptArray)calloc(1, sizeof(AdptArray));
    if (pArr == NULL)
        return NULL;
    pArr->size = 0;
    pArr->arr = NULL;
    pArr->copyFunc = copyFunc;
    pArr->delFunc = delFunc;
    pArr->printFunc = printFunc;
    return pArr;
}

void DeleteAdptArray(PAdptArray pArr) {
    if(pArr == NULL)
        return;
    for(int i = 0; i<pArr->size;i++) {
        if((pArr->arr)[i] != NULL)
            pArr->delFunc((pArr->arr)[i]);
    }
    free(pArr->arr);
    free(pArr);
}

Result SetAdptArrayAt(PAdptArray pArr, int index, PElement elem) {
    PElement* newArr;
    if(pArr == NULL)
        return FAIL;

    // Extend array
    if(index >= pArr->size) {
        if((newArr = (PElement*)calloc((index+1), sizeof(PElement))) == NULL)
            return FAIL;
        memcpy(newArr, pArr->arr, (pArr->size)*sizeof(PElement));
        free(pArr->arr);
        pArr->arr = newArr;
    }

    // swap new element
    if((pArr->arr)[index] != NULL)
        pArr->delFunc((pArr->arr)[index]);
    (pArr->arr)[index] = pArr->copyFunc(elem);

    // update size
    pArr->size = (index >= pArr->size) ? (index+1) : pArr->size;
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray pArr, int index) {
    if(pArr == NULL)
        return NULL;
    if((pArr->arr)[index] == NULL)
        return NULL;
    PElement elem = pArr->copyFunc((pArr->arr)[index]);
    return elem;
}

int GetAdptArraySize(PAdptArray pArr) {
    return pArr->size;
}

void PrintDB(PAdptArray pArr) {
    if(pArr == NULL)
        return;
    for(int i=0;i<pArr->size;i++)
        if((pArr->arr)[i] != NULL)
            pArr->printFunc((pArr->arr)[i]);
}