#ifndef PROGRESSBARCHARS_h
#define PROGRESSBARCHARS_h

const byte START_DIV_0_OF_1[8] = {
  B01111,
  B11000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11000,
  B01111
}; // Char début 0 / 1

const  byte START_DIV_1_OF_1[8] = {
  B01111,
  B11000,
  B10011,
  B10111,
  B10111,
  B10011,
  B11000,
  B01111
}; // Char début 1 / 1

const  byte DIV_0_OF_2[8] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
}; // Char milieu 0 / 2

const  byte DIV_1_OF_2[8] = {
  B11111,
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
}; // Char milieu 1 / 2

const byte DIV_2_OF_2[8] = {
  B11111,
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
  B11111
}; // Char milieu 2 / 2

const  byte END_DIV_0_OF_1[8] = {
  B11110,
  B00011,
  B00001,
  B00001,
  B00001,
  B00001,
  B00011,
  B11110
}; // Char fin 0 / 1

const  byte END_DIV_1_OF_1[8] = {
  B11110,
  B00011,
  B11001,
  B11101,
  B11101,
  B11001,
  B00011,
  B11110
}; // Char fin 1 / 1


#endif
