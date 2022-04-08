//Encender y apagar el LED RGB en modo manual y automatico sin interrupciones

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

    /* Configuraci�n de GPIOs */
    LED_Init();                                                         //Configurar el RGB como salida
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);      // P1.4 -> Entrada con Pull-up
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);      // P1.1 -> Entrada con Pull-up

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                       //Led rojo como salida, para referencia
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //Inicializar el led rojo apagado
    P2->OUT = contador;                                                 //Inicializar RGB apagado

    while(1)
    {
//------ Inicio de secuencia de cambio de modo---------------------------------------------------------------------------------------
        if (!(P1IN & BIT1)){
            //Ciclo para detectar el tiempo de presi�n - BOT�N 1
            for(j = 0;j < 350000;j++){                                    //Retardo para 2 segundos
                //Revisa el mantenimiento del bot�n
                if (!(P1IN & BIT1)){                                      //Si se mantiene presionado deja que i aumente
                    if(j > 349000){                                       //Compara si el boton 1 ya se presion� casi 2 segundos
                        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //Enciende led rojo como confirmaci�n
                        for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmaci�n
                        }
                        //Ciclo para detectar el tiempo de presi�n - BOT�N 2
                        for(i = 0;i < 350000;i++){
                            //Revisa el mantenimiento del bot�n
                            if(!(P1IN & BIT4)){
                                if(i > 349000){
                                    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //Apaga led rojo como confirmaci�n
                                    for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmaci�n
                                    }
                                    if(P1IN & BIT4){
                                        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //Enciende led rojo como confirmaci�n
                                        for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmaci�n
                                        }
                                        if(P1IN & BIT1){
                                            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //Apaga led rojo como confirmaci�n
                                            for(i = 0;i<75000;i++){                           //Retardo para visualizar el led de confirmaci�n
                                            }
//-----------------------------------------------------------------------------------------------------------------
                                            modo ^= 0b1;                                      //CAMBIO DE MODO
                                            i = 350000;                                       //Se sale del ciclo
                                            j = 350000;                                       //Se sale del ciclo
//-----------------------------------------------------------------------------------------------------------------
                                        }else{
                                            i = 350000;                                       //Se sale del ciclo
                                            j = 350000;                                       //Se sale del ciclo
                                        }//end liberar bot�n 1
                                    }else{
                                        i = 350000;                                       //Se sale del ciclo
                                        j = 350000;                                       //Se sale del ciclo
                                    }//end liberar bot�n 2
                                }//end confirmaci�n de bot�n 2
                            }else{
                                i = 350000;                                       //Se sale del ciclo
                                j = 350000;                                       //Se sale del ciclo
                            }//end presi�n bot�n 2
                        }//end retardo bot�n 2
                    }//end confirmaci�n bot�n 1
                }else{
                    j = 350000;                                           //Se sale del ciclo
                }//end else
            }//end for 2 secs
        }//--------------------- end if (inicio de secuencia) -----------------------------------------------------------------------

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);    //Apaga el led de referencia

        //Evaluaci�n del modo actual
        if(modo == 0b1){
            ModoManual();
        }else{
           ModoAutomatico();
        }//end if modo

    }//end loop

}


void LED_Init(void){
    P2->SEL0 &= ~0X07;       // Configuraci�n GPIOs 2.0, 2.1, 2.2
    P2->SEL1 &= ~0X07;
    P2->DIR |= 0X07;        // 2.0, 2.1, 2.2 como salida
}

void ModoManual(void){
    if(!(P1IN & BIT4)){
        // Por defecto la velocidad de trabajo es de 3 MHz
        // Retardo antirrebote de 30 milisegundos 30*3000000/1000
        for(i = 0;i<90000;i++){
        }

        // Acci�n para P1.4 = 0
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
