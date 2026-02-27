#include <stdio.h>
#include "telemetry.h"
#include <time.h>
#include <stdlib.h>

int main() {
    // Aracın temel değişkenleri
    float batarya_sicakligi = 0;
    int baslangic_sarji = 0;
    char kapi_durumu = 'K';
    int fren_pedali = 1;

    printf("--- ELEKTROMOBİL GÜVENLİK KONTROLÜ ---\n");

    // Kullanıcıdan input alma ve girilen değer kontrolü kısmı
    printf("Batarya Sıcaklığı (C): ");
    scanf("%f", &batarya_sicakligi);

    if (batarya_sicakligi < -100 || batarya_sicakligi > 200) {
        printf("SONUÇ: HATA: Mantıksız sıcaklık değeri! (-100 ile 200 arasonda olmalı)\n");
        return 1;
    }

    printf("Şarj Yüzdesi (%%): ");
    scanf("%d", &baslangic_sarji);

    if (baslangic_sarji < 0 || baslangic_sarji > 100) {
        printf("SONUÇ: HATA: Hatalı sensör verisi! Şarj %%0-%%100 arasında olmalı)\n");
        return 1;
    }

    printf("Kapı Durumu (A: Açık, K: Kapalı): ");
    scanf(" %c", &kapi_durumu); 

    if (kapi_durumu != 'A' && kapi_durumu != 'K') {
        printf("SONUÇ: HATA: Geçersiz karakter! (Sadece 'A' veya 'K' giriniz)\n");
        return 1;
    }

    printf("Fren Pedalı (1:Basılı, 0:Değil): ");
    scanf("%d", &fren_pedali);

    // Sürüşten önceki kontroller
    if (batarya_sicakligi > 60) {
        printf("SONUÇ: KRİTİK HATA: Motor Aşırı Isındı! Sürüş Engellendi.\n");
        return 1;
    } 
    if (baslangic_sarji < 10) {
        printf("SONUÇ: UYARI: Batarya Kritik Seviyede! Sürüş Başlatılamaz.\n");
        return 1;
    } 
    if (kapi_durumu == 'A') {
        printf("SONUÇ: HATA: Kapılar Açık! Lütfen Kapatınız.\n");
        return 1;
    } 
    if (fren_pedali == 0) {
        printf("SONUÇ: BİLGİ: Güvenlik için frene basarak tekrar deneyin.\n");
        return 1;
    } 
    printf("SONUÇ: BAŞARILI: SİSTEM HAZIR. MOTOR BAŞLATILIYOR...\n");
    
    int secim = 0;
    srand(time(NULL));

    sistemi_kur(batarya_sicakligi, baslangic_sarji);
    while (1) { // Seçim döngüsü kısmı
        printf("--- TUFAN ELEKTROMOBİL SÜRÜŞ SİMÜLASYONU ---\n");
        printf("1. Gaza Bas\n");
        printf("2. Frene Bas\n");
        printf("3. Rejeneratif Frenleme Yap\n");
        printf("4. Anlık Telemetri ve İstatistikleri Oku\n");
        printf("5. Sistemi Kapat\n");
        if (scanf("%d", &secim) != 1 || secim < 1 || secim > 5) {
                printf("Lütfen sadece 1-5 değerlerinden birini giriniz!\n");
                while (getchar() != '\n');
                continue;
        }
        switch (secim) {
                case 1: gaza_bas(); break;
                case 2: frene_bas(); break;
                case 3: rejen_fren(); break;
                case 4: telemetri_ve_istatistik_yazdir(); break;
                case 5: sistemi_kapat(); return 0;
        }
    }
    return 0;
}