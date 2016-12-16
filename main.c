/*
 * main.c
 *
 *  Created on: 8 дек. 2016
 *      Author: garik
 */
/*** Программный ШИМ ***/
/* адаптация под 3 фазы 400 Гц */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t counter=0;
uint8_t lev_ch1, lev_ch2, lev_ch3, lev_ch4, lev_ch5, lev_ch6;
uint8_t buf_lev_ch1, buf_lev_ch2, buf_lev_ch3, buf_lev_ch4, buf_lev_ch5, buf_lev_ch6;
uint8_t direction;


/*** прерывание по переполнению Т0 ***/
ISR(TIMER0_OVF_vect)
{
	counter++;
	  if (counter==0) //счетчик перехода таймера через ноль
	  {
	    buf_lev_ch1=lev_ch1; //значения длительности ШИМ
	    PORTB |=(1<<PB0); //подаем 1 на все каналы
	  }
	  if (counter==85)
	  {
	    buf_lev_ch2=85+lev_ch2;
	    PORTB |=(1<<PB1);
	  }
	  if (counter==170)
	  {
	    buf_lev_ch3=170+lev_ch3;
	    PORTB |=(1<<PB2);
	  }
	  if (counter==0) //счетчик перехода таймера через ноль
	  {
	    buf_lev_ch4=lev_ch4; //значения длительности ШИМ
	    PORTB &= ~(1<<PB3); //подаем 1 на все каналы
	  }
	  if (counter==85)
	  {
	    buf_lev_ch5=85+lev_ch5;
	    PORTB &= ~(1<<PB4);
	  }
	  if (counter==170)
	  {
	    buf_lev_ch6=170+lev_ch6;
	    PORTB &= ~(1<<PB5);
	  }

	  if (counter==buf_lev_ch1) PORTB&=~(1<<PB0); //подаем 0 на канал
	  if (counter==buf_lev_ch2) PORTB&=~(1<<PB1); //по достижении
	  if (counter==buf_lev_ch3) PORTB&=~(1<<PB2); //заданной длительности.
	  if (counter==buf_lev_ch4) PORTB|=(1<<PB3);
	  if (counter==buf_lev_ch5) PORTB|=(1<<PB4);
	  if (counter==buf_lev_ch6) PORTB|=(1<<PB5);

}

/*** процедура задержки в микросекундах ***/
void delay_us(unsigned char time_us)
{
register unsigned char i;

	for(i = 0; i < time_us; i++)	// 4 цикла
	{
		asm (" PUSH  R0 ");		// 2 цикла
		asm (" POP   R0 ");		// 2 цикла
		// 8 циклов = 1 us для 8MHz
	}
}
/*** процедура задержки в миллисекундах ***/
void delay_ms(unsigned int time_ms)
{
register unsigned int i;

for(i = 0; i < time_ms; i++)
	{
		delay_us(250);
		delay_us(250);
		delay_us(250);
		delay_us(250);
	}
}

int main (void)
{
	lev_ch1=0;
	lev_ch2=0;
	lev_ch3=0;
	lev_ch4=0;
	lev_ch5=0;
	lev_ch6=0;

	PORTC = 0x00;	//debug

	DDRB |= (1 << PB1)|(1 << PB0); // PB5,4,3,2,1,0 - выходы

	TCCR0 |= (1 << CS00); // без предделителя
	TIMSK |= (1 << TOIE0); // разрешение прерывания

	sei();// глобальное разрешение прерываний

	while(1)
	{

	      lev_ch1++; //увеличиваем значения

	      delay_ms(50); //пауза 50мс


		//PORTC ^= (1 << PC5);  //debug
	}
}
