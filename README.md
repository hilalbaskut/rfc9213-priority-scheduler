# RFC 9213 Uyumlu API Gateway İstek Zamanlayıcısı

## Proje Özeti

Bu proje, RFC 9213 standardına uyumlu bir HTTP API Gateway İstek Zamanlayıcısıdır. **Min-Heap** veri yapısını kullanarak HTTP isteklerini dinamik olarak öncelik sırasına göre işler. Zamanlayıcı, istek urgency ve incremental parametrelerine dayanarak en kritik istekleri önce işler, böylece kaynak kullanımını optimize eder ve API Gateway performansını artırır.

### Teknik Özellikler
- **Veri Yapısı**: Min-Heap tabanlı dinamik istek yönetimi
- **Standart**: RFC 9213 Structured Fields for HTTP
- **Dil**: C (ANSI/C99 compatible)
- **Derleyici**: GCC (MinGW on Windows)
- **Bellek Yönetimi**: Valgrind ile kontrollü, Address Sanitizer entegrasyonu

---

## Gereksinimler

### Tüm Platformlar
- **GCC Derleyicisi**: gcc 9.0 veya daha yeni sürüm
  - **Windows**: MinGW (GCC for Windows) yüklenmiş olmalı
  - **Linux/MacOS**: Standart GCC paketi

### Linux/MacOS
- `make` build tool'u
- `valgrind` (bellek testi için, opsiyonel)

### Windows
- MinGW (gcc içerir) veya
- build.bat dosyası (make gerektirmez)

---

## Derleme ve Çalıştırma

### Linux/MacOS Kullanıcıları (Makefile Kullanımı)

1. **Projeyi Derleme**:
   ```bash
   make
   ```
   Kaynak dosyaları derleyip `gateway_scheduler` executable dosyasını oluşturur.

2. **Programı Çalıştırma**:
   ```bash
   make run
   ```
   Derleme yapıp sonrasında programı otomatik olarak çalıştırır.

3. **Bellek Testi (Valgrind ile)**:
   ```bash
   make valgrind
   ```
   Programı çalıştırırken bellek sızıntılarını ve hata durumlarını kontrol eder.

4. **Temizlik** (İsteğe bağlı):
   ```bash
   make clean
   ```
   Derlenmiş dosyaları ve executable'ı siler.

### Windows Kullanıcıları

**Yöntem 1: build.bat Kullanımı (Önerilen)**

Komut isteminde (cmd) proje klasöründe aşağıdaki komutu çalıştırın:
```cmd
build.bat
```
Bu komut otomatik olarak projeyi derler ve çalıştırır. `gateway_scheduler.exe` dosyasını oluşturur.

**Yöntem 2: Manuel Derleme**

Eğer build.bat işe yaramazsa, komut isteminde aşağıdaki komutu çalıştırın:
```cmd
gcc -Wall -Wextra -g -Iinclude src/main.c src/priority_queue.c src/rfc_parser.c -o gateway_scheduler.exe
gateway_scheduler.exe
```

**Not**: MinGW'nin gcc komutunun sistem PATH ortamında tanımlı olduğundan emin olun.

### Dosya Açıklamaları

| Dosya | Açıklama |
|-------|----------|
| `main.c` | Programın giriş noktası. İstek verilerini okur ve zamanlayıcıyı yönetir. |
| `priority_queue.c` | Min-Heap veri yapısının C implementasyonu. İstekleri öncelik sırasına göre yönetir. |
| `rfc_parser.c` | RFC 9213 standardındaki HTTP başlıklarını ayrıştırır ve öncelik değerleri çıkarır. |
| `priority_queue.h` | Min-Heap fonksiyonlarının deklarasyonu (insert, extract_min, heapify vb.). |
| `rfc_parser.h` | HttpRequest yapısı ve RFC header parsing fonksiyonlarının arayüzü. |

---

## RFC 9213 ve Min-Heap Mimarisi

### RFC 9213 Nedir?
RFC 9213 (Structured Fields for HTTP), HTTP istekleri için **Priority** başlığını tanımlar. Bu başlık, isteklerin urgency (aciliyet) ve incremental (artırımsal) özelliklerini belirler:

- **Urgency** (0-7): İsteğin aciliyet seviyesi (0 = en acil, 7 = en az acil)
- **Incremental** (bool): İsteğin artırımsal olup olmadığı

### Min-Heap Nedir?
Min-Heap, her ebeveyn düğüm, çocuk düğümünden daha küçük bir değere sahip olan bir ikili ağaç veri yapısıdır. Bu yapı aşağıdaki özellikleri sağlar:

- **En küçük öğeye hızlı erişim**: O(1) zamanda
- **Dinamik ekleme/çıkarma**: O(log n) zamanda
- **Bellek verimliliği**: Dizi tabanlı implementasyon

### Neden Min-Heap?

İstek zamanlama senaryosunda istekler sürekli eklenir ve çıkarılır. Min-Heap kullanımı diğer veri yapılarından daha verimlidir:

| Veri Yapısı | Ekleme | Min Çıkarma | Avantaj/Dezavantaj |
|-------------|--------|-------------|-------------------|
| Sıralı Dizi | O(n) | O(1) | Çıkarma hızlı ancak ekleme yavaş |
| Bağlı Liste | O(n) | O(1) | Çıkarma hızlı ancak ekleme yavaş |
| **Min-Heap** | **O(log n)** | **O(log n)** | **Dengeli ve dinamik işlemler için ideal** |

Min-Heap, dinamik ve öncelik tabanlı zamanlama problemleri için en uygun seçimdir.

---

## Detaylı Mimari Bilgisi

Proje mimarisi, dosya bağımlılıkları, Big-O analizi ve tasarım kararları hakkında **ayrıntılı bilgi** için lütfen [ARCHITECTURE.md](ARCHITECTURE.md) dosyasını inceleyiniz.

### Hızlı Mimari Özeti
- **Modüler Tasarım**: Header/source ayrımı ile yeniden kullanılabilir kod
- **Performans**: Min-Heap, O(log n) dinamik operasyonlar sağlar
- **RFC Uyumluluğu**: RFC 9213 standardına tam uyumlu başlık ayrıştırma
- **Bellek Güvenliği**: Address Sanitizer ve Valgrind ile kontrollü

---

## Hata Giderme

### "gcc komutu tanınmıyor" (Windows)
**Çözüm**: MinGW'nin PATH ortam değişkeninde olmadığı anlamına gelir.
1. MinGW yüklü olduğundan emin olun
2. MinGW bin klasörünü PATH'e ekleyin veya
3. Tam yolu kullanın: `C:\Program Files\MinGW\bin\gcc.exe`

### "make komutu tanınmıyor" (Windows)
**Çözüm**: Bunun yerine `build.bat` dosyasını kullanın:
```cmd
build.bat
```

### Derleme hataları oluşuyor
**Çözüm**: Aşağıdakileri kontrol edin:
1. Tüm dosyaların mevcut olduğundan emin olun (src/ ve include/ klasörleri)
2. GCC'nin düzgün kurulu olduğundan emin olun
3. Komut satırında proje ana klasöründe olduğunuzdan emin olun

### Bellek testi yapılmak isteniyorsa (Linux/MacOS)
```bash
make valgrind
```
Program üzerinde bellek sızıntıları ve hata durumlarının detaylı raporu alırsınız.

---

## Kurulum Adım Adım

### Windows Kurulumu
```cmd
REM 1. Komut İstemini (cmd) açın

REM 2. Proje klasörüne gidin
cd "C:\Users\<YourUsername>\OneDrive\Masaüstü\Final ödevi3"

REM 3. build.bat ile derleyin ve çalıştırın
build.bat

REM Veya manuel derleme için:
gcc -Wall -Wextra -g -Iinclude src/main.c src/priority_queue.c src/rfc_parser.c -o gateway_scheduler.exe
gateway_scheduler.exe
```

---

## İletişim ve Destek

Sorunlarla karşılaşırsanız lütfen aşağıdakileri kontrol edin:
- Dosya yapısının doğru olduğundan emin olun
- GCC derleyicisinin güncel olduğundan emin olun
- [ARCHITECTURE.md](ARCHITECTURE.md) dosyasında mimari detayları inceleyiniz

---
