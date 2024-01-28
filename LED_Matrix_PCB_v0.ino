/**********************************************************************
Code for DIY LED matrix board

Inspired by 
https://github.com/Freenove/Freenove_Ultimate_Starter_Kit_for_ESP32/tree/master/C
and
https://xantorohara.github.io/led-matrix-editor/
**********************************************************************/

int latchPin = 2;  // IC 74HC595 #1 & #2 RCK
int clockPin = 4;  // IC 74HC595 #1 & #2 SCK
int dataPin = 15;  // IC 74HC595 #1 SER

/* IC 74HC595 #1 we use it to set column's pins.
This array is depend on the physical pins connection.
*/
byte cols[] = {
  0b11110111,  //pin QE col 1
  0b01111111,  //pin QA col 2
  0b11101111,  //pin QD col 3
  0b11111110,  //pin QH col 4
  0b11111011,  //pin QF col 5
  0b11111101,  //pin QG col 6
  0b11011111,  //pin QC col 7
  0b10111111   //pin QB col 8
};

/* IC 74HC595 #1 we use it to set row's pins.
This array is depend on the physical pins connection.
*/
byte rows[] = {
  0b00010000,  //pin QD row 1
  0b00000010,  //pin QG row 2
  0b01000000,  //pin QB row 3
  0b00000100,  //pin QF row 4
  0b10000000,  //pin QA row 5
  0b00100000,  //pin QC row 6
  0b00000001,  //pin QH row 7
  0b00001000   //pin QE row 8
};

// Byte arrays describing different symbols.
// Inspired by https://int10h.org/oldschool-pc-fonts/fontlist/font?ibm_bios

// smiling face
byte s_smile1[]{
  0b01111110,
  0b10000001,
  0b10100101,
  0b10000001,
  0b10111101,
  0b10011001,
  0b10000001,
  0b01111110
};

// numbers
byte s_0[]{
  0b01111100,
  0b11000110,
  0b11001110,
  0b11011110,
  0b11110110,
  0b11100110,
  0b01111100,
  0b00000000
};

byte s_1[]{
  0b00110000,
  0b01110000,
  0b00110000,
  0b00110000,
  0b00110000,
  0b00110000,
  0b11111100,
  0b00000000
};

byte s_2[]{
  0b01111000,
  0b11001100,
  0b00001100,
  0b00111000,
  0b01100000,
  0b11001100,
  0b11111100,
  0b00000000,
};

byte s_3[]{
  0b01111000,
  0b11001100,
  0b00001100,
  0b00111000,
  0b00001100,
  0b11001100,
  0b01111000,
  0b00000000
};

byte s_4[]{
  0b00011100,
  0b00111100,
  0b01101100,
  0b11001100,
  0b11111110,
  0b00001100,
  0b00011110,
  0b00000000
};

byte s_5[]{
  0b11111100,
  0b11000000,
  0b11111000,
  0b00001100,
  0b00001100,
  0b11001100,
  0b01111000,
  0b00000000
};

byte s_6[]{
  0b00111000,
  0b01100000,
  0b11000000,
  0b11111000,
  0b11001100,
  0b11001100,
  0b01111000,
  0b00000000
};

byte s_7[]{
  0b11111100,
  0b11001100,
  0b00001100,
  0b00011000,
  0b00110000,
  0b00110000,
  0b00110000,
  0b00000000
};

byte s_8[]{
  0b01111000,
  0b11001100,
  0b11001100,
  0b01111000,
  0b11001100,
  0b11001100,
  0b01111000,
  0b00000000
};

byte s_9[]{
  0b01111000,
  0b11001100,
  0b11001100,
  0b01111100,
  0b00001100,
  0b00011000,
  0b01110000,
  0b00000000
};

// frame animation
byte frame_animation[][8] = {
  { 0b11111111,
    0b00000000,
    0b00000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b11111111 },
  { 0b01111111,
    0b10000000,
    0b00000000,
    0b11000000,
    0b11000000,
    0b00000000,
    0b10000000,
    0b01111111 },
  { 0b00111111,
    0b11000000,
    0b00000000,
    0b01100000,
    0b01100000,
    0b00000000,
    0b11000000,
    0b00111111 },
  { 0b00011111,
    0b111000000,
    0b00000000,
    0b00110000,
    0b00110000,
    0b00000000,
    0b11100000,
    0b00011111 },
  { 0b00001111,
    0b11110000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b11110000,
    0b00001111 },
  { 0b00000111,
    0b11111000,
    0b00000000,
    0b00001100,
    0b00001100,
    0b00000000,
    0b11111000,
    0b00000111 },
  { 0b00000011,
    0b11111100,
    0b00000000,
    0b00000110,
    0b00000110,
    0b00000000,
    0b11111100,
    0b00000011 },
  { 0b00000001,
    0b11111110,
    0b00000000,
    0b00000011,
    0b00000011,
    0b00000000,
    0b11111110,
    0b00000001 },
  { 0b00000000,
    0b11111111,
    0b00000000,
    0b00000001,
    0b00000001,
    0b00000000,
    0b11111111,
    0b00000000 },
  { 0b01010101,
    0b10101010,
    0b00000000,
    0b00000011,
    0b00000011,
    0b00000000,
    0b10101010,
    0b01010101 },
  { 0b10101010,
    0b01010101,
    0b00000000,
    0b00000110,
    0b00000110,
    0b00000000,
    0b01010101,
    0b10101010 },
  { 0b01010101,
    0b10101010,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b10101010,
    0b01010101 },
  { 0b10101010,
    0b01010101,
    0b00000000,
    0b00110000,
    0b00110000,
    0b00000000,
    0b01010101,
    0b10101010 },
  { 0b01010101,
    0b10101010,
    0b00000000,
    0b01100000,
    0b01100000,
    0b00000000,
    0b10101010,
    0b01010101 },
  { 0b10101010,
    0b01010101,
    0b00000000,
    0b11000000,
    0b11000000,
    0b00000000,
    0b01010101,
    0b10101010 },
  {
    0b01010101,
    0b10101010,
    0b00000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b01010101,
    0b10101010,
  }
};

// the array length is the animation frames count
int frame_animation_len = sizeof(frame_animation) / 8;

// another frame animation
byte frame_animation_1[][8]{
  { 0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111 },
  { 0b00000000,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b00000000 },
  { 0b00000000,
    0b00000000,
    0b00111100,
    0b00100100,
    0b00100100,
    0b00111100,
    0b00000000,
    0b00000000 },
  { 0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000 },
  { 0b00000000,
    0b00000000,
    0b00111100,
    0b00100100,
    0b00100100,
    0b00111100,
    0b00000000,
    0b00000000 },
  { 0b00000000,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b00000000 },
  { 0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111 },
};

int frame_animation_1_len = sizeof(frame_animation_1) / 8;


void setup() {

  // preparing our pins to work with the shift registers
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {

  // turn on all LEDs for 2s (2000ms)
  disp_all_on(2000);

  // display LEDs at the corners of the matrix
  disp_one_led(1, 1, 300);
  disp_one_led(8, 1, 300);
  disp_one_led(8, 8, 300);
  disp_one_led(1, 8, 300);

  // display LEDs one by one, diagonal from 1,1 to 8,8
  for (int i = 1; i <= 8; i++) {
    disp_one_led(i, i, 200);
  }

  // display LEDs one by one, diagonal from 1,8 to 8,1
  for (int i = 8; i >= 1; i--) {
    disp_one_led(9 - i, i, 200);
  }

  // display LEDs one by one by rows, from 1,1
  for (int i = 1; i <= 8; i++) {
    for (int j = 1; j <= 8; j++) {
      disp_one_led(i, j, 50);
    }
  }

  // display LEDs one by one by columns, from 1,1
  for (int i = 1; i <= 8; i++) {
    for (int j = 1; j <= 8; j++) {
      disp_one_led(j, i, 50);
    }
  }

  // display row LEDS one by one from 1
  for (int i = 1; i <= 8; i++) {
    disp_one_row(i, 300);
  }

  // display column LEDS one by one from 1
  for (int i = 1; i <= 8; i++) {
    disp_one_col(i, 300);
  }

  // smiling face
  disp_8x8(s_smile1, 2000);

  // numbers
  disp_8x8(s_0, 2000);
  disp_8x8(s_1, 2000);
  disp_8x8(s_2, 2000);
  disp_8x8(s_3, 2000);
  disp_8x8(s_4, 2000);
  disp_8x8(s_5, 2000);
  disp_8x8(s_6, 2000);
  disp_8x8(s_7, 2000);
  disp_8x8(s_8, 2000);
  disp_8x8(s_9, 2000);

  // frame animation
  for (int i = 0; i < frame_animation_len; i++) {
    disp_8x8(frame_animation[i], 200);
  }

  // frame animation 1
  for (int i = 0; i < frame_animation_1_len; i++) {
    disp_8x8(frame_animation_1[i], 200);
  }

  // random pixels
  for (int i = 0; i < 20; i++) {
    disp_random(500);
  }
}

/* This is the most important function to interface with the shift registers.
It receives 2 bytes: one for the register, which controls rows, and one 
for the register which controls columns. We write LOW to the latchPin. this means 
the registers ready to receive data. Then we are using standard shiftOut function 
to send data to the register. First call of the function will pass data to the 
first register. it's row data, and the first register is for columns, but the data
will be "pushed" to the second register when we call shiftOut again. As a result 
we'll have row byte in the "row" (second) register, and columns data in the "column" (first) 
register. When we set latchPin to HIGH the data we provided will affect the register's pins. 
They will be set in high level when it's 1 and in the low level when it's 0.
*/
void send_data_to_ICs(byte rws, byte cls) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, rws);
  shiftOut(dataPin, clockPin, LSBFIRST, cls);
  digitalWrite(latchPin, HIGH);
}

/* 
Function to turn on all the leds.
We don't use any calculations to set bits in the correct order.
We are just need to set all row's pins to 1 and all col's pins to 0.
Hereinafter we'll use del parameter to set delay for some action.
*/
void disp_all_on(int del) {
  send_data_to_ICs(0b11111111, 0b00000000);
  delay(del);
}

// Function to turn off all the leds.
void disp_all_off(int del) {
  send_data_to_ICs(0b00000000, 0b00000000);
  delay(del);
}

/* We are using rows and cols arrays to send data to the ICs.
the arrays are indexed from 0, but I decide that it's more
convenient to say "display dot 1, 1" than "0, 0", so when we call
this function we give it logical row/col numbers and it translates
them to the arrays elements.
*/
void disp_one_led(int row, int col, int del) {
  send_data_to_ICs(rows[row - 1], cols[col - 1]);
  delay(del);
}

// Function to display one row.
void disp_one_row(int row, int del) {
  send_data_to_ICs(rows[row - 1], 0b00000000);
  delay(del);
}

// Function to display one column.
void disp_one_col(int col, int del) {
  send_data_to_ICs(0b11111111, cols[col - 1]);
  delay(del);
}

// Function to generate and display a random pattern.
void disp_random(int del) {
  byte rand_img[8];
  for (int i = 0; i < 8; i++) {
    rand_img[i] = (byte)random(0, 256);
  }
  disp_8x8(rand_img, del);
}

/* Function to display 8x8 bitmap using afterglow effect.
It receives an array element and delay in ms. It will extract
bits from byte values and display them one by one very fast. 
When it decodes and shows all elements in the array it checks if it has time to 
show the array again.
*/
void disp_8x8(byte symbol[8], int del) {
  unsigned long beginTime = millis();
  do {
    for (int i = 0; i < 8; i++) {
      for (byte j = 0; j < 8; j++) {
        if ((symbol[i] >> j) & 1 == 1) {
          disp_one_led(i + 1, 8 - j, 0);
        }
      }
    }
  } while (millis() - beginTime < del);
}
