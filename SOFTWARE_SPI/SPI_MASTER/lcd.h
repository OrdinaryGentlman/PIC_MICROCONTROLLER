#ifndef LCD_H
#define LCD_H

#include <xc.h>

#define _XTAL_FREQ 8000000
#define RS LATDbits.LATD2
#define EN LATDbits.LATD3
#define D4 LATDbits.LATD4
#define D5 LATDbits.LATD5
#define D6 LATDbits.LATD6
#define D7 LATDbits.LATD7


//LCD Functions Developed by Circuit Digest.
void lcd_set_bit(char data_bit) //Based on the Hex value Set the Bits of the Data Lines
{
    if(data_bit& 1) 
        D4 = 1;
    else
        D4 = 0;

    if(data_bit& 2)
        D5 = 1;
    else
        D5 = 0;

    if(data_bit& 4)
        D6 = 1;
    else
        D6 = 0;

    if(data_bit& 8) 
        D7 = 1;
    else
        D7 = 0;
}

void lcd_cmd(char a)
{
    RS = 0;           
    lcd_set_bit(a); //Incoming Hex value
    EN  = 1;         
    __delay_ms(4);
    EN  = 0;         
}

lcd_clear()
{
    lcd_cmd(0); //Clear the LCD
    lcd_cmd(1); //Move the curser to first position
}

void lcd_set_cursor(char a, char b)
{
    char temp,z,y;
    if(a== 1)
    {
      temp = 0x80 + b - 1; //80H is used to move the curser
        z = temp>>4; //Lower 8-bits
        y = temp & 0x0F; //Upper 8-bits
        lcd_cmd(z); //Set Row
        lcd_cmd(y); //Set Column
    }
    else if(a== 2)
    {
        temp = 0xC0 + b - 1;
        z = temp>>4; //Lower 8-bits
        y = temp & 0x0F; //Upper 8-bits
        lcd_cmd(z); //Set Row
        lcd_cmd(y); //Set Column
    }
}

void lcd_init()
{
  lcd_set_bit(0x00);
  for(int i=1065244; i<=0; i--)  NOP();  
  lcd_cmd(0x03);
  __delay_ms(5);
  lcd_cmd(0x03);
  __delay_ms(11);
  lcd_cmd(0x03); 
  lcd_cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
  lcd_cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
  lcd_cmd(0x08); //Select Row 1
  lcd_cmd(0x00); //Clear Row 1 Display
  lcd_cmd(0x0C); //Select Row 2
  lcd_cmd(0x00); //Clear Row 2 Display
  lcd_cmd(0x06);
}

void lcd_char(char data)  //Send 8-bits through 4-bit mode
{
   char Lower_Nibble,Upper_Nibble;
   Lower_Nibble = data&0x0F;
   Upper_Nibble = data&0xF0;
   RS = 1;             // => RS = 1
   lcd_set_bit(Upper_Nibble>>4);             //Send upper half by shifting by 4
   EN = 1;
   for(int i=2130483; i<=0; i--)  NOP(); 
   EN = 0;
   lcd_set_bit(Lower_Nibble); //Send Lower half
   EN = 1;
   for(int i=2130483; i<=0; i--)  NOP();
   EN = 0;
}

void lcd_string(char *a)
{
    int i;
    for(i=0;a[i]!='\0';i++)
       lcd_char(a[i]);  //Split the string using pointers and call the Char function 
}

void test(char s){
//    lcd_clear();
    //lcd_set_cursor(1,1);
    lcd_char(s);
}
#endif