# 🚦 Pedestrian Traffic Light dengan Interrupt (Arduino)

## 📌 Deskripsi

Project ini merupakan simulasi **lampu lalu lintas dengan tombol penyeberang (pedestrian crossing)** menggunakan Arduino. Sistem ini menggunakan **interrupt** untuk mendeteksi tombol, sehingga lebih responsif dibandingkan polling biasa.

Ketika tombol ditekan:

1. Lampu kendaraan berubah menjadi merah
2. Lampu pedestrian berubah menjadi hijau
3. Setelah beberapa detik, sistem masuk ke fase transisi (lampu kuning berkedip)
4. Sistem kembali ke kondisi awal

## ⚙️ Fitur Utama

* ✅ Menggunakan **Interrupt (attachInterrupt)**
* ✅ Implementasi **Finite State Machine (FSM)**
* ✅ Debounce sederhana untuk tombol
* ✅ Mode transisi dengan **kedip lampu kuning**
* ✅ Mendukung 2 jalur pedestrian

## 🔌 Konfigurasi Pin

### 🚗 Lampu Kendaraan

| Fungsi | Pin |
| ------ | --- |
| Merah  | 8   |
| Kuning | 9   |
| Hijau  | 10  |

### 🚶 Lampu Pedestrian

| Fungsi      | Pin |
| ----------- | --- |
| Ped 1 Merah | 4   |
| Ped 1 Hijau | 5   |
| Ped 2 Merah | 6   |
| Ped 2 Hijau | 7   |

### 🔘 Tombol

| Fungsi | Pin           |
| ------ | ------------- |
| Tombol | 2 (Interrupt) |

## ⏱️ Pengaturan Waktu

| Parameter           | Nilai   |
| ------------------- | ------- |
| Waktu penyeberangan | 5000 ms |
| Delay kedip kuning  | 500 ms  |
| Jumlah kedip        | 3x      |
| Debounce            | 50 ms   |

## 🔄 State Machine

Sistem memiliki 3 state utama:

### 1. AWAL

* Kendaraan: 🟢 Hijau
* Pedestrian: 🔴 Merah

### 2. PENYEBERANGAN

* Kendaraan: 🔴 Merah
* Pedestrian: 🟢 Hijau

### 3. TRANSISI

* Lampu kuning kendaraan berkedip
* Pedestrian kembali merah

## ⚡ Cara Kerja Interrupt

* Tombol terhubung ke pin **2**
* Menggunakan:

  ```cpp
  attachInterrupt(digitalPinToInterrupt(PIN_TOMBOL), ISR_Tombol, FALLING);
  ```
* Saat tombol ditekan:

  * ISR hanya mengubah flag `tombolDitekan = true`
  * Proses utama dijalankan di `loop()`

## 🧠 Alur Program

1. Sistem mulai di state **AWAL**
2. Menunggu tombol ditekan
3. Jika ditekan:

   * Masuk ke **PENYEBERANGAN**
   * Delay 5 detik
4. Masuk ke **TRANSISI**

   * Lampu kuning berkedip
5. Kembali ke **AWAL**

## 🛠️ Cara Menggunakan

1. Upload kode ke Arduino
2. Rangkai LED sesuai pin
3. Gunakan push button di pin 2 (dengan pull-up internal)
4. Tekan tombol untuk simulasi penyeberangan

## 💡 Catatan Penting

* Tombol menggunakan **INPUT_PULLUP**, jadi logika terbalik:

  * Tidak ditekan = HIGH
  * Ditekan = LOW
* ISR harus ringan (tidak boleh pakai delay atau Serial)
* Delay masih digunakan di loop (cukup untuk simulasi sederhana)

## 🚀 Pengembangan Selanjutnya

* Mengganti `delay()` dengan **millis() (non-blocking)**
* Menambahkan buzzer untuk pedestrian
* Menambahkan countdown timer (7-segment / LCD)
* Integrasi dengan IoT (monitoring traffic)

## 📷 Ilustrasi Sistem

<img width="1919" height="916" alt="image" src="https://github.com/user-attachments/assets/fc119e83-f6ec-4d11-b7e8-dab8f257004a" />

