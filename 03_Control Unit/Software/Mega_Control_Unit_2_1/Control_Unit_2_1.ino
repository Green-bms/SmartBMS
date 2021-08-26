
/*
  Control_Unit
  
  Master Unit of Green BMS project.
  Control Unit is based on Arduino Mega.
  Control Unit functions are:
    - To communicate by I2c with all the cell modules of the battery pack
    - To receive from cell modules the values of: cell voltage, cell pole temperature, balancing resistor temperature for battery pack management system
    - To send to cell modules the balancing command
    - To perform the following safety functions by relays:
              - Charge ok: open two relays contact when just one cell of the pack reach the high value or I2C communication with Cell modules is loss
              - Discharge ok: open a relay contact when just one cell of the pack goes down the minimum value or I2C communication with Cell modules is loss
              - Alarm led: close a contact to supply a led in case of alarm
              - Balancing: close a contact when just one cell of the pack is balancing: this information can be used to reduce the charging current 
              - System OK: close a contact when Control Unit is working.
   - To communicate with an Android device by a Bluetooth module. Android device is the human machine interface from wich is possible to check the BMS status and set parameters.
    
 
 Versions:

  - Ver. 3.s.2.01 date 26/08/2021 created by Sergio Ghirardelli (first release)
   
*/

#include <Wire.h>
#include "functions_4.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h> 

struct cell_str {
  
  /*bytes coming from cell modules*/
  byte valHigh;
  byte valLow;
  byte temp1;
  byte temp2;
  byte tinyLife;
 
  /*bytes sent to cell modules*/
  byte bleed_cmd;
  byte arduLife;
  byte cmd;
 
  /*voltage calculation variables*/
  int millivolt;  
  float volt;
  

  /*variables used to "veryhigh" value check */
  unsigned long elapsed_veryhigh;
  unsigned long count_veryhigh;
  bool mem_veryhigh;
  bool veryhigh;
  
  /*variables used to "high" value check */
  unsigned long elapsed_high;
  unsigned long count_high;
  bool mem_high;
  bool high;

  /*variables used to "bal" value check */
  unsigned long elapsed_bal;
  unsigned long count_bal;
  bool mem_bal;
  bool bal;

  /*variables used to "low" value check */
  unsigned long elapsed_low;
  unsigned long count_low; 
  bool mem_low;
  bool low;

  /*variables used to "err" value check */
  bool in_err;
  unsigned long elapsed_err;
  unsigned long count_err;
  bool err;
  unsigned long high_err_elapsed;
  unsigned long high_err_count;
  bool high_err;
  bool high_err_mem;

  bool normal; //the cell has a value in the normal range

  /*variables used to check communication with cell module*/
  bool check;
  bool net_fault;
  byte life_mem;
  unsigned long t_life;
  unsigned long net_elapsed;
  unsigned long net_count;

  /*variables used for cell pole temperature management*/
  int pole_temp;
  unsigned long elapsed_pole_t;
  unsigned long count_pole_t;
  bool mem_pole_t;
  bool high_pole_temp;

  /*variables used for balance resistors temperature management*/
  int bal_temp;
  unsigned long elapsed_bal_t;
  unsigned long count_bal_t;
  bool mem_bal_t;
  bool high_bal_temp;
  bool bal_enable;

  /*test bit to cell module */
  bool ardu_test;
  
};

struct cell_str cell[41];

/*variables used for sampling each cell module data exchange  */
int n; //number of cells
int i; //address incremented every cycle

/*variables used to find the highest and the lowest cells voltage*/
int cell_max_mem;
int cell_min_mem;
int cell_max;
int cell_min;

/*variables used to calculate the total battery pack voltage */
float pack_voltage;
float pack_voltage_mem;

/*variable used to calculate if just one cell is in error status*/
bool just_err; 
bool just_err_mem;

/*variable used to calculate if just one cell is in high status*/
bool just_high; 
bool just_high_mem;

/*variable used to calculate if just one cell is in very high status*/
bool just_veryhigh; 
bool just_veryhigh_mem;

/*variable used to calculate if just one cell is in bal status*/
bool just_bal; 
bool just_bal_mem;

/*variable used to calculate if just one cell is in low status*/
bool just_low; 
bool just_low_mem;

/*variable used to calculate if just one cell pole temperature is in high status*/
bool just_pole; 
bool just_pole_mem;

/*variable used to calculate if just one cell balancing resistor is in high status*/
bool just_bal_t; 
bool just_bal_t_mem;

/*variable used to calculate if just one cell is balancing*/
bool just_bleed;
bool just_bleed_mem;

/*variable used to calculate if all the cells reach the bal status*/
bool all_bal; 
bool all_bal_mem;

/*variable used for current sensor*/
int cur_filter_mem[10];
int cur_filter_index;
long current_mA;
float current_A;
long cur_after_offset;
bool standby;
bool charge_fdbk;
bool discharge_fdbk;
float load;
int current_filtered;
int current_channel;
long cur_sign;
long current_map;
bool current_error;

/*variable used for SOC calculation*/
float soc_mAh; 
float soc_mAh_max;
unsigned long h;
int soc_perc;
int soc_perc_old;

/*variable used for oscillators*/
unsigned long t_500;
bool osc_500;
unsigned long t_800;
bool osc_800;
unsigned long t_2000;
bool osc_2000;
unsigned long t_10000;
bool osc_10000;
unsigned long t_30000;
bool osc_30000;

/*variables for store index*/
unsigned long t_index;
int status_index;
float soc_mah_stored;
int soc_mem_address;
bool pack_ch_stored;
int ch_mem_address;
bool pack_bal_stored;
int bal_mem_address;

/*parameters*/
int first_code; // code written in case of first use
int password; // password to connect by bluetooth app

unsigned long set_status_time; //  Delay time for each status settings

int set_veryhigh; //  Very High Voltage threshold (set): alarm
int set_veryhigh_hys; //  Very High Voltage Reset (when voltage decreases below this value)
int set_high; //  High Voltage threshold : not alarm but stop charging
int set_high_hys; // High Voltage Reset (when voltage decreases below this value)
int set_bal; // Balance Voltage threshold (set) : start of balancing. Battery pack is considered charged when all the cells reach this value
int set_bal_hys; // Balance Voltage Reset (when voltage decreases below this value)
int set_low; // Low Voltage threshold (set): alarm and stop discharging
int set_low_hys; // Low Voltage Reset (when voltage increases above this value)

unsigned long set_timeout; // Cell communication timeout error

int set_high_pole_t; //  High cell pole temperature threshold (set): alarm and stop charging or discharging
int set_high_pole_t_hys; //  High cell pole temperature Reset (when temperature decreases below this value)
int set_high_bal_t; //  High balance resistor temperature threshold (set): alarm and stop charging
int set_high_bal_t_hys; // High balance resistor Reset (when temperature decreases below this value)
int set_bal_diff_on; // System starts to balance the cells in "bal" status when the difference between the highest and lowest cell is above this value
int set_bal_diff_off; // System stops to balance the cells in "bal" status when the difference between the highest and lowest cell decreases below this value

int set_bal_percentage; // Balancing current in percentage of max_current
int set_volt_charge_offset; // offset between cell voltage during max charging and when limiter is on;

int set_current_gain; // Current signal gain
int set_current_offset; // Current signal offset
int set_current_max; // Max value of current signal corresponding to 5V
int set_current_min; // Min value of current signal corresponding to 0V

int bal_calculated; // balance threshold after offset calculation

unsigned int set_bat_cap; // battery capacity in Ah

/*HMI Bluetooth communication variables*/
SoftwareSerial SSerial(10, 11);
String line;

int hmi_page; // page selected on HMI : 0=not connected  1=connection page  2=Overview page  3=Cells page  4=Settings page   
int x1,x2, x3, x4, x5; 
int z2, z3, z4, z5; 
bool data_sent; 
bool data_valid; 
bool all_data; 
bool overview_data; 
int param_set;  
int ch_status; 
float cell_max_volt;
float cell_min_volt;
int HMI_sign;
int cell_to_be_tested;
bool cell_test;
bool cell_test_time;
unsigned long cell_test_elapsed;
unsigned long cell_test_count;
int cell_to_be_reset;
bool cell_reset;
bool mem_reset;
unsigned long password_reset_elapsed;
unsigned long password_reset_count;
bool password_reset_q;
bool password_reset_mem;
bool password_reset_n;
int HMI_life;
int HMI_life_mem;
bool charge_on;
bool limiter_on;
bool pack_charged;
bool pack_ch_p;
bool pack_ch_p_mem;
bool pack_ch_n;
bool pack_ch_n_mem;
bool pack_balanced;
bool pack_bal_p;
bool pack_bal_p_mem;
bool pack_bal_n;
bool pack_bal_n_mem;

bool mem_reset_q;
bool mem_reset_mem;
bool mem_reset_n;
unsigned long mem_reset_elapsed;
unsigned long mem_reset_count;


/*variables used to test some functions*/
String trappola;
String caio;
int trucco;
bool tapullo;
unsigned long cycle_time_stop;
unsigned long cycle_time_start;



void setup() {

wdt_disable();

Wire.begin(); // join i2c bus 
Serial.begin(9600); // start serial for output
Serial.println( "Setup Success" );

SSerial.begin(9600);

line = "";

pinMode(2, OUTPUT); // charge enable relay
pinMode(3, OUTPUT); // charge enable relay (redundant)
pinMode(4, OUTPUT); // balance relay: closed if just one cell is balancing
pinMode(5, OUTPUT); // discharge enable relay
pinMode(6, OUTPUT); // alarm lamp
pinMode(7, OUTPUT); // spare
pinMode(9, OUTPUT); // heartbit

pinMode(12, INPUT_PULLUP); // mem_reset

digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(7, LOW);
digitalWrite(9, LOW);

EEPROM.get( 0,first_code );

if (first_code==28411){ eeprom_read();  } // parameters reading
else { eeprom_init();  } // factory parameters reading
 
/* cells vaiables inizialization */
i=1;
cell_max_mem=0;
cell_min_mem=5000;
cell_min=5000;
pack_voltage_mem=0;
just_err_mem=false;
just_high_mem=false;
just_low_mem=false;
just_pole_mem=false;
all_bal_mem=true;
just_bal_mem=false;
just_veryhigh_mem=false;
just_bal_t_mem=false;
just_bleed_mem=false;

/* pack status inizialization */
soc_mAh = soc_mah_stored;
pack_charged = pack_ch_stored;
pack_balanced = pack_bal_stored;      
       
/* HMI communication inizialization */
hmi_page = 0;
x1 = 1;
x2 = 1;
z2 = 1;
x3 = 1;
z3 = 1;
x4 = 1;
z4 = 1;
x5 = 1;
z5 = 1;
data_sent = false;
data_valid = false;
all_data = false;
HMI_life = 0;
HMI_life_mem = 0;

wdt_enable(WDTO_1S); //watchdog enabled and set to 1 seconds

}



void loop() {
        cycle_time_start = micros();
          
        /*Bytes reading from cell modules*/
        Wire.requestFrom(i, 5);
        while(Wire.available() > 0) {
        cell[i].valHigh = Wire.read();
        cell[i].valLow = Wire.read();
        cell[i].temp1 = Wire.read();
        cell[i].temp2 = Wire.read();
        cell[i].tinyLife = Wire.read();
        }

        /*Cells voltage calculation*/
        cell[i].millivolt = cell[i].valHigh << 8 | cell[i].valLow; 

   
        /*Cell very high status check: this condition generates an alarm*/
        cell[i].veryhigh = THR_UP(cell[i].millivolt, set_veryhigh, set_veryhigh_hys, set_status_time, cell[i].elapsed_veryhigh, cell[i].count_veryhigh, cell[i].mem_veryhigh);

        /*Cell high status check: this condition causes the stop of charging*/
        cell[i].high = THR_UP(cell[i].millivolt, set_high, set_high_hys, set_status_time, cell[i].elapsed_high, cell[i].count_high, cell[i].mem_high);

        /*Cell bal status check: this condition causes the cell balancing*/
        cell[i].bal = THR_UP(cell[i].millivolt, bal_calculated, set_bal_hys, set_status_time, cell[i].elapsed_bal, cell[i].count_bal, cell[i].mem_bal);

        /*Cell low status check: this condition causes the cell balancing*/
        cell[i].low =THR_DOWN(cell[i].millivolt, set_low, set_low_hys, set_status_time, cell[i].elapsed_low, cell[i].count_low, cell[i].mem_low);

        /*Cell module communication check*/
        cell[i].check = Life (cell[i].tinyLife, 1000, cell[i].life_mem, cell[i].t_life); 
        cell[i].net_fault = TON (cell[i].check, set_timeout, cell[i].net_elapsed, cell[i].net_count); 

        /*Cell err status check: this condition causes the relevant alarm and the stop of the charging*/
        if (cell[i].net_fault || (cell[i].millivolt>4500) || (cell[i].millivolt<2300)) {cell[i].err=true;}
        else {cell[i].err=false;}

        /*Cell normal status check*/
        if (!cell[i].veryhigh && !cell[i].high && !cell[i].bal && !cell[i].low && !cell[i].err ) {cell[i].normal=true;}
        else {cell[i].normal=false;}

        /*Cell pole temperature management*/
        cell[i].pole_temp = map (cell[i].temp2,0,255,-40,125);
        cell[i].high_pole_temp = THR_UP(cell[i].pole_temp, set_high_pole_t, set_high_pole_t_hys, set_status_time, cell[i].elapsed_pole_t, cell[i].count_pole_t, cell[i].mem_pole_t);

        /*Balance resistor temperature management*/
        cell[i].bal_temp = map (cell[i].temp1,0,255,-40,125);
        cell[i].high_bal_temp = THR_UP(cell[i].bal_temp, set_high_bal_t, set_high_bal_t_hys, set_status_time, cell[i].elapsed_bal_t, cell[i].count_bal_t, cell[i].mem_bal_t);

        /*highest voltage and lowest voltage calculation*/
        cell_max_mem = max (cell_max_mem,cell[i].millivolt);
        cell_min_mem = min (cell_min_mem,cell[i].millivolt);

        /*battery pack total voltage calculation*/
        cell[i].volt = float (cell[i].millivolt) / 1000;
        pack_voltage_mem = pack_voltage_mem + cell[i].volt;
          
        /*Control Unit sign of life for cell module*/
        cell[i].arduLife++;

        /*Just one cell in err status calculation*/
        just_err_mem = just_err_mem || cell[i].err;
        
        /*Just one cell in high status calculation*/
        just_high_mem = just_high_mem || cell[i].high;

        /*Just one cell in very high status calculation*/
        just_veryhigh_mem = just_veryhigh_mem || cell[i].veryhigh;

        /*Just one cell in bal status calculation*/
        just_bal_mem = just_bal_mem || cell[i].bal;
        
        /*Just one cell in low status calculation*/
        just_low_mem = just_low_mem || cell[i].low;

        /*Just one cell pole temperature in high status calculation*/
        just_pole_mem = just_pole_mem || cell[i].high_pole_temp;

        /*Just one cell balance resistor temperature in high status calculation*/
        just_bal_t_mem = just_bal_t_mem || cell[i].high_bal_temp;
               
        /*All cells in bal status calculation*/
        all_bal_mem = all_bal_mem && cell[i].bal;

        /* Balance enable calculation */
        if (((cell[i].millivolt - cell_min) > set_bal_diff_on) && !just_err && !cell[i].high_bal_temp)   {cell[i].bal_enable=true;}
        if ((just_err) || cell[i].high_bal_temp || ((cell[i].bal_enable) && ((cell[i].millivolt - cell_min) < set_bal_diff_off)))  {cell[i].bal_enable=false;}

        /*Cell balance command*/
        if ((cell[i].bal_enable && cell[i].bal && !pack_balanced) || cell[i].ardu_test) {
          cell[i].bleed_cmd = map (set_bal_percentage, 0, 100, 0, 255);
          bitSet(cell[i].cmd, 0);
          }
        else {
          cell[i].bleed_cmd = 0;
          bitClear(cell[i].cmd, 0);
          }
          
         /*Just one cell is balancing*/
        bool cell_bleed = bitRead(cell[i].cmd, 0);
        just_bleed_mem = just_bleed_mem || cell_bleed ;
       
          
        /*Cell test command*/
        if (cell[i].ardu_test) {bitSet(cell[i].cmd, 1);}
        else {bitClear(cell[i].cmd, 1);}


        /*Cell reset command*/
        if (!cell_reset) {cell[i].high_err = PosTrig (( TON (cell[i].err, 3000, cell[i].high_err_elapsed, cell[i].high_err_count)), cell[i].high_err_mem);}
        
        if (cell[i].high_err) {
          bitSet(cell[i].cmd, 2);
          cell_reset = false;
          }
        else {bitClear(cell[i].cmd, 2);}

        /*Cell voltage compensation setting enable*/
        if (hmi_page == 3) { bitSet(cell[i].cmd, 3); }
        else {bitClear(cell[i].cmd, 3);}
                  
          
        /*Send data to cell module*/
        Wire.beginTransmission(i);
        Wire.write(cell[i].bleed_cmd);
        Wire.write(cell[i].arduLife);
        Wire.write(cell[i].cmd);
        Wire.endTransmission(); 

        
        
        if (i >= n){
          cell_max = cell_max_mem;
          cell_min = cell_min_mem;
          cell_max_mem = 0;
          cell_min_mem = 5000;
          pack_voltage = pack_voltage_mem;
          pack_voltage_mem = 0;
          just_err = just_err_mem;
          just_err_mem = false;
          just_high = just_high_mem;
          just_high_mem = false;
          just_veryhigh = just_veryhigh_mem;
          just_veryhigh_mem = false;
          just_bal = just_bal_mem;
          just_bal_mem = false;
          just_low = just_low_mem;
          just_low_mem = false;
          just_pole = just_pole_mem;
          just_pole_mem = false;
          just_bal_t = just_bal_t_mem;
          just_bal_t_mem = false;
          all_bal = all_bal_mem;
          all_bal_mem = true;
          just_bleed = just_bleed_mem;
          just_bleed_mem = false;
          i=1;
          }
            
        else {i++;}     
       

        cell_max_volt = float (cell_max) / 1000;
        cell_min_volt = float (cell_min) / 1000;
        
        /* Current sensor input */
        current_channel=analogRead(A0);
        current_filtered = FILTER_INT (current_channel, cur_filter_index, cur_filter_mem);

        long cur_min_mA = (long (abs(set_current_min))) * 1000;
        long cur_max_mA = (long (set_current_max)) * 1000;
        cur_sign = current_filtered - 512;

        if (cur_sign >= 0) {current_map = (cur_max_mA / 512) * cur_sign;}
        if (cur_sign < 0) {current_map = (cur_min_mA / 512) * cur_sign;}
        long cur_after_gain = current_map * set_current_gain / 1000;
        cur_after_offset = cur_after_gain + set_current_offset;
        current_mA =((cur_after_offset+250)-((cur_after_offset+250)%500));
        float current_adj = float (current_mA);
        current_A = current_adj / 1000.00;

   

        if ((cur_after_offset<3000)&&(cur_after_offset>-5000)){
          current_mA=0;
          standby=true;
          charge_fdbk=false;
          discharge_fdbk=false;  
          ch_status=0;
          }
        else if (cur_after_offset>=3000){
          standby=false;
          charge_fdbk=true;
          discharge_fdbk=false;
          ch_status=1;
          }
        else if (cur_after_offset<=-5000){
          standby=false;
          charge_fdbk=false;
          discharge_fdbk=true;
          ch_status=2;
          }   

       if ((cur_after_offset<=-140000) || (cur_after_offset>=140000)) {
          current_error = true;
          }  
       else  {current_error = false;}

        /* soc calculation */

        if (millis() - h >= 100) 
        {
            soc_mAh_max = float (set_bat_cap) * 1000.00;
            if (((ch_status==2)||(ch_status==1)) && !current_error) {
              soc_mAh = soc_mAh + (float (cur_after_offset) / 36000.00);
              soc_mAh= constrain(soc_mAh, 0, soc_mAh_max);
            }
            
        h += 100;
        }
                
        soc_perc = int (soc_mAh*100.00/soc_mAh_max);
        if (soc_perc != soc_perc_old){
          soc_mah_stored = soc_mAh;
          EEPROM.put( soc_mem_address,soc_mah_stored );
          }
        soc_perc_old = soc_perc;

        
        if (all_bal) {
          soc_mAh = soc_mAh_max;
          soc_perc = 100;
          } 

        /* Charge enable relay - HIGH = enabled */

        if ((just_err) || (just_high) || (just_veryhigh) || (pack_charged) || (just_pole)) {
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          charge_on = false;
          }
        else {
          digitalWrite(2, HIGH);
          digitalWrite(3, HIGH);
          charge_on = true;
          }
        
        /* Discharge enable relay - HIGH = enabled */

        if ((just_err) || (just_low) || (just_pole)) {digitalWrite(5, LOW);}
        else {digitalWrite(5, HIGH);}
        
        /* Balance status relay - HIGH = enabled */

        if ((just_bleed) && (charge_on)) {
          digitalWrite(4, HIGH);
          limiter_on = true;
          }
        else {
          digitalWrite(4, LOW);
          limiter_on = false;
          }

        /*Pack charged status*/
        
        if (all_bal) {pack_charged = true;} 
        if ((pack_charged) && (cell_min < set_bal_hys )) {pack_charged = false;}
        pack_ch_p = PosTrig (pack_charged, pack_ch_p_mem);
        pack_ch_n = NegTrig (pack_charged, pack_ch_n_mem); 
        if (pack_ch_p || pack_ch_n){
            pack_ch_stored = pack_charged;
            EEPROM.put( ch_mem_address,pack_ch_stored );
            }

    
         /*Pack balanced status*/
        
        if ((pack_charged) && (all_bal) && (!just_bleed)) {pack_balanced = true;} 
        if (!pack_charged) {pack_balanced = false;}
        pack_bal_p = PosTrig (pack_balanced, pack_bal_p_mem);
        pack_bal_n = NegTrig (pack_balanced, pack_bal_n_mem); 
        if (pack_bal_p || pack_bal_n){
            pack_bal_stored = pack_balanced;
            EEPROM.put( bal_mem_address,pack_bal_stored );
            }


        /*Balance parameter according to max charging or limiting*/
        
        if ((charge_on) && (!limiter_on)) { bal_calculated = set_bal + set_volt_charge_offset;}
        else { bal_calculated = set_bal;}
    
                
        /* Alarm lamp relay */
        
        if (((just_err) || (just_low) || (just_pole) || (just_bal_t) || (just_veryhigh)) && osc_2000) {digitalWrite(6, HIGH);}
        else {digitalWrite(6, LOW);}

        

        /* Heartbit */

        if (!mem_reset) {        
              if (osc_500) {digitalWrite(9, LOW);}
              else {digitalWrite(9, HIGH);}
              }
        else if (((mem_reset_elapsed>5000) && (mem_reset_elapsed<5500)) || ((mem_reset_elapsed>9500) && (mem_reset_elapsed<10000)))  {digitalWrite(9, HIGH);}
        else {digitalWrite(9, LOW);}
        
        /* Cell test from HMI */
        
        cell_test_time = TON (cell_test, 5000, cell_test_elapsed, cell_test_count);

        if (cell_test_time) {cell_test = false;}
        
        if (cell_test) {cell[cell_to_be_tested].ardu_test = true;}  
        else {cell[cell_to_be_tested].ardu_test = false;}
                                                   

        /* Cell reset from HMI */

        if (cell_reset) {
          
          cell[cell_to_be_reset].high_err = true;
                 
          }  
        

        /* Password and memory reset */

        mem_reset = !digitalRead (12);

        password_reset_q = TON (mem_reset, 5000, password_reset_elapsed, password_reset_count);
        password_reset_n = NegTrig (password_reset_q, password_reset_mem);

        mem_reset_q = TON (mem_reset, 10000, mem_reset_elapsed, mem_reset_count);
        mem_reset_n = NegTrig (mem_reset_q, mem_reset_mem);

        if (password_reset_n) {
            password = 1234;
            EEPROM.put( 2,password );
            }
        if (mem_reset_n) { eeprom_init(); }


      
        
  
        /*Oscillator period: 500 ms*/
        
        if (millis() - t_500 >= 250) 
        {
         
          osc_500 = !osc_500;
          t_500 += 250;
        }

        /*Oscillator period: 800 ms*/

        if (millis() - t_800 >= 400) 
        {
  

          /*Data sending to HMI */
          if (hmi_page == 1)  {

                if (x1 <= 3) {
                  SSerial.println("$#p002" + (String (password)) + "$");
                  x1++;
                  }
                               
                 }        


          if (hmi_page == 2) {
                if (overview_data) { SSerial.println("$#p012" + (String (set_bat_cap)) + "$#p014" + (String (n)) +  "$"); }
                
                if (x2 >= 3) {
                            x2 = 1;
                            SSerial.println("$#m000$");
                            }
                            else { x2++; } 

                 if (!overview_data) {
                            if  ((current_A>-10) && (current_A<10)) { SSerial.println ( "$#g001" + (String (pack_voltage,1)) + "$#g002" + (String (current_A,2)) + "$#g003" +  (String (soc_perc)) + "$#g004" + (String (cell_max_volt)) + "$#g005" + (String (cell_min_volt)) + "$#g006" + (String (ch_status)) + "$");}
                            else  { SSerial.println ( "$#g001" + (String (pack_voltage,1)) + "$#g002" + (String (current_A,0)) + "$#g003" +  (String (soc_perc)) + "$#g004" + (String (cell_max_volt)) + "$#g005" + (String (cell_min_volt)) + "$#g006" + (String (ch_status)) + "$");}
                            }
                 } 


          if (hmi_page == 3)  {

                  int r = n % 2;
                  int q = (n / 2) + r;
                  int j = x3 * 2;
                  int k = j-1;
                  
                  if (x3<5) { SSerial.println ("$#v00" + (String (k)) + (String (cell[k].volt)) + "$#t00" + (String (k)) + (String (cell[k].pole_temp))  + "$#u00" + (String (k)) + (String (cell[k].bal_temp)) + "$#z00" + (String (k)) + (String (cell[k].veryhigh)) + (String (bitRead(cell[k].cmd, 0))) + (String (cell[k].low)) + (String (cell[k].err)) + (String (cell[k].high_pole_temp)) + (String (cell[k].high_bal_temp)) + "$#v00" + (String (j)) + (String (cell[j].volt)) + "$#t00" + (String (j)) + (String (cell[j].pole_temp))  + "$#u00" + (String (j)) + (String (cell[j].bal_temp)) + "$#z00" + (String (j)) + (String (cell[j].veryhigh)) + (String (bitRead(cell[j].cmd, 0))) + (String (cell[j].low)) + (String (cell[j].err)) + (String (cell[j].high_pole_temp)) + (String (cell[j].high_bal_temp)) + "$"); }                                              
                  if (x3==5) { SSerial.println ("$#v00" + (String (k)) + (String (cell[k].volt)) + "$#t00" + (String (k)) + (String (cell[k].pole_temp))  + "$#u00" + (String (k)) + (String (cell[k].bal_temp)) + "$#z00" + (String (k)) + (String (cell[k].veryhigh)) + (String (bitRead(cell[k].cmd, 0))) + (String (cell[k].low)) + (String (cell[k].err)) + (String (cell[k].high_pole_temp)) + (String (cell[k].high_bal_temp)) + "$#v0" + (String (j)) + (String (cell[j].volt)) + "$#t0" + (String (j)) + (String (cell[j].pole_temp))  + "$#u0" + (String (j)) + (String (cell[j].bal_temp)) + "$#z0" + (String (j)) + (String (cell[j].veryhigh)) + (String (bitRead(cell[j].cmd, 0))) + (String (cell[j].low)) + (String (cell[j].err)) + (String (cell[j].high_pole_temp)) + (String (cell[j].high_bal_temp)) + "$");  }                                             
                  if (x3>5) { SSerial.println ("$#v0" + (String (k)) + (String (cell[k].volt)) + "$#t0" + (String (k)) + (String (cell[k].pole_temp))  + "$#u0" + (String (k)) + (String (cell[k].bal_temp)) + "$#z0" + (String (k)) + (String (cell[k].veryhigh)) + (String (bitRead(cell[k].cmd, 0))) + (String (cell[k].low)) + (String (cell[k].err)) + (String (cell[k].high_pole_temp)) + (String (cell[k].high_bal_temp)) + "$#v0" + (String (j)) + (String (cell[j].volt)) + "$#t0" + (String (j)) + (String (cell[j].pole_temp))  + "$#u0" + (String (j)) + (String (cell[j].bal_temp)) + "$#z0" + (String (j)) + (String (cell[j].veryhigh)) + (String (bitRead(cell[j].cmd, 0))) + (String (cell[j].low)) + (String (cell[j].err)) + (String (cell[j].high_pole_temp)) + (String (cell[j].high_bal_temp)) + "$"); }                                              
                     
                  if (x3 >= q) { x3 = 1; }
                  else { x3++; }          
                                    
                                             
                 }   


          if (hmi_page == 4) {
                 if ((all_data) && (z4 <= 2)) { 
                        if (x4==1) { SSerial.println("$#p012" + (String (set_bat_cap)) + "$#p014" + (String (n)) + "$#p024" + (String (set_veryhigh)) + "$#p026" + (String (set_veryhigh_hys)) + "$#p028" + (String (set_high))+ "$#p030" + (String (set_high_hys)) +  "$"); }
                        if (x4==2) { SSerial.println("$#p060" + (String (set_bal_percentage)) + "$#p070" + (String (set_current_max))+ "$#p072" + (String (set_current_min)) + "$#p074" + (String (set_current_gain)) + "$#p076" + (String (set_current_offset)) + "$#p002" + (String (password)) + "$"); }
                        if (x4==3) { SSerial.println("$#p034" + (String (set_bal_hys)) + "$#p036" + (String (set_low)) + "$#p038" + (String (set_low_hys)) + "$#p040" + (String (set_status_time)) + "$#p044" + (String (set_timeout))+ "$#p032" + (String (set_bal)) + "$"); }   
                        if (x4==4) { SSerial.println("$#p048" + (String (set_high_pole_t)) + "$#p050" + (String (set_high_pole_t_hys)) + "$#p052" + (String (set_high_bal_t)) + "$#p054" + (String (set_high_bal_t_hys)) + "$#p056" + (String (set_bal_diff_on)) + "$#p058" + (String (set_bal_diff_off)) + "$#p062" + (String (set_volt_charge_offset)) + "$"); }   
                           
                                                                          
                        if (x4 >= 4) {
                            x4 = 1;
                            z4++;
                            }
                            else { x4++; }  
                        }
                  else if ((all_data) && (z4 > 2)) {
                        SSerial.println("$#f000$");
                        x4 = 1;
                        z4 = 1;
                        }
                  if (data_sent) {

                         if (param_set == 2) { SSerial.println("$#p002" + (String (password)) + "$");}
                         if (param_set == 12) { SSerial.println("$#p012" + (String (set_bat_cap)) + "$");}
                         if (param_set == 14) { SSerial.println("$#p014" + (String (n)) + "$");}
                         if (param_set == 24) { SSerial.println("$#p024" + (String (set_veryhigh)) + "$");}
                         if (param_set == 26) { SSerial.println("$#p026" + (String (set_veryhigh_hys)) + "$");}
                         if (param_set == 28) { SSerial.println("$#p028" + (String (set_high)) + "$");}
                         if (param_set == 30) { SSerial.println("$#p030" + (String (set_high_hys)) + "$");}  
                         if (param_set == 32) { SSerial.println("$#p032" + (String (set_bal)) + "$");}
                         if (param_set == 34) { SSerial.println("$#p034" + (String (set_bal_hys)) + "$");} 
                         if (param_set == 36) { SSerial.println("$#p036" + (String (set_low)) + "$");}
                         if (param_set == 38) { SSerial.println("$#p038" + (String (set_low_hys)) + "$");}
                         if (param_set == 40) { SSerial.println("$#p040" + (String (set_status_time)) + "$");}
                         if (param_set == 44) { SSerial.println("$#p044" + (String (set_timeout)) + "$");}
                         if (param_set == 48) { SSerial.println("$#p048" + (String (set_high_pole_t)) + "$");}
                         if (param_set == 50) { SSerial.println("$#p050" + (String (set_high_pole_t_hys)) + "$");}
                         if (param_set == 52) { SSerial.println("$#p052" + (String (set_high_bal_t)) + "$");}
                         if (param_set == 54) { SSerial.println("$#p054" + (String (set_high_bal_t_hys)) + "$");}
                         if (param_set == 56) { SSerial.println("$#p056" + (String (set_bal_diff_on)) + "$");}
                         if (param_set == 58) { SSerial.println("$#p058" + (String (set_bal_diff_off)) + "$");}
                         if (param_set == 60) { SSerial.println("$#p060" + (String (set_bal_percentage)) + "$");}
                         if (param_set == 62) { SSerial.println("$#p062" + (String (set_volt_charge_offset)) + "$");}
                         if (param_set == 70) { SSerial.println("$#p070" + (String (set_current_max)) + "$");}
                         if (param_set == 72) { SSerial.println("$#p072" + (String (set_current_min)) + "$");}
                         if (param_set == 74) { SSerial.println("$#p074" + (String (set_current_gain)) + "$");}
                         if (param_set == 76) { SSerial.println("$#p076" + (String (set_current_offset)) + "$");}
                         
                         if (x4 >= 4) {
                            x4 = 1;
                            data_sent = false;
                            param_set = 0;
                            }
                            else { x4++; }
                        }

                       
                 }       
         
          osc_800 = !osc_800;
           
          t_800 += 400;
          }

                
        /*Oscillator period: 2 sec*/

        if (millis() - t_2000 >= 1000) 
        {
          osc_2000 = !osc_2000;
          t_2000 += 1000;
        }


        /*Oscillator period: 10 sec*/

        if (millis() - t_10000 >= 5000) 
        {
          HMI_sign++;
          if (HMI_sign > 999) { HMI_sign = 0; }

                           
          SSerial.println("$#i000" + (String (HMI_sign)) + "$#i001" + (String ((just_err) || (just_low) || (just_pole) || (just_bal_t) || (just_veryhigh))) +"$");        
          osc_10000 = !osc_10000;
          t_10000 += 5000;
        }



        /*Oscillator period: 30 sec*/

        if (millis() - t_30000 >= 15000) 
        {
          if (HMI_life_mem == HMI_life) { hmi_page = 0; } // if HMI is disconnected, reset hmi_page
          HMI_life_mem = HMI_life;
          osc_30000 = !osc_30000;
          t_30000 += 15000;
        }


      /*periodic Status index increment (every 12 hours)*/

        if (millis() - t_index >= 43200000) 
        {
          status_index = status_index + 10;
          if (status_index >= 200) { status_index = 0; }
          EEPROM.put( 80,status_index );
          if (first_code!=28411) {
              first_code = 28411;
              EEPROM.put( 0,first_code );
          }
          soc_mem_address = 100 + status_index;
          EEPROM.put( soc_mem_address,soc_mah_stored );
          ch_mem_address = 104 + status_index;
          EEPROM.put( ch_mem_address,pack_ch_stored );
          bal_mem_address = 106 + status_index;
          EEPROM.put( bal_mem_address,pack_bal_stored );
               
          t_index += 43200000;
        }




        /*data reading from HMI */
        if (SSerial.available()){
          char ch = SSerial.read();
          
          
          if ((ch == '\n') || (ch == '\r')){
              process(line);
      
              line = ""; // Clear received buffer      
        }
          else {
              if ((ch == '#') || (ch == '1') || (ch == '2') || (ch == '3') || (ch == '4') || (ch == '5') || (ch == '6') || (ch == '7') || (ch == '8') || (ch == '9') || (ch == '0') || (ch == 's') || (ch == 'h') || (ch == 'e') || (ch == 'p') || (ch == 'g') || (ch == 'q') || (ch == 'r') || (ch == 'm') || (ch == '-')) { line += ch; }  
          }
        }


serial_monitor ();      

wdt_reset();

}

/*reading parameters and status from eeprom */
void eeprom_read() {
      EEPROM.get( 2,password );
      EEPROM.get( 12,set_bat_cap);
      EEPROM.get( 14,n );
      EEPROM.get( 24,set_veryhigh );
      EEPROM.get( 26,set_veryhigh_hys );
      EEPROM.get( 28,set_high );
      EEPROM.get( 30,set_high_hys );
      EEPROM.get( 32,set_bal );
      EEPROM.get( 34,set_bal_hys );
      EEPROM.get( 36,set_low );
      EEPROM.get( 38,set_low_hys );
      EEPROM.get( 40,set_status_time );
      EEPROM.get( 44,set_timeout );
      EEPROM.get( 48,set_high_pole_t );
      EEPROM.get( 50,set_high_pole_t_hys );
      EEPROM.get( 52,set_high_bal_t );
      EEPROM.get( 54,set_high_bal_t_hys );
      EEPROM.get( 56,set_bal_diff_on );
      EEPROM.get( 58,set_bal_diff_off );
      EEPROM.get( 60,set_bal_percentage );
      EEPROM.get( 62,set_volt_charge_offset );
      EEPROM.get( 70,set_current_max );
      EEPROM.get( 72,set_current_min );
      EEPROM.get( 74,set_current_gain );
      EEPROM.get( 76,set_current_offset );
      EEPROM.get( 80,status_index );
      soc_mem_address = 100 + status_index;
      ch_mem_address = 104 + status_index;
      bal_mem_address = 106 + status_index;
      EEPROM.get( soc_mem_address,soc_mah_stored );
      EEPROM.get( ch_mem_address,pack_ch_stored );
      EEPROM.get( bal_mem_address,pack_bal_stored );
      }

/*eeprom initializing with factory parameters */
void eeprom_init() {

      first_code = 0;
      password = 1234;
      set_bat_cap = 60;
      n = 16;
      set_veryhigh = 3600; 
      set_veryhigh_hys = 3550; 
      set_high = 3450; 
      set_high_hys = 3420; 
      set_bal = 3400; 
      set_bal_hys =3350; 
      set_low = 2900; 
      set_low_hys = 3100; 
      set_status_time = 3000;
      set_timeout = 6000; 
      set_high_pole_t = 45; 
      set_high_pole_t_hys = 35; 
      set_high_bal_t = 80; 
      set_high_bal_t_hys = 50; 
      set_bal_diff_on = 40; 
      set_bal_diff_off = 20; 
      set_bal_percentage = 100; 
      set_volt_charge_offset = 0; 
      set_current_max = 200;
      set_current_min = -200;
      set_current_gain = 1000;
      set_current_offset = 0;
      status_index = 0;
      soc_mah_stored = 0;
      soc_mem_address = 100 + status_index;
      pack_ch_stored = false;
      ch_mem_address = 104 + status_index;
      pack_bal_stored = false;
      bal_mem_address = 106 + status_index;

       
      EEPROM.put( 0,first_code );
      EEPROM.put( 2,password );
      EEPROM.put( 12,set_bat_cap);
      EEPROM.put( 14,n );
      EEPROM.put( 24,set_veryhigh );
      EEPROM.put( 26,set_veryhigh_hys );
      EEPROM.put( 28,set_high );
      EEPROM.put( 30,set_high_hys );
      EEPROM.put( 32,set_bal );
      EEPROM.put( 34,set_bal_hys );
      EEPROM.put( 36,set_low );
      EEPROM.put( 38,set_low_hys );
      EEPROM.put( 40,set_status_time );
      EEPROM.put( 44,set_timeout );
      EEPROM.put( 48,set_high_pole_t );
      EEPROM.put( 50,set_high_pole_t_hys );
      EEPROM.put( 52,set_high_bal_t );
      EEPROM.put( 54,set_high_bal_t_hys );
      EEPROM.put( 56,set_bal_diff_on );
      EEPROM.put( 58,set_bal_diff_off );
      EEPROM.put( 60,set_bal_percentage );
      EEPROM.put( 62,set_volt_charge_offset );
      EEPROM.put( 70,set_current_max );
      EEPROM.put( 72,set_current_min );
      EEPROM.put( 74,set_current_gain );
      EEPROM.put( 76,set_current_offset );
      EEPROM.put( 80,status_index );
      EEPROM.put( soc_mem_address,soc_mah_stored );
      EEPROM.put( ch_mem_address,pack_ch_stored );
      EEPROM.put( bal_mem_address,pack_bal_stored );
      
      }


/*processing of data received from HMI */
void process(String line) {
         
            
                        trappola = line;
                        tapullo = true;
                        
                        if (line.startsWith("#s")){
                             String pippo = line;
                             pippo.remove(0,2);
                             hmi_page = pippo.toInt();
                            }
                        
                        if (line.startsWith("#m")){
                             String pippo = line;
                             pippo.remove(0,2);
                             HMI_life = pippo.toInt();
                            }

            
                        if (hmi_page == 0) {
                             x1 = 1;
                             x2 = 1;
                             x3 = 1;
                             x4 = 1;
                             x5 = 1; 
                             z2 = 1;
                             z3 = 1;
                             z4 = 1;
                             z5 = 1;
                             
                             }
            
                        if (hmi_page == 1) {
                             x2 = 1;
                             x3 = 1;
                             x4 = 1;
                             x5 = 1; 
                             z2 = 1;
                             z3 = 1;
                             z4 = 1;
                             z5 = 1;
                             
                             }
                         
                        
                        if (hmi_page == 2) {
                            x1 = 1;
                            x3 = 1;
                            x4 = 1;
                            x5 = 1; 
                            z3 = 1;
                            z4 = 1;
                            z5 = 1;
                            if (line.startsWith("#h1"))  { overview_data = true; } 
                            if (line.startsWith("#h0"))  { overview_data = false; }
                           }
            
                        if (hmi_page == 3) {
                            x1 = 1;
                            x2 = 1;
                            x4 = 1;
                            x5 = 1; 
                            z2 = 1;
                            z4 = 1;
                            z5 = 1;
                        
                                      if ((line.startsWith("#q")) && !cell_test) {
                                                  String pippo = line;
                                                  pippo.remove(0,2);
                                                  cell_to_be_tested = pippo.toInt();
                                                  cell_test = true;
                                                  }
                                       if (line.startsWith("#r")) {
                                                  String pippo = line;
                                                  pippo.remove(0,2);
                                                  cell_to_be_reset = pippo.toInt();
                                                  cell_reset = true;
                                                  }
                         }   
                           
                        
                         if (hmi_page == 4) {
                             x1 = 1;
                             x2 = 1;
                             x3 = 1;
                             x5 = 1; 
                             z2 = 1;
                             z3 = 1;
                             z5 = 1;
                             if (line.startsWith("#e1"))  { all_data = true; } 
                             if (line.startsWith("#e0"))  { all_data = false; } 
                                                            
                                  
                                
                             if (line.startsWith("#p")){
                                data_sent = true;
                                
                                String topolino = line.substring(2,5);
                                param_set = topolino.toInt();    
                                                        
                                
                                if (line.startsWith("#p002")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    password = pippo.toInt();
                                    EEPROM.put( 2,password );
                                    } 
            
                                if (line.startsWith("#p012")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_bat_cap = pippo.toInt();
                                    EEPROM.put( 12,set_bat_cap );
                                    } 
            
                                if (line.startsWith("#p014")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    n = pippo.toInt();
                                    EEPROM.put( 14,n );
                                    } 
            
                                if (line.startsWith("#p024")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_veryhigh = pippo.toInt();
                                    EEPROM.put( 24,set_veryhigh );
                                    } 
            
                                if (line.startsWith("#p026")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_veryhigh_hys = pippo.toInt();
                                    EEPROM.put( 26,set_veryhigh_hys );
                                    } 
                                
                                if (line.startsWith("#p028")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_high = pippo.toInt();
                                    EEPROM.put( 28,set_high );
                                    } 
            
                                if (line.startsWith("#p030")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_high_hys = pippo.toInt();
                                    EEPROM.put( 30,set_high_hys );
                                    } 
            
                                if (line.startsWith("#p032")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_bal = pippo.toInt();
                                    EEPROM.put( 32,set_bal );
                                    } 
            
                                if (line.startsWith("#p034")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_bal_hys = pippo.toInt();
                                    EEPROM.put( 34,set_bal_hys );
                                    } 
            
                                if (line.startsWith("#p036")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_low = pippo.toInt();
                                    EEPROM.put( 36,set_low );
                                    } 
            
                                if (line.startsWith("#p038")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_low_hys = pippo.toInt();
                                    EEPROM.put( 38,set_low_hys );
                                    } 
            
                                if (line.startsWith("#p040")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_status_time = pippo.toInt();
                                    EEPROM.put( 40,set_status_time );
                                    } 
            
                                if (line.startsWith("#p044")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_timeout = pippo.toInt();
                                    EEPROM.put( 44,set_timeout );
                                    } 
            
                                if (line.startsWith("#p048")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_high_pole_t = pippo.toInt();
                                    EEPROM.put( 48,set_high_pole_t );
                                    } 
                              
                                if (line.startsWith("#p050")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_high_pole_t_hys = pippo.toInt();
                                    EEPROM.put( 50,set_high_pole_t_hys );
                                    } 
            
                                if (line.startsWith("#p052")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_high_bal_t = pippo.toInt();
                                    EEPROM.put( 52,set_high_bal_t );
                                    }
            
                                if (line.startsWith("#p054")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_high_bal_t_hys = pippo.toInt();
                                    EEPROM.put( 54,set_high_bal_t_hys );
                                    }
            
                                if (line.startsWith("#p056")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_bal_diff_on = pippo.toInt();
                                    EEPROM.put( 56,set_bal_diff_on );
                                    }
            
                                if (line.startsWith("#p058")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_bal_diff_off = pippo.toInt();
                                    EEPROM.put( 58,set_bal_diff_off );
                                    }
            
                                if (line.startsWith("#p060")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_bal_percentage = pippo.toInt();
                                    EEPROM.put( 60,set_bal_percentage );
                                    }

                                 if (line.startsWith("#p062")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_volt_charge_offset = pippo.toInt();
                                    EEPROM.put( 62,set_volt_charge_offset );
                                    }
              
                                if (line.startsWith("#p070")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_current_max = pippo.toInt();
                                    EEPROM.put( 70,set_current_max );
                                    }
            
                                if (line.startsWith("#p072")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_current_min = pippo.toInt();
                                    EEPROM.put( 72,set_current_min );
                                    }
            
                                if (line.startsWith("#p074")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_current_gain = pippo.toInt();
                                    EEPROM.put( 74,set_current_gain );
                                    }
            
                                if (line.startsWith("#p076")) {
                                    String pippo = line;
                                    pippo.remove(0,5);
                                    set_current_offset = pippo.toInt();
                                    EEPROM.put( 76,set_current_offset );
                                    }
                                                
              
                                first_code = 28411;
                                EEPROM.put( 0,first_code );
                                
                                  
                               }
            
                             else if (line.startsWith("#c")) { data_sent = false; }
                             } 
                             
                              
                          if (hmi_page == 5) {
                            x1 = 1;
                            x2 = 1;
                            x3 = 1;
                            x4 = 1; 
                            z2 = 1;
                            z4 = 1;
                            z5 = 1;
                           } 

                  

            
                  
}       

int freeRam () {
extern int __heap_start, *__brkval;
int v;
return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


/*procedure used to show a variable value on serial monitor for checking. IMPORTANT: not more than 3 variables at time */
void serial_monitor () {
   
/*   Serial.print(var1);
   Serial.print(" ");
   Serial.print(var2);
   Serial.print(" ");
   Serial.println(var3);*/


   cycle_time_stop = micros() - cycle_time_start;
    
}
