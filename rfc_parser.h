/*
 * rfc_parser.h
 * RFC 9213 API Gateway Zamanlayıcısı
 * Açıklama: HTTP isteklerinin öncelik başlıklarını parse eder ve istek yapılarını oluşturur.
 */

#ifndef RFC_PARSER_H
#define RFC_PARSER_H

#include <stdio.h>
#include <stdlib.h>

// HTTP isteklerini temsil eden yapı
typedef struct HttpRequest {
    int id; // İsteğin benzersiz tanımlayıcısı
    int urgency; // Aciliyet seviyesi (0-7)
    int incremental; // Artımlı bayrak (0 veya 1)
    char* raw_priority_header; // RFC 9213 başlığı
    char* method; // HTTP metodu (GET, POST, vb.)
    char* path; // İstek yolu
} HttpRequest;

// RFC 9213 öncelik başlığını parse eder
// Karmaşıklık: O(n)
HttpRequest* parse_priority_header(const char* raw_header);

// Yeni bir HttpRequest nesnesi oluşturur ve başlığı parse eder
// Karmaşıklık: O(n)
HttpRequest* create_request(int id, const char* method, const char* path, const char* priority_header);

// HttpRequest nesnesini yazdırır
// Karmaşıklık: O(1)
void print_request(const HttpRequest* request);

// HttpRequest için ayrılan belleği serbest bırakır
// Karmaşıklık: O(1)
void free_request(HttpRequest* request);

#endif // RFC_PARSER_H