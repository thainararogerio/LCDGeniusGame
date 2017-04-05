unsigned short digitsMap[11];
unsigned short num_sequence[100];
unsigned short displays[4];
unsigned short currentDisplay = 0;
unsigned short button = 0;
unsigned short current_record = 0;

unsigned int current_level = 0;
unsigned int seed = 0;
unsigned int missed = 0;

unsigned int S_A = 1;
unsigned int S_B = 2;
unsigned int S_C = 4;
unsigned int S_D = 8;
unsigned int S_E = 16;
unsigned int S_F = 32;
unsigned int S_G = 64;

//int0 = 0b00000110; 6
//int1 = 0b00000101; 5
//int2 = 0b00000011; 3
unsigned int INT_0 = 6;
unsigned int INT_1 = 5;
unsigned int INT_2 = 3;
unsigned int INT_TMR_2 = 1;

void play_sound(unsigned int a,unsigned int b)
{
    TMR0IE_BIT = 0;
    Sound_Play(a, b);
    TMR0IE_BIT = 1;
}

void clean_display()
{
    displays[0] = 255;
    displays[1] = 255;
    displays[2] = 255;
    displays[3] = 255;
}

void init_timer() {
    T0CON = 0xC4;
    TMR0L = 0x06;
    GIE_BIT = 1;
    TMR0IE_BIT = 1;
}

void write_display(unsigned short d, unsigned short numero) {
    PORTE = 0;
    PORTA.F5 = 0;

    if (numero == 255) {
       PORTD = 0x00;
    } else {
       PORTD = digitsMap[numero];
    }

    // Display1 = 001 = 1, display2 = 010 = 2, display3 = 100 = 4, display4 = 1
    switch (d)
    {
        case 1:
            PORTE = 0x01;
        break;
        case 2:
            PORTE = 0x02;
        break;
        case 3:
            PORTE = 0x04;
        break;
        case 4:
            PORTA.F5 = 1;
        break;
    }
}

void interrupt()
{
        if (TMR0IF_BIT) {
                TMR0IF_BIT = 0;
                TMR0L = 0x06;
        }

    write_display(currentDisplay+1, displays[currentDisplay]);
    currentDisplay++;

    if (currentDisplay > 3)  {
		currentDisplay = 0;
    }
}

void fill_num_display(unsigned int num) {
        displays[0] = 255;
        displays[1] = 255;
        displays[2] = 255;
        displays[3] = 255;

        if (num >= 1000) {
         displays[0] = num / 1000;
        } if (num >= 100) {
         displays[1] = (num % 1000) / 100;
        } if (num >= 10) {
         displays[2] = (num % 100) / 10;
        }

    displays[3] = num % 10;
}

void fill_word_display(char *palavra)
{
    int i = 0;
    displays[0] = 255;
    displays[1] = 255;
    displays[2] = 255;
    displays[3] = 255;
        
    while (palavra[i] != 0)
    {
       displays[i] = palavra[i];
       i++;
       if(i > 4)
          break;
    }
}

int cod_portb(){ return (PORTB & 0b00000111); }

int cod_portc(){ return (PORTC & 0b00000001); }

unsigned short read_carac()
{
   if (COD_PORTB() == INT_0){ Delay_Ms(50);
   while (COD_PORTB() == INT_0); return('1'); }
   if (COD_PORTB() == INT_1){ Delay_Ms(50);
   while (COD_PORTB() == INT_1); return('2'); }
   if (COD_PORTB() == INT_2){ Delay_Ms(50);
   while (COD_PORTB() == INT_2); return('3'); }
   if (COD_PORTC() == INT_TMR_2){ Delay_Ms(50);
   while (COD_PORTC() == INT_TMR_2); return('4'); }

   return(255);
}

char read_button()
{
    char button;
    button = read_carac();

    while(button == 255)
        button = read_carac();

    return button;
}

void fill_digits_map()
{
    digitsMap[0] = S_A + S_B + S_C + S_D + S_E + S_F;
    digitsMap[1] = S_B + S_C;
    digitsMap[2] = S_A + S_B + S_D + S_E + S_G;
    digitsMap[3] = S_A + S_B + S_C + S_D + S_G;
    digitsMap[4] = S_B + S_C + S_F + S_G;
    digitsMap[5] = S_A + S_C + S_D + S_F + S_G;
    digitsMap[6] = S_A + S_C + S_D + S_E + S_F + S_G;
    digitsMap[7] = S_A + S_B + S_C;
    digitsMap[8] = S_A + S_B + S_C + S_D + S_E + S_F + S_G;
    digitsMap[9] = S_A + S_B + S_C + S_F + S_G;
    digitsMap['A'] = S_A + S_B + S_C + S_E + S_F + S_G;
    digitsMap['B'] = S_C + S_D + S_E + S_F + S_G;
    digitsMap['C'] = S_A + S_D + S_E + S_F;
    digitsMap['D'] = S_B + S_C + S_D + S_E + S_G;
    digitsMap['E'] = S_A + S_E + S_F + S_G + S_D;
    digitsMap['F'] = S_A + S_E + S_F + S_G;
    digitsMap['G'] = S_A + S_D + S_E + S_C + S_F + S_G;
    digitsMap['H'] = S_B + S_C + S_E + S_F + S_G;
    digitsMap['I'] = S_B + S_C;
    digitsMap['L'] = S_D + S_E + S_F;
    digitsMap['N'] = S_E + S_G + S_C;
    digitsMap['O'] = S_A + S_B + S_C + S_D + S_E + S_F;
    digitsMap['P'] = S_A + S_B + S_E + S_F + S_G;
    digitsMap['R'] = S_E + S_G;
    digitsMap['S'] = S_A + S_C + S_D + S_F + S_G;
    digitsMap['T'] = S_B + S_C + S_G;
    digitsMap['U'] = S_B + S_C + S_D + S_E + S_F;
    digitsMap['V'] = S_B + S_C + S_D + S_E + S_F;
    digitsMap['Y'] = S_B + S_C + S_D + S_F + S_G;
    digitsMap['Z'] = S_A + S_B + S_D + S_E + S_G;
}

void print_num_sequence()
{
   int i;
   for (i = 0; i <= current_level; ++i)
   {
      switch(num_sequence[i])
      {
         case 0:
           fill_num_display(1111);
           play_sound(466, 150);
           break;
         case 1:
           fill_num_display(2222);
           play_sound(587, 150);
           break;
         case 2:
           fill_num_display(3333);
           play_sound(698, 150);
           break;
         case 3:
           fill_num_display(4444);
           play_sound(392, 150);
           break;
      }
      Delay_ms(300);
      clean_display();
      Delay_ms(300);
   }
}

int read_player_sequence()
{
   int i, gotItRight = 0;
   char num;

   for (i = 0; i <= current_level; i++)
   {
     num = read_button();

     switch(num)
     {
      case '1':
		   gotItRight = (num_sequence[i] == 0);
           if (gotItRight) { fill_num_display(1111);play_sound(466, 150); }
           break;
      case '2':
		   gotItRight = (num_sequence[i] == 1);
           if (gotItRight) { fill_num_display(2222);play_sound(587, 150); }
           break;
      case '3':
		   gotItRight = (num_sequence[i] == 2);
           if (gotItRight) { fill_num_display(3333);play_sound(698, 150); }
           break;
      case '4':
		   gotItRight = (num_sequence[i] == 3);
           if (gotItRight) { fill_num_display(4444);play_sound(392, 150); }
           break;
      case 255:
		   gotItRight = 0;
           break;
     }
     Delay_ms(100);
     clean_display();
     Delay_ms(100);

     if (!gotItRight)
        break;
  }
  
  return gotItRight;
}

void write_(unsigned short address, unsigned short dado)
{
 I2C1_Start();
 I2C1_Wr(0xA0); //display address + writing action beginner
 I2C1_Wr(address);
 I2C1_Wr(dado);
 I2C1_Stop();
 Delay_ms(10);
}

unsigned short read_(unsigned short address)
{
 unsigned short dado;
 I2C1_Start();
 I2C1_Wr(0xA0); //display address + writing action beginner
 I2C1_Wr(address); //points to the given address
 I2C1_Repeated_Start();
 I2C1_Wr(0xA1); //display address + reading action beginner
 dado = I2C1_Rd(0u);//no acknowledge ?
 I2C1_Stop();
 return (dado);
}

int main()
{
    char carac;
    
    TRISD = 0;
    TRISE = 0;
    TRISA = 0;
    TRISB = 0b00000111;
    TRISC = 0b00000001;

    I2C1_Init(100000);
    Delay_ms(100);
    UART1_Init(19200);
    Delay_ms(100);

    write_(0,11);

    init_timer();//here we begin to receive the key events

    adcon1 = 0x0F;

    fill_digits_map();
    clean_display();
        
    Sound_Init(&PORTC, 2);//PORTC.F2

    clean_display();
    fill_word_display("CLIC");
    carac = read_carac();
    while(carac == 255)
    {
       seed++;
       carac = read_carac();
    }
    clean_display();

    if (carac == '1')
    {
       write_(127, 0);
       current_record = 0;
       fill_word_display("NO");
       Delay_ms(1000);
       fill_word_display("RECO");
       Delay_ms(1500);
    }
    else
    {
       current_record = read_(127);
       fill_word_display("RECO");
       Delay_ms(1000);
       clean_display();
       Delay_ms(500);
       fill_num_display(current_record);
       Delay_ms(1500);
    }
    
    srand(seed);
    clean_display();
    Delay_ms(500);
    fill_word_display("PLAY");
    Delay_ms(1500);
       
    while(!missed)
    {
       num_sequence[current_level] = rand() & 0b00000011;
       print_num_sequence();
       missed = !read_player_sequence();

       if (missed)
       {
         clean_display();
         fill_word_display("FAIL");
         play_sound(300, 450);
         play_sound(600, 450);
         Delay_ms(2000);
         
         fill_word_display("SCOR");
         Delay_ms(1500);
         fill_num_display(current_level);
         Delay_ms(1500);
         clean_display();
         
         if (current_level > current_record)
         {
          write_(127, current_level);//new record
          fill_word_display("UHUL");
          Delay_ms(1000);
          fill_word_display("NOVO");//NOVO means NEW in Portuguese
          Delay_ms(1000);
          fill_word_display("RECO");
          Delay_ms(1000);
          clean_display();
         }
       }
       else
       {
         current_level++;
         clean_display();
         fill_word_display("GOOD");
         Delay_ms(1000);
       }
    }
}