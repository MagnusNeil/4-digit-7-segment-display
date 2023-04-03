//Pins
const int D1 = 4;
const int D2 = 17;
const int D3 = 18;
const int D4 = 16;
const int A = 33;
const int B = 19;
const int C = 26;
const int D = 14;
const int E = 13;
const int F = 32;
const int G = 25;
const int DP = 27;

//Counter variables and pointers to each of the variables used to display the correct numbers.
int cnt1 = 0;
int* cntPtr1 = &cnt1;
int cnt2 = 20;
int* cntPtr2 = &cnt2;
int cnt3 = 40;
int* cntPtr3 = &cnt3;
int cnt4 = 60;
int* cntPtr4 = &cnt4;

const int flashTime = 2; //How long each digit will be on in ms
int timerCounter = 0; //Variable used to tell when the number in the display should increment
const int timerCounterMax = 124; //Maximum value of timerCounter before the display increments

//Arrays to change the display
const int arraySize = 12;
int pins[arraySize] = {D1, D2, D3, D4, A, B, C, D, E, F, G, DP};
int inputArray[arraySize] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}; //Everything is turned off

const int stringArraySize = 16;
char hexValues[stringArraySize][5] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000",
                                      "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
int valueChart[3] = {0,0,0};

//Used for userinput from the serial monitor
String readFromUser;
char readFromUserChar[4];

//All possible states for the display (only numbers, also including a variation with the decimal point) in hexadecimal
const int numberOfStates = 80;
char allowedStates[numberOfStates][4] = {"7FC", "760", "7DA", "7F2", "766", "7B6", "7BE", "7E0", "7FE", "7E6",
                                         "7FD", "761", "7DB", "7F3", "767", "7B7", "7BF", "7E1", "7FF", "7E7",
                                         "BFC", "B60", "BDA", "BF2", "B66", "BB6", "BBE", "BE0", "BFE", "BE6",
                                         "BFD", "B61", "BDB", "BF3", "B67", "BB7", "BBF", "BE1", "BFF", "BE7",
                                         "DFC", "D60", "DDA", "DF2", "D66", "DB6", "DBE", "DE0", "DFE", "DE6",
                                         "DFD", "D61", "DDB", "DF3", "D67", "DB7", "DBF", "DE1", "DFF", "DE7",
                                         "EFC", "E60", "EDA", "EF2", "E66", "EB6", "EBE", "EE0", "EFE", "EE6",
                                         "EFD", "E61", "EDB", "EF3", "E67", "EB7", "EBF", "EE1", "EFF", "EE7"};

void changeDisplay(char newValue[4]){
  for(int i = 0; i < 3; ++i){
    if(newValue[i]-'0' < 10){
      valueChart[i] = newValue[i]-'0';
    }
    else{
      valueChart[i] = newValue[i]-'7';
    }
  }
  for(int i = 0; i < arraySize; ++i){
    if(i < 4){
      inputArray[i] = hexValues[valueChart[0]][i]-'0';
    }else if(i >= 4 && i < 8){
      inputArray[i] = hexValues[valueChart[1]][i-4]-'0';
    }else{
      inputArray[i] = hexValues[valueChart[2]][i-8]-'0';
    }
  }
}

void incrementCounter(int* cntPtr1, int* cntPtr2, int* cntPtr3, int* cntPtr4){
  ++(*cntPtr4);
  if((*cntPtr4 - 60) == 10){
    *cntPtr4 = 60;
    ++(*cntPtr3);
  }
  if((*cntPtr3 - 40) == 10){
    *cntPtr3 = 40;
    ++(*cntPtr2);
  }
  if((*cntPtr2 - 20) == 10){
    *cntPtr2 = 20;
    ++(*cntPtr1);
  }
  if(*cntPtr1 == 10){
    *cntPtr1 = 0;
  }  
}

void setUserValue(){
  while(Serial.available() == 0){
  }

  readFromUser = Serial.readString();
  for(int i = 0; i < 3; ++i){
    readFromUserChar[i] = readFromUser[i];
  }
  changeDisplay(readFromUserChar);
}

void updateDisplay(int* cntPtr1, int*cntPtr2, int* cntPtr3, int*cntPtr4, int flashTime){
  changeDisplay(allowedStates[*cntPtr4]);
  for(int i = 0; i < arraySize; ++i){
    digitalWrite(pins[i], inputArray[i]);
  }
  delay(flashTime);
  changeDisplay(allowedStates[*cntPtr3]);
  for(int i = 0; i < arraySize; ++i){
    digitalWrite(pins[i], inputArray[i]);
  }
  delay(flashTime);
  changeDisplay(allowedStates[*cntPtr2]);
  for(int i = 0; i < arraySize; ++i){
    digitalWrite(pins[i], inputArray[i]);
  }
  delay(flashTime);
  changeDisplay(allowedStates[*cntPtr1]);
  for(int i = 0; i < arraySize; ++i){
    digitalWrite(pins[i], inputArray[i]);
  }
  delay(flashTime);
}

void setup() {
  for(int i = 0; i < arraySize; ++i){
    pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
  for(int i = 0; i < arraySize; ++i){
    digitalWrite(pins[i], inputArray[i]);
  }
}

void loop() {
  updateDisplay(cntPtr1, cntPtr2, cntPtr3, cntPtr4, flashTime);
  if(timerCounter == timerCounterMax){
    incrementCounter(cntPtr1, cntPtr2, cntPtr3, cntPtr4);
    timerCounter = 0;
  }else{
    ++timerCounter;
  }
}
