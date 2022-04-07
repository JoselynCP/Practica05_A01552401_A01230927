
//////////////////////////////// LIBRARIEs | HEADERs //////////////////////////
#include "device_config.h"
#include <math.h>
///////////////////////////////// DIRECTIVEs //////////////////////////////////
#define _XTAL_FREQ 1000000
#define DELAY_KNIGHT  300 // Tiempo entre el encendido de cada LED en el Knight Ryder
#define DELAY_PRESSED 500 // Tiempo para ver si es pulsado el botón
#define DELAY_NOPRESEED 250 //Tiempo para ver si ya no es pulsado el botón

//////////////////////////////// DATA TYPEs ///////////////////////////////////
enum conf{ digital = 0b00000000, input = 0b11111111, output = 0b00000000}; 
enum cycle{min  = 0, max = 7};
enum exponent{ bbase = 2, limit = 8 };

////////////////////////////////  ISR  ////////////////////////////////////////
void __interrupt ( high_priority ) high_isr( void ); // Para alta prioridad
void __interrupt ( low_priority ) low_isr( void ); // Para baja prioridad

////////////////////////////// FUNCTIONs DECLARATIONS //////////////////////////
void portsInit( void ); //Función prototipo de inicilización de los puertos
void KnightRider(void); //Función prototipo del barrido de LEDs para cuando se acierte el presionar el botón correspondiente al LED

///////////////////////////////////// MAIN ////////////////////////////////////
void main( void ){
    portsInit();
    char posrand, push = 0; // bariables tipo char para las posición random que se obtenga y el valor del pulsador 
    while(1){  int posrand = rand() % limit ; // 
        switch(posrand) { // Para signarle a cada posición random un bit de los 8, uno para cada LED
            case 0: posrand = 0b00000001; 
                break;
            case 1: posrand = 0b00000010;
                break;
            case 2: posrand = 0b00000100;
                break;
            case 3: posrand = 0b00001000;
                break;
            case 4: posrand = 0b00010000;
                break;
            case 5: posrand = 0b00100000; 
                break;
            case 6: posrand = 0b01000000;
                break;
            default: posrand =0b10000000;
                break;
        }
        LATB = posrand; // Se asocia a la salida de los LEDs con la posición random obtenida de rand() % limit  en 8 bits
        __delay_ms(DELAY_PRESSED); // Delay para verificar si se pulsó el botón
        push = ~PORTA; // Complemento a uno de los puertos A correspondientes a las entradas de los pulsadores
    if (posrand == push) // Si el número de LED es igual al pulsador activado 
        {
             KnightRider(); // Manda a llamar a la función de barrido 
            __delay_ms(DELAY_NOPRESEED);
        }
    }
}
/////////////////////////////////// FUNCTIONS//////////////////////////////////
void KnightRider(){ // Función para hacer el recorrido de LEDs
    for (int i = min; i <= max; i++) // del bit 0 al bit 7 , dirección 1
           {
                LATB =  pow(bbase, i); // 
                __delay_ms(DELAY_KNIGHT); // Tiempo de encendido de cada LED
            }
            for (int i = max; i >= min; i--) // del bit 7 al bit 0, dirección de regreso
            {
                LATB =  pow(bbase, i);
                __delay_ms(DELAY_KNIGHT);
            } 
}
void portsInit( void ){
    ANSELA = digital;                       // A como digital
    TRISA  = input;                          // Entrada de los botones pulsadores
    ANSELB = digital;                       // B como digital
    TRISB  = output;                          // Salida de los LEDs
   }

