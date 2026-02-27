#include <stdio.h>
#include <stdlib.h>
#include "telemetry.h"

// Aracın temel değişkenleri
static float batarya_sicakligi = 0.0f;
static float anlik_hiz = 0.0f;
static int sarj_yuzdesi = 0;
static float motor_sicakligi = 0.0f;

// Aracın değişkenlerinin değişim kayıtları için arrayler
static float hizlanma_kayitlari[MAX_KAYIT];
static float yavaslama_kayitlari[MAX_KAYIT];
static float rejen_kayitlari[MAX_KAYIT];

// Yapılan işlem sayısı ve array için index değişkenleri
static int hizlanma_kaydi = 0;
static int yavaslama_kaydi = 0;
static int rejen_kaydi = 0;

void sicaklik_kontrol() { // Sıcaklık kontolleri için fonk
    if (motor_sicakligi > 90.0f || batarya_sicakligi > 70.0f) {
        printf("KRİTİK HATA: Aşırı Isınma!\n");
        exit(1); 
    }

    if (batarya_sicakligi < 20.0f) {
        batarya_sicakligi = 20.0f;
    }
    if (motor_sicakligi < 20.0f) {
        motor_sicakligi = 20.0f;
    }
}
void gaza_bas() { // Aracın hızını arttıran fonk, şarjı azaltır, sıcaklığı arttırır
    if (sarj_yuzdesi <= 1) {
        printf("Batarya Tükendi!\n");
        sarj_yuzdesi = 0;
        return;
    }

    float eklenecek_hiz = (rand() % 81 + 20) / 10.0f;
    anlik_hiz += eklenecek_hiz;
    sarj_yuzdesi -= 2;
    motor_sicakligi += 5;
    batarya_sicakligi += 2;

    if (anlik_hiz > 70) {
        anlik_hiz = 70.f;
    }
    if (hizlanma_kaydi < MAX_KAYIT) {
        hizlanma_kayitlari[hizlanma_kaydi] = eklenecek_hiz;
        hizlanma_kaydi++;
    }

    sicaklik_kontrol();
    
    printf("[BİLGİ] Gaza basıldı. Araç %.1f km/s hızlandı. Motor: %.1f°C, Batarya: %.1f°C\n", 
    eklenecek_hiz, motor_sicakligi, batarya_sicakligi);
}
void frene_bas() { // Aracın hızını azaltan fonk, sıcaklığı düşürür
    if (anlik_hiz <= 0.0f) {
        printf("Araç zaten duruyor!\n");
        return;
    }

    float azaltilcak_hiz = (rand() % 101 + 50) / 10.0f;
    anlik_hiz -= azaltilcak_hiz;
    motor_sicakligi -= 3;
    batarya_sicakligi -= 1;

    if (anlik_hiz < 0) {
        anlik_hiz = 0.0f;
    }

    if (yavaslama_kaydi < MAX_KAYIT) {
        yavaslama_kayitlari[yavaslama_kaydi] = azaltilcak_hiz;
        yavaslama_kaydi++;
    }

    sicaklik_kontrol();

    printf("[BİLGİ] Frene basıldı. Araç %.1f km/s yavaşladı. Güncel Hız: %.1f km/s\n",
        azaltilcak_hiz, anlik_hiz);
}
void rejen_fren() { // Aracın hızını azaltıp şarjı arttıran fonk, sıcaklıkları değiştirir
    if (anlik_hiz <= 0.0f) {
        printf("Araç dururken fren yapılamaz!\n");
        return;
    }

    float azaltilacak_hiz = (rand() % 81 + 20) / 10.0f;
    anlik_hiz -= azaltilacak_hiz;
    sarj_yuzdesi++;
    motor_sicakligi -= 2;
    batarya_sicakligi++;

    if (sarj_yuzdesi>100) {
        sarj_yuzdesi = 100;
    }

    if (anlik_hiz < 0.0) {
        anlik_hiz = 0;
    }

    if (rejen_kaydi < MAX_KAYIT) {
        rejen_kayitlari[rejen_kaydi] = azaltilacak_hiz;
        rejen_kaydi++;
    }
    sicaklik_kontrol();

    printf("[BİLGİ] Rejeneratif fren devrede. Araç %.1f km/s yavaşladı. Batarya şarj oluyor.\n",
    azaltilacak_hiz);
}
float ortalama_hesapla(float arr[], int boyut) { // Telemetrideki ortalama hesapları için oluşturulmuş fonk
    if (boyut != 0) {
        float sum = 0.0f;
    
        for (int i = 0; i < boyut; i++) {
            sum += arr[i];
        }
        
        return sum / boyut;
    }
    else {
        return 0.0f;
    }
}
void telemetri_ve_istatistik_yazdir() { // Telemetri ve kayıtların ortalamasını yazdıran fonk
    printf("\n--- ANLIK TELEMETRİ ---\n");
    printf("Anlık Hız: %.1f km/s\n", anlik_hiz);
    printf("Batarya: %%%d\n", sarj_yuzdesi);
    printf("Motor Sıcaklığı: %.1f °C\n", motor_sicakligi);
    printf("Batarya Sıcaklığı: %.1f °C\n", batarya_sicakligi);
    printf("-----------------------\n");
    printf("--- SÜRÜŞ İSTATİSTİKLERİ ---\n");
    printf("Gaza Basma Sayısı: %d (Ortalama Artış: %.2f km/s)\n", hizlanma_kaydi, ortalama_hesapla(hizlanma_kayitlari, hizlanma_kaydi));
    printf("Frene Basma Sayısı: %d (Ortalama Düşüş: %.2f km/s)\n",yavaslama_kaydi, ortalama_hesapla(yavaslama_kayitlari, yavaslama_kaydi));
    printf("Rejeneratif Frenleme Sayısı: %d (Ortalama Düşüş: %.2f km/s)\n",rejen_kaydi, ortalama_hesapla(rejen_kayitlari, rejen_kaydi));
    printf("-----------------------\n");
}
void sistemi_kapat() { // Son bir istatistik yazdırıp programı kapatan fonk
    printf("[BİLGİ] Sistem kapatılıyor... Son Sürüş İstatistikleri:\n");
    printf("--- SÜRÜŞ İSTATİSTİKLERİ ---\n");
    printf("Gaza Basma Sayısı: %d (Ortalama Artış: %.2f km/s)\n", hizlanma_kaydi, ortalama_hesapla(hizlanma_kayitlari, hizlanma_kaydi));
    printf("Frene Basma Sayısı: %d (Ortalama Düşüş: %.2f km/s)\n",yavaslama_kaydi, ortalama_hesapla(yavaslama_kayitlari, yavaslama_kaydi));
    printf("Rejeneratif Frenleme Sayısı: %d (Ortalama Düşüş: %.2f km/s)\n",rejen_kaydi, ortalama_hesapla(rejen_kayitlari, rejen_kaydi));
    printf("-----------------------\n");
    printf("[BİLGİ] Motor güvenli bir şekilde kapatıldı. İyi günler!\n");
    exit(0);
}
void sistemi_kur(float baslangic_sicakligi, int baslangic_sarji) { // Değişkenlerin ilk değerlerini atayan fonk
    batarya_sicakligi = motor_sicakligi = baslangic_sicakligi;
    anlik_hiz = 0.0f;
    sarj_yuzdesi = baslangic_sarji; 
}