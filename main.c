#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define ARRAY_SIZE(a) sizeof (a) /sizeof(a[0])


#define TIME_TEST(testCode, time) { \
    clock_t start_time = clock () ; \
    testCode \
        clock_t end_time = clock () ;\
    clock_t sort_time = end_time - start_time ; \
    time = ( double ) sort_time / CLOCKS_PER_SEC ; \
 }


// функция сортировки
typedef struct SortFunc {
    void (*sort )(int *a, size_t n); // указатель на функцию
    // сортировки
    char name[64]; // имя сортировки,
    // используемое при выводе
} SortFunc;

// функция генерации
typedef struct GeneratingFunc {
    void (*generate )(int *a, size_t n); // указатель на функцию
    // генерации последоват.
    char name[64]; // имя генератора,
    // используемое при выводе
} GeneratingFunc;


typedef struct SortFuncNComps {
    unsigned long long (*sort )(int *a, size_t n);

    char name[64];

    long long nComps;
} SortFuncNComps;

void inputArray_(int *const a, const size_t n) {
    for (size_t i = 0; i < n; i++)
        scanf("%d", &a[i]);
}

void outputArray_(const int *const a, const size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int isOrdered(const int *a, const size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (a[i] < a[i - 1])
            return 0;
    }

    return 1;
}

void generateRandomArray(int *a, size_t size) {
    for (int i = 0; i < size; ++i)
        a[i] = rand();
}

void generateOrderedArray(int *a, size_t size) {
    for (int i = 0; i < size - 1; i++)
        a[i] = i;
}

void generateOrderedBackwards(int *a, size_t size) {
    int i = 0;
    int j = size - 1;
    while (j > 0)
        a[i++] = j--;
}

void checkTime(void (*sortFunc )(int *, size_t),
               void (*generateFunc )(int *, size_t),
               size_t size, char *experimentName) {
    static size_t runCounter = 1;
// генерация последовательности
    static int innerBuffer[100000];
    generateFunc(innerBuffer, size);
    printf("Run #%zu| ", runCounter++);
    printf("Name: %s\n", experimentName);
    // замер времени
    double time;
    TIME_TEST
    ({
         sortFunc(innerBuffer, size);
     }, time);

    // результаты замера
    printf("Status:");
    if (isOrdered(innerBuffer, size)) {
        printf("OK! Time : %.3fs.\n", time);

        // запись в файл
        char filename[256];
        sprintf(filename, "./data/%s.csv", experimentName);
        FILE *f = fopen(filename, "a");
        if (f == NULL) {
            printf(" FileOpenError %s", filename);
            exit(1);
        }
        fprintf(f, "%zu; %.3f\n", size, time);
        fclose(f);
    } else {
        printf("Wrong!\n");

        // вывод массива, который не смог быть отсортирован
        outputArray_(innerBuffer, size);

        exit(1);
    }
}

void checkNComps(unsigned long long (*sortFunc )(int *, size_t),
                 void (*generateFunc )(int *, size_t),
                 size_t size, char *experimentName) {
    static size_t runCounter = 1;
// генерация последовательности
    static int innerBuffer[100000];
    generateFunc(innerBuffer, size);
    printf("Run #%zu| ", runCounter++);
    printf(" Name : %s\n", experimentName);
    // получение количества операций
    unsigned long long nComps = sortFunc(innerBuffer, size);

    // результаты
    printf(" Status : ");
    if (isOrdered(innerBuffer, size)) {
        printf("OK! NComps : %.3lld.\n", nComps);

        // запись в файл
        char filename[256];
        sprintf(filename, "./data/%s.csv", experimentName);
        FILE *f = fopen(filename, "a");
        if (f == NULL) {
            printf(" FileOpenError %s", filename);
            exit(1);
        }
        fprintf(f, "%zu; %.3lld\n", size, nComps);
        fclose(f);
    } else {
        printf("Wrong!\n");

        // вывод массива, который не смог быть отсортирован
        outputArray_(innerBuffer, size);

        exit(1);
    }
}

void bubbleSort(int arr[], int n) {
    for (size_t i = 0; i < n - 1; i++)
        for (size_t j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

void selectionSort(int *num, int size) {
    int min, temp;
    for (int i = 0; i < size - 1; i++) {
        min = i;
        for (int j = i + 1; j < size; j++) {
            if (num[j] < num[min])
                min = j;
        }
        temp = num[i];
        num[i] = num[min];
        num[min] = temp;
    }
}

void insertionSort(int *a, const size_t size) {
    for (size_t i = 1; i < size; i++) {
        int t = a[i];
        int j = i;
        while (j > 0 && a[j - 1] > t) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = t;
    }
}

void combSort(int *a, const size_t n) {
    size_t step = n;
    int swapped = 1;
    while (step > 1 || swapped) {
        if (step > 1)
            step /= 1.24733;
        swapped = 0;
        for (size_t i = 0, j = i + step; j < n; i++, j++)
            if (a[i] > a[j]) {
                swap(&a[i], &a[j]);
                swapped = 1;
            }
    }
}

void shellSort(int *array, int size) {
    for (int s = size / 2; s > 0; s /= 2) {
        for (int i = s; i < size; ++i) {
            for (int j = i - s; j >= 0 && array[j] > array[j + s]; j -= s) {
                int temp = array[j];
                array[j] = array[j + s];
                array[j + s] = temp;
            }
        }
    }
}

int digit(int n, int k, int N, int M) {
    return (n >> (N * k) & (M - 1));
}

void radixSort_(int *l, int *r, int N) {
    int k = (32 + N - 1) / N;
    int M = 1 << N;
    int sz = r - l;
    int *b = (int *) malloc(sizeof(int) * sz);
    int *c = (int *) malloc(sizeof(int) * M);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < M; j++)
            c[j] = 0;

        for (int *j = l; j < r; j++)
            c[digit(*j, i, N, M)]++;

        for (int j = 1; j < M; j++)
            c[j] += c[j - 1];

        for (int *j = r - 1; j >= l; j--)
            b[--c[digit(*j, i, N, M)]] = *j;

        int cur = 0;
        for (int *j = l; j < r; j++)
            *j = b[cur++];
    }
    free(b);
    free(c);
}

void radixSort(int *a, size_t n) {
    radixSort_(a, a + n, 8);
}

void timeExperiment() {
    SortFunc sorts[] = {
            {bubbleSort,    "bubbleSort"},
            {selectionSort, "selectionSort"},
            {insertionSort, "insertionSort"},
            {combSort,      "combSort"},
            {shellSort,     "shellSort"},
            {radixSort,     "radixSort"},
    };
    const unsigned FUNCS_N = ARRAY_SIZE(sorts);

    // описание функций генерации
    GeneratingFunc generatingFuncs[] = {
            {generateRandomArray,      "random"},
            {generateOrderedArray,     "ordered"},
            {generateOrderedBackwards, "orderedBackwards"}
    };
    const unsigned CASES_N = ARRAY_SIZE(generatingFuncs);

    // запись статистики в файл
    for (size_t size = 10000; size <= 100000; size += 10000) {
        printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        printf("Size : %d\n", size);
        for (int i = 0; i < FUNCS_N; i++) {
            for (int j = 0; j < CASES_N; j++) {
                // генерация имени файла
                static char filename[128];
                sprintf(filename, "%s_%s_time",
                        sorts[i].name, generatingFuncs[j].name);
                checkTime(sorts[i].sort,
                          generatingFuncs[j].generate,
                          size, filename);
            }
        }
        printf("\n");
    }
}

long long getSelectionSortNComp(int *a, size_t n) {
    long long nComps = 0;
    for (int i = 0; ++nComps && i < n; i++) {
        int min = a[i];
        int minIndex = i;
        for (int j = i + 1; ++nComps && j < n; j++)
            if (++nComps && a[j] < min) {
                min = a[j];
                minIndex = j;
            }
        if (++nComps && i != minIndex)
            swap(&a[i], &a[minIndex]);
    }
    return nComps;
}

long long getBubbleSortNComp(int arr[], int n) {
    long long nComps = 0;
    for (size_t i = 0; ++nComps && i < n - 1; i++)
        for (size_t j = 0; ++nComps && j < n - i - 1; j++)
            if (++nComps && arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
    return nComps;
}

long long getInsertionSortNComp(int *a, const size_t size) {
    long long nComps = 0;
    for (size_t i = 1; ++nComps && i < size; i++) {
        int t = a[i];
        int j = i;
        while (++nComps && j > 0 && a[j - 1] > t) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = t;
    }
    return nComps;
}

long long getShellSortNComp(int *array, int size) {
    long long nComps = 0;
    for (int s = size / 2; ++nComps && s > 0; s /= 2) {
        for (int i = s; ++nComps && i < size; ++i) {
            for (int j = i - s; ++nComps && j >= 0 && array[j] > array[j + s]; j -= s) {
                int temp = array[j];
                array[j] = array[j + s];
                array[j + s] = temp;
            }
        }
    }
    return nComps;
}

long long getCombSortNComp(int *a, const size_t n) {
    long long nComps = 0;
    size_t step = n;
    int swapped = 1;
    while (++nComps && step > 1 || swapped) {
        if (++nComps && step > 1)
            step /= 1.24733;
        swapped = 0;
        for (size_t i = 0, j = i + step; ++nComps && j < n; i++, j++)
            if (++nComps && a[i] > a[j]) {
                swap(&a[i], &a[j]);
                swapped = 1;
            }
    }
    return nComps;
}

long long getRadixSortNComp_(int *l, int *r, int N) {
    long long nComps = 0;
    int k = (32 + N - 1) / N;
    int M = 1 << N;
    int sz = r - l;
    int *b = (int *) malloc(sizeof(int) * sz);
    int *c = (int *) malloc(sizeof(int) * M);
    for (int i = 0; ++nComps && i < k; i++) {
        for (int j = 0; ++nComps && j < M; j++)
            c[j] = 0;

        for (int *j = l; ++nComps && j < r; j++)
            c[digit(*j, i, N, M)]++;

        for (int j = 1; ++nComps && j < M; j++)
            c[j] += c[j - 1];

        for (int *j = r - 1; ++nComps && j >= l; j--)
            b[--c[digit(*j, i, N, M)]] = *j;

        int cur = 0;
        for (int *j = l; ++nComps && j < r; j++)
            *j = b[cur++];
    }
    free(b);
    free(c);

    return nComps;
}

long long getRadixSortNComp(int *a, const size_t n) {
    return getRadixSortNComp_(a, a + n, 8);
}

void NCompsExperiment() {
// описание функций сортировки
    SortFuncNComps sorts[] = {
            {getBubbleSortNComp,    " getBubbleSortNComp "},
            {getSelectionSortNComp, " getSelectionSortNComp "},
            {getInsertionSortNComp, " getInsertionSortNComp "},
            {getShellSortNComp,     " getShellSortNComp "},
            {getCombSortNComp,      " getCombSortNComp "},
            {getRadixSortNComp,     " getRadixSortNComp "}
    };

    const unsigned FUNCS_N = ARRAY_SIZE(sorts);

// описание функций генерации
    GeneratingFunc generatingFuncs[] = {
// генерируется случайный массив
            {generateRandomArray,      " random "},
// генерируется массив 0, 1, 2, ..., n - 1
            {generateOrderedArray,     " ordered "},
// генерируется массив n - 1, n - 2, ..., 0
            {generateOrderedBackwards, " orderedBackwards "}
    };
    const unsigned CASES_N = ARRAY_SIZE
                             (generatingFuncs);

// запись статистики в файл
    for (size_t size = 10000; size <= 100000; size += 10000) {
        printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        printf(" Size : %d\n", size);
        for (int i = 0; i < FUNCS_N; i++) {
            for (int j = 0; j < CASES_N; j++) {
// генерация имени файла
                static char filename[128];
                sprintf(filename,
                        "%s%s_nComps ",
                        sorts[i].name, generatingFuncs[j].name);
                checkNComps(sorts[i]
                                    .sort,
                            generatingFuncs[j].generate,
                            size, filename);
            }
        }
        printf("\n");
    }
}


int main() {
    NCompsExperiment();
}


