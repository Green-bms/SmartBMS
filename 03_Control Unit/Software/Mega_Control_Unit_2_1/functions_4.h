#ifndef functions_4_h
#define functions_4_h

#include <Arduino.h>

/* TON Timer function.*/

bool TON(bool en, unsigned long preset, unsigned long &elapsed, unsigned long &count){
 if (en==true)
{
  if (elapsed<preset)
  {
    elapsed=millis()-count;
    return false;
  }
  else
  {
    elapsed=preset;
    return true; 
  }
}
else
{
  elapsed=0;
  count=millis();
  return false;
}
}


/*THR_UP - function that set a true status when the input value go up the threshold parameter for the preset time (using TON).
The function output is reset when the input value goes down the hysteresis parameter*/

bool THR_UP (int value, int thr, int thr_hys, unsigned long preset, unsigned long &elapsed, unsigned long &count, bool &high){
  bool trans_high = TON((value>=thr),preset,elapsed,count);
  if (trans_high==true) {high=true;}
  if ((high==true)&&(value<thr_hys)) {high=false;}
  if (value<thr_hys){high=false;}
  return high;
}


/*THR_DOWN - function that set a true status when the input value go down the threshold parameter for the preset time (using TON).
The function output is reset when the input value goes up the hysteresis parameter*/

bool THR_DOWN (int value, int thr, int thr_hys, unsigned long preset, unsigned long &elapsed, unsigned long &count, bool &low){
  bool trans_low = TON((value<=thr),preset,elapsed,count);
  if (trans_low==true) {low=true;}
  if ((low==true)&&(value>thr_hys)) {low=false;}
  if (value>thr_hys){low=false;}
  return low;
}


/*FILTER_INT - function that filter an integer number making the average of the last 10 values*/

int FILTER_INT (int input,int &index, int val_mem[10]){
val_mem[index]=input;
int output=(val_mem[0]+val_mem[1]+val_mem[2]+val_mem[3]+val_mem[4]+val_mem[5]+val_mem[6]+val_mem[7]+val_mem[8]+val_mem[9])/10;
if (index >= 9){index=0;}
else {index++;}
return output;  
}


/*FILTER_LONG - function that filter a long number making the average of the last 10 values*/

long FILTER_LONG (long input,long &index, long val_mem[10]){
val_mem[index]=input;
long output=(val_mem[0]+val_mem[1]+val_mem[2]+val_mem[3]+val_mem[4]+val_mem[5]+val_mem[6]+val_mem[7]+val_mem[8]+val_mem[9])/10;
if (index >= 9){index=0;}
else {index++;}
return output;  
}


/*FILTER_FLOAT - function that filter a float number making the average of the last 10 values*/

float FILTER_FLOAT (float input,int &index, float val_mem[10]){
val_mem[index]=input;
int output=(val_mem[0]+val_mem[1]+val_mem[2]+val_mem[3]+val_mem[4]+val_mem[5]+val_mem[6]+val_mem[7]+val_mem[8]+val_mem[9])/10;
if (index >= 9){index=0;}
else {index++;}
return output;  
}


/*PosTrig - function that return the positive edge of a boolean variable*/

bool PosTrig (bool in, bool &memory)
{
if (!in) { memory = false;}
if (in && !memory) { memory = true; return 1; } 
else { return 0; } 
}


/*NegTrig - function that return the negative edge of a boolean variable*/

bool NegTrig (bool in, bool &memory)
{
if (in) { memory = true;}
if (!in && memory) { memory = false; return 1; } 
else { return 0; } 
}



/*readVcc - function that return the supply voltage of the microcontroller*/

  long readVcc() {
  uint8_t low;
  uint8_t high;
  long result;
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  low  = ADCL; // must read ADCL first - it then locks ADCH
  high = ADCH; // unlocks both
  result = (high << 8) | low;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

/* Life - Funcion that check the comunication with other microcontrollers using a sign of life byte*/

bool Life (byte number, unsigned long sample, byte &mem, unsigned long &t){
    if (millis() - t >= sample) 
    {
        mem=number;
        t += sample;     
    }
    if (mem==number){return true;}
    else {return false;}
}

#endif
