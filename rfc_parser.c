#include "rfc_parser.h"
#include <string.h>
#include <ctype.h>

// Ham öncelik başlığını parse eder
HttpRequest* parse_priority_header(const char* raw_header) {
    if (!raw_header) {
        return NULL;
    }

    HttpRequest* request = malloc(sizeof(HttpRequest));
    if (!request) {
        return NULL;
    }

    request->urgency = 3; // Varsayılan değer
    request->incremental = 0; // Varsayılan değer

    const char* u_pos = strstr(raw_header, "u=");
    if (u_pos) {
        // u= den sonra gelen karakterin rakam olduğunu kontrol et
        if (isdigit(*(u_pos + 2))) {
            int urgency = *(u_pos + 2) - '0';
            if (urgency >= 0 && urgency <= 7) {
                request->urgency = urgency;
            }
        }
    }

    // RFC 9213 artımlı bayrağını kontrol et. Sadece ", i" formatını yakala.
    // "u=invalid" gibi başka kelimelerin içindeki "i" harfini yakalamaktan kaçın.
    const char* i_pos = strstr(raw_header, ", i");
    if (i_pos) {
        const char* after_i = i_pos + 3; // ", i" sonrası
        // i=?1 veya i (sonda) veya i, (başka parametreden önce) durumlarını kontrol et
        if (*after_i == '\0' || *after_i == ',' || 
            strncmp(after_i, "=?1", 3) == 0) {
            request->incremental = 1;
        }
    }

    request->raw_priority_header = strdup(raw_header ? raw_header : "");
    return request;
}

// Yeni bir HttpRequest nesnesi oluşturur
HttpRequest* create_request(int id, const char* method, const char* path, const char* priority_header) {
    if (!method || !path || !priority_header) {
        return NULL;
    }

    HttpRequest* request = parse_priority_header(priority_header);
    if (!request) {
        return NULL;
    }

    request->id = id;
    request->method = strdup(method ? method : "");
    request->path = strdup(path ? path : "");

    return request;
}

// HttpRequest nesnesini yazdırır
void print_request(const HttpRequest* req) {
    if (!req) {
        return;
    }

    printf("HttpRequest {\n");
    printf("  id: %d,\n", req->id);
    printf("  urgency: %d,\n", req->urgency);
    printf("  incremental: %d,\n", req->incremental);
    printf("  raw_priority_header: %s,\n", req->raw_priority_header ? req->raw_priority_header : "(null)");
    printf("  method: %s,\n", req->method ? req->method : "(null)");
    printf("  path: %s\n", req->path ? req->path : "(null)");
    printf("}\n");
}

// HttpRequest için ayrılan belleği serbest bırakır
void free_request(HttpRequest* req) {
    if (!req) {
        return;
    }

    free(req->raw_priority_header);
    free(req->method);
    free(req->path);
    free(req);
}