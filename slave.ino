#include <avr/io.h>
#include <stdio.h>

//Start serial communication, set speed, and use 1-byte data format
void UART_Init(unsigned int ubrr) {  //function to configure serial communication
    UBRR0H = (unsigned char)(ubrr >> 8);  //set baud rate
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  //enable receive and transmit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //set data format as 8 data bit and 1 stop bit
}

void UART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));  //wait until transmit buffer is empty
    UDR0 = data;  //put data into register
}

void UART_PrintNumber(uint8_t num) {
    char buffer[4];  //short number (max256)
    sprintf(buffer, "%d\n", num);  //convert number, only sends characters
    for (int i = 0; buffer[i] != '\0'; i++) {  //loop through each character
        UART_Transmit(buffer[i]);  //send each character
    }
}

void SPI_SlaveInit(void) {
    DDRB |= (1 << PB4); // Slave sends data back to master through this pin

    DDRB |= (1 << PB2);   // SS output (IMPORTANT)
    PORTB &= ~(1 << PB2); // force LOW

    SPCR = (1 << SPE);  //enable SPI, slave mode by default
}

uint8_t SPI_SlaveReceive(void) {
    while (!(SPSR & (1 << SPIF)));  //wait until data is received
    return SPDR;
}


//Wait for data from Master, then print it. Do this forever.
int main(void) {
    SPI_SlaveInit();
    UART_Init(200);

    while (1) {
        uint8_t data = SPI_SlaveReceive();
        UART_PrintNumber(data);
    }
}