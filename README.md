# 🎶 Game Irama Rapa’i Berbasis Arduino + LCD I2C

## 📌 Deskripsi Proyek

Proyek ini adalah **game irama (rhythm game)** sederhana menggunakan **sensor suara** yang dirancang khusus untuk **alat musik tradisional rapa’i**.

Pemain harus memukul rapa’i mengikuti **pola DUM dan TAK** yang ditampilkan di LCD.
Setiap pukulan yang benar akan mengubah **titik (target)** menjadi **blok**, sedangkan pukulan salah **diabaikan** (tidak reset, tidak ada penalti visual).

Game ini menggunakan **logika anti double input** sehingga **1 pukulan = 1 input**, meskipun sensor masih mendeteksi suara tinggi beberapa milidetik.

---

## 🎯 Fitur Utama

* 🎵 Deteksi **DUM** dan **TAK** menggunakan threshold suara
* 🧠 Pola irama **acak** setiap level
* 🟡 Target tampil sebagai **titik**
* ⬛ Pukulan benar → titik berubah jadi **block**
* ❌ Pukulan salah → **diabaikan**
* 🛡️ **Anti double input** (cooldown + release detection)
* 📟 Monitoring nilai ADC secara realtime di LCD
* 💡 LED indikator saat input benar
* 🔁 Level otomatis berganti setelah selesai

---

## 🧩 Perangkat Keras (Hardware)

| Komponen                                    | Keterangan            |
| ------------------------------------------- | --------------------- |
| Arduino Uno / Nano                          | Board utama           |
| Sensor Suara (KY-037 / KY-038 / Mic Analog) | Input suara           |
| LCD 16x2 + I2C                              | Tampilan game         |
| LED                                         | Indikator input benar |
| Resistor                                    | Untuk LED             |
| Rapa’i                                      | Alat musik input      |

---

## 🔌 Konfigurasi Pin

| Komponen           | Pin Arduino |
| ------------------ | ----------- |
| Sensor suara (OUT) | A0          |
| LED Hijau          | D10         |
| LCD I2C SDA        | SDA         |
| LCD I2C SCL        | SCL         |

---

## ⚙️ Konfigurasi Threshold (PENTING)

```cpp
const int THRESHOLD_DUM = 50;
const int THRESHOLD_TAK = 80;
```

### Penjelasan:

* **DUM** → pukulan bass / lembut
* **TAK** → pukulan keras / tajam
* Threshold harus disesuaikan dengan **hasil kalibrasi sensor & rapa’i**

> ⚠️ Jika DUM sering terbaca sebagai TAK → naikkan `THRESHOLD_TAK`
> ⚠️ Jika pukulan tidak terbaca → turunkan `THRESHOLD_DUM`

---

## 🛡️ Sistem Anti Double Input

Sistem ini mencegah **1 pukulan terbaca 2 kali**.

### Digunakan 2 metode:

1. **Cooldown waktu**
2. **Release detection (tunggu suara turun)**

```cpp
bool sudahPukul = false;
unsigned long lastHit = 0;
const int hitCooldown = 180;
```

Input baru hanya diterima jika:

* Suara melewati threshold
* Belum dalam status `sudahPukul`
* Suara sudah turun di bawah ambang setelah pukulan sebelumnya

---

## 🧠 Logika Game

### 1️⃣ Tampilan Pola

* Baris atas → **TAK**
* Baris bawah → **DUM**
* Target ditampilkan sebagai **titik**

### 2️⃣ Input Pemain

| Kondisi            | Aksi                                     |
| ------------------ | ---------------------------------------- |
| Benar              | Titik → Block, lanjut ke step berikutnya |
| Salah              | Diabaikan                                |
| Selesai semua step | Menang & level baru                      |

---

## 📟 Tampilan LCD

| Area LCD          | Fungsi            |
| ----------------- | ----------------- |
| Kolom 0–7         | Pola irama        |
| Kolom 10          | Pemisah visual    |
| Baris atas kanan  | `V:` nilai ADC    |
| Baris bawah kanan | `S:` progres step |

---

## 🧪 Tips Kalibrasi

1. Jalankan mode monitor ADC
2. Catat:

   * Noise (diam)
   * DUM max
   * TAK max
3. Tentukan threshold:

```text
THRESHOLD_DUM ≈ 70–80% DUM max
THRESHOLD_TAK ≈ 60–70% TAK max
```

---

## 🏆 Kondisi Menang

* Semua pola berhasil diikuti
* LCD menampilkan pesan **"CAKEP!"**
* LED berkedip
* Pola baru di-generate otomatis

---

## 🚀 Pengembangan Lanjutan (Opsional)

* 🎚️ Peak detector (ambil puncak suara)
* 📈 Velocity (keras pukulan = skor)
* ⏱️ Timer & combo
* 🔴 LED merah untuk error
* 🥁 Mode multiplayer / score

---

## ✨ Penutup

Proyek ini menggabungkan:

* **Teknik embedded system**
* **Signal processing dasar**
* **Game logic**
* **Pelestarian musik tradisional**
