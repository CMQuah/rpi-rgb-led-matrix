#include <string>
#include <cstring>

#include <fstream>
#include <iostream>
#include <fcntl.h>  

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

using namespace std;  


class gpioFs {      
  public:     
    int pinNumber;        // **GPIO pin number not number of pins(int variable)
    string direction;  //direction out
    int value;
    gpioFs(int pinNumber)
    {
        this->pinNumber = pinNumber;
        this->direction = "out";
        this->value = 0;         
    }

    void gpioValueSet(int value){
        if (this->direction == "out"){
            string dirName = "/sys/class/gpio/gpio";
            string s_pinNumber = to_string(this->pinNumber);
            dirName = dirName + s_pinNumber +"/value";
            fstream gpioValuefh;
            gpioValuefh.open(dirName, ios::out);
            if (! gpioValuefh) {
                perror("Unable to set");
                exit(1);
            } else{

                this->value = value;
                gpioValuefh << value;
            }
        gpioValuefh.close();
        } else{
            //TODO handle this???
        }

    }
    int gpioValueGetter(){
        string dirName = "/sys/class/gpio/gpio";
        string s_pinNumber = to_string(this->pinNumber);
        dirName = dirName + s_pinNumber +"/value";
        fstream gpioValuefh;
	    gpioValuefh.open(dirName, ios::in);
	    if (!gpioValuefh) {
		    cout << "No such file";
	    }
	    else {
            int value;
            gpioValuefh >> value;
            
	    }
	    gpioValuefh.close();
        this->value = value;
        return this->value;
    }

    void debug(){
        //TODO; debug code for pins that not used 26 28 ?????

        cout<< pinNumber <<" " << direction <<" "<< value <<" "<< endl;
    }
    ~gpioFs(){
        int fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /sys/class/gpio/unexport");
            return;
        }
        char port_str[10];
        sprintf(port_str, "%d", this->pinNumber);
        if (write(fd, port_str, strlen(port_str)) != strlen(port_str)) {
            perror("Error writing to /sys/class/gpio/unexport");
            return;
            close(fd);
        }
        
    }
};



void GPIODirectionSetter(int pinNumber,string direction){
    //TODO: check pin based on hardware mapping ?
    string dirName = "/sys/class/gpio/gpio"; 
    string s_pinNumber = to_string(pinNumber);
    fstream fd;
    dirName = dirName + s_pinNumber +"/direction";
    fd.open(dirName, ios::out);
    if (! fd) {
        perror("Unable to set");
        exit(1);
    }
    fd << direction;
    fd.close();
};

void initGPIOFsOutPin(){
    //based on hardwareMappings
    //int gpioPin [19] = { 3, 4, 5, 7, 8, 9, 12 ,13 ,14 ,15 ,17, 18 ,19 ,20, 21 ,22 ,23 ,25, 27 };
    int gpioPin [1] = {21};
    //fstream fd;
    string direction = "out";
    //fd.open("/sys/class/gpio/export", ios::out);
    // if (! fd) {
    //     perror("Unable to open /sys/class/gpio/export");
    //     exit(1);
    // }


    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/export");
        return;
    }

    // echo 3 > /sys/class/gpio/export
    for (int x : gpioPin){  
        // fd << x;
        char port_str[10];
        sprintf(port_str, "%d", x);
        if (write(fd, port_str, strlen(port_str)) != strlen(port_str)) {
            perror("Error writing to /sys/class/gpio/export");
        return;
    }
    close(fd);
    GPIODirectionSetter(x, direction);    
    }
    // fd.close();

};

int main(void){    
    initGPIOFsOutPin();
    
    
    
    
    
    gpioFs gpio21 = gpioFs(21);
    gpio21.gpioValueSet(1);
    usleep(50000);
    gpio21.gpioValueSet(0);
    gpio21.debug();
    return 0;
}