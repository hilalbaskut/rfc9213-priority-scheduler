# Proje Mimarisi

## Dosyalar Arası İlişki

```
main.c 
   ├─> priority_queue.h 
   │       ├─> rfc_parser.h
   │       │       └─> rfc_parser.c
   │       └─> priority_queue.c
   │               ├─> priority_queue.h
   │               └─> rfc_parser.h
   └─> rfc_parser.h
```

### İlişki Açıklaması:
- **main.c**: Programın giriş noktası. priority_queue.h ve rfc_parser.h'a bağlıdır.
- **priority_queue.h**: Min-Heap tanımını içerir. rfc_parser.h (HttpRequest) üzerinde bağımlı.
- **priority_queue.c**: priority_queue.h ve string işlemleri için <string.h> kullanır.
- **rfc_parser.h**: HTTP isteği yapısını ve fonksiyon bildirimlerini içerir.
- **rfc_parser.c**: rfc_parser.h'a bağlıdır. RFC 9213 header parsing işlemini gerçekleştirir.

## Big-O Analiz Tablosu

| İşlem          | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|----------------|--------------------|-------------------|
| insert         | O(log n)           | O(1)              |
| extract_min    | O(log n)           | O(1)              |
| heapify_up     | O(log n)           | O(1)              |
| heapify_down   | O(log n)           | O(1)              |
| init_heap      | O(1)               | O(n)              |
| parse_header   | O(k)               | O(1)              |

## Tasarım Kararları

### Modüler Mimari
- **Neden header/source ayrımı?**
  - Kodun okunabilirliğini ve yeniden kullanılabilirliğini artırır.
  - Birden fazla dosyada aynı işlevselliği kullanmayı kolaylaştırır.
  - Değişikliklerin etkisini sınırlayarak bakım maliyetini düşürür.

### Neden Min-Heap?
- **Sıralı Dizi veya Bağlı Liste Yerine Min-Heap**:
  - **Ekleme**: Min-Heap'te O(log n), sıralı dizide O(n).
  - **Min Çıkarma**: Min-Heap'te O(log n), sıralı dizide O(1) ama ekleme O(n).
  - **Genel**: Min-Heap, dinamik ve öncelik tabanlı zamanlama için daha uygundur.

### RFC 9213 Entegrasyonu
- **RFC 9213 Nedir?**
  - HTTP istekleri için önceliklendirme mekanizması sağlar.
  - "urgency" ve "incremental" parametreleriyle isteklerin sırasını belirler.
- **Entegrasyon**:
  - `parse_priority_header` fonksiyonu, RFC 9213 başlıklarını işler.
  - Min-Heap, öncelik sırasına göre istekleri işler ve zamanlar.

Bu tasarım, hem performans hem de standart uyumluluğu açısından optimize edilmiştir.