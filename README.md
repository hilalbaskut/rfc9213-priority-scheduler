# API Gateway Request Scheduler

Bu proje, bir API Gateway için RFC 9213 uyumlu bir İstek Zamanlayıcı uygulamasıdır. Min-Heap veri yapısını kullanarak HTTP isteklerini öncelik sırasına göre işler.

## Derleme ve Çalıştırma

Projeyi derlemek ve çalıştırmak için aşağıdaki adımları izleyin:

1. Derleme:
   ```bash
   make
   ```

2. Programı çalıştırma:
   ```bash
   make run
   ```

3. Bellek sızıntılarını kontrol etmek için Valgrind kullanımı:
   ```bash
   make valgrind
   ```

4. Temizlik:
   ```bash
   make clean
   ```

## Proje Dosya Yapısı

```
Final ödevi3/
├── src/
│   ├── main.c
│   ├── priority_queue.c
│   └── rfc_parser.c
├── include/
│   ├── priority_queue.h
│   └── rfc_parser.h
├── Makefile
└── README.md
```

## RFC 9213 ve Min-Heap

### RFC 9213 Nedir?
RFC 9213, HTTP istekleri için önceliklendirme mekanizmasını tanımlar. Bu mekanizma, "urgency" ve "incremental" gibi parametrelerle isteklerin öncelik sırasını belirler.

### Min-Heap Nedir?
Min-Heap, her düğümün ebeveyninden daha küçük bir değere sahip olduğu bir veri yapısıdır. Bu özellik, en küçük öğeyi hızlı bir şekilde bulmayı sağlar.

### Neden Min-Heap Kullanıldı?
Min-Heap, istekleri öncelik sırasına göre işlemek için idealdir. Array yerine Min-Heap kullanmamızın sebebi:
- **Ekleme ve çıkarma işlemleri**: Min-Heap'te $O(\log n)$ karmaşıklıkla gerçekleştirilir.
- **Öncelikli erişim**: En düşük öncelikli öğeye $O(1)$ karmaşıklıkla erişilir.

**Array vs Min-Heap Karşılaştırması:**
- Sıralı Dizi: Ekleme $O(n)$, çıkarma $O(1)$
- Min-Heap: Ekleme $O(\log n)$, çıkarma $O(\log n)$

Min-Heap, dinamik istek zamanlama senaryolarında (istekler sürekli eklenir/çıkarılır) daha verimlidir.