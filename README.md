# Quadtree Compression 

## 1. Penjelasan Singkat Program
Quadtree Compressor dengan menggunakan 5 metode yaitu:
- Variance.
- Mean Absolute Different.
- Max Pixel Different.
- Entropy.
- SSIM [BONUS].

## 2. Dependencies
Dependencies yang dibutuhkan adalah
- Freeimage melalui MinGW.
- Makefile (Windows/Linux).

### Instalasi Freeimage
#### Windows [MinGW]
- Run MINGW64.exe dengan Administrator .
- Synchronise Database dengan Command  ``pacman -Syu``.
- Install Freeimage dengan Command ``pacman -S mingw64/mingw-w64-x86_64-freeimage``.
- Synchronise Database lagi dengan Command ``pacman -Syu``.
- Install Makefile dengan Command ``pacman -S make``.

#### Linux [Apt]
- Jalankan Command ``sudo apt upgrade``.
- Jalankan Command ``sudo apt install libfreeimage-dev``.
- Jalankan Command ``sudo apt install build-essential``.

## 3. Compiling Program
### Windows/Linux
- Cukup jalankan makefile dengan Command ``make build``.
- Makefile akan menentukan tipe OS sendirinya.

## 4. How to Use Program
- Run binary dengan Command ``.\bin\<linuxver/winver>.exe``.
- Program akan meminta Absolute Path Source Image.
- Setelah Menerima Path, Program akan Mengecek/Error Handling.
- Program akan Meminta Threshold dan Min Block Size dengan Masing2 Memiliki Error Handling Sendiri.
- Setelah Melakukan Compression Program akan Meminta Absolute Path Output Image.
- Jika ingin Melakukan Generate GIF, Program akan Meminta Absolute Path Output GIF.

## 5. Author
- Muhammad Adha Ridwan - 13523098
- Naufarrel Zhafif Abhista - 13523149
