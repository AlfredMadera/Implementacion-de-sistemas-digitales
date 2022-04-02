
/*Autor: Jesús Alfredo
 *  Semaforo de modo automatico y precaución*/

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

int modo = 1;
int i;

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    //Configurar los GPIOs
    //Pines P1.1 y P1.4 como entradas pullup
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);

    //Pines 3.2, 3.3, 4.1, 4.3, 4.6, 6.5, 6.4, 6.1, 4.0, 4.2, 4.4, 4.5, 1.0 como salidas
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2 + GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1 + GPIO_PIN3 + GPIO_PIN6 + GPIO_PIN0 + GPIO_PIN2 + GPIO_PIN4 + GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN5 + GPIO_PIN4 + GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    //Inicializar pines de salida
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2 + GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1 + GPIO_PIN3 + GPIO_PIN6 + GPIO_PIN0 + GPIO_PIN2 + GPIO_PIN4 + GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN5 + GPIO_PIN4 + GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    //Configurar interrupciones  de entradas
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    //Habilitar interrupción del PORT1
    Interrupt_enableInterrupt(INT_PORT1);

    //Habilitar la interrupción master
    Interrupt_enableMaster();

    while(1)
    {
        if (modo == 1){
        int j;

        //Encender rojos
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN4); //Enciende rojo
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN2); //Enciende rojo
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5); //Enciende rojo
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1); //Enciende rojo

//------ Semaforo1 --------------------------------------------------------------------------------------------------------
        GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN5);  //Apaga amarillo
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN4); //Enciende rojo
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);  //Apaga rojo
        //Transición a verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0); //Enciende verde
        for (j=0;j<20;j++){
            delate1();
        }
        //Parpadeo verde
        for (j=0;j<5;j++){
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);  //Apaga verde
            delate1();
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0); //Enciende verde
            delate1();
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);  //Apaga verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1); //Enciende amarillo
        delate();

        if (modo == 1){
//------ Semaforo2 --------------------------------------------------------------------------------------------------------
        GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);  //Apaga amarillo
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN2); //Enciende rojo
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);  //Apaga rojo
        //Transición a verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4); //Enciende verde
        for (j=0;j<20;j++){
            delate1();
        }
        //Parpadeo verde
        for (j=0;j<5;j++){
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);  //Apaga verde
            delate1();
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4); //Enciende verde
            delate1();
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);  //Apaga verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3); //Enciende amarillo
        delate();
        }
        if (modo == 1){
//------ Semaforo3 --------------------------------------------------------------------------------------------------------
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3);  //Apaga amarillo
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5); //Enciende rojo
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);  //Apaga rojo
        //Transición a verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN3); //Enciende verde
        for (j=0;j<20;j++){
            delate1();
        }
        //Parpadeo verde
        for (j=0;j<5;j++){
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);  //Apaga verde
            delate1();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN3); //Enciende verde
            delate1();
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);  //Apaga verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2); //Enciende amarillo
        delate();

        }
        if (modo == 1){

//------ Semaforo4 --------------------------------------------------------------------------------------------------------
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);  //Apaga amarillo
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1); //Enciende rojo
        GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN4);  //Apaga rojo
        //Transición a verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6); //Enciende verde
        for (j=0;j<20;j++){
            delate1();
        }
        //Parpadeo verde
        for (j=0;j<5;j++){
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);  //Apaga verde
            delate1();
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6); //Enciende verde
            delate1();
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);  //Apaga verde
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN5); //Enciende amarillo
        delate();
        }

        }else{

//--------------------------------------------------------------------------------------------------------------------------------------
        //Modo precaución
        //Apagar luces
        if (i > 0){
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2 + GPIO_PIN3);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1 + GPIO_PIN3 + GPIO_PIN6 + GPIO_PIN0 + GPIO_PIN2 + GPIO_PIN4 + GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN5 + GPIO_PIN4 + GPIO_PIN1);
            i = 0;
        }

        //Precaución

            GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1); //Enciende amarillo
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3); //Enciende amarillo
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2); //Enciende amarillo
            GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN5); //Enciende amarillo
            delate1();
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1); //Apaga amarillo
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3); //Apaga amarillo
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2); //Apaga amarillo
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN5); //Apaga amarillo
            delate1();


//--------------------------------------------------------------------------------------------------------------------------------------
}
}
}

/*Servicio de interupción*/
void PORT1_IRQHandler (void){
    uint32_t status;
    i = 0;
    int j = 0;
    int cont = 0;
    //Retardo antirrebote
    for (i=0;i<50000;i++){

    }
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);

    //Función de la interrupción
    //Comprobar secuencia de presionado de botones
    if (!(P1IN & BIT1)){
        for (i=0;i<700000;i++){
                if (!(P1IN & BIT1)){
                    cont++;                             //Aumenta contador para checar que mantiene el botón 1
                    if (cont > 500000){                 //Si lo mantiene más de 2s checa el botón 4
                        cont = 0;
                        //Encender led
                        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                        delate();
                        if (!(P1IN & BIT4) && !(P1IN & BIT1)){
                            for (j=0;j<700000;j++){
                                if (!(P1IN & BIT4) && !(P1IN & BIT1)){
                                    cont++;             //Aumenta contador para checar que mantiene ambos botones
                                }else{
                                    cont = 0;
                                    j = 700000;         //Si suelta el boton, la secuencia se cancela
                                    i = 700000;
                                }
                            }
                            if (cont > 500000){
                                cont = 0;
                                //Apagar led
                                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                                delate();
                                if ((P1IN & BIT4) && !(P1IN & BIT1)){
                                    //Encender led
                                    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                                    delate();
                                    if ((P1IN & BIT4) && (P1IN & BIT1)){
                                        //Apagar led. Secuencia completa
                                        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                                        if(modo == 1){
                                            modo = 0;
                                        }else{
                                            modo = 1;
                                        }
                                    }
                                }

                            }
                        }
                    }
                }else{
                    cont = 0;
                    i = 700000;
                }
        }
    }
    //Apagar led
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);


    /*
    */

}

void delate (void){
    //Función para hacer retardos entre luces
    int i;
    for (i=0;i<1000000;i++){

    }
}

void delate1 (void){
    //Función para hacer retardos de parpadeo
    int i;
    for (i=0;i<50000;i++){

    }
}
