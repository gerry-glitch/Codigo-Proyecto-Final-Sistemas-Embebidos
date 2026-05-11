#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo miServo;

unsigned long tiempoReferencia = 0;
const unsigned long INTERVALO_DISPENSAR = 30000; // 30 segundos
bool dispensando = false;

void setup() {
  miServo.attach(9);
  miServo.write(0); 
  
  lcd.init();
  lcd.backlight();
  
  // Saludo inicial
  lcd.setCursor(0, 0);
  lcd.print("Buen dia");
  delay(3000);
  lcd.clear();
  
  tiempoReferencia = millis(); // Inicia el conteo después del saludo
}

void loop() {
  unsigned long tiempoActual = millis();
  unsigned long tiempoTranscurrido = tiempoActual - tiempoReferencia;

  if (!dispensando) {
    // Lógica mientras espera (Servo en 0°)
    if (tiempoTranscurrido >= INTERVALO_DISPENSAR) {
      dispensando = true;
      tiempoReferencia = millis(); // Reinicia para controlar los 5 seg de entrega
    } else {
      // Calcular cuántos segundos faltan
      int segundosRestantes = (INTERVALO_DISPENSAR - tiempoTranscurrido) / 1000;
      
      lcd.setCursor(0, 0);
      lcd.print("Proximas en:    "); // Espacios para limpiar residuos de texto
      lcd.setCursor(0, 1);
      lcd.print(segundosRestantes);
      lcd.print(" seg         ");
    }
  } 
  else {
    // Lógica de entrega (Servo en 180°)
    miServo.write(180);
    lcd.setCursor(0, 0);
    lcd.print("Recoja sus     ");
    lcd.setCursor(0, 1);
    lcd.print("pastillas      ");

    // Esperar los 5 segundos de entrega
    if (tiempoTranscurrido >= 5000) {
      miServo.write(0);
      dispensando = false;
      lcd.clear();
      tiempoReferencia = millis(); // Reinicia para los próximos 30 seg
    }
  }
}