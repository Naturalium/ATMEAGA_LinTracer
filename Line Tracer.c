#include <mega128.h>
#include <stdio.h>
#include <delay.h>

int ADC[6], STD;        //Standard
unsigned int ST, FF, FL, FR, FLL, FRR, SL, SR, FSR, FSL, FS;         //Status, Front Forward, Front Left, Front Right, Stop Left, Stop Right
int p1, p2, p3, p4, p5, p6;



void IO()
{
    DDRA = 0xFF;        //LED
    DDRB = 0xFF;        //OC1A, OC1B
    //DDRE = 0b10101010;  //PE1, PE3, PE5, PE7
    DDRF = 0x00;        //ADC            
    DDRD = 0b00001111;
}


void TC()
{
    //SREG = 0xC8;                        // 1/16 
    TCCR1A = 0b10100010;
    TCCR1B = 0b00011101;
    TCCR1C = 0x00;
    TIMSK |= ((1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1)); 
    TIFR |= ((1 << OCF1A) | (1 << OCF1B) | (1 << TOV1));
    TCNT1 = 0x00;
    ICR1 = 100;
}


void SSING()            //Sensing
{
    PORTA = 0xFF;
    delay_ms(500);
    PORTA = 0x00;
    STD = ADC[0] - 250;        // 값을 기준치에 적용
    PORTA = 0xFF;
    delay_ms(500);
    PORTA = 0x00;        
}

void wheel(int LPWM, int RPWM)          //모터 속도 제어 함수       50최대
{
    OCR1A = RPWM + 4;
    OCR1B = LPWM;    
}


void LEDP(void)                             //LED처리
{
    p1 = 0b11111110;
    p2 = 0b11111101;
    p3 = 0b11111011;
    p4 = 0b11110111;
    p5 = 0b11101111;
    p6 = 0b11011111;
    
     if(ADC[0] < STD)
    {
        PORTA = p1;
        if(ADC[1] < STD)
        {
            PORTA &= p2;
            if(ADC[2] < STD)
            {
                PORTA &= p3;
            }
        }
    }
    else if(ADC[1] < STD)
    {   
        PORTA = p2;  
        if(ADC[0] < STD)
        {
             PORTA &=p1;
             if(ADC[2] < STD)
             {
                PORTA &= p3;
                if(ADC[3] < STD)
                {
                    PORTA &= p4;
                }
             }
        }
        else if(ADC[2] < STD)
        {
            PORTA &= p3;
            if(ADC[3] < STD)
            {
                PORTA &= p4; 
            }
        }
    }
    else if(ADC[2] < STD)
    {
        PORTA = p3;
        if(ADC[1] < STD)
        {
            PORTA &= p2;
            if(ADC[3] < STD)
            {
                PORTA &= p4; 
            }
        }
        else if(ADC[3] < STD)
        {
            PORTA &= p4;
            if(ADC[1] < STD)
            {
                PORTA &= p2;
            }
        }
    }
    else if(ADC[3] < STD)
    {
        PORTA = p4;
        if(ADC[2] <STD)
        {
            PORTA &= p3;
            if(ADC[4] < STD)
            {
                PORTA &= p5;
            }
        }
        else if(ADC[4] < STD)
        {
            PORTA &= p5;
            if(ADC[2] < STD)
            {
                PORTA &= p3;            
            }
        }
    }
    else if(ADC[4] < STD)
    {
        PORTA = p5;
        if(ADC[5] < STD)
        {
            PORTA &= p6;
            if(ADC[3] <STD)
            {
                PORTA &= p4;
            }
        }
        else if(ADC[3] < STD)
        {
            PORTA &= p4;
            if(ADC[5] < STD)
            {
                PORTA &= p6;
            }
        }
    }
    else if(ADC[5] < STD)
    {
        PORTA = p6;
        if(ADC[4] < STD)
        {
            PORTA &= p5;
        }
    }
    else if(ADC[2] > STD | ADC[3] > STD | ADC[1] > STD | ADC[0] > STD | ADC[4] > STD | ADC[5] > STD)
                {
                    PORTA = 0b11111111;
                }
    
}

void COMP()                             //라인 인식 구분
{
    
    FF = 1;
    FL = 2;
    FR = 3;
    FLL = 4;
    FRR = 5;
    SL = 6;
    SR = 7;   
    FSR = 8;
    FSL = 9;
    FS = 10;
    
   


//////////////////////////////////////////////////상태

   
    
     if(ADC[0] < STD)
    {
        ST = SL;  
        if(ADC[1] < STD)
        {
            ST = SL;
            if(ADC[2] < STD)
            {
                ST = FSL;
            }
        }
        if(ADC[2] < STD)
            {
                ST = FSL;
            }
    }
    else if(ADC[5] < STD)
    {
        ST = SR; 
        if(ADC[4] < STD)
        {
            ST = SR;
            if(ADC[3] < STD)
            {
                ST = FSR;
            }
        } 
        if(ADC[3] < STD)
            {
                ST = FSR;
            }
    }
    else if(ADC[2] < STD)
    {
        ST = FF;
        if(ADC[1] < STD)
        {
            ST = FL;
        }
    }
    
                                                                       
    
    else if(ADC[3] < STD)
    {
        ST = FF;
        if(ADC[4] < STD)
        {
            ST = FR;
        }
    }
    else if(ADC[1] < STD)
    {
        ST = FLL;
        if(ADC[0] < STD)
        {
            ST = FLL;
        } 
        else if(ADC[2] < STD)
        {
            ST = FL;
        }
    } 
   
    else if(ADC[4] < STD)
    {
        ST = FRR;
        if(ADC[5] < STD)
        {
            ST = FRR;
        }
        else if(ADC[3] < STD)
        {
            ST = FR;
        }
    }

    else ST = FS;   
}
   

void ADCA()
{
    int i = 0;
     for(i; i < 6; i++)
     {
        ADMUX = i;
        ADCSRA = 0xC6;
        delay_ms(5);
        ADC[i] = ADCW;
     }
}


void USART()
{
    UCSR0A = 0x0;
    UCSR0B = 0b00001000;
    UCSR0C = 0b00000110;
    UBRR0H = 0;
    UBRR0L = 103;
}

/////////////////////////////////////////////////////////////////////////


void main(void)
{
    IO();
    //USART();                         //모든 인터럽트 활성화 하면 USART,
    
    ADCA();
    SSING();
    TC();
    PORTB = 0x00;
    //PORTE = 0b00101000;
    PORTD = 0b000000110;  
    
    while(1)
    {   
        /////////////////////////////////////
        ADCA();
        LEDP();
        COMP();
        //printf("%2d %2d %2d %2d %2d %2d %2d\r\n", ADC[0], ADC[1], ADC[2], ADC[3], ADC[4], ADC[5], ST);
        //PORTE = 0b00101000; 
        PORTD = 0b00000110;
        ///////////FF     
        
        ///////////////////////
                   
        if(ST == FS)
        {
            while(1)
            {  
                //PORTE = 0b10000010;
                PORTD = 0b00001001;
                wheel(35,35); 
                PORTA = 0xFF;
                ADCA(); 
                COMP();
                
                if(ADC[2] < STD | ADC[3] <STD | ADC[1] < STD | ADC[0] < STD | ADC[4] < STD | ADC[5] < STD)
                {  
                    wheel(35,35);
                    delay_ms(200);
                    PORTA = 0b11000000;
                    break;
                }
            }
        }
        
        if(ST == FF)
        {
            wheel(40,40);
        }         
        ///////////FL
        if(ST == FL)
        {
            
            wheel(32,40);
        }
        ///////////FR
        if(ST == FR)
        {   
            wheel(40,32);
        }
        ///////////FLL
        if(ST == FLL)
        {
            wheel(25,40);
        }
        ///////////FRR
        if(ST == FRR)
        {
            wheel(40,25);
        }
        ///////////SR
        if(ST == SR)
        {   
            /*
            while(1)
            {
                wheel(36,36);
                ADCA();
                PORTA = 0b11110011;
                if(ADC[2] > STD | ADC[3] >STD | ADC[1] > STD | ADC[0] > STD | ADC[4] > STD | ADC[5] > STD)
                {
                    break;
                }
            }
            */
            while(1)
            {
                ADCA(); 
                COMP();
                PORTA = 0b11000111;
                wheel(40,15);     
                if(ADC[0] < STD)
                {
                    wheel(0,0);
                    break;
                }
             }
        }
        ///////////SL
        if(ST == SL)
        {   
            /*   
            while(1)
            {
                wheel(36,36);
                ADCA();
                PORTA = 0b11110011;
                if(ADC[2] > STD | ADC[3] >STD | ADC[1] > STD | ADC[0] > STD | ADC[4] > STD | ADC[5] > STD)
                {
                    break;
                }                                       //case 1
            } 
            */       
            
            //PORTE = 0xFF;
            //delay_ms(100);   
            while(1)
            {
                ADCA();
                COMP();
                PORTA = 0b11111000;
                wheel(15,40);     
                if(ADC[2] < STD)
                {
                    wheel(0,0);
                    break;
                }
             }
        }
         
        if(ST == FSL)
        {
            PORTE = 0xFF;
            delay_ms(100);
            //PORTE = 0b00101000;
            wheel(30,30);
            delay_ms(180);
            //PORTE = 0b10001000; 
            PORTD = 0b00000101;
            while(1)
            {                
                ADCA();
                LEDP();
                wheel(35,35);     
                if(ADC[2] < STD)
                {
                    wheel(0,0);
                    delay_ms(100);
                    break;
                }
            }
        }
        if(ST == FSR)
        {             
            PORTE = 0xFF;
            delay_ms(100);
            //PORTE = 0b00101000;
            wheel(30,30);     
            delay_ms(180);
            //PORTE = 0b00100010; 
            PORTD = 0b00001010;
            while(1)
            {
                ADCA();
                LEDP();
                wheel(40,40);     
                if(ADC[3] < STD)
                {
                    wheel(0,0); 
                    delay_ms(100);
                    break;
                }
             }
        }
         
    }
    
     
} 
