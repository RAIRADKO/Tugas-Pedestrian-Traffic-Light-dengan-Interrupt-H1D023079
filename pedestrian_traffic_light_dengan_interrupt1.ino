// ── Pin Kendaraan ──────────────────────────────────────────
constexpr uint8_t PIN_KEN_MERAH  = 8;
constexpr uint8_t PIN_KEN_KUNING = 9;
constexpr uint8_t PIN_KEN_HIJAU  = 10;

// ── Pin Pedestrian ─────────────────────────────────────────
constexpr uint8_t PIN_PED1_MERAH = 4;
constexpr uint8_t PIN_PED1_HIJAU = 5;
constexpr uint8_t PIN_PED2_MERAH = 6;
constexpr uint8_t PIN_PED2_HIJAU = 7;

// ── Pin Tombol ─────────────────────────────────────────────
constexpr uint8_t PIN_TOMBOL = 2;

// ── Waktu (ms) ─────────────────────────────────────────────
constexpr unsigned long WAKTU_PENYEBERANGAN = 5000;
constexpr unsigned long WAKTU_KUNING_KEDIP  = 500;
constexpr uint8_t       JUMLAH_KEDIP_KUNING = 3;
constexpr unsigned long WAKTU_DEBOUNCE      = 50;

// ── State machine (HARUS sebelum semua fungsi) ─────────────
enum class State : uint8_t {
  AWAL,
  PENYEBERANGAN,
  TRANSISI
};

static State stateSekarang = State::AWAL;

// ── Forward declarations ───────────────────────────────────
void terapkanState(State s);
void matikanSemua();
void kedipKuning();

// ── Flag ISR ───────────────────────────────────────────────
volatile bool tombolDitekan = false;

// ── Helper: set LED ────────────────────────────────────────
inline void setLED(uint8_t pin, bool nyala) {
  digitalWrite(pin, nyala ? HIGH : LOW);
}

// ── Matikan semua LED ──────────────────────────────────────
void matikanSemua() {
  const uint8_t pins[] = {
    PIN_KEN_MERAH, PIN_KEN_KUNING, PIN_KEN_HIJAU,
    PIN_PED1_MERAH, PIN_PED1_HIJAU,
    PIN_PED2_MERAH, PIN_PED2_HIJAU
  };
  for (uint8_t p : pins) digitalWrite(p, LOW);
}

// ── Terapkan state ─────────────────────────────────────────
void terapkanState(State s) {
  matikanSemua();
  switch (s) {
    case State::AWAL:
      setLED(PIN_KEN_HIJAU,  true);
      setLED(PIN_PED1_MERAH, true);
      setLED(PIN_PED2_MERAH, true);
      Serial.println("State: AWAL - Kendaraan HIJAU, Pedestrian MERAH");
      break;

    case State::PENYEBERANGAN:
      setLED(PIN_KEN_MERAH,  true);
      setLED(PIN_PED1_HIJAU, true);
      setLED(PIN_PED2_HIJAU, true);
      Serial.println("State: PENYEBERANGAN - Kendaraan MERAH, Pedestrian HIJAU");
      break;

    case State::TRANSISI:
      setLED(PIN_PED1_MERAH, true);
      setLED(PIN_PED2_MERAH, true);
      Serial.println("State: TRANSISI - Kuning kedip");
      break;
  }
  stateSekarang = s;
}

// ── Kedipkan kuning ────────────────────────────────────────
void kedipKuning() {
  for (uint8_t i = 0; i < JUMLAH_KEDIP_KUNING; i++) {
    setLED(PIN_KEN_KUNING, true);
    delay(WAKTU_KUNING_KEDIP);
    setLED(PIN_KEN_KUNING, false);
    delay(WAKTU_KUNING_KEDIP);
  }
}

// ── ISR ────────────────────────────────────────────────────
void ISR_Tombol() {
  tombolDitekan = true;
}

// ── Setup ──────────────────────────────────────────────────
void setup() {
  const uint8_t outputPins[] = {
    PIN_KEN_MERAH, PIN_KEN_KUNING, PIN_KEN_HIJAU,
    PIN_PED1_MERAH, PIN_PED1_HIJAU,
    PIN_PED2_MERAH, PIN_PED2_HIJAU
  };
  for (uint8_t p : outputPins) pinMode(p, OUTPUT);

  pinMode(PIN_TOMBOL, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TOMBOL), ISR_Tombol, FALLING);

  Serial.begin(9600);
  Serial.println("=== Pedestrian Traffic Light ===");

  terapkanState(State::AWAL);
  Serial.println("Tekan tombol untuk aktifkan penyeberangan.");
}

// ── Loop utama ─────────────────────────────────────────────
void loop() {
  if (!tombolDitekan) return;

  tombolDitekan = false;

  delay(WAKTU_DEBOUNCE);
  if (digitalRead(PIN_TOMBOL) == HIGH) {
    Serial.println("[INFO] Debounce - sinyal diabaikan.");
    return;
  }

  Serial.println("\n[INTERRUPT] Tombol ditekan!");

  terapkanState(State::PENYEBERANGAN);
  delay(WAKTU_PENYEBERANGAN);

  terapkanState(State::TRANSISI);
  kedipKuning();

  terapkanState(State::AWAL);
  Serial.println("Siap menerima interrupt lagi.");
}