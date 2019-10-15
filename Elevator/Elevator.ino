#include <Stepper.h>
#include <LiquidCrystal.h>
#include <Servo.h>

int STEPS_PER_REV = 2048;

int A = 8;
int B = 9;
int C = 10;
int D = 11;

Stepper steppermotor(STEPS_PER_REV, A, C ,B , D);
LiquidCrystal lcd(A8, A9, A12, A13, A14, A15);
Servo servo;

//Buttons in the Elevator
const int b1 = 30;
const int b2 = 31;
const int b3 = 32;
const int b4 = 33;

//Buttons every floor up/down
const int b4d = 34;
const int b3u = 35;
const int b3d = 36;
const int b2u = 37;
const int b2d = 38;
const int b1u = 39;

//Memory
int bStateMem = 1;
int mem = 1;

//Memory for every Floor
int floor1 = 0;
int floor2 = 0;
int floor3 = 0;
int floor4 = 0;

//Going up or down
int elevGoingUp = 0;
int elevGoingDown = 0;

//Button Interrupt
const int interruptPin = 18;

//Arrow Char
byte arrow_u[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,  
  B00100
}; 

byte arrow_d[8] = {  
  B00100,
  B00100,
  B00100,
  B00100,  
  B00100,  
  B11111,
  B01110,
  B00100
}; 

void setup()
{
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);

  pinMode(b4d, INPUT_PULLUP);
  pinMode(b3u, INPUT_PULLUP);
  pinMode(b3d, INPUT_PULLUP);
  pinMode(b2u, INPUT_PULLUP);
  pinMode(b2d, INPUT_PULLUP);
  pinMode(b1u, INPUT_PULLUP);

  pinMode(interruptPin, INPUT_PULLUP);


  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("----Floor 1----");
  steppermotor.setSpeed(15);

  Serial.begin(9600);
  servo.attach(2);

  lcd.createChar (5, arrow_u);     // load arrow_u to memory 5
  lcd.createChar (6, arrow_d);     // load arrow_d to memory 6

  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptFunction, CHANGE);

  servo.write(0);

  steppermotor.setSpeed(700);

  floor1 = 0;
  floor2 = 0;
  floor3 = 0;
  floor4 = 0;

}
 
void loop()
{
  int b1State, b2State, b3State, b4State;
  int b4down, b3up, b3down, b2up, b2down, b1up;


  //For Elevator
  b1State = digitalRead(b1);
  b2State = digitalRead(b2);
  b3State = digitalRead(b3);
  b4State = digitalRead(b4);

  //For every floor
  b4down = digitalRead(b4d);
  b3up = digitalRead(b3u);
  b3down = digitalRead(b3d);
  b2up = digitalRead(b2u);
  b2down = digitalRead(b2d);
  b1up = digitalRead(b1u);
  
  if(b1State == LOW ) {
    mem = bStateMem;
    bStateMem = 1;

    if(mem != bStateMem) {
      int numSpin = bStateMem - mem;

      if(numSpin == -3) {
        mem = mem - 1;
        for(int i = 1; i <= 3; i++) {
      
              steppermotor.step(-6900);
              lcd.setCursor(0,0);
              lcd.print("----Floor ");
              lcd.print(mem--);
              lcd.print("---");
      
              if(floor3 == 1 && i == 1) {
              steppermotor.step(0);

              openDoor();
              closeDoor();
              floor3 = 0;
              }
        
             if(floor2 == 1 && i == 2) {
              steppermotor.step(0);

              openDoor();
              closeDoor();
              floor2 = 0;
            }
          }

          mem = bStateMem;
          bStateMem = 1;

          openDoor();
          closeDoor();
          
      }

      if(numSpin == -2) {
        steppermotor.step(-6900);
        displayFloor2();

        if(floor2 == 1) {
          mem = bStateMem;
          bStateMem = 2;

          openDoor();
          closeDoor();
          floor2 = 0;
        }

        steppermotor.step(-6900);
        displayFloor1();

        mem = bStateMem;
        bStateMem = 1;

        openDoor();
        closeDoor();
      }

      if(numSpin == -1) {
        steppermotor.step(-6900);
        displayFloor1();

        mem = bStateMem;
        bStateMem = 1;

        openDoor();
        closeDoor();
      }
      
    }     
  }

  else if (b2State == LOW) {
    mem = bStateMem;
    bStateMem = 2;
    
    if(mem != bStateMem) {

      int numSpin = bStateMem - mem;

      if(numSpin == 1) {
      
          steppermotor.step(6900);
          displayFloor2();

          openDoor();
          closeDoor();

          if(floor3 == 1) {
              steppermotor.step(6900);
              displayFloor3();

              mem = bStateMem;
              bStateMem = 3;
         
              openDoor();
              closeDoor();
              floor3 = 0;
              
              if(floor4 == 1) {
                steppermotor.step(6900);
                displayFloor4();

                mem = bStateMem;
                bStateMem = 4;

                openDoor();
                closeDoor();
                floor4 = 0;
              }
            }
      
          if(floor4 == 1) {
            steppermotor.step(6900);
            displayFloor3();

            mem = bStateMem;
            bStateMem = 4;

            if(floor3 == 1) {
              mem = bStateMem;
              bStateMem = 3;
              
              openDoor();
              closeDoor();

              floor3 = 0;
            }
            
            steppermotor.step(6900);
            displayFloor4();

            openDoor();

            closeDoor();
        
            floor4 = 0;
          }
      }

      if(numSpin == -1) {
        steppermotor.step(-6900);
        displayFloor2();
        mem = bStateMem;
        bStateMem = 2;

        openDoor();
        closeDoor();

        if(floor1 = 1) {
          steppermotor.step(-6900);
          displayFloor1();
          mem = bStateMem;
          bStateMem = 1;

          openDoor();
          closeDoor();
          floor1 = 0;
        }
      }

      if(numSpin == -2) {
        steppermotor.step(-6900);
        displayFloor3();

        if(floor3 == 1) {
          steppermotor.step(0);
          mem = bStateMem;
          bStateMem = 3;
          openDoor();
          closeDoor();

          floor3 = 0;
        }

        steppermotor.step(-6900);
        displayFloor2();
        mem = bStateMem;
        bStateMem = 2;

        openDoor();
        closeDoor();
        floor2 = 0;

        if(floor1 == 1) {
          steppermotor.step(-6900);
          displayFloor1();
          mem = bStateMem;
          bStateMem = 1;

          openDoor();
          closeDoor();
          floor1 = 0;    
        }
        
      }

      floor2 = 0;
    }
  }

  else if (b3State == LOW) {
    
    mem = bStateMem;
    bStateMem = 3;

    if( mem != bStateMem) {

      int numSpin = bStateMem - mem;

      if(numSpin == 1) {
        steppermotor.step(6900);
        displayFloor3();
        mem = bStateMem;
        bStateMem = 3;

        openDoor();
        closeDoor();

        if(floor4 == 1) {
          steppermotor.step(6900);
          displayFloor4();
          mem = bStateMem;
          bStateMem = 4;

          openDoor();
          closeDoor();
          floor4 = 0;
        }
      }

      if(numSpin == 2) {
      
          steppermotor.step(6900);
          displayFloor2();
      
          if(floor2 == 1) {
            steppermotor.step(0);
            mem = bStateMem;
            bStateMem = 2;

            openDoor();
            closeDoor();
            floor2 = 0;
          }

          steppermotor.step(6900);
          displayFloor3();
          mem = bStateMem;
          bStateMem = 3;

          openDoor();
          closeDoor();
          floor3 = 0;

          if(floor4 == 1) {

            steppermotor.step(6900);
            displayFloor4();
            mem = bStateMem;
            bStateMem = 4;

            openDoor();
            closeDoor();
            floor4 = 0;
        }
      }

      if(numSpin == -1) {
        steppermotor.step(-6900);
        displayFloor3();
        mem = bStateMem;
        bStateMem = 3;

        openDoor();
        closeDoor();

          if(floor2 == 1) {
            steppermotor.step(-6900);
            displayFloor2();
            mem = bStateMem;
            bStateMem = 2;

            openDoor();
            closeDoor();
            floor2 = 0;

            if(floor1 == 1) {
              steppermotor.step(-6900);
              displayFloor1();
              mem = bStateMem;
              bStateMem = 1;

              openDoor();
              closeDoor();
              floor1 = 0;
              floor3 = 0;
            }
        }

        if(floor1 == 1) {
          steppermotor.step(-2047);
          displayFloor2();

          if(floor2 == 1) {
            mem = bStateMem;
            bStateMem = 2;

            openDoor();
            closeDoor();
            floor2 = 0;
          }

          steppermotor.step(-6900);
          displayFloor1();

          mem = bStateMem;
          bStateMem = 1;

          openDoor();
          closeDoor();
          floor1 = 0;
          floor3 = 0;
        }

      }
    
    }
  }
  
   else if (b4State == LOW) {
    mem = bStateMem;
    bStateMem = 4;

    if( mem != bStateMem) {

      int numSpin = bStateMem - mem;

      
      if(numSpin == 3) {
        mem = mem + 1;
        for(int i = 1; i <= numSpin; i++) {
      
        steppermotor.step(6900);
        lcd.setCursor(0,0);
        lcd.print("----Floor ");
        lcd.print(mem++);
        lcd.print("---");
      
        if(floor2 == 1 && i == 1) {
          steppermotor.step(0);

          openDoor();
          closeDoor();
          floor2 = 0;
        }
        
        if(floor3 == 1 && i == 2) {
          steppermotor.step(0);

          openDoor();
          closeDoor();
          floor3= 0;
        }
      }
      
      mem = bStateMem;
      bStateMem = 4;
      floor4 = 0;

      openDoor();
      closeDoor();

      }

      if(numSpin == 2) {
        steppermotor.step(6900);
        displayFloor3();

        if(floor3 == 1) {
          mem = bStateMem;
          bStateMem = 3;

          openDoor();
          closeDoor();
          floor3 = 0;
        }

        steppermotor.step(6900);
        displayFloor4();

        mem = bStateMem;
        bStateMem = 4;

        openDoor();
        closeDoor();
        
      }

      if(numSpin == 1) {
        steppermotor.step(6900);
        displayFloor4();
        mem = bStateMem;
        bStateMem = 4;

        openDoor();
        closeDoor();
      }

    }
  }
  
}



void floorState(int currentFloor, int gotoFloor) {
  int numSpin;

  openDoor();

  delay(1500);
  
  numSpin = gotoFloor - currentFloor;

  if(numSpin > 0) {
    lcd.setCursor(1,1);
    lcd.print(char(5));
    lcd.print(char(5));
    lcd.print(" Going Up ");
    lcd.print(char(5));
    lcd.print(char(5));
  }
  else {
    lcd.setCursor(0,1);
    lcd.print(char(6));
    lcd.print(char(6));
    lcd.print(" Going Down ");
    lcd.print(char(6));
    lcd.print(char(6));
  }

  delay(500);

  //Spinning and Changing of Floor Number
  if(numSpin > 0) {
    currentFloor = currentFloor + 1;
    for(int i = 1; i <= numSpin; i++) {
    steppermotor.step(6900);
    lcd.setCursor(0,0);
    lcd.print("----Floor ");
    lcd.print(currentFloor++);
    lcd.print("----");
    }
  }

  else if(numSpin < 0) {
    currentFloor = currentFloor - 1;
    for(int i = 1; i <= (-1 * numSpin); i++) {
      steppermotor.step(-6900);
      lcd.setCursor(0,0);
      lcd.print("----Floor ");
      lcd.print(currentFloor--);
      lcd.print("----");
     }
    }

   lcd.setCursor(0,1);
   lcd.print("                   ");
    
   closeDoor();
   //End Spinning ang Changing Floor Number

}

void interruptFunction() {
  int b1State, b2State, b3State, b4State;

  b1State = digitalRead(b1);
  b2State = digitalRead(b2);
  b3State = digitalRead(b3);
  b4State = digitalRead(b4);


  if(b1State == LOW) {
    floor1 = 1;  
  }
  
  if(b2State == LOW) {
    floor2 = 1;
  }

  if(b3State == LOW) {
    floor3 = 1;
  }

  if(b4State == LOW) {
    floor4 = 1;
  }
  
}

void closeDoor() {
   delay(2000);
   for (int i = 90; i >= 0; i -= 1) {
    servo.write(i);
    delay(15);
   }
}

void openDoor() {
  delay(500);
  for (int i = 0; i <= 90; i += 1) {
    servo.write(i);
    delay(15);
   }
}

void displayFloor1() {
    lcd.setCursor(0,0);
    lcd.print("----Floor 1---");
}

void displayFloor2() {
    lcd.setCursor(0,0);
    lcd.print("----Floor 2---");
}


void displayFloor3() {
    lcd.setCursor(0,0);
    lcd.print("----Floor 3---");
}


void displayFloor4() {
    lcd.setCursor(0,0);
    lcd.print("----Floor 4---");
}



  
