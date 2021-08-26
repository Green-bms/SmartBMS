/*
  Cell_mod
  
  Single Lithium Battery Cell module management included in Green BMS project.
  Cell module is based on Attiny 84 controller supplied by the cell and programmed with Arduino UNO (Arduino as isp).
  Cell module functions are:
    - To communicate by I2c with the Master Unit called "Control Unit"
    - To send to Control Unit the values of: cell voltage, cell pole temperature, balancing resistor temperature for battery pack management system
    - To execute the cell balancing (by a mosfet) according to Control Unit command, 
  Each cell module I2C address can be programmed by the user, by push buttons, checking tens and units values by the blinking of two leds
  It is possible to set address from 1 to 99. Address is stored on Attiny EEPROM.
  Cell module sketch includes safety conditions that stop balancing command and reset the program in case of:
    - Internal Whatchdog error (reset after 8 sec.)
    - Loss of I2C Communication with Control Unit (reset after 20 sec.)

  Versions:

  - Ver. 2.s.1.06 date 03/06/2021 created by Sergio Ghirardelli (first release)
  
  
   
*/

#include <TinyWireS.h>
#include <EEPROM.h>
#include "functions_4.h"
#include <avr/wdt.h> 

/*Variable declaration. Meaning of each varable is indicated on instructions comments */
unsigned long t, sample, t0, t1;

unsigned long u_elapsed, u_count, da_elapsed, da_count;
unsigned long both_elapsed, both_count;
unsigned long net_elapsed, net_count;
bool osc, but_u, but_da, u, da, both, both_p, both_mem, prog, prog_p, prog_mem, prog_n, prog_mem2, osc_n, osc_mem;
bool u_n, u_mem, da_n, da_mem;
int u_number, da_number, new_address, u_led_preset, u_led_elapsed, da_led_preset, da_led_elapsed, address, gain_offset;
float val_diff, val_gain;

int temp1_input, temp1_index, temp1_mem[10], temp1_mv, temp1_filtered;
int temp2_input, temp2_index, temp2_mem[10], temp2_mv, temp2_filtered;
long volt, volt_index, volt_mem[10], volt_filtered, vx;
long vbal, vbal_index, vbal_mem[10];

long input, index, val_mem[10], val;
float rbal, rfuse;
unsigned long t_life;
bool check, ardu_test, ardu_bleed, ardu_rst, net_fault, comp_enable, bleed_on;
byte temp1, temp2, tiny_life, ardu_life, ardu_bal, ardu_cmd, life_mem, tiny_bal;

/*balance oscillator variables*/
unsigned long elapsed_time1;
unsigned long count_time1; 
bool time1_q;
unsigned long elapsed_time2;
unsigned long count_time2; 
bool time2_q;
unsigned long elapsed_time3;
unsigned long count_time3; 
bool time3_q;


void(* Riavvia)(void) = 0; //Sketch Reset procedure

void setup() {
  wdt_disable(); //watchdog dog check diseable
  
  /*Set of starting values*/
  sample = 0;
  u = 0;
  da = 0;
  u_number = 0;
  da_number = 0;
  prog = false;
  tiny_bal = 0;
  rbal = 3000.0; //balance resistor in milli-ohm
  rfuse = 48; // fuse resistence causing voltage drop during balancing
  time1_q = false;
  time2_q = false;
  time3_q = false;
  bleed_on = false;

  /*I/O pins configuration  */
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  address = EEPROM.read(0); //module I2C address reading from EEPROM
  EEPROM.get (2,gain_offset);
  new_address = address;
  
  TinyWireS.begin(address); // join i2c network
  TinyWireS.onRequest(requestEvent);
  TinyWireS.onReceive(receiveEvent);
  
  digitalWrite(9, LOW); //leds lights on for 2 seconds to show the sketch reset 
  digitalWrite(10, LOW);
  
  wdt_enable(WDTO_8S); //watchdog enabled and set to 8 seconds
  delay(2000);
}


void loop() {

/*Push buttons managements*/
  but_u = !digitalRead(3); //digital channels reading
  but_da = !digitalRead(7);
  u = TON(but_u, 50, u_elapsed, u_count); //buttons must be pressed for at least 50 msec
  da = TON(but_da, 50, da_elapsed, da_count);
  both = TON ((u and da), 3000, both_elapsed, both_count);//both buttons are pressed for 3 seconds
  both_p = PosTrig(both, both_mem); //positive edge of both variable
  prog_p = PosTrig(prog, prog_mem); //positive edge of prog: when program mode is set
  prog_n = NegTrig(prog, prog_mem2); //negative edge of prog: when program mode is reset
  u_n = NegTrig(u, u_mem); //negative edge of u: when SW1 button is released
  da_n = NegTrig (da, da_mem); //negative edge of da: when SW2 button is released

/*Program mode or Normal mode calling*/
  if (prog) {
    program();
  }

  else {
    normal();
  }

/*I2C and watchdog reset*/
  TinyWireS_stop_check();
  wdt_reset();
}


/*Normal mode of cell module working*/
void normal() {

  if (prog_n) { // executed one time at the end of program mode 
    sample = 20; // voltage and temperatures reading sample time (in msec)
    digitalWrite(9, HIGH); // both leds on to show the end of program mode
    digitalWrite(10, HIGH);
    address = new_address; // I2C address takes the programmed value
    EEPROM.write(0, address); // I2C address is written to Attiny EEPROM
    TinyWireS.begin(address); // join i2c network
    TinyWireS.onRequest(requestEvent);
    TinyWireS.onReceive(receiveEvent);
    delay(3000); //three seconds of freezing before starting the normal mode
  }

  prog = both_p; //in case of pressing of both buttons for 3 seconds, system moves to program mode


  if (millis() - t >= sample) // executed one time every sample time
  {
    /*Attiny supply voltage reading and filtering*/
    input = readVcc();
    val = FILTER_LONG (input, index, val_mem);
 
    
    /*Temperature sensor 1  reading and filtering*/
    temp1_input = analogRead(A1);
    temp1_filtered = FILTER_INT (temp1_input, temp1_index, temp1_mem);

    /*Temperature sensor 1  reading and filtering*/
    temp2_input = analogRead(A2);
    temp2_filtered = FILTER_INT (temp2_input, temp2_index, temp2_mem);
  
    t += sample;

  }

  if (millis() - t1 >= 100) // executed one time every 100 msec
  {
    osc = !osc; //this creates a square wave with a period of 200 msec
    tiny_life++; //increment of sign of life to be sent to Control Unit
    t1 += 100;
  }

 
  ardu_test = bitRead (ardu_cmd, 1); //reading of "test" command status coming from Control Unit
   
  /*Leds alternate blinking if Control Unit sends a "test" command */
  if (osc)
  {
    if (ardu_test) {
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
    }
  }
  else
  {
    if (ardu_test) {
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
    }

  }

 /*stops balancing in case of cell voltage decreases below 3200 mV, by MOS-p channel */
  if (volt > 3200) {digitalWrite(0, LOW);}
     else  {digitalWrite(0, HIGH);}
    

  check = Life (ardu_life, 1000, life_mem, t_life); //check of sign of life value coming from Control Unit
  net_fault = TON (check, 20000, net_elapsed, net_count); //this variable is set after 20 seconds of communication loss 
  ardu_rst = bitRead (ardu_cmd, 2); //reading of "reset" command status coming from Control Unit

  /*in case of communication loss OR reset command from Control Unit, Attiny is Reset*/
  if ((net_fault)||(ardu_rst)) {
    Riavvia();
  }

  
  if (!ardu_test) { // leds management in normal condition without "test" command from Control Unit

    /*D1 led blinks every 1000msec (check function time) if communication with Control Unit is ok*/
    if (check) {
      digitalWrite(9, HIGH);
    }
    else {
      digitalWrite(9, LOW);
    }


    /*D2 led blinks every 600msec if Sketch is executed*/
    if (millis() - t0 >= 600)
    {
      digitalWrite(10, HIGH);
      t0 += 600;
    }
    else {
      digitalWrite(10, LOW);
    }
  }


   /*Voltage value adjustment by push buttons.*/
  
  comp_enable = bitRead (ardu_cmd, 3);
  
  if (comp_enable && u_n) {
    gain_offset++;
    EEPROM.put (2,gain_offset);
  }

  if (comp_enable && da_n) {
    gain_offset--;
    EEPROM.put (2,gain_offset);
  }

  /*Voltage value calculation */
  float cell_val = float (val);
  float cell_gain = (1000 - ((cell_val-3000)*0.0067)) + gain_offset;
  float cell_calc = (cell_val * cell_gain) / 1000;
  vx = long (cell_calc);

/*balancing oscillator*/
  time1_q = TON (!time2_q, 5000, elapsed_time1, count_time1);
  time2_q = TON (time1_q, 50, elapsed_time2, count_time2);

  ardu_bleed = bitRead (ardu_cmd, 0); //reading of "bleeding" command status coming from Control Unit
  
  time3_q = TON (!bleed_on, 25, elapsed_time3, count_time3);
  
  if (ardu_bleed && !time1_q){
    tiny_bal = ardu_bal;
    bleed_on = true;
    } //in case of "bleeding" command, set balancing value according to parameter coming from Control Unit
  else {
          tiny_bal = 0; //set balancing value to zero if no command of bleeding arrives
          bleed_on = false;
          if (time3_q) {
          volt = vx; //update voltage value for LCU, only when balancing is off for at least 25 msec
          }
    } 
  analogWrite(8, tiny_bal); //write PWM balancing value to Mosfet 

    
 
  temp1_mv = map(temp1_filtered, 0, 1023, 0, volt);//temperature mV signal calculation according to ADC supply voltage
  temp1 = map (temp1_mv, 100, 1750, 0, 255); //temperature converted in a byte value

  temp2_mv = map(temp2_filtered, 0, 1023, 0, volt);
  temp2 = map (temp2_mv, 100, 1750, 0, 255);

}



/*Program Mode to set the cell module I2C address*/
void program() {

  if (prog_p) {// executed one time at the start of program mode 
    sample = 300;// sample time of leds blinking management (in msec)
   
    /*decrement of tens and units numbers*/
    u_number--;
    da_number--;
    u_led_elapsed = 0;
    da_led_elapsed = 0;
    digitalWrite(9, HIGH); // both leds on to show the end of program mode
    digitalWrite(10, HIGH);
    delay(3000); //three seconds of freezing before starting the program mode
  }
  prog = !both_p; //in case of pressing of both buttons for 3 seconds, system returnes to normal mode
  
  if (u_n) { //increment of units value after SW1 button release
    u_number++;
  }
  if (da_n) { //increment of tens value after SW2 button release
    da_number++;
  }
  if (u_number > 9) { //restart of units value
    u_number = 0;
  }
  if (da_number > 9) { //restart of tens value
    da_number = 0;
  }
  new_address = (da_number * 10) + u_number; //new address number calculation
  
  /*preset values of leds blink mangement */
  u_led_preset = u_number + 2;
  da_led_preset = da_number + 2;
  osc_n = NegTrig(osc, osc_mem);

  if (millis() - t >= sample) //oscillator for blinking
  {
    osc = !osc;
    t += sample;

  }



  if (osc)
  {
    if ((u_led_elapsed >= 1) && (u_led_elapsed <= u_number)) { //blinking of Units led to show the set value
      digitalWrite(10, HIGH);
    }
    else  {
      digitalWrite(10, LOW);
    }

    if ((da_led_elapsed >= 1) && (da_led_elapsed <= da_number)) { //blinking of Tens led to show the set value
      digitalWrite(9, HIGH);
    }
    else  {
      digitalWrite(9, LOW);
    }

  }
  else
  {
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }

  if (osc_n) //led blinking counters increment or reset every negative edge of the oscillator
  {
    u_led_elapsed++;
    if (u_led_elapsed > u_led_preset) {
      u_led_elapsed = 0;
    }
    da_led_elapsed++;
    if (da_led_elapsed > da_led_preset) {
      da_led_elapsed = 0;
    }
  }

}

/*I2C bytes transmission according to Control Unit request: tot.5 bytes*/
void requestEvent() {
  byte valLow = volt & 0xff;
  byte valHigh = (volt >> 8);
  TinyWireS.send(valHigh);
  TinyWireS.send(valLow);
  TinyWireS.send(temp1);
  TinyWireS.send(temp2);
  TinyWireS.send(tiny_life);
}

/*I2C bytes reception according to Control Unit transmission: tot.3 bytes*/
void receiveEvent() {
  ardu_bal = TinyWireS.receive();
  ardu_life = TinyWireS.receive();
  ardu_cmd = TinyWireS.receive();
}
