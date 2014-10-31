/* a simple ambient light program by Lukas Epple
 * you might know him as sternenseemann */
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

void fade_to(uint8_t colors[]) {
	/* this is a pretty shitty fading algorithm but
	 * the result looks ok so what?
	 * Seriously: I didn't have too much time so I
	 * quickly hacked together this, hopefully
	 * I'll rewrite it one day! */
	int differences[3];
	uint8_t current[] = { OCR0, OCR1A, OCR1B };

	differences[0] = current[0] - colors[0];
	differences[1] = current[1] - colors[1];
	differences[2] = current[2] - colors[2];

	while(1) {
		int i;
		for(i = 0; i < 3; i++) {
			if(differences[i] == 0) {
				continue;
			} else if(differences[i] < 0) {
				current[i] += 1;
				differences[i] += 1;
			} else {
				current[i] -= 1;
				differences[i] -= 1;
			}
		}
		OCR0 = current[0];
		OCR1A = current[1];
		OCR1B = current[2];

		_delay_ms(2);

		if(differences[0] == 0 && differences[1] == 0 && differences[2] == 0) {
			break;
		}

	}

}
int main( void ) {
	DDRB |= (1 << PB3); // OC0
	DDRD |= (1 << PD4) | (1 << PD5); // OC1B OC1A

	/* setup the timers in Fast PWM non-inverted mode */
	TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS00);

	TCCR1A |= (1 << WGM12) | (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1) | (1 << CS00);
	TCCR1B |= (1 << WGM12) | (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1) | (1 << CS00);

	uint8_t colors[3];
	while(1) {
		int i;
		for(i = 0; i < 3; i++) {
			colors[i] = rand() % 0xFF;
		}

		fade_to(colors);
	}

	return 0;
}
