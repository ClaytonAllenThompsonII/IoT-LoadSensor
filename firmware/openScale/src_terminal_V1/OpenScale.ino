#include "HX711.h" //Original Repository Created by Bodge https://github.com/bogde/HX711
#include "openscale.h" //Contains EPPROM locations for settings
#include <Wire.h> //Needed to talk to on board TMP102 temp sensor
#include <EEPROM.h> //Needed to record user settings
#include <OneWire.h> //Needed to read DS18B20 temp sensors

#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers

#define FIRMWARE_VERSION "1.2"

//Global variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
long setting_uart_speed; //This is the baud rate that the system runs at, default is 9600. Can be 1,200 to 1,000,000
byte setting_units; //Lbs or kg?
unsigned int setting_report_rate;
long setting_calibration_factor; //Value used to convert the load cell reading to lbs or kg
long setting_tare_point; //Zero value that is found when scale is tared
boolean setting_timestamp_enable; //Prints the number of miliseconds since boot next to weight reading
byte setting_decimal_places; //How many decimals to display
byte setting_average_amount; //How many readings to take before reporting reading
byte setting_local_temp_enable; //Prints the local temperature in C
byte setting_remote_temp_enable; //Prints the remote temperature in C
byte setting_status_enable; //Turns on/off the blinking status LED
byte setting_serial_trigger_enable; //Takes reading when serial character is received
byte setting_raw_reading_enable; //Prints the raw, 24bit, long from the HX711, ex: 8355808
byte setting_trigger_character; //The character that will cause OpenScale to report a reading
boolean setupMode = false; //This is set to true if user presses x

const byte escape_character = 'x'; //This is the ASCII character we look for to break reporting
const int minimum_powercycle_time = 500; //Anything less than 500 can cause reading problems
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

const byte statusLED = 13;  //Flashes with each reading

HX711 scale; //Setup interface to scale

OneWire remoteSensor(4);  //Setup reading one wire temp sensor on pin 4 (a 4.7K resistor is necessary)
byte remoteSensorAddress[8];
boolean remoteSensorAttached = false;

void setup()
{
  pinMode(statusLED, OUTPUT);

  scale.begin(DAT, CLK);

  //Power down various bits of hardware to lower power usage
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();

  //Shut off Timer2, Timer1, ADC
  ADCSRA &= ~(1 << ADEN); //Disable ADC
  ACSR = (1 << ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  DIDR1 = (1 << AIN1D) | (1 << AIN0D); //Disable digital input buffer on AIN1/0

  power_timer1_disable();
  power_timer2_disable();
  power_adc_disable();
  power_spi_disable();

  //During testing reset everything
  //for(int x = 0 ; x < 30 ; x++)
  //  EEPROM.write(x, 0xFF);

  Wire.begin();

  readSystemSettings(); //Load all system settings from EEPROM

  //Setup UART
  Serial.begin(setting_uart_speed);
  displaySystemHeader(); //Product title and firmware version

  checkEmergencyReset(); //Look to see if the RX pin is being pulled low

  scale.set_scale(setting_calibration_factor);
  scale.set_offset(setting_tare_point);

  //Calculate the minimum time between reports
  int minTime = calcMinimumReadTime();
  Serial.print(F("Minimum time between reports: "));
  Serial.println(minTime);

  //Look for a special case where the report rate time is less than the allowed minimum
  if (setting_report_rate < minTime) setting_report_rate = minTime;

  Serial.print(F("Press "));
  Serial.print((char)escape_character);
  Serial.println(F(" to bring up settings"));

  Serial.println(F("Readings:"));

  powerUpScale(); //Be sure the scale is powered up
}


void loop()
{

  long startTime = millis();

  //Take average of readings with calibration and tare taken into account
  float currentReading = scale.get_units(setting_average_amount);

  //Print time stamp
  if (setting_timestamp_enable == true)
  {
    Serial.print(startTime);
    Serial.print(F(","));
  }

  //Print calibrated reading
  Serial.print(currentReading, setting_decimal_places);
  Serial.print(F(","));
  if (setting_units == UNITS_LBS) Serial.print(F("lbs"));
  if (setting_units == UNITS_KG) Serial.print(F("kg"));
  Serial.print(F(","));

  //Print raw reading
  if (setting_raw_reading_enable == true)
  {
    long rawReading = scale.read_average(setting_average_amount); //Take average reading over a given number of times

    Serial.print(rawReading);
    Serial.print(F(","));
  }

  //Print local temp
  if (setting_local_temp_enable == true)
  {
    Serial.print(getLocalTemperature(), setting_decimal_places);
    Serial.print(F(","));
  }

  //Print remote temp
  if (setting_remote_temp_enable == true)
  {
    if (remoteSensorAttached == true)
    {
      Serial.print(getRemoteTemperature(), setting_decimal_places);
      Serial.print(F(","));
    }
    else
    {
      Serial.print(F("0,")); //There is no sensor to check
    }
  }

  if (setting_status_enable == true) toggleLED();

  Serial.println();
  Serial.flush();

  //Hang out until the end of this report period
  while (1)
  {
    //If we see escape char then drop to setup menu
    if (Serial.available())
    {
      toggleLED();
      char incoming = Serial.read();

      if (setting_status_enable == false) digitalWrite(statusLED, LOW); //Turn off LED

      if (incoming == escape_character) 
      {
        setupMode = true;  //For Trigger Character Feature
        break; //So we can enter the setup menu
      }
    }

    if ((millis() - startTime) >= setting_report_rate) break;
  }

  //If we are serially triggered then wait for incoming character
  if (setupMode == false && setting_serial_trigger_enable == true)
  {
    //Power everything down and go to sleep until a char is received

    delay(100); //Give the micro time to clear out the transmit buffer
    //Any less than this and micro doesn't sleep

    char incoming = 0;

    //Wait for a trigger character or x from user
    while (incoming != setting_trigger_character && incoming != 'x')
    {
      while (Serial.available() == false) {

        delay(1);
        //We go into deep sleep here. This will save 10-20mA.
        power_twi_disable();
        power_timer0_disable(); //Shut down peripherals we don't need

        sleep_mode(); //Stop everything and go to sleep. Wake up if serial character received

        power_timer0_enable();
        power_twi_enable();
      }

      incoming = Serial.read();
      if (incoming == escape_character) setupMode = true;
    }
  }

  //If the user has pressed x go into system setup
  if (setupMode == true)
  {
    system_setup();
    setupMode = false;

    if (setting_status_enable == false) digitalWrite(statusLED, LOW); //Turn off LED
  }
}