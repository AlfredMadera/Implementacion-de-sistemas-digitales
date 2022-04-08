//Encender y apagar el LED RGB en modo manual y automatico CON INTERRUPCIONES

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

int contador = 0x00;
int modo = 0b0;
int i, j;

int main(void)
{

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Configuración de GPIOs */
    LED_Init();                                                         //Configurar el RGB como salida
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);      // P1.4 -> Entrada con Pull-up
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);      // P1.1 -> Entrada con Pull-up

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                       //Led rojo como salida, para referencia
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //Inicializar el led rojo apagado
    P2->OUT = contador;                                                 //Inicializar RGB apagado

    while(1)
    {


        //Limpiar banderas de interrupción
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
        //Habilitar la interrupción de cada pin de entrada
        GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
        //Habilitar la interrupción del puerto de entrda
        Interrupt_enableInterrupt(INT_PORT1);
        //Habilitar la interrupción general
        Interrupt_enableMaster();

        //Evaluación del modo actual
        if(modo == 0b0){
            ModoAutomatico();
        }
    }//end loop

}

void PORT1_IRQHandler (void){
    uint32_t status;
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);

    if(modo == 0b1){
        ModoManual();
    }

                //Ciclo para detectar el tiempo de presión - BOTÓN 1
                for(j = 0;j < 350000;j++){                                    //Retardo para 2 segundos
                    //Revisa el mantenimiento del botón
                    if (!(P1IN & BIT1)){                                      //Si se mantiene presionado deja que i aumente
                        if(j > 349000){                                       //Compara si el boton 1 ya se presionó casi 2 segundos
                            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //Enciende led rojo como confirmación
                            for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmación
                            }
                            //Ciclo para detectar el tiempo de presión - BOTÓN 2
                            for(i = 0;i < 350000;i++){
                                //Revisa el mantenimiento del botón
                                if(!(P1IN & BIT4)){
                                    if(i > 349000){
                                        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //Apaga led rojo como confirmación
                                        for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmación
                                        }
                                        if(P1IN & BIT4){
                                            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //Enciende led rojo como confirmación
                                            for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmación
                                            }
                                            if(P1IN & BIT1){
                                                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //Apaga led rojo como confirmación
                                                for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmación
                                                }
    //-----------------------------------------------------------------------------------------------------------------
                                                modo ^= 0b1;                                      //CAMBIO DE MODO
                                                i = 350000;                                       //Se sale del ciclo
                                                j = 350000;                                       //Se sale del ciclo
    //-----------------------------------------------------------------------------------------------------------------
                                            }else{
                                                i = 350000;                                       //Se sale del ciclo
                                                j = 350000;                                       //Se sale del ciclo
                                            }//end liberar botón 1
                                        }else{
                                            i = 350000;                                       //Se sale del ciclo
                                            j = 350000;                                       //Se sale del ciclo
                                        }//end liberar botón 2
                                    }//end confirmación de botón 2
                                }else{
                                    i = 350000;                                       //Se sale del ciclo
                                    j = 350000;                                       //Se sale del ciclo
                                }//end presión botón 2
                            }//end retardo botón 2
                        }//end confirmación botón 1
                    }else{
                        j = 350000;                                           //Se sale del ciclo
                    }//end else
                }//end for 2 secs

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);    //Apaga el led de referencia

}

void LED_Init(void){
    P2->SEL0 &= ~0X07;       // Configuración GPIOs 2.0, 2.1, 2.2
    P2->SEL1 &= ~0X07;
    P2->DIR |= 0X07;        // 2.0, 2.1, 2.2 como salida
}

void ModoManual(void){
    if(!(P1IN & BIT4)){
        // Por defecto la velocidad de trabajo es de 3 MHz
        // Retardo antirrebote de 30 milisegundos 30*3000000/1000
        for(i = 0;i<90000;i++){
        }

        // Acción para P1.4 = 0
        contador = contador + 1;
        P2->OUT = contador;
        if (contador >= 8){
             contador = 0;
         }
    }
}

void ModoAutomatico(void){
    for(i = 0;i<175000;i++){         //Retardo de 1 segundo obtenido empiricamente
       }
   contador = contador + 1;
   P2->OUT = contador;
   if (contador >= 8){
        contador = 0;
    }

}
