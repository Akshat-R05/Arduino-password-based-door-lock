#include <Keypad.h> 
#include <LiquidCrystal.h> // This library allows an Arduino/Genuino board to control LiquidCrystal displays (LCDs) 
#include <Servo.h>// this library allows an arduino to control servo motors

#define Password_Length 5 // gives room for 4 characters for password and 1 null character , # define creates a macro 

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);//This library allows an Arduino/Genuino board to control LiquidCrystal displays (LCDs) 

int pos = 0; // variable to store servo position

char Data[Password_Length];
char Master[Password_Length] = "1234";
byte data_count = 0, master_count = 0;

bool Pass_is_good;
bool door = false;
char customKey;


/*---preparing keypad---*/

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);//This function is used to assign the Arduino pins (declared in “row[]” and “col[]”) to the button (buttons are stored in variables of rows and cols)This is saved in any function name and then all the built-in functions of this library are called in code with this particular function name.


/*--- Main Action ---*/
void setup()
{
  myservo.attach(9);//(pin servo is attached to(only 9 and 10 supported by servo library))
  ServoClose();
  lcd.begin(16, 2);//specifies the dimensions (columns and rows) of the display.
  lcd.print("Protected Door");//prints text on lcd
  loading("Loading");
  lcd.clear();// clears the lcd screen and positions cursor in upper left corner
}


void loop()
{
  if (door == true)
  {
    customKey = customKeypad.getKey();
    if (customKey == '#')
    {
      lcd.clear();
      ServoClose();
      lcd.print("Door is closed");
      delay(1000);
      door = false;
    }
  }
  else
    Open();

}

void loading (char msg[]) {
  lcd.setCursor(0, 1);//position the lcd cursor at which next text will be written
  lcd.print(msg);

  for (int i = 0; i < 9; i++) {
    delay(100);          // waiting time before enter password comes on screen
    lcd.print(".");
  }
}

void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0;//clear array for new data
  }
  return;
}

void ServoClose()
{
  for (pos = 90; pos >= 0; pos -= 10) { 
    myservo.write(pos);//the value of angle for servo to rotate
  }
}

void ServoOpen()// opens servo 0 to 90
{
  for (pos = 0; pos <= 90; pos += 10) {
    myservo.write(pos);  
  }
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  
  customKey = customKeypad.getKey();
  if (customKey) // to make sure a key is actually pressed
  {
    Data[data_count] = customKey;// store character in data array
    lcd.setCursor(data_count, 1);// move cursor to show each new character
    lcd.print(Data[data_count]);//print character at said cursor
    data_count++;              // increment data array by 1 to store new character and keep count of characters added
  }

  if (data_count == Password_Length - 1)// if the array index is equal to the number of expected chars, compare data to master
  {
    if (!strcmp(Data, Master))// equal to (strcmp(Data, Master) == 0)
    {
      lcd.clear();
      ServoOpen();// servo will move from 0 to 90 degree
      lcd.print(" Door is Open ");
      door = true;
      delay(3000);
      loading("Waiting");
      lcd.clear();
      lcd.print(" Time is up! ");
      delay(1000);
      ServoClose();
      door = false;      
    }
    else
    {
      lcd.clear();
      lcd.print(" Wrong Password ");
      door = false;
    }
    delay(1000);
    lcd.clear();
    clearData();
  }
}

