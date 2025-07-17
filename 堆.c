#include <stdlib.h>
typedef struct {
    int *arr;
    int size;
} Heap;
int* make_int_arr(int size) {
    return (int*)calloc(size, 4);
}
void init_heap(Heap* heap, int total_size) {
    heap->arr = make_int_arr(total_size);
    heap->size = 0;
}
void clear_heap(Heap* heap) {
    free(heap->arr);
    heap->size = 0;
}
void swap(int* x, int* y) {
    *x = (*x) ^ (*y);
    *y = (*x) ^ (*y);
    *x = (*x) ^ (*y);
}
void push_heap(Heap* heap, int x) {
    int* arr = heap->arr;
    arr[++(heap->size)] = x;
    int now = heap->size;
    while (now) {
        int next = now >> 1;
        if (arr[next] > arr[now]) {
            swap(arr + next, arr + now);
        } else {
            break;
        }
        now = next;
    }
}
void pop_heap(Heap* heap) {
    int* arr = heap->arr;
    swap(arr + 1, arr + heap->size);
    --(heap->size);
    int now = 1;
    while ((now << 1) <= heap->size) {
        int next = now << 1;
        if (next + 1 <= heap->size && arr[next + 1] < arr[next]) {
            ++next;
        }
        if (arr[next] < arr[now]) {
            swap(arr + next, arr + now);
        } else {
            break;
        }
        now = next;
    }
}
int get_top(Heap heap) {
    return heap.arr[1];
}