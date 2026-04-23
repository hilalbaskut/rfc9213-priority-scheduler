/*
 * priority_queue.h
 * RFC 9213 API Gateway Zamanlayıcısı
 * Açıklama: Min-Heap veri yapısı ile HTTP isteklerini öncelik sırasına göre işler.
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "rfc_parser.h"
#include <stdio.h>
#include <stdlib.h>

// Min-Heap yapısı HttpRequest nesnelerini tutmak için
typedef struct MinHeap {
    HttpRequest* data; // HttpRequest nesneleri dizisi
    int size; // Heap'teki mevcut eleman sayısı
    int capacity; // Heap'in maksimum kapasitesi
} MinHeap;

// MinHeap'i verilen kapasiteyle başlatır
// Karmaşıklık: O(1)
MinHeap* init_heap(int capacity);

// MinHeap'e HttpRequest ekler
// Karmaşıklık: O(log n), n = heap'teki eleman sayısı
void insert_request(MinHeap* heap, HttpRequest* request);

// MinHeap'ten minimum (en yüksek öncelikli) HttpRequest'i çıkarır
// Karmaşıklık: O(log n)
HttpRequest* extract_min(MinHeap* heap);

// Heap özelliğini sağlamak için öğeyi yukarı taşır
// Karmaşıklık: O(log n)
void heapify_up(MinHeap* heap, int index);

// Heap özelliğini sağlamak için öğeyi aşağı taşır
// Karmaşıklık: O(log n)
void heapify_down(MinHeap* heap, int index);

// MinHeap için ayrılan belleği serbest bırakır
// Karmaşıklık: O(n), n = heap'teki eleman sayısı
void free_heap(MinHeap* heap);

// MinHeap'in içeriğini yazdırır
// Karmaşıklık: O(n)
void print_heap(const MinHeap* heap);

// MinHeap'in boş olup olmadığını kontrol eder
// Karmaşıklık: O(1)
int is_empty(const MinHeap* heap);

// MinHeap'i yeni kapasiteyle yeniden boyutlandırır
// Karmaşıklık: O(n), n = yeni kapasite
// Dönüş: 1 başarı, 0 başarısızlık
int resize_heap(MinHeap* heap, int new_capacity);

#endif // PRIORITY_QUEUE_H