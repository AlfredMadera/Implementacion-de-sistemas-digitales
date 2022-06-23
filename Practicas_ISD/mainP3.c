#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include <port_lib_by_nuts.h>


typedef struct display_7s { //definiendo una estructura para el control de display de 7s
    uint8_t puerto[7];
    uint8_t pin[7];
}display_7s;

typedef struct buttons { //definiendo una estructura para el control botones o señales de entrada
    uint8_t puerto;
    uint8_t pin;
}buttons;

struct display_7s unidades;
struct display_7s decenas;

struct buttons signal_input;

//Varibles para el conteo de tiempo

volatile unsigned long milesimas=0;

volatile uint8_t state = 0; // 0 = start count time , 1= end count time, 2:waitsignal
                       //a,b,c,d,e,f,g;
bool num_to_7s[12][7]= {{1,1,1,1,1,1,0},//0
                        {0,1,1,0,0,0,0},//1
                        {1,1,0,1,1,0,1},//2
                        {1,1,1,1,0,0,1},//3
                        {0,1,1,0,0,1,1},//4
                        {1,0,1,1,0,1,1},//5
                        {1,0,1,1,1,1,1},//6
                        {1,1,1,0,0,0,0},//7
                        {1,1,1,1,1,1,1},//8
                        {1,1,1,1,0,1,1}//9
                        {1,0,0,1,1,1,1}//E
                        {0,0,0,0,1,0,1}};//r


void config_ports(void){

    //-------------------Declaracion de Salidas GPIO------------
    unidades.puerto[0] = 1; // <---- Definir pines;
    unidades.pin[0] = 1;
    unidades.puerto[1] = 1;
    unidades.pin[1] = 1;
    unidades.puerto[2] = 1;
    unidades.pin[2] = 1;
    unidades.puerto[3] = 1;
    unidades.pin[3] = 1;
    unidades.puerto[4] = 1;
    unidades.pin[4] = 1;
    unidades.puerto[5] = 1;
    unidades.pin[5] = 1;
    unidades.puerto[6] = 1;
    unidades.pin[6] = 1;

    decenas.puerto[0] = 1;
    decenas.pin[0] = 1;
    decenas.puerto[1] = 1;
    decenas.pin[1] = 1;
    decenas.puerto[2] = 1;
    decenas.pin[2] = 1;
    decenas.puerto[3] = 1;
    decenas.pin[3] = 1;
    decenas.puerto[4] = 1;
    decenas.pin[4] = 1;
    decenas.puerto[5] = 1;
    decenas.pin[5] = 1;
    decenas.puerto[6] = 1;
    decenas.pin[6] = 1;

    uint8_t i;
    for(i=0;i<7;i++){ //inicializando como pines de salida
        ini_pin_out(unidades.puerto[i], unidades.pin[i]);
        ini_pin_out(decenas.puerto[i], decenas.pin[i]);
    }

    //-------------------Declaracion de Entradas GPIO------------

    signal_input.puerto = 4;
    signal_input.pin=1;

    GPIO_setAsInputPinWithPullDownResistor(signal_input.puerto, signal_input.pin); //Puerto de entrada

    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(signal_input.puerto, signal_input.pin); //Habilitando interrupcion ambos flancos

}

void numbers_to_7sDisp(uint8_t n,bool logica){ //logica, (true)1= catodo comun,(false)0=anodocomun
    uint8_t d = n/10;
    uint8_t u = n-(d*10);

    uint8_t i;
    for(i=0;i<7;i++){
        if(logica){
            digitalWrite(unidades.puerto[i], unidades.pin[i], num_to_7s[u][i]);
            digitalWrite(decenas.puerto[i], decenas.pin[i], num_to_7s[d][i]);
        }
        else{
            digitalWrite(unidades.puerto[i], unidades.pin[i], !num_to_7s[u][i]);
            digitalWrite(decenas.puerto[i], decenas.pin[i], !num_to_7s[d][i]);
        }
    }
}
void dis_error(bool logica){
    uint8_t i;
    for(i=0;i<7;i++){
        if(logica){
            digitalWrite(unidades.puerto[i], unidades.pin[i], num_to_7s[12][i]);
            digitalWrite(decenas.puerto[i], decenas.pin[i], num_to_7s[11][i]);
        }
        else{
            digitalWrite(unidades.puerto[i], unidades.pin[i], !num_to_7s[12][i]);
            digitalWrite(decenas.puerto[i], decenas.pin[i], !num_to_7s[11][i]);
        }
    }
}

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    config_ports();

    Interrupt_enableMaster(); //habilitando todas las interrups

    while(1)
    {
        switch(state){
            case 0:{
                unsigned int i;
                for(i=0;i<(3000000/1000);i++);
                milesimas++; //contando tiempo
                break;
            }
            case 1:{
                float periodo = (milesimas*0.001);
                float frecuencia = 1/periodo;

                if(frecuencia>=1 && frecuencia<100){
                    numbers_to_7sDisp((uint8_t)frecuencia, true);//true para display de catodo comun
                }
                else{
                    dis_error(true);//true para display de catodo comun
                }

                milesimas=0;

                break;
            }
        }

    }
}

void PORT4_IRQHandler (void){
    if(digitalRead(signal_input.puerto, signal_input.pin)){ // flaco de subida
        GPIO_clearInterruptFlag(signal_input.puerto, signal_input.pin);
        if(state==0){
            state=1;
        }
        else{
            state=0;
        }
    }
    /*
}
