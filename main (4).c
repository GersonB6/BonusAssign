#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
    extraMemoryAllocated += sz;
    size_t *ret = malloc(sizeof(size_t) + sz);
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

void DeAlloc(void *ptr)
{
    size_t *pSz = (size_t *)ptr - 1;
    extraMemoryAllocated -= *pSz;
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free((size_t *)ptr - 1);
}

size_t Size(void *ptr)
{
    return ((size_t *)ptr)[-1];
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (int *)Alloc(sizeof(int) * dataSz);
        int i;
        for (i = 0; i < dataSz; i++)
        {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
        fclose(inFile);
    }

    return dataSz;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSort(int *pData, int n)
{
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            if (pData[j] < pData[min_idx])
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            swap(&pData[i], &pData[min_idx]);
        }
    }
}

void insertionSort(int *pData, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = pData[i];
        j = i - 1;

        while (j >= 0 && pData[j] > key)
        {
            pData[j + 1] = pData[j];
            j = j - 1;
        }
        pData[j + 1] = key;
    }
}

void bubbleSort(int *pData, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (pData[j] > pData[j + 1])
            {
                swap(&pData[j], &pData[j + 1]);
            }
        }
    }
}

void merge(int *pData, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            pData[k] = L[i];
            i++;
        }
        else
        {
            pData[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        pData[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        pData[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortHelper(int *pData, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSortHelper(pData, l, m);
        mergeSortHelper(pData, m + 1, r);
        merge(pData, l, m, r);
    }
}

void mergeSort(int *pData, int l, int r)
{
    mergeSortHelper(pData, l, r);
}

void heapify(int *pData, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && pData[l] > pData[largest])
    {
        largest = l;
    }

    if (r < n && pData[r] > pData[largest])
    {
        largest = r;
    }

    if (largest != i)
    {
        swap(&pData[i], &pData[largest]);
        heapify(pData, n, largest);
    }
}

void heapSort(int *pData, int n)
{
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
    {
        heapify(pData, n, i);
    }

    for (i = n - 1; i > 0; i--)
    {
        swap(&pData[0], &pData[i]);
        heapify(pData, i, 0);
    }
}

void printArray(int *pData, int dataSz)
{
    int i;
    printf("\tData:\n\t");
    for (i = 0; i < 100 && i < dataSz; ++i)
    {
        printf("%d ", pData[i]);
    }
    printf("\n\t");

    int sz = dataSz - 100;
    for (i = sz; i < dataSz; ++i)
    {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
  clock_t start, end;
  int i;
    double cpu_time_used;
  char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

  for (i=0;i<3;++i)
  {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)Alloc(sizeof(int)*dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n",dataSz);
    printf("---------------------------\n");

    printf("Selection Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    selectionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Insertion Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    insertionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Bubble Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    bubbleSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    heapSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    DeAlloc(pDataCopy);
    DeAlloc(pDataSrc);
  }

}