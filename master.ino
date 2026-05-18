#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit(void) {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);  
    //MOSI-send data; SCK-clock; SS-select slave

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  // Enable SPI, Master mode, fosc/16
    //Turn SPI on, make this device the Master, and set clock speed to fosc/16
    PORTB |= (1 << PB2);  //set SS as high
}
void SPI_MasterTransmit(uint8_t data) {
    PORTB &= ~(1 << PB2);  //Select slave → communication starts
    SPDR = data;  //load data on data register
    while (!(SPSR & (1 << SPIF)));  // Wait until transfer is complete
    PORTB |= (1 << PB2);  //SS HIGH, end communication
}
int main(void) {
    SPI_MasterInit();  //initialize SPI

    uint8_t values[3] = {85, 170, 255};

    while (1) {
        for (int i = 0; i < 3; i++) {
            SPI_MasterTransmit(values[i]);
            _delay_ms(1000);
        }
    }
}