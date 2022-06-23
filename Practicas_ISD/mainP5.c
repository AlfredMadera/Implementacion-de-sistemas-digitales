#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <port_lib_by_nuts.h>

//------Variables para conteo de tiempo--------
volatile unsigned long milecimas = 0;
volatile unsigned long current_mills = 0;

volatile float periodo = 0;
volatile float frecuencia =0;

const unsigned int f_max = 1000;

const float resolution = 1/f_max;

const eUSCI_UART_ConfigV1 uartConfig =
{
       EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
       78,                                     // BRDIV = 78
       2,                                       // UCxBRF = 2
       0,                                       // UCxBRS = 0
       EUSCI_A_UART_NO_PARITY,                  // No Parity
       EUSCI_A_UART_LSB_FIRST,                  // LSB First
       EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
       EUSCI_A_UART_MODE,                       // UART mode
       EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
       EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};


typedef struct buttons { //definiendo una estructura para el control botones o señales de entrada
    uint8_t puerto;
    uint8_t pin;
}buttons;

struct buttons signal_input;

void config_ports(void){

    //-------------------Declaracion de Entradas GPIO------------

    signal_input.puerto = 4;
    signal_input.pin=1;

    GPIO_setAsInputPinWithPullDownResistor(signal_input.puerto, signal_input.pin); //Puerto de entrada

    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(signal_input.puerto, signal_input.pin); //Habilitando interrupcion ambos flancos

}

void config_UART_A0(void){
    /* P1.2 and P1.3 en modo UART  */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION );

    /* DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);  //configurando reloj en 12MHz

    /* Configuración UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);  //uart configurado a 9600 bauls

    /* Habilitamos UART */
    UART_enableModule(EUSCI_A0_BASE);

    /* Interrupciones */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT); //Configurando interrupcio RX
    Interrupt_enableInterrupt(INT_EUSCIA0);
}

void config_timer(void){

    TIMER_A0 -> CCR[0] = 12000000/f_max; //ValorTOP_para_Frecuencia_interrucion_timer = clock/ frec_deseada.

    //Modo
    TIMER_A0 ->CCTL[0] = TIMER_A_CCTLN_OUTMOD_0 + TIMER_A_CCTLN_CCIE;

    TIMER_A0 -> CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR | TIMER_A_CTL_IE;

    Interrupt_enableInterrupt(INT_TA0_0);  //habilitando la interrucion en compraracion de CCR[0]
}

unsigned long milis(void){
    return milecimas;
}

//---varibles de estado
volatile uint8_t state = 0; //0= wait signal, 1=show results

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    config_ports();
    config_UART_A0();
    config_timer();

    Interrupt_enableMaster();

    while(1)
    {
        switch(state){
            case 2:{//mostrar datos;
                char mensaje[50];
                uint8_t i;
                sprintf(mensaje, "Periodo = %0.4f, Frecuencia = %0.4f\n", periodo, frecuencia); //se construye el mensaje del tiempo trascurrido desde inicio
                for(i=0;i<strlen(mensaje); i++){
                    UART_transmitData(EUSCI_A0_BASE, mensaje[i]);  //se imprime caracter por caracte para TX UART
                }
                state = 0;

                break;
            }
            default:{
                break;
            }
        }

    }
}

void PORT4_IRQHandler (void){
    if(digitalRead(signal_input.puerto, signal_input.pin)){ // flaco de subida
        GPIO_clearInterruptFlag(signal_input.puerto, signal_input.pin);
        if(state==0){
            current_mills= milis();
            state=1;
        }
        else if(state==1){
            periodo = (milis() - current_mills)*resolution;
            frecuencia = 1/periodo;
            state=2;
        }
    }

}

void TA0_0_IRQHandler(void){
    //Se entra a esta interrucion cada milisegudo (definido por la frecuencia de 1000 en la formula)
    TIMER_A0->CCTL[0] &= ~0x0001; //limpia bandera de interruccion

    milecimas++;    //se incrementa el tiempo en milesimas

}
