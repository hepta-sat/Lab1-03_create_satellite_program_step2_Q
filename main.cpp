#include "mbed.h"
DigitalOut condition(LED1);
RawSerial gs(USBTX,USBRX,9600);
Timer sattime;
int rcmd = 0, cmdflag = 0; //command variable

//getting command and command flag
void commandget()
{
    rcmd = gs.getc();
    cmdflag = 1;
}
//interrupting process by command receive
void receive(int rcmd, int cmdflag)
{
    gs.attach(commandget,Serial::RxIrq);
}
//initializing
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    condition = 0;
}

int main()
{
    gs.printf("From Sat : Nominal Operation\r\n");
    int flag = 0; //condition flag
    float batvol, temp; //voltage, temperature 
    sattime.start();
    receive(rcmd,cmdflag); //interrupting
    for(int i=0;i<50;i++){
        //satellite condition led
        condition = !condition;
        
        //senssing HK data(dummy data)
        batvol = 3.7;
        temp   = 28.5;
        
        //Transmitting HK data to Ground Station(GS)
        gs.printf("HEPTASAT::Condition = %d, Time = %f [s], batvol = %2f [V], temp = %2f [deg C]\r\n",flag,sattime.read(),batvol,temp);
        wait_ms(1000);
        
        if(cmdflag == 1){
            if(rcmd == 'a'){
                for(int j=0;j<5;j++){
                    gs.printf("Hello World!\r\n");
                    condition = 1;
                    wait_ms(1000);
                }
            }
            initialize(); //initializing
        }
    }
    sattime.stop();
    gs.printf("From Sat : End of operation\r\n");
}