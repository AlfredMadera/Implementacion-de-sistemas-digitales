/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
// Inicio de variables
   int cont=0x01;
   int cont2=0x01;
   int cont3=0x01;
    int i=0x00;
    bool botonbajo=true;
    bool botonbajo2=true;
    bool botonbajo3=true;
    bool aux=true;
    bool aux3=true;
    bool aux4=true;
   int aux2=0x00;
/**
 * main.c
 * Genere una señal PWM simetric ad e 20kHZ
 * Fuente de reloj SMCLK
 * Señal de salida P10.4 (TA3.0)
 * DCOCLK = 3MHz -> SMCLK -> Timer Clock
 * CCR = 75
 *
 */
int estado=0;
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
 //Configuracion de ssalida p10.4
    P10->DIR |= BIT4|BIT5;
    //Reset de pines 10.4 y 10.6
    P10->OUT &= ~(BIT4|BIT5);
    //Seleccion de funcion periferica primaria
    P10 -> SEL0 |= BIT4|BIT5;

    //Confuguracion entradas (Botones de placa y externo)
GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN5);

// Habilitamos acceso a esctritura al registro CS

    CS -> KEY = CS_KEY_VAL;
// Configura el divisor de señal SMLCK/8
    CS -> CTL1 |= CS_CTL1_DIVA_3;

    // Deshabilitamos acceso a esctritura al registro CS
    CS -> KEY = 0;
    //Timer A3 de modo ascendente, fuente de reloj SMCLK ) 3MHz
    //Limpieza del TAR
TIMER_A3 ->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR ;
    //Cargar base de tiempo en CCR0
TIMER_A3->CCR[0] = 60000;
//Modo toggle registro captura comparación
TIMER_A3 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_4;
TIMER_A3 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_7;
//Timer A0 de modo ascendente, fuente de reloj SMCLK ) 3MHz
TIMER_A0 ->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR ;
    //Cargar base de tiempo en CCR0
TIMER_A0->CCR[0] = 60000;
//Modo toggle registro captura comparación
TIMER_A0 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_4;
TIMER_A0 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_7;
//Timer A1 de modo ascendente, fuente de reloj SMCLK ) 3MHz
TIMER_A1 ->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR ;
    //Cargar base de tiempo en CCR0
TIMER_A1->CCR[0] = 60000;
//Modo toggle registro captura comparación
TIMER_A1 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_4;
TIMER_A1 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_7;

// Inicio de código principal

for(;;){

    if(aux==false){ //Modo automático de movimiento
        TIMER_A3->CCR[1] = 3000;
        for(i=0;i<50000;i++){
                            }
        TIMER_A3->CCR[1] = 4500;
        for(i=0;i<50000;i++){
                              }
          TIMER_A3->CCR[1] = 6000;
    }

    if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_LOW)){
        botonbajo=true;
    }
    else if((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)&&(cont==1)&&(botonbajo==true)){ // Estado A de servomotor 1
    cont++;
    botonbajo=false;
aux=false;
}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)&&(cont==2)&&(botonbajo==true)){  // Estado B de servomotor 1
    TIMER_A3->CCR[1] = 300;
    cont++;
    aux=true;
    botonbajo=false;

}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)&&(cont==3)&&(botonbajo==true)){ // Estado C de servomotor 1
    TIMER_A3->CCR[1] = 4500;
    cont++;
    aux=true;

    botonbajo=false;
}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)&&(cont==4)&&(botonbajo==true)){ // Estado D de servomotor 1
    TIMER_A3->CCR[1] = 12000;
    cont=1;
    aux=true;

    botonbajo=false;

}

 // de aqui
    if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_LOW)){
        botonbajo2=true;
    }
    else if((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)&&(cont2==1)&&(botonbajo2==true)){ // Estado A de servomotor 2
    cont2++;
    botonbajo2=false;
aux3=false;
}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)&&(cont2==2)&&(botonbajo2==true)){ // Estado B de servomotor 2
    TIMER_A1->CCR[1] = 300;
    cont2++;
    aux3=true;
    botonbajo2=false;

}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)&&(cont2==3)&&(botonbajo2==true)){ // Estado C de servomotor 2
    TIMER_A1->CCR[1] = 4500;
    cont2++;
    aux3=true;

    botonbajo2=false;
}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)&&(cont2==4)&&(botonbajo2==true)){ // Estado D de servomotor 2
    TIMER_A1->CCR[1] = 12000;
    cont2=1;
    aux3=true;
    botonbajo2=false;
}

if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5)==GPIO_INPUT_PIN_LOW)){
        botonbajo3=true;
    }
    else if((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5)==GPIO_INPUT_PIN_HIGH)&&(cont3==1)&&(botonbajo3==true)){ // Estado A de servomotor 3
    cont3++;
    botonbajo3=false;
aux4=false;
}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5)==GPIO_INPUT_PIN_HIGH)&&(cont3==2)&&(botonbajo3==true)){ // Estado B de servomotor 3
    TIMER_A0->CCR[1] = 300;
    cont3++;
    aux4=true;
    botonbajo3=false;

}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5)==GPIO_INPUT_PIN_HIGH)&&(cont3==3)&&(botonbajo3==true)){ // Estado C de servomotor 3
    TIMER_A0->CCR[1] = 4500;
    cont3++;
    aux4=true;
    botonbajo3=false;
}
else if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5)==GPIO_INPUT_PIN_HIGH)&&(cont3==4)&&(botonbajo3==true)){ // Estado D de servomotor 3
    TIMER_A0->CCR[1] = 12000;
    cont3=1;
    aux4=true;
    botonbajo3=false;
}


}
}
