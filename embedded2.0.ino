#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- KONFIGURASI LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- PIN ---
const int soundPin = A0;
const int ledGreen = 10;

// --- THRESHOLD ---
const int THRESHOLD_DUM = 50;
const int THRESHOLD_TAK = 80;

// --- KARAKTER ---
byte kotak[8] = {
  B11111, B11111, B11111, B11111,
  B11111, B11111, B11111, B11111
};
byte titik[8] = {
  B00000, B00000, B00000, B00100,
  B00100, B00000, B00000, B00000
};

// --- GAME ---
const int MAX_STEPS = 8;
int targetIrama[MAX_STEPS];
int stepSekarang = 0;

// --- ANTI DOUBLE INPUT ---
bool sudahPukul = false;
unsigned long lastHit = 0;
const int hitCooldown = 180;

// --- LCD MONITOR ---
unsigned long lastUpdateLCD = 0;
const int intervalUpdate = 150;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A1));

  pinMode(ledGreen, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, kotak);
  lcd.createChar(1, titik);

  acakPola();
  tampilkanUI();
}

void loop() {
  int nilaiSuara = analogRead(soundPin);

  // --- MONITOR LCD ---
  if (millis() - lastUpdateLCD > intervalUpdate) {
    lcd.setCursor(11, 0);
    lcd.print("V:");
    lcd.setCursor(13, 0);
    if (nilaiSuara < 100) lcd.print(" ");
    lcd.print(nilaiSuara);

    lcd.setCursor(11, 1);
    lcd.print("S:");
    lcd.print(stepSekarang);
    lcd.print("/");
    lcd.print(MAX_STEPS);

    lastUpdateLCD = millis();
  }

  // --- DETEKSI ---
  int deteksi = 0;
  if (nilaiSuara >= THRESHOLD_TAK) deteksi = 2;
  else if (nilaiSuara >= THRESHOLD_DUM) deteksi = 1;

  // --- LOGIKA ANTI 2x INPUT ---
  if (deteksi > 0 && !sudahPukul && millis() - lastHit > hitCooldown) {
    sudahPukul = true;
    lastHit = millis();

    if (deteksi == targetIrama[stepSekarang]) {
      digitalWrite(ledGreen, HIGH);

      int baris = (deteksi == 2) ? 0 : 1;
      lcd.setCursor(stepSekarang, baris);
      lcd.write(0);

      delay(120);
      digitalWrite(ledGreen, LOW);

      stepSekarang++;
    }
    // SALAH → DIABAIKAN
  }

  // --- RELEASE DETECTION ---
  // Tunggu sampai suara benar-benar turun
  if (nilaiSuara < THRESHOLD_DUM - 10) {
    sudahPukul = false;
  }

  // --- MENANG ---
  if (stepSekarang >= MAX_STEPS) {
    victorySequence();
  }
}

// --- FUNGSI ---

void acakPola() {
  for (int i = 0; i < MAX_STEPS; i++) {
    targetIrama[i] = random(1, 3);
  }
}

void tampilkanUI() {
  lcd.clear();
  for (int i = 0; i < MAX_STEPS; i++) {
    if (targetIrama[i] == 2) {
      lcd.setCursor(i, 0); lcd.write(1);
    } else {
      lcd.setCursor(i, 1); lcd.write(1);
    }
  }
  lcd.setCursor(10, 0); lcd.print("|");
  lcd.setCursor(10, 1); lcd.print("|");
}

void victorySequence() {
  lcd.clear();
  lcd.setCursor(4, 0); lcd.print("CAKEP!");
  lcd.setCursor(1, 1); lcd.print("Next Level");

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledGreen, HIGH); delay(100);
    digitalWrite(ledGreen, LOW); delay(100);
  }

  delay(1000);
  stepSekarang = 0;
  acakPola();
  tampilkanUI();
}
