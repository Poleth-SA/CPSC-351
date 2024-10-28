/// Group 6
/// Group Members: Alexia Rodriguez, Christian Beshara, Jacqueline Villasenor, Poleth Salmeron Acosta

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE (sizeof(list)/sizeof(*list))

// The list of integers to be sorted
//from hints
int list[] = {7, 12, 19, 3, 18, 4, 2, -5, 6, 15, 8};
int result[SIZE] = {0};

//Structures for passing data to threads
//From hints
typedef struct {
    int* subArray;
    unsigned int size;
} SortingThreadParameters;

typedef struct {
    SortingThreadParameters left;
    SortingThreadParameters right;
} MergingThreadParameters;

//now create the first sorting thread, passing it paramsLeft as a parameter
//Bubble sort to sorting threads
void* sortThread(void* params) {
    SortingThreadParameters* p = (SortingThreadParameters*)params;
    int* arr = p->subArray;
    int n = p->size;
    
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    return NULL;
}

//Merge function
void* mergeThread(void* params) {
    MergingThreadParameters* p = (MergingThreadParameters*)params;
    int* left = p->left.subArray;
    int* right = p->right.subArray;
    int leftSize = p->left.size;
    int rightSize = p->right.size;
    
    int i = 0, j = 0, k = 0;
    
    //Merge the two sorted subarrays into result array
    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            result[k++] = left[i++];
        } else {
            result[k++] = right[j++];
        }
    }
    
    //to copy the remaining elements from the left subarray
    while (i < leftSize) {
        result[k++] = left[i++];
    }
    
    //to cpoy the remaining elements from the right subarray
    while (j < rightSize) {
        result[k++] = right[j++];
    }
    
    return NULL;
}

int main() {
    pthread_t sortThread1, sortThread2, mergeThread1;
    
    printf("Original array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
    
    //Create parameters for first sorting thread
    SortingThreadParameters* paramsLeft = malloc(sizeof(SortingThreadParameters));
    paramsLeft->subArray = list;
    paramsLeft->size = SIZE/2;
    
    //Second sorting thread
    SortingThreadParameters* paramsRight = malloc(sizeof(SortingThreadParameters));
    paramsRight->subArray = list + paramsLeft->size;
    paramsRight->size = SIZE - paramsLeft->size;
    
    //Create and start sorting threads
    pthread_create(&sortThread1, NULL, sortThread, paramsLeft);
    pthread_create(&sortThread2, NULL, sortThread, paramsRight);
    
    //Wait for sorting threads to complete
    pthread_join(sortThread1, NULL);
    pthread_join(sortThread2, NULL);
    
    printf("Left subarray after sorting: ");
    for (int i = 0; i < paramsLeft->size; i++) {
        printf("%d ", paramsLeft->subArray[i]);
    }
    printf("\n");
    
    printf("Right subarray after sorting: ");
    for (int i = 0; i < paramsRight->size; i++) {
        printf("%d ", paramsRight->subArray[i]);
    }
    printf("\n");
    
    //Create parameters for merge thread
    MergingThreadParameters* paramsMerge = malloc(sizeof(MergingThreadParameters));
    paramsMerge->left = *paramsLeft;
    paramsMerge->right = *paramsRight;
    
    //Create and start merge thread
    pthread_create(&mergeThread1, NULL, mergeThread, paramsMerge);
    
    //Wait for merge thread to complete
    pthread_join(mergeThread1, NULL);
    
    printf("Final sorted array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(paramsLeft);
    free(paramsRight);
    free(paramsMerge);
    
    return 0;
}