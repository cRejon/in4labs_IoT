#include <Arduino.h>

static volatile uint8_t receiveData = 0;
static volatile uint8_t writeData = 0;
volatile bool read = false;

void setup() 
{
    Serial.begin(9600);

    PORTMUX.TWISPIROUTEA |= PORTMUX_SPI01_bm;  // select ALT2 for SPI pins: 
                                            // pins PE[3:0] from microcontroller 
                                            // pins [13:10] from Uno Wifi Rev2
    
    Serial.println(PORTMUX_SPI01_bm, BIN);
    PORTE.DIR &= ~PIN0_bm; /* Set MOSI pin direction to input */
    PORTE.DIR |= PIN1_bm; /* Set MISO pin direction to output */
    PORTE.DIR &= ~PIN2_bm; /* Set SCK pin direction to input */
    PORTE.DIR &= ~PIN3_bm; /* Set SS pin direction to input */

    SPI0.CTRLA = 0;                   /* Reset register */
    SPI0.CTRLA = SPI_ENABLE_bm        /* Enable module */
               & (~SPI_MASTER_bm)     /* SPI module in Slave mode */
               | SPI_PRESC_DIV16_gc;  /* SPI Clock frequency: 1 MHz */

    SPI0.CTRLB |= SPI_MODE_0_gc; /* Set SPI mode to SPI mode 0 */
    //SPI0.CTRLB = 0xc0; // Enable buffering and set buffer mode to 2-bytes
    
    SPI0.INTCTRL = SPI_IE_bm; /* SPI Interrupt enable */
    
    //#define interrupts() sei()
    sei(); /* Enable Global Interrupts */
}

//ISR(SPI_STC_vect)
ISR(SPI0_INT_vect)
{
    receiveData = SPI0.DATA;
    
    SPI0.DATA = writeData;

    SPI0.INTFLAGS = SPI_IF_bm; /* Clear the Interrupt flag by writing 1 */

    read = true;
}

void loop()
{    
    if(read){
        Serial.print("Data received: ");
        Serial.print(receiveData, BIN);
        Serial.println();
        read = false;
    }
}