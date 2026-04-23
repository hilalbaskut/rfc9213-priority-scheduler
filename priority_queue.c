/*
 * priority_queue.c
 * RFC 9213 API Gateway Zamanlayıcısı
 * Açıklama: Min-Heap veri yapısını kullanarak HTTP isteklerini öncelik sırasına göre işler.
 * Neden Array Yerine Min-Heap?
 * - Ekleme: Heap O(log n) vs Sıralı Dizi O(n)
 * - Min çıkarma: Heap O(log n) vs Dizi O(1) ama ekleme O(n)
 * - Genel: Heap, dinamik istek zamanlama için daha uygundur.
 */

#include "priority_queue.h"
#include <string.h>

// Belirtilen kapasitede bir MinHeap başlatır
// Karmaşıklık: O(1)
MinHeap* init_heap(int capacity) {
    if (capacity <= 0) return NULL;

    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;

    heap->data = malloc(sizeof(HttpRequest) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Heap özelliğini sağlamak için elemanı yukarı taşır
// Karmaşıklık: O(log n)
void heapify_up(MinHeap* heap, int index) {
    if (!heap || index <= 0 || index >= heap->size) return;

    int parent = (index - 1) / 2;
    while (index > 0 && heap->data[index].urgency < heap->data[parent].urgency) {
        HttpRequest temp = heap->data[index];
        heap->data[index] = heap->data[parent];
        heap->data[parent] = temp;

        index = parent;
        parent = (index - 1) / 2;
    }
}

// Heap özelliğini sağlamak için elemanı aşağı taşır
// Karmaşıklık: O(log n)
void heapify_down(MinHeap* heap, int index) {
    if (!heap || index < 0 || index >= heap->size) return;

    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left].urgency < heap->data[smallest].urgency) {
        smallest = left;
    }

    if (right < heap->size && heap->data[right].urgency < heap->data[smallest].urgency) {
        smallest = right;
    }

    if (smallest != index) {
        HttpRequest temp = heap->data[index];
        heap->data[index] = heap->data[smallest];
        heap->data[smallest] = temp;

        heapify_down(heap, smallest);
    }
}

// Ekleme İşlemi: MinHeap'e HttpRequest ekler
// Karmaşıklık: O(log n)
// NOT: Deep copy yapılıyor. Aksi halde main.c'de free_request() çağrıldığında
// heap içindeki veriler bozulur. Pointer alanları strdup() ile kopyalanır.
void insert_request(MinHeap* heap, HttpRequest* req) {
    if (!heap || !req) return;

    if (heap->size == heap->capacity) {
        // resize_heap başarısız olursa, ekleme işlemini iptal et
        if (!resize_heap(heap, heap->capacity * 2)) {
            fprintf(stderr, "Hata: İstek eklenemedi. Bellek yetersiz.\n");
            return;
        }
    }

    // Deep copy ile verileri heap'e kopyala
    heap->data[heap->size].id = req->id;
    heap->data[heap->size].urgency = req->urgency;
    heap->data[heap->size].incremental = req->incremental;
    heap->data[heap->size].raw_priority_header = strdup(req->raw_priority_header ? req->raw_priority_header : "");
    heap->data[heap->size].method = strdup(req->method ? req->method : "");
    heap->data[heap->size].path = strdup(req->path ? req->path : "");
    
    heap->size++;
    // Eklenen öğeyi heap özelliğini sağlamak için yukarı taşı
    heapify_up(heap, heap->size - 1);
}

// Minimum (En yüksek öncelikli) HttpRequest'i çıkar
// Karmaşıklık: O(log n)
// NOT: Deep copy yapılıyor. Pointer alanları strdup() ile kopyalanır.
HttpRequest* extract_min(MinHeap* heap) {
    if (!heap || heap->size == 0) return NULL;

    HttpRequest* min = malloc(sizeof(HttpRequest));
    if (!min) return NULL;

    // Deep copy: Pointer alanlarını strdup() ile kopyala
    min->id = heap->data[0].id;
    min->urgency = heap->data[0].urgency;
    min->incremental = heap->data[0].incremental;
    min->raw_priority_header = strdup(heap->data[0].raw_priority_header ? heap->data[0].raw_priority_header : "");
    min->method = strdup(heap->data[0].method ? heap->data[0].method : "");
    min->path = strdup(heap->data[0].path ? heap->data[0].path : "");
    
    // Kök elemanın pointer alanlarını serbest bırak (min'e zaten kopyalandı)
    free(heap->data[0].raw_priority_header);
    free(heap->data[0].method);
    free(heap->data[0].path);

    // Son elemanın pointer sahipliğini köke devret (deep copy değil, taşıma)
    // Bellek sızıntığını ve çift free'yi önlemek için
    heap->data[0].id = heap->data[heap->size - 1].id;
    heap->data[0].urgency = heap->data[heap->size - 1].urgency;
    heap->data[0].incremental = heap->data[heap->size - 1].incremental;
    heap->data[0].raw_priority_header = heap->data[heap->size - 1].raw_priority_header;
    heap->data[0].method = heap->data[heap->size - 1].method;
    heap->data[0].path = heap->data[heap->size - 1].path;
    heap->size--;

    heapify_down(heap, 0);
    return min;
}

// Daha fazla eleman için MinHeap'i yeniden boyutlandırır
// Karmaşıklık: O(n)
// NOT: realloc başarısız olursa stderr'e hata yazdırır
int resize_heap(MinHeap* heap, int new_capacity) {
    if (!heap || new_capacity <= heap->capacity) return 1;

    HttpRequest* new_data = realloc(heap->data, sizeof(HttpRequest) * new_capacity);
    if (!new_data) {
        // realloc başarısız olduysa stderr'e hata yazdır
        fprintf(stderr, "Hata: Heap yeniden boyutlandırılamadı. Eski veri korundu.\n");
        return 0; // Başarısızlık
    }

    heap->data = new_data;
    heap->capacity = new_capacity;
    return 1; // Başarı
}

// MinHeap için ayrılan belleği serbest bırakır
// Karmaşıklık: O(n)
void free_heap(MinHeap* heap) {
    if (!heap) return;

    for (int i = 0; i < heap->size; i++) {
        free(heap->data[i].raw_priority_header);
        free(heap->data[i].method);
        free(heap->data[i].path);
    }

    free(heap->data);
    free(heap);
}

// MinHeap'in içeriğini yazdırır
// Karmaşıklık: O(n)
void print_heap(const MinHeap* heap) {
    if (!heap) {
        printf("Heap tanımsız!\n");
        return;
    }

    printf("MinHeap {\n");
    for (int i = 0; i < heap->size; i++) {
        printf("  [%d]: id=%d, urgency=%d, incremental=%d, method=%s, path=%s\n",
               i, heap->data[i].id, heap->data[i].urgency, heap->data[i].incremental,
               heap->data[i].method ? heap->data[i].method : "(null)",
               heap->data[i].path ? heap->data[i].path : "(null)");
    }
    printf("}\n");
}

// MinHeap'in boş olup olmadığını kontrol eder
// Karmaşıklık: O(1)
// NOT: NULL heap için -1 döndürür, boş heap için 1 döndürür
int is_empty(const MinHeap* heap) {
    if (!heap) {
        return -1; // NULL heap durumu
    }
    return heap->size == 0;
}