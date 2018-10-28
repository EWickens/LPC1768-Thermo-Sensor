#include "mbed.h"
#include "C12832.h"
#include "LM75B.h"


//Initialises the Potentiometer pins
AnalogIn pot1(p19);
AnalogIn pot2(p20);

//Initializes the LED pins
DigitalOut redLED(p23);
DigitalOut greenLED(p24);

C12832 lcd(p5, p7, p6, p8, p11); //Initializes the LCD pins
LM75B sensor(p28,p27); //Initializes the temperature sensor pins

float upperBound;
float lowerBound;
float currentTemp;

//Functions as the header for the UpdateLCD function
void UpdateLCD (float upperBound, float lowerBound, float currentTemp);


int main() { 

    redLED = 0x1;
    greenLED = 0x1;
    
    while(1) {    
        
        //Maps the potentiometer ranges to 100-1 instead of 1.0-0.0
        
        float upperBound = pot2 * 50;
        float lowerBound = pot1 * upperBound;
        //Fetches the current temperature from the sensor
        currentTemp = sensor.read();        
        
        //Updates the LCD
        UpdateLCD(currentTemp, upperBound, lowerBound);
       
        //Above upper bound - red LED comes on
        if(currentTemp > upperBound){
            redLED = 0x0;
        }
        
        //If the temperature is between the bounds
        else if(currentTemp > lowerBound && currentTemp < upperBound){ //Replace this with a ternary operator
            float dynamicRange = upperBound-lowerBound; //Range between the upper and lower bound
            float tempToUpperRange = upperBound-currentTemp; //Range between upper and current temp
            float scaleFactor = dynamicRange/tempToUpperRange; //Factor to scale
            
            //Sets the LED's on
            
            redLED = 0x0;
            greenLED = 0x0;

            if(scaleFactor > 0){
                //This accounts for capping the rate at 5 flashes by decreasing the wait time just after the lower bound
                if((1.0/scaleFactor) > .8){ 
                    wait(1.0/scaleFactor);
                    }
                else{
                    //This caps the rate at 5 flashes per second
                    wait(0.2 + 1.0/scaleFactor);
                    }
            }
            
            // Sets the LED's off.
            redLED = 0x1;
            greenLED = 0x1;
            }
            
    }
}
//Function to update the LCD with the current temperature and bound positions.
void UpdateLCD (float currentTemp, float upperBound, float lowerBound){
    lcd.cls();
    lcd.locate(0,3);
    
    //Displays the Upper Lower and Current temperature to 2 decimal places.
    lcd.printf("Temp: %.2f\nUpper:%.2f,\nLower:%.2f",currentTemp, upperBound,lowerBound);
    wait(0.05);
}