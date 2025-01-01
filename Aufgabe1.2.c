#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Für die Zeitmessung

//#define mit

// Funktion zum Tauschen von zwei Elementen
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partitionierungsfunktion für QuickSort
int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// QuickSort-Funktion
void quickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Funktion für den Bubble Sort
void bubbleSort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Funktion für den Insertion Sort
void insertionSort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Funktion zur Erzeugung des Ausgabedateinamens
void createSortedFileName(char *inputFileName, char *outputFileName) {
    strcpy(outputFileName, inputFileName);
    char *dot = strrchr(outputFileName, '.');
    if (dot) {
        *dot = '\0';
    }
    strcat(outputFileName, "_sorted.txt");
}

// Hilfsfunktion zum Schreiben des Arrays in eine Datei
void printArrayToFile(int *arr, int size, FILE *outputFile) {
    for (int i = 0; i < size; i++) {
        fprintf(outputFile, "%d ", arr[i]);
        if ((i + 1) % 10 == 0) {
            fprintf(outputFile, "\n");
        }
    }
    fprintf(outputFile, "\n");
}

// Funktion zur Überprüfung, ob das Array sortiert ist
int isSorted(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    char *inputFileName = argv[1];
    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    int *numbers = NULL;
    int size = 0, capacity = 10;
    numbers = (int *)malloc(capacity * sizeof(int));
    if (!numbers) {
        perror("Error allocating memory");
        fclose(inputFile);
        return 1;
    }

    while (fscanf(inputFile, "%d", &numbers[size]) == 1) {
        size++;
        if (size >= capacity) {
            capacity *= 2;
            numbers = (int *)realloc(numbers, capacity * sizeof(int));
            if (!numbers) {
                perror("Error reallocating memory");
                fclose(inputFile);
                return 1;
            }
        }
    }
    fclose(inputFile);

    char outputFileName[256];
    createSortedFileName(inputFileName, outputFileName);

    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Error opening output file");
        free(numbers);
        return 1;
    }

    int *numbersCopy = (int *)malloc(size * sizeof(int));
    if (!numbersCopy) {
        perror("Error allocating memory for copy");
        free(numbers);
        fclose(outputFile);
        return 1;
    }

    // Bubble Sort
    memcpy(numbersCopy, numbers, size * sizeof(int));
    clock_t start = clock();
    bubbleSort(numbersCopy, size);
    clock_t end = clock();
    double bubbleSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    fprintf(outputFile, "Bubble Sort Results (Time: %.6f seconds):\n", bubbleSortTime);
#ifdef mit
    printArrayToFile(numbersCopy, size, outputFile);
    if (isSorted(numbersCopy, size)) {
        fprintf(outputFile, "Bubble Sort - Array is correctly sorted.\n");
    } else {
        fprintf(outputFile, "Bubble Sort - Array is not sorted.\n");
    }
#endif
    // Insertion Sort
    memcpy(numbersCopy, numbers, size * sizeof(int));
    start = clock();
    insertionSort(numbersCopy, size);
    end = clock();
    double insertionSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    fprintf(outputFile, "\nInsertion Sort Results (Time: %.6f seconds):\n", insertionSortTime);
#ifdef mit
    printArrayToFile(numbersCopy, size, outputFile);
    if (isSorted(numbersCopy, size)) {
        fprintf(outputFile, "Insertion Sort - Array is correctly sorted.\n");
    } else {
        fprintf(outputFile, "Insertion Sort - Array is not sorted.\n");
    }
#endif
    // Quick Sort
    memcpy(numbersCopy, numbers, size * sizeof(int));
    start = clock();
    quickSort(numbersCopy, 0, size - 1);
    end = clock();
    double quickSortTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    fprintf(outputFile, "\nQuick Sort Results (Time: %.6f seconds):\n", quickSortTime);
#ifdef mit
    printArrayToFile(numbersCopy, size, outputFile);
    if (isSorted(numbersCopy, size)) {
        fprintf(outputFile, "Quick Sort - Array is correctly sorted.\n");
    } else {
        fprintf(outputFile, "Quick Sort - Array is not sorted.\n");
    }
#endif
    fclose(outputFile);
    printf("Sorted numbers and timings written to %s\n", outputFileName);

    free(numbers);
    free(numbersCopy);

    return 0;
}
