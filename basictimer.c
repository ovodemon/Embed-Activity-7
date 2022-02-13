/**********************************************************************/
// ENGR-2350 Activity-Baic_timer
// Name: Yijia Zhou
// RIN: 661995479
// This is the base project for several activities and labs throughout
// the course.  The outline provided below isn't necessarily *required*
// by a C program; however, this format is required within ENGR-2350
// to ease debugging/grading by the staff.
/**********************************************************************/

// We'll always add this include statement. This basically takes the
// code contained within the "engr_2350_msp432.h" file and adds it here.
#include "engr2350_msp432.h"

// Add function prototypes here, as needed.
void Timer_Init();
void GPIO_Init();

void UpdateTime();
// Add global variables here, as needed.

uint32_t array[4] = {0,0,0,0};
//uint32_t array[4] = {0,30,59,10};
Timer_A_UpModeConfig Timer;

int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Place initialization code (or run-once) code here
    Timer_Init();
    GPIO_Init();
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);

    while(1){
        // Place code that runs continuously in here

        if (Timer_A_getInterruptStatus(TIMER_A0_BASE)){
            Timer_A_clearInterruptFlag(TIMER_A0_BASE);
            UpdateTime();
            GPIO_toggleOutputOnPin(GPIO_PORT_P5,GPIO_PIN7);
        }
    }
}

// Add function declarations here as needed
void GPIO_Init(){
    GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN7); //Set P5.7 to output
}

void Timer_Init(){

//    Timer_A_configureUpMode(Timer_A1, Timer_A_UpModeConfig & Timer_A0_BASE);
    Timer.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    Timer.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    Timer.timerPeriod = 37500;
    Timer_A_configureUpMode(TIMER_A0_BASE, &Timer);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

void UpdateTime(){
    array[0]++;  // Increment tenth of seconds
    if(array[0] == 10){  // If a whole second has passed...
        array[0] = 0;    // Reset tenth of seconds
        array[1]++;      // And increment seconds
        if(array[1] == 60){ // If a minute has passed...
            array[1] = 0;   // Reset seconds
            array[2]++;     // Increment minutes
            if(array[2] == 60){  // and so on...
                array[2] = 0;
                array[3]++;
                if(array[3] == 24){
                    array[3] = 0;
                }
            }
        }
    }
    printf("%2u:%02u:%02u.%u\r",array[3],array[2],array[1],array[0]);
}
// Add interrupt functions last so they are easy to find
