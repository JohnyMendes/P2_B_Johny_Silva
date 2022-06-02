// Johny Silva Mendes
// PRÁCTICA 2: INTERRUPCIONES
// Objetivo: realizar dos tipos de interrupciones.

// Práctica B: Interrupción por temporizador

  // Se declara las variables interruptCounter & totalInterruptCounter
  // interruptCounter: maneja el contador internamente por si ocurre algo que no debería
  // totalInterruptCounter: cuenta el num total de interrupciones des del inicio del programa.
  
#include <Arduino.h>
volatile int interruptCounter;

int totalInterruptCounter;

hw_timer_t * timer = NULL; // temporizador de hardware utilizado para las interrupciones.

// sincroniza el loop principal y el ISR en el manejo de variables compartidas.
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// IRAM_ATTR: Contabiliza el número de interrupciones
// Entra y sale del modo CRITICAl para evitar comportamientos erroneos

void IRAM_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

// SETUP: ajusta el reloj de 80Mhz de la placa ESP32 al num de tics deseados
// Frequency(80Mhz)/escalado(2ndo parametro de la funcion timerBegin).
// Temporizador: se inicia y se configura a 1000000 de tics por segundo.
// timerAttachInterrupt: para detectar y ejecutar el ISR a cada salto de alarma.
// timerAlarmWrite: especifica en que punto del contador la interrupcion debe ser generada.
// a 1000000 de tics, saltará la alarma y sucederá la interrupción.
// true llama a reiniciar el contador timer una vez esto haya ocurrido.
// se habilita el contador.

void setup(){

Serial.begin(9600);
timer = timerBegin(0, 80, true);
timerAttachInterrupt(timer, &onTimer, true);
timerAlarmWrite(timer, 1000000 , true);
timerAlarmEnable(timer);
}

// LOOP: manipula la interrupción. 
// Si el num de interrupciones > 0 entonces el cont global se reduce.
// interruptCounter es compartida con el ISR y el loop, por tanto, se ejecuta esta accion como CRITICAL.
// por último se incrementa el num de interrupciones desde el inicio del prog y se muestra por pantalla
void loop() {

if (interruptCounter > 0) {
portENTER_CRITICAL(&timerMux);
interruptCounter--;
portEXIT_CRITICAL(&timerMux);
totalInterruptCounter++;
Serial.print("An interrupt as occurred. Total number: ");
Serial.println(totalInterruptCounter);
}

}