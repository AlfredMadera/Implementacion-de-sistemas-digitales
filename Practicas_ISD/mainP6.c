/*
 * Pr�ctica 6
 * -Adquirir se�al anal�gica.
 * -Aplicar filtro EMA, SMA y FIR.
 * -Filtro FIR debe ser dise�ado como: filtro pasa bajas, Fc=1200kHz, m�todo de ventanas, ventana rectangular.
 * -Enviar todas las se�ales obtenidas a la salida de los filtros por UART, tambi�n la original.
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Declaraci�n de variables
static volatile uint16_t ValorADC;
static volatile float VoltajeNormalizado, y, alpha, y_n, suma, y2, y3, pi, hn;
static volatile char Signals[33], Original[6], EMA[9], SMA[9], FIR[9];
static volatile int n, M;
static volatile double fc, wc;

const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                      // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    // Inicializaci�n de variables
    ValorADC = 0;
    VoltajeNormalizado = 0;
    y = 0;
    y_n = 0;
    alpha = 0.1;
    n = 1;
    y2 = 0;
    suma = 0;
    M = 21;
    fc = 1200;
    wc = fc/200000;
    pi = 3.1416;

    // Configuraci�n de GPIOs
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,                // GPIO P5.5 como entrada anal�gica
                                                   GPIO_TERTIARY_MODULE_FUNCTION);     // A0

    /* P1.2 and P1.3 en modo UART  */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

//-- ADC -------------------------------------------------------------------------
    /* Configura Flash estado espera */
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);

    /* Configuracion DCO 12 MHZ*/
    PCM_setPowerState(PCM_AM_LDO_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Habilitamos FPU*/
    FPU_enableModule();
    FPU_enableLazyStacking();

    // Configuraci�n del ADC
    ADC14_enableModule();                                                                   // Se habilita el modulo
    ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_DIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE);      // Se inicializa el modulo con reloj maestro,

    // Configuraci�n de memoria del ADC
    ADC14_configureSingleSampleMode(ADC_MEM0, true);                                // La conversi�n se almacena en la memoria MEM0
                                                                                    // repitiendo la conversi�n
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,         // Referencia de memoria default
                                        ADC_INPUT_A0, ADC_NONDIFFERENTIAL_INPUTS);  // Canal A0 del P5.5. Sin diferencial entre canales

    // Configuraci�n de TIMER
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    // Se habilita la conversi�n ADC
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();

//-- UART -----------------------------------------------------------------------------------------
    /* Configuraci�n UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Habilitamos UART */
    UART_enableModule(EUSCI_A0_BASE);

//-- Interrupciones -------------------------------------------------------------------------------
    ADC14_enableInterrupt(ADC_INT0);            // Se habilita la interrupci�n del canal 0 del ADC
    Interrupt_enableInterrupt(INT_ADC14);      // Se habilita la interrupcion por ADC

    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);

    Interrupt_enableMaster();                  // Se habilitan las interrupciones en general

//--------------------------------------------------------------------------------------------
    while(1)
    {
        PCM_gotoLPM0();
    }
}
//--------------------------------------------------------------------------------------------

    /* Servicio de interrupci�n ADC */
void ADC14_IRQHandler(void){
    uint16_t status = ADC14_getEnabledInterruptStatus();    // Estatus de la interrupci�n de activaci�n
    ADC14_clearInterruptFlag(ADC_INT0);                     // Se limpia la bandera de interrupci�n
    if (ADC_INT0 & status){
        // Obtener la se�al de voltaje
        ValorADC = ADC14_getResult(ADC_MEM0);
        VoltajeNormalizado = (ValorADC*3.3)/16383;          // Se normaliza el voltaje a 3.3V con resoluci�n de 14 bits

        // Aplicar filtro EMA
        y = (alpha * (VoltajeNormalizado)) + ((1 - alpha)*y_n); // Expresi�n del filtro EMA
        y_n = y;

        // Aplicar filtro SMA
        suma = VoltajeNormalizado + suma;
        y2 = suma/n;                                // Expresi�n del filtro SMA
        n++;

        // Aplicar filtro FIR
        hn = (wc/pi) * (sin(wc * (n - (M - 1)/2)) / wc*(n - (M - 1)/2));
        y3 = hn * VoltajeNormalizado;                                       // Filtro FIR aplicado

        // Enviar se�ales por UART
        sprintf(Original, "%.3f,", VoltajeNormalizado);                 // Se guardan las se�ales como strings
        sprintf(EMA, "%.3f,", y);
        sprintf(SMA, "%.3f,", y2);
        sprintf(FIR, "%.3f\r\n", y3);

        char aux1 = strcat(Original,EMA);                       // Se concatenan las se�ales
        char aux2 = strcat(aux1,SMA);
        char aux3 = strcat(aux2,FIR);
        UART0_OutString(aux3);                                  // Se envian por UART
    }
}

/*Funcion para envio de string */
void UART0_OutString(char *pt){
    while(*pt){
        UART0_OutChar(*pt);
        pt++;
    }
}
void UART0_OutChar(char letra){
    UART_transmitData(EUSCI_A0_BASE, letra);
}






