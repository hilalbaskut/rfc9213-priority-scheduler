/*
 * main.c
 * RFC 9213 API Gateway Zamanlayıcısı
 * Açıklama: Programın giriş noktası. Min-Heap kullanarak HTTP isteklerini öncelik sırasına göre işler.
 */

// Windows'ta UTF-8 konsol desteği için
#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"
#include "rfc_parser.h"

int main() {
    // Windows'ta UTF-8 konsol kodlaması etkinleştir
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    printf("=== API Gateway İstek Zamanlayıcısı (RFC 9213) ===\n\n");

    // MinHeap oluştur
    MinHeap* heap = init_heap(10);
    if (!heap) {
        fprintf(stderr, "Heap oluşturulamadı!\n");
        return 1;
    }

    printf("=== İstekler Ekleniyor ===\n\n");

    // HttpRequest nesneleri oluştur ve ekle
    const char* headers[] = {
        "u=0",
        "u=1, i",
        "u=3",
        "u=5",
        "u=7, i=?1",
        NULL, // Eksik header
        "u=invalid", // Hatalı header
        "u=2",
        "u=4, i",
        "u=6"
    };

    const char* methods[] = {"GET", "POST", "DELETE", "PUT"};
    const char* paths[] = {"/api/resource1", "/api/resource2", "/api/resource3"};

    for (int i = 0; i < 10; i++) {
        const char* header = headers[i % 10];
        const char* method = methods[i % 4];
        const char* path = paths[i % 3];

        HttpRequest* req = create_request(i + 1, method, path, header ? header : "");
        if (!req) {
            fprintf(stderr, "HttpRequest oluşturulamadı!\n");
            continue;
        }

        insert_request(heap, req);
        printf("Eklendi: ");
        print_request(req);
        free_request(req); // Belleği serbest bırak
    }

    printf("\n=== Kuyruk Durumu ===\n\n");
    print_heap(heap);

    printf("\n=== Kuyruk İşleniyor (Min-Heap Extract) ===\n\n");

    while (!is_empty(heap)) {
        HttpRequest* min_req = extract_min(heap);
        if (min_req) {
            printf("Çıkarıldı: ");
            print_request(min_req);
            free_request(min_req);
        }
    }

    // Heap'i serbest bırak
    free_heap(heap);

    printf("\nBellek başarıyla serbest bırakıldı.\n");
    return 0;
}