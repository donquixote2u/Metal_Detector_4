// Arduino Single Coil Metal Detector 

/* V2 - Production Version with dynamic thresholds and auto calibrate, Adjusted LED Brightness to maximum
   V3 – increased delay 5000us to 20000us to compensate for single coil, increased dly1= 600 tested on single ok 12/03/2016
*/

int ledthreshold1 = 40;

int threshdiff = 15; // Add to average to become specific threshold where LED will light (Changed V21 from 25 to 20)

long pulse1 = 0; // Stores the length of pulses in microseconds  
long pulse2 = 0; // Stores the length of pulses in microseconds
long pulse3 = 0; // Stores the length of pulses in microseconds
long pulseav = 0; // Stores value of output for calibration

int minthresh = 20; // Minimum threshold for baseline reading 
int startdelay = 700; // Start at this value and work down for calibration

long pcounterA; // Counts the number of pulses in each cycle

int dly1 = 0; // Delay before sampling pulsewidth A

void setup()
{
Serial.begin(115200); // Setupserial interface for test data outputs
pinMode(2,INPUT); // Calibrate Button
pinMode(3,OUTPUT); // Pulse output from D3 Arduino
pinMode(4,INPUT); // Signal from LM339 in read by D4 on Arduino
pinMode(14,OUTPUT); // LED output to indicate a find on A0
}

void loop()
{
// Reset the pulse counter to zero for this cycle of waveform
pcounterA = 0;

// Auto Calibrate Coil Sensitivity if button pushed

if((digitalRead(2) == LOW) ){ 
  delay(100); // Debounce by waiting then checking again
  if((digitalRead(2) == LOW) ){   // Carry out operation
    calibrate(); // Sets individual dly variable
    }
}

// Pulse and read Coil 3 times ---------------------------------------------
digitalWrite(3,HIGH); // Set the output pin to high
delay(4); // Delay 5000 microseconds before setting low on output pin
digitalWrite(3,LOW); // Set low on output pin
delayMicroseconds(dly1); // Wait before reading pulses

pcounterA = pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20);

digitalWrite(3,HIGH); // Set the output pin to high
delay(4); // Delay 5000 microseconds before setting low on output pin
digitalWrite(3,LOW); // Set low on output pin
delayMicroseconds(dly1); // Wait before reading pulses

pcounterA = pcounterA + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20);

digitalWrite(3,HIGH); // Set the output pin to high
delay(4); // Delay 5000 microseconds before setting low on output pin
digitalWrite(3,LOW); // Set low on output pin
delayMicroseconds(dly1); // Wait before reading pulses

pcounterA = pcounterA + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20);

Serial.print(pcounterA);
Serial.print(" ");
// Show delay variables
Serial.print("      ");
Serial.print(dly1);
Serial.print("      ");
Serial.print(pulse1);
Serial.print(" ");
Serial.print(pulse2);
Serial.print(" ");
Serial.print(pulse3);
Serial.print(" ");
Serial.println(pulseav);

// Display results
digitalWrite(14,LOW); // Set the output pin to Low to deactivate the LED

if(pcounterA < ledthreshold1)
{
digitalWrite(14,HIGH); // Set the output pin to high to activate the LED
}

/* delay(10);  // Sets up LED brightness
// Turn off all LEDs
digitalWrite(14,LOW); // Set the output pin to Low to deactivate the LED
*/
}


// Subroutines

void calibrate()      {
// Turn off all LEDs
digitalWrite(14,LOW); // Set the output pin to Low to deactivate the LED
// Calibrate Coil D delay threshold  ------------------------------------------------------------
dly1 = startdelay; // Start with very high threshold and work down in steps of 10
pulseav = 0;

while (pulseav <= minthresh ){
  digitalWrite(3,HIGH); // Set the output pin to high
  delay(4); // Delay 5000 microseconds before setting low on output pin
  digitalWrite(3,LOW); // Set low on output pin
  delayMicroseconds(dly1); // Wait before reading pulses

  pulse1 = pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20);

  digitalWrite(3,HIGH); // Set the output pin to high
  delay(4); // Delay 5000 microseconds before setting low on output pin
  digitalWrite(3,LOW); // Set low on output pin
  delayMicroseconds(dly1); // Wait before reading pulses

  pulse2 = pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20);
  
  digitalWrite(3,HIGH); // Set the output pin to high
  delay(4); // Delay 5000 microseconds before setting low on output pin
  digitalWrite(3,LOW); // Set low on output pin
  delayMicroseconds(dly1); // Wait before reading pulses

  pulse3 = pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20)+ pulseIn(4,HIGH,20) + pulseIn(4,HIGH,20);
  
  pulseav = (pulse1 + pulse2 + pulse3)/3;
  
  // Decrement threshold if output too low
  if(pulseav < minthresh) {
    dly1 = dly1 - 10;
    }
} 

// Flash LED to prove something done
digitalWrite(14,HIGH); // Set the output pin to high to activate the LED
delay(5);
digitalWrite(14,LOW); // Set the output pin to high to activate the LED
  
ledthreshold1 = pulseav + threshdiff; 
}
