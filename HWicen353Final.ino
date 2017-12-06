/*
 * William Fang
 * Richmond Dea
 * Group 6
 * ICEN353 Final Project
 */

//Initialize Pins

const int redButton = 2;
const int yellowButton = 3;
const int blueButton = 4;
const int greenButton = 5;
//const int buzzerPin = 6;
//const int buzzerPin = 7;
const int redLed = 8;
const int yellowLed = 9;
const int blueLed = 10;
const int greenLed = 11;

//piezzo initialization

const int piezzo = 12;

//Sequence for Simon Says will be generated in this array.
//Up to 20 steps. In the setup, it will be filled with color LEDs from 8 to 11 (red to green led pins)

int ledPattern[20];

//We correspond the tones of the Piezzo from red to green.
//Red - Green, 8 - 11, for pins respectively, so we will need to subtract by 8 to get the proper corresponding sound
//Red - Green, 0 - 3 for the Piezzo Element sounds, in increments of 500 Hz
const int piezzoTones[4] = {500, 1000, 1500, 2000};

//This is how many steps (buttons pushed) has successfully in this sequence
//The amount of steps is based on how many sequences have been completed.
int steps = 0;

//This is how many correct sequence of steps have been finished.
//Each correct sequence will add 1 extra LED to press (step) to the previous sequence and make a new sequence out of it.
int sequence = 1;

//Time is recorded in an unsigned long when using time = millis(). This is the time since Arduino started running
unsigned long time;

//This is the pin corresponding to the button that the user presses.
int buttonState = 0;    

//A failed button press will result in gamecontinue = 0, failing the game
int gamecontinue = 0;

void setup() {
  Serial.begin(9600); //needed for print functions
  //Button Pin (Inputs)
  pinMode(redButton, INPUT_PULLUP);
  pinMode(yellowButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);

  //LED Pin (Outputs) 
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  //Buzzer Pin
  pinMode(piezzo, OUTPUT);
  
  randomSeed(analogRead(0)); //unplugged 0 generates a new random number generator
  for(int i = 0; i < 20; i++)
  {
    ledPattern[i] = random(redLed, greenLed + 1); //A random set of 20 colors for patterns, from pins 8 - 11
  } //Max here is max - 1 so add a +1 for greenLed to compensate
}

void loop() {
  //buttonState = digitalRead(buttonPin);
  //Basically, just keep showing users patterns and then let them push in pattern 
  //Until a mistake is made (verifyPattern ends game on failure), game will gamecontinue.
  showPattern();
  steps = 0;
  gamecontinue = verifyPattern();
  if(gamecontinue == 0)
  {
    Serial.println("failed");
    gamefail(); //game fails if gamecontinue is 0, pattern verification failed
    while(1)
    {
      //fail means waiting forever until reset...
    }
  }
  sequence++; //game gets harder if you didn't fail and continue!
  steps = 0;
}

//Method shows the pattern to the user
void showPattern()
{
  Serial.println("showing pattern!");
  for(steps = 0; steps < sequence; steps++) //Pattern is shown up to how many steps they have gotten up to in the game. The sequence is increased by 1 per successful round
  {
    time = millis();
    Serial.println(steps);
    while(millis() - time < 500) //0.5 millisec flash. Until the time passed since last time query is 0.5 sec, keep LED flashed
    {
      digitalWrite(ledPattern[steps], HIGH);
      tone(piezzo, piezzoTones[ledPattern[steps]-8]); //8 - 11/R - G for Pins, 0 - 3/R - G for Piezzo
    }//piezzoTones[ledPattern[steps]-8]
    time = millis();
    
    while(millis() - time < 500) //0.5 millisec wait. Turn off last LED and prepare for the next LED.
    {
      digitalWrite(ledPattern[steps], LOW);
      noTone(piezzo); //turn off piezzo
    }
  }
}

//This is the button pressing part of the game.
int verifyPattern()
{
    while(steps < sequence) //this round continues until the user has passed the steps in the sequence required
    {
      Serial.println("verifying");
      buttonState = buttonFunction(); //Function will return pin value
      if(buttonState > -1) //when the user presses a button (value of button push corresponds to LED)
      {
      Serial.print("buttonState pressed: "); 
      Serial.println(buttonState);  
      tone(piezzo, piezzoTones[buttonState - 8]); //play button sound and light up
      digitalWrite(buttonState, HIGH);
      delay(500);
      noTone(piezzo);
      digitalWrite(buttonState, LOW);
      if(ledPattern[steps] == buttonState) //if the button matches step in sequence, continue
      {
      Serial.println("Next Step");  
      steps++;
      }
      else
      {
        Serial.println("FAILED! Wrong button"); //wrong button to step!
        return 0;
      }
      }
    }
    Serial.println("Added step to new sequence"); //the round has been completed, returning 1 indicates success to the loop!
    return 1;
}

int buttonFunction() //this function reads what button the user presses, and returns value of corresponding LED pin.
{
  while(1)
  //wait forever until input!
  {
  int buttonPushed = 0;
  if (digitalRead(redButton) == 0) 
  {
    Serial.println("Push Red");
    return redLed;
  }
  else if (digitalRead(yellowButton) == 0) 
  {
    Serial.println("Push Yellow");
    return yellowLed;
  }
  else if (digitalRead(blueButton) == 0) 
  {
    Serial.println("Push Blue");
    return blueLed;
  }
  else if (digitalRead(greenButton) == 0) 
  {
    Serial.println("Push Green");
    return greenLed;
  }
  }
  Serial.println("??? Returned -1"); //Should never happen
  return -1;
}

void gamefail()
{ //Tone played when game fails
  Serial.println("Faillled");
  for(int i = 0; i < 3; i++) //LEDs flash 3 times as losing tone plays!
  {
  tone(piezzo, 100);
  digitalWrite(redLed, HIGH);
  digitalWrite(yellowLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(greenLed, HIGH);
  delay(300);
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(blueLed, LOW);
  digitalWrite(greenLed, LOW);
  delay(300);
  }
  noTone(piezzo);
  while(1)
    {
      //fail means waiting forever until reset...
    }
}



