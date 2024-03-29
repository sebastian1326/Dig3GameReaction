#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
// Display Cátodo Común
const int catodo[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

// Display Ánodo Común
const int anodo[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18};

// Definir pines para el decodificador 
#define PIN_A 26    //Menos significativo DCBA
#define PIN_B 21
#define PIN_C 27
#define PIN_D 19
#define PIN_E 20
#define PIN_F 28
#define PIN_G 22
#define PIN_dp 18
// Define otros pines para el display
#define PIN_DISPLAY_D1 15
#define PIN_DISPLAY_D2 14
#define PIN_DISPLAY_D3 17
#define PIN_DISPLAY_D4 16
// Define pines para leds
#define PIN_LED_A 12 
#define PIN_LED_V 13 
#define PIN_LED_R 11 
// Define pines para botones
#define PIN_BT_START 7 
#define PIN_BT_A 10 
#define PIN_BT_V 8 
#define PIN_BT_R 9 
// Tiempo de actualización del display en milisegundos
#define INTERVAL_US 1000000 // Intervalo de tiempo en microsegundos (1 segundo)

#define INTERVAL_USd 5000 // Por ejemplo, actualiza cada 3 milisegundos
volatile bool Band_dis=false;
volatile uint32_t timer_ticks = 0; // Contador de ticks del temporizador
#define DEBOUNCE_TIME_MS 50
#define LOCKOUT_TIME_MS 200

volatile uint32_t last_pressed_time = 0;
volatile bool button_locked = false;

uint8_t numD=0;
//volatile uint32_t timer_disp = 0; // Contador de ticks del temporizador
// Función de interrupción del temporizador
void timer_callback1(struct repeating_timer *t) {
    timer_ticks++; // Incrementar el contador de ticks
}
void timer_callback2(struct repeating_timer *t) {
    Band_dis=true;
}
/*
void button_pressed_isr(uint pin) {
    uint32_t current_time = time_us_32();

    if (!button_locked && current_time - last_pressed_time >= DEBOUNCE_TIME_MS * 1000) {
        // Aquí colocarías tu código para manejar la pulsación del botón
        switch (pin) {
            case PIN_BT_A:
                //handle_button_1();
                button_A=true;
                break;
            case PIN_BT_V:
                //handle_button_2();
                button_A=true;
                break;
            case PIN_BT_R:
                //ndle_button_3();
                button_R=true;
                break;
            default:
                break;
        }
        //printf("Botón %d presionado\n", pin);

        button_locked = true;
        last_pressed_time = current_time;
        sleep_ms(LOCKOUT_TIME_MS);
        button_locked = false;
    }
}
*/
/*
// Función para actualizar el display de manera multiplexada
void updateDisplay(uint8_t digit, bool *displayState, clock_t *lastUpdateTime) {
    // Obtener el tiempo actual
    clock_t currentTime = clock();

    // Verificar si ha pasado el tiempo de actualización
    if ((currentTime - *lastUpdateTime) >= (DISPLAY_UPDATE_INTERVAL * CLOCKS_PER_SEC / 1000)) {
        // Actualizar el display con el nuevo dígito
        showDigit(digit);

        // Cambiar el estado del display para la próxima multiplexación
        for (int i = 0; i < 4; i++) {
            if (i == digit) {
                displayState[i] = true; // Encender el dígito correspondiente
            } else {
                displayState[i] = false; // Apagar los demás dígitos
            }
        }

        // Actualizar el tiempo de última actualización
        *lastUpdateTime = currentTime;
    }
}*/
// Función para encender un display específico
void setDisplay(uint8_t display) {
    /*
    gpio_put(PIN_DISPLAY_D1, display & 0x01);
    gpio_put(PIN_DISPLAY_D2, display & 0x02);
    gpio_put(PIN_DISPLAY_D3, display & 0x03);
    gpio_put(PIN_DISPLAY_D4, display & 0x04);
    // Configurar otros pines del decodificador según sea necesario
    //gpio_put(PIN_DISPLAY_D1, 1); // Encender el primer display
    sleep_ms(1000); // Tiempo de visualización del número en el display actual
    //gpio_put(PIN_DISPLAY_D1, 0); // Apagar el primer display
    
    gpio_put(PIN_DISPLAY_D1, display & 0x0);
    gpio_put(PIN_DISPLAY_D2, display & 0x0);
    gpio_put(PIN_DISPLAY_D3, display & 0x0);
    gpio_put(PIN_DISPLAY_D4, display & 0x0);
    */
   switch (display) {
        case 1:
            gpio_put(PIN_DISPLAY_D1, 1);
            gpio_put(PIN_DISPLAY_D2, 0);
            gpio_put(PIN_DISPLAY_D3, 0);
            gpio_put(PIN_DISPLAY_D4, 0);
            gpio_put(PIN_dp, 1);
            break;
        case 2:
            gpio_put(PIN_DISPLAY_D1, 0);
            gpio_put(PIN_DISPLAY_D2, 1);
            gpio_put(PIN_DISPLAY_D3, 0);
            gpio_put(PIN_DISPLAY_D4, 0);
            gpio_put(PIN_dp, 0);
            break;
        case 3:
            gpio_put(PIN_DISPLAY_D1, 0);
            gpio_put(PIN_DISPLAY_D2, 0);
            gpio_put(PIN_DISPLAY_D3, 1);
            gpio_put(PIN_DISPLAY_D4, 0);
            gpio_put(PIN_dp, 0);
            break;
        case 4:
            gpio_put(PIN_DISPLAY_D1, 0);
            gpio_put(PIN_DISPLAY_D2, 0);
            gpio_put(PIN_DISPLAY_D3, 0);
            gpio_put(PIN_DISPLAY_D4, 1);
            gpio_put(PIN_dp, 0);
            break;
        // Continúa con los casos para los demás dígitos
        // Puedes definir las combinaciones de segmentos según tu display de 7 segmentos
        default:
            // Apaga todos los segmentos por defecto
            gpio_put(PIN_DISPLAY_D1, 0);
            gpio_put(PIN_DISPLAY_D2, 0);
            gpio_put(PIN_DISPLAY_D3, 0);
            gpio_put(PIN_DISPLAY_D4, 0);
            gpio_put(PIN_dp, 0);
            break;
    }
    // Obtener el tiempo actual
    //clock_t currentTime = clock();
    //if ((currentTime - *lastUpdateTime) >= (DISPLAY_UPDATE_INTERVAL * CLOCKS_PER_SEC / 1000)) {
    //sleep_ms(2); // Tiempo de visualización del número en el display actual
    if (Band_dis) {
            //gpio_put(LED_PIN, 1); // Encender el LED si el temporizador ha expirado
        Band_dis=false;
        numD++;
        if (numD>=4)
        {
            numD=0;/* code */
        }
        
        switch (display) {
            case 1:
                gpio_put(PIN_DISPLAY_D1, 0);
                gpio_put(PIN_dp, 0);
                gpio_put(PIN_DISPLAY_D2, 0);
                gpio_put(PIN_DISPLAY_D3, 0);
                gpio_put(PIN_DISPLAY_D4, 0);
                break;
            case 2:
                gpio_put(PIN_DISPLAY_D1, 0);
                gpio_put(PIN_DISPLAY_D2, 0);
                gpio_put(PIN_DISPLAY_D3, 0);
                gpio_put(PIN_DISPLAY_D4, 0);
                gpio_put(PIN_dp, 0);
                break;
            case 3:
                gpio_put(PIN_DISPLAY_D1, 0);
                gpio_put(PIN_DISPLAY_D2, 0);
                gpio_put(PIN_DISPLAY_D3, 0);
                gpio_put(PIN_DISPLAY_D4, 0);
                gpio_put(PIN_dp, 0);
                break;
            case 4:
                gpio_put(PIN_DISPLAY_D1, 0);
                gpio_put(PIN_DISPLAY_D2, 0);
                gpio_put(PIN_DISPLAY_D3, 0);
                gpio_put(PIN_DISPLAY_D4, 0);
                gpio_put(PIN_dp, 0);
                break;
            // Continúa con los casos para los demás dígitos
            // Puedes definir las combinaciones de segmentos según tu display de 7 segmentos
            default:
                // Apaga todos los segmentos por defecto
                gpio_put(PIN_DISPLAY_D1, 0);
                gpio_put(PIN_DISPLAY_D2, 0);
                gpio_put(PIN_DISPLAY_D3, 0);
                gpio_put(PIN_DISPLAY_D4, 0);
                //gpio_put(PIN_dp, 0);
                break;
        
        }
        //*lastUpdateTime = currentTime;
    }
}

// Función para mostrar un número en un display específico
void displayNumber(uint8_t number) {
    uint8_t display_number = number; // Obtener el número a mostrar en el display actual
    setDisplay(0); // Seleccionar el primer display

    // Configurar los pines de los segmentos según el número a mostrar en el display actual
    switch (display_number) {
        case 0:
            // Encender segmentos a, b, c, d, e, f; apagar segmento g
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 1);
            gpio_put(PIN_F, 1);
            gpio_put(PIN_G, 0);
            //gpio_put(PIN_dp, 1);
            break;
        case 1:
            // Apagar segmentos a, b; encender segmentos c, d, e, f, g
            gpio_put(PIN_A, 0);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 0);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 0);
            gpio_put(PIN_G, 0);
            break;
        case 2:
            // Encender segmentos a, b, d, e, g; apagar segmentos c, f
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 0);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 1);
            gpio_put(PIN_F, 0);
            gpio_put(PIN_G, 1);
            break;
        case 3:
            // Encender segmentos a, b, c, d, g; apagar segmentos e, f
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 0);
            gpio_put(PIN_G, 1);
            break;
        case 4:
            // Encender segmentos b, c, f, g; apagar segmentos a, d, e
            gpio_put(PIN_A, 0);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 0);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 1);
            gpio_put(PIN_G, 1);
            break;
        case 5:
            // Encender segmentos a, c, d, f, g; apagar segmentos b, e
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 0);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 1);
            gpio_put(PIN_G, 1);
            break;
        case 6:
            // Encender segmentos a, c, d, e, f, g; apagar segmento b
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 0);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 1);
            gpio_put(PIN_F, 1);
            gpio_put(PIN_G, 1);
            break;
        case 7:
            // Encender segmentos a, b, c; apagar segmentos d, e, f, g
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 0);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 0);
            gpio_put(PIN_G, 0);
            break;
        case 8:
            // Encender todos los segmentos
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 1);
            gpio_put(PIN_F, 1);
            gpio_put(PIN_G, 1);
            break;
        case 9:
            // Encender segmentos a, b, c, f, g; apagar segmentos d, e
            gpio_put(PIN_A, 1);
            gpio_put(PIN_B, 1);
            gpio_put(PIN_C, 1);
            gpio_put(PIN_D, 1);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 1);
            gpio_put(PIN_G, 1);
            break;
        default:
            // Dígito no válido, apagar todos los segmentos
            gpio_put(PIN_A, 0);
            gpio_put(PIN_B, 0);
            gpio_put(PIN_C, 0);
            gpio_put(PIN_D, 0);
            gpio_put(PIN_E, 0);
            gpio_put(PIN_F, 0);
            gpio_put(PIN_G, 1);
            break;
    }

    // Encender el display actual
    
}
void imp_val (uint16_t num){

    // Obtener dígito de las unidades (último dígito)
    uint8_t n1 = num % 10;

    // Obtener dígito de las decenas (segundo dígito de derecha a izquierda)
    uint8_t n2 = (num / 10) % 10;

    // Obtener dígito de las centenas (tercer dígito de derecha a izquierda)
    uint8_t n3 = (num / 100) % 10;

    // Obtener dígito de los millares (primer dígito de derecha a izquierda)
    uint8_t n4 = (num / 1000) % 10;
    switch (numD)
    {
    case 0:
        displayNumber(n4); // Mostrar el número actual en los displays
        setDisplay(1);
        break;
    case 1:
        displayNumber(n3); // Mostrar el número actual en los displays
        setDisplay(2);
        break;
    case 2:
        displayNumber(n2); // Mostrar el número actual en los displays
        setDisplay(3);
        break;
    case 3:
        displayNumber(n1); // Mostrar el número actual en los displays
        setDisplay(4);
        break;
    default:
        break;
    }
    
    
    
}
int genNumA(int maximo) {
    srand(time(NULL)); // Inicializar la semilla para la generación de números aleatorios

    // Generar un número aleatorio entre 0 y RAND_MAX (un valor grande)
    int numero_aleatorio = rand();

    // Ajustar el número aleatorio al rango deseado (1 a maximo)
    numero_aleatorio = (numero_aleatorio % maximo) + 1;

    return numero_aleatorio;
}
// Función para verificar si ha transcurrido un tiempo específico en segundos
bool verificarTiempoTranscurrido(int segundos,bool rst) {
    static time_t inicio = 0; // Variable estática para almacenar el tiempo de inicio
    if(rst){inicio=0;}
    if (inicio == 0) {
        inicio = time(NULL); // Obtener el tiempo de inicio al llamar por primera vez
    }

    time_t actual = time(NULL); // Obtener el tiempo actual
    time_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido

    return tiempo_transcurrido >= segundos; // Devolver verdadero si ha transcurrido el tiempo especificado
}
// Función para verificar si ha transcurrido un tiempo específico en segundos
bool verificarTiempoTranscurrido2(int segundos,bool rst) {
    static time_t inicio = 0; // Variable estática para almacenar el tiempo de inicio
    if(rst){inicio=0;}
    if (inicio == 0) {
        inicio = time(NULL); // Obtener el tiempo de inicio al llamar por primera vez
    }

    time_t actual = time(NULL); // Obtener el tiempo actual
    time_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido

    return tiempo_transcurrido >= segundos; // Devolver verdadero si ha transcurrido el tiempo especificado
}
int main() {
    stdio_init_all();

    // Configurar los pines del decodificador como salidas
    gpio_init(PIN_A);
    gpio_set_dir(PIN_A, GPIO_OUT);
    gpio_init(PIN_B);
    gpio_set_dir(PIN_B, GPIO_OUT);
    gpio_init(PIN_C);
    gpio_set_dir(PIN_C, GPIO_OUT);
    gpio_init(PIN_D);
    gpio_set_dir(PIN_D, GPIO_OUT);
    gpio_init(PIN_E);
    gpio_set_dir(PIN_E, GPIO_OUT);
    gpio_init(PIN_F);
    gpio_set_dir(PIN_F, GPIO_OUT);
    gpio_init(PIN_G);
    gpio_set_dir(PIN_G, GPIO_OUT);
    gpio_init(PIN_dp);
    gpio_set_dir(PIN_dp, GPIO_OUT);
    // Configurar otros pines del decodificador según sea necesario

    // Configurar los pines de los displays como salidas
    gpio_init(PIN_DISPLAY_D1);
    gpio_set_dir(PIN_DISPLAY_D1, GPIO_OUT);
    gpio_init(PIN_DISPLAY_D2);
    gpio_set_dir(PIN_DISPLAY_D2, GPIO_OUT);
    gpio_init(PIN_DISPLAY_D3);
    gpio_set_dir(PIN_DISPLAY_D3, GPIO_OUT);
    gpio_init(PIN_DISPLAY_D4);
    gpio_set_dir(PIN_DISPLAY_D4, GPIO_OUT);
    // Configurar los pines de los LEDS como salidas
    gpio_init(PIN_LED_A);
    gpio_set_dir(PIN_LED_A, GPIO_OUT);
    gpio_init(PIN_LED_V);
    gpio_set_dir(PIN_LED_V, GPIO_OUT);
    gpio_init(PIN_LED_R);
    gpio_set_dir(PIN_LED_R, GPIO_OUT);
    //Configuracion de los pines de botones como entradas
    gpio_init(PIN_BT_START);
    gpio_set_dir(PIN_BT_START, GPIO_IN);
    gpio_init(PIN_BT_A);
    gpio_set_dir(PIN_BT_A, GPIO_IN);
    //gpio_pull_up(PIN_BT_A);
    //gpio_set_irq_enabled_with_callback(PIN_BT_A, GPIO_IRQ_EDGE_FALL, true, &button_pressed_isr);
    gpio_init(PIN_BT_V);
    gpio_set_dir(PIN_BT_V, GPIO_IN);
    //gpio_pull_up(PIN_BT_V);
    //gpio_set_irq_enabled_with_callback(PIN_BT_V, GPIO_IRQ_EDGE_FALL, true, &button_pressed_isr);
    gpio_init(PIN_BT_R);
    gpio_set_dir(PIN_BT_R, GPIO_IN);
    //gpio_pull_up(PIN_BT_R);
    //gpio_set_irq_enabled_with_callback(PIN_BT_R, GPIO_IRQ_EDGE_FALL, true, &button_pressed_isr);
    //clock_t lastUpdateTime = clock(); // Tiempo de la última actualización
    // Configurar un temporizador para controlar la secuencia de LEDs
    struct repeating_timer timer1, timer2;
    add_repeating_timer_us(INTERVAL_US, timer_callback1, NULL, &timer1);
    //struct repeating_timer timer;
    add_repeating_timer_us(INTERVAL_USd, timer_callback2, NULL, &timer2);
    // Bucle infinito para mostrar números en los displays
    uint16_t number = 0;
    //uint8_t number1 = 1;
    //uint8_t dis = 1;
    bool band1=false;
    bool band2=false;
    bool band3=false;
    bool band4=false;
    uint8_t ledHigh=0;
    bool bandReset=false;
    bool button_state=false;
    bool band_sec=false;
    bool button_A = false;
    bool button_V = false;
    bool button_R = false;
    bool rst = false;
    bool rst2 = false;
    //bool btState[3] = {false}; // Estado de cada dígito del display
    //uint8_t contador = 0; // Dígito a mostrar
    //clock_t lastUpdateTime1 = clock(); // Tiempo de la última actualización
    while (1) {
        /*
        uint32_t ticks_mod = timer_ticks % 4; // Módulo 3 para repetir la secuencia
        
            if (ticks_mod == 0) {
                gpio_put(PIN_LED_A, 1);
                gpio_put(PIN_LED_V, 1);
                gpio_put(PIN_LED_R, 1);
                //break;
            } else if (ticks_mod == 1) {
                gpio_put(PIN_LED_A, 0);
                gpio_put(PIN_LED_V, 1);
                gpio_put(PIN_LED_R, 1);
                //break;
            } else if(ticks_mod == 2){
                gpio_put(PIN_LED_A, 0);
                gpio_put(PIN_LED_V, 0);
                gpio_put(PIN_LED_R, 1);
            } else {
                gpio_put(PIN_LED_A, 0);
                gpio_put(PIN_LED_V, 0);
                gpio_put(PIN_LED_R, 0);
                //band1=false;
                band2=true;
                //break;
            }
            if(band2){
                imp_val(number);
                tight_loop_contents(); // Permitir que el programa siga ejecutándose sin bloquear
        
            }*/
        /*
         // Actualizar el display de manera multiplexada
        updateDisplay(contador, displayState, &lastUpdateTime);

        // Ejecutar otras instrucciones en paralelo

        // Incrementar el contador para el siguiente dígito
        contador++;
        

        // Esperar un tiempo mínimo para permitir otras operaciones
        usleep(100); // Espera de 100 microsegundos
        
        // Leer el estado del botón
        */
        if (bandReset) {
            band1=false;
            band2=false;
            band3=false;
            band4=false;
            //bandReset=0;
            ledHigh=0;
            button_state=false;
            band_sec=false;
            imp_val(number);
            tight_loop_contents(); // Permitir que el programa siga ejecutándose sin bloquear
            //inicio = 0;
            rst,rst2=true;
            timer_ticks=0;
        }
        
        if(!band1){
            button_state = gpio_get(PIN_BT_START);
        }
        
        //updateDisplay(contador, displayState, &lastUpdateTime);
        // Verificar si el botón ha sido presionado (flanco ascendente)
        if (button_state || band1) {
            if(button_state){
                timer_ticks=0;
                button_state=false;
            }
            
            band1=true;
            bandReset=false;
            if(!band_sec){
                printf("Botón presionado\n");
            uint32_t ticks_mod = timer_ticks % 4; // Módulo 3 para repetir la secuencia
        
            if (ticks_mod == 0) {
                gpio_put(PIN_LED_A, 1);
                gpio_put(PIN_LED_V, 1);
                gpio_put(PIN_LED_R, 1);
                //break;
            } else if (ticks_mod == 1) {
                gpio_put(PIN_LED_A, 0);
                gpio_put(PIN_LED_V, 1);
                gpio_put(PIN_LED_R, 1);
                //break;
            } else if(ticks_mod == 2){
                gpio_put(PIN_LED_A, 0);
                gpio_put(PIN_LED_V, 0);
                gpio_put(PIN_LED_R, 1);
            } else {
                gpio_put(PIN_LED_A, 0);
                gpio_put(PIN_LED_V, 0);
                gpio_put(PIN_LED_R, 0);
                //band1=false;
                band2=true;
                band_sec=true;
                number =genNumA(10);
                ledHigh=genNumA(3);
                rst=true;
            }
            }
            
            //imp_val(number);
            
            
            if (band2){
                
                //static uint16_t numA=number;
                
                if(verificarTiempoTranscurrido(number,rst)){
                    //if(rst){rst=false;}
                    band2=false;
                    band3=true;
                    //break;
                }else{rst=false;}
                
                //sleep(genNumA(10));
            }
            if (band3){//Me prennde el led numero del led que se genero aleatoriamente
                band3=false;
                band4=true;
                number=number+(ledHigh*100);
                switch (ledHigh)
                {
                case 1:
                    gpio_put(PIN_LED_A, 1);
                    break;
                case 2:
                    gpio_put(PIN_LED_V, 1);
                    break;
                case 3:
                    gpio_put(PIN_LED_R, 1);
                    break;
                default:
                    gpio_put(PIN_LED_A, 1);
                    gpio_put(PIN_LED_V, 1);
                    gpio_put(PIN_LED_R, 1);
                    break;
                }
            }
            
            if(band4){
                button_A = gpio_get(PIN_BT_A);
                button_V = gpio_get(PIN_BT_V);
                button_R = gpio_get(PIN_BT_R);
                tight_loop_contents();
                static clock_t inicio = 0; // Variable estática para almacenar el tiempo de inicio

                if (inicio == 0) {
                    inicio = clock(); // Obtener el tiempo de inicio al llamar por primera vez
                }
                
                clock_t actual = clock(); // Obtener el tiempo actual
                clock_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido
                double tiempo_transcurrido_ms = ((double)tiempo_transcurrido * 1000) / CLOCKS_PER_SEC;
                tiempo_transcurrido=tiempo_transcurrido*10;
                number=(uint16_t)tiempo_transcurrido;
                if(button_A || button_V || button_R){
                    //time_t actual = time(NULL); // Obtener el tiempo actual
                    //time_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido
                    band4=false;
                    gpio_put(PIN_LED_A, 0);
                    gpio_put(PIN_LED_V, 0);
                    gpio_put(PIN_LED_R, 0);
                    switch (ledHigh){
                        case 1:
                            if(button_A){
                                //tiempo_transcurrido=tiempo_transcurrido*1000;
                                //number=(uint16_t)tiempo_transcurrido;
                                
                            }else{
                                //tiempo_transcurrido=(tiempo_transcurrido+1)*1000;
                                number=number+1000;
                            }

                            break;
                        case 2:
                            if(button_R){
                                //tiempo_transcurrido=tiempo_transcurrido*1000;
                                //number=(uint16_t)tiempo_transcurrido;
                                
                            }else{
                                //tiempo_transcurrido=(tiempo_transcurrido+1)*1000;
                                number=number+1000;
                            }
                            break;
                        case 3:
                            if(button_V){
                                //tiempo_transcurrido=tiempo_transcurrido*1000;
                                //number=(uint16_t)tiempo_transcurrido;
                                
                            }else{
                                //tiempo_transcurrido=(tiempo_transcurrido+1)*1000;
                                //number=(uint16_t)tiempo_transcurrido;
                                number=number+1000;
                            }
                            break;
                        default:
                            gpio_put(PIN_LED_A, 1);
                            gpio_put(PIN_LED_V, 1);
                            gpio_put(PIN_LED_R, 1);
                            break;
                    }
                    bandReset=true;
                    inicio=0;
                    
                }
                /*
                if(button_A & ledHigh==1){
                    time_t actual = time(NULL); // Obtener el tiempo actual
                    time_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido
                    tiempo_transcurrido=tiempo_transcurrido*100;
                    number=(uint16_t)tiempo_transcurrido;
                    bandReset=true;
                    inicio=0;
                }else if (button_V & ledHigh==2){
                    time_t actual = time(NULL); // Obtener el tiempo actual
                    time_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido
                    tiempo_transcurrido=tiempo_transcurrido*100;
                    number=(uint16_t)tiempo_transcurrido;
                    bandReset=true;
                    inicio=0;
                }else if (button_R & ledHigh==3){
                    time_t actual = time(NULL); // Obtener el tiempo actual
                    time_t tiempo_transcurrido = actual - inicio; // Calcular el tiempo transcurrido
                    tiempo_transcurrido=tiempo_transcurrido*100;
                    number=(uint16_t)tiempo_transcurrido;
                    bandReset=true;
                    inicio=0;
                }
                */
                
    
                if(verificarTiempoTranscurrido2(10,rst2)){
                    //if(rst2){rst2=false;}
                    bandReset=true;
                    inicio=0;
                    //rst,rst2=true;
                    //break;
                    gpio_put(PIN_LED_A, 0);
                    gpio_put(PIN_LED_V, 0);
                    gpio_put(PIN_LED_R, 0);
                }else{rst2=false;}
                
            }
            imp_val(number);
            tight_loop_contents(); // Permitir que el programa siga ejecutándose sin bloquear
            /*
            //imp_val(number);
            //tight_loop_contents(); // Permitir que el programa siga ejecutándose sin bloquear
        // Esperar un tiempo para evitar lecturas múltiples durante el rebote
        //sleep_ms(100); // Espera de 100 milisegundos
*/
        
        }else{
            // Esperar un tiempo para evitar el uso excesivo de la CPU
            sleep_ms(1); // Espera de 10 milisegundos
        }
        
        
        // Controlar la secuencia de LEDs basada en el contador de ticks
        

        //if (number1 > 4) {
        //    number1 = 1; // Volver a 0 después de mostrar todos los números del 0 al 9
        //}
        /*

        gpio_put(PIN_DECODER_A, 1);
        gpio_put(PIN_DECODER_B, 1);
        gpio_put(PIN_DECODER_C, 0);
        gpio_put(PIN_DECODER_D, 0);
        gpio_put(PIN_DISPLAY_D1, 1);
        gpio_put(PIN_DISPLAY_D2, 1);
        gpio_put(PIN_DISPLAY_D3, 1);
        gpio_put(PIN_DISPLAY_D4, 1);
        sleep_ms(1000); // Esperar 1 segundo antes de mostrar el siguiente número
        //number++; // Incrementar el número para el siguiente ciclo
        if (number > 9) {
            number = 0; // Volver a 0 después de mostrar todos los números del 0 al 9
        }*//*
        displayNumber(number);
        setDisplay(number1);
        //sleep_ms(1000); // Esperar 1 segundo antes de mostrar el siguiente número
        number++; // Incrementar el número para el siguiente ciclo
        
        if (number > 9) {
            number = 0; // Volver a 0 después de mostrar todos los números del 0 al 9
        }
        number1++; // Incrementar el número para el siguiente ciclo
        
        if (number1 > 4) {
            number1 = 1; // Volver a 0 después de mostrar todos los números del 0 al 9
        }*/
        
    }

    return 0;
}