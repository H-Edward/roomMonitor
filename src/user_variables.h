#ifndef USER_VARIABLES_H


// You can change any of the following values to suit your needs.
// Note that the SCD4X needs to be connected to valid I2C pins
// and the TM1637 display needs to be connected to valid GPIO pins


#define DEBOUNCE_DELAY 500 // time in ms to wait before accepting another button press
#define TEMP_BUTTON 14 
#define HUM_BUTTON 15
#define CO2_BUTTON 16
#define PWR_BUTTON 17

// The pins for the TM1637 display
#define CLK_TM1637 18 
#define DIO_TM1637 19

// The LED for the CO2 warning
#define CO2_LED 20
#define WARNNG_CO2_THRESHOLD 1600 // ppm threshold for the CO2 warning

#endif // USER_VARIABLES_H