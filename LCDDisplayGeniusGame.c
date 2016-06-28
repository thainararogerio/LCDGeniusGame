// LCD module connections
sbit LCD_RS at RE0_bit;
sbit LCD_EN at RE1_bit;

sbit LCD_RS_Direction at TRISE0_bit;
sbit LCD_EN_Direction at TRISE1_bit;

sbit LCD_D0 at RD0_bit;
sbit LCD_D1 at RD1_bit;
sbit LCD_D2 at RD2_bit;
sbit LCD_D3 at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_D0_Direction at TRISD0_bit;
sbit LCD_D1_Direction at TRISD1_bit;
sbit LCD_D2_Direction at TRISD2_bit;
sbit LCD_D3_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;
// End LCD module connections

unsigned short sequencia_num[100];
unsigned short botao = 0;
unsigned short tecla = 0;
unsigned short pontuacao_record = 0;

unsigned int nivel_atual = 0;
unsigned int semente = 0;
unsigned int errou = 0;

//int0 = 0b00000110; 6
//int1 = 0b00000101; 5
//int2 = 0b00000011; 3
unsigned int INT_0 = 6;
unsigned int INT_1 = 5;
unsigned int INT_2 = 3;
unsigned int INT_TMR_2 = 1;

char i;
const char superior_esquerdo[] = {0,0,0,3,7,15,31,30};
const char superior_direito[]  = {0,0,0,24,28,30,31,15};
const char inferior_esquerdo[]  = {30,31,15,7,3,0,0,0};
const char inferior_direito[]  = {15,31,30,28,24,0,0,0};
short TEMP_STR[7];

void toca_som(unsigned int a,unsigned int b)
{
    TMR0IE_BIT = 0;
    Sound_Play(a, b);
    TMR0IE_BIT = 1;
}

void escreve_display(unsigned short d, unsigned short numero) {

}

void popula_displays_num(unsigned int num) {
     IntToStr(num,TEMP_STR);
     Lcd_Out(1,10,TEMP_STR);
}

int cod_portb(){ return (PORTB & 0b00000111); }

int cod_portc(){ return (PORTC & 0b00000001); }

unsigned short le_carac()
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

char le_botao()
{
    char botao;
    botao = le_carac();

    while(botao == 255)
        botao = le_carac();

    return botao;
}
void imprime_1() { LCD_Chr(1, 8, 0); }
void imprime_2() { LCD_Chr(1, 9, 1); }
void imprime_3() { LCD_Chr(2, 8, 2); }
void imprime_4() { LCD_Chr(2, 9, 3); }

void imprime_1_(unsigned char linha, unsigned char coluna) { LCD_Chr(linha, coluna, 0); }
void imprime_2_(unsigned char linha, unsigned char coluna) { LCD_Chr(linha, coluna, 1); }
void imprime_3_(unsigned char linha, unsigned char coluna) { LCD_Chr(linha, coluna, 2); }
void imprime_4_(unsigned char linha, unsigned char coluna) { LCD_Chr(linha, coluna, 3); }

void imprime_sequencia_num()
{
   int i;
   for (i = 0; i <= nivel_atual; ++i)
   {
      switch(sequencia_num[i])
      {
         case 0:
           imprime_1();
           toca_som(466, 150);
           break;
         case 1:
           imprime_2();
           toca_som(587, 150);
           break;
         case 2:
           imprime_3();
           toca_som(698, 150);
           break;
         case 3:
           imprime_4();
           toca_som(392, 150);
           break;
      }
      Delay_ms(300);
      Lcd_Cmd(_LCD_CLEAR);
      Delay_ms(300);
   }
}

int le_sequencia_jogador()
{
   int i, acertou = 0;
   char num;

   for (i = 0; i <= nivel_atual; i++)
   {
     num = le_botao();

     switch(num)
     {
      case '1':
           acertou = (sequencia_num[i] == 0);
           if (acertou) { imprime_1();toca_som(466, 150); }
           break;
      case '2':
           acertou = (sequencia_num[i] == 1);
           if (acertou) { imprime_2();toca_som(587, 150); }
           break;
      case '3':
           acertou = (sequencia_num[i] == 2);
           if (acertou) { imprime_3();toca_som(698, 150); }
           break;
      case '4':
           acertou = (sequencia_num[i] == 3);
           if (acertou) { imprime_4();toca_som(392, 150); }
           break;
      case 255:
           acertou = 0;
           break;
     }
     Delay_ms(100);
     Lcd_Cmd(_LCD_CLEAR);
     Delay_ms(100);

     if (!acertou)
        break;
  }
  
  return acertou;
}

void escreve(unsigned short address, unsigned short dado)
{
 I2C1_Start();
 I2C1_Wr(0xA0); //endereco disp + escrever
 I2C1_Wr(address);
 I2C1_Wr(dado);
 I2C1_Stop();
 Delay_ms(10);
}

unsigned short le(unsigned short address)
{
 unsigned short dado;
 I2C1_Start();
 I2C1_Wr(0xA0); //endereco disp + escrever
 I2C1_Wr(address); //aponta p/ endereco dado
 I2C1_Repeated_Start();
 I2C1_Wr(0xA1); //endereco disp + ler
 dado = I2C1_Rd(0u);//no acknowledge ?
 I2C1_Stop();
 return (dado);
}

void Move_Delay() {
  Delay_ms(500);
}

void animacao_inicio()
{
   imprime_1_(1,2);
   imprime_2_(1,3);
   imprime_3_(2,2);
   imprime_4_(2,3);
   Delay_ms(300);
       Lcd_Cmd(_LCD_CLEAR);
   imprime_1_(1,5);
   imprime_2_(1,6);
   imprime_3_(2,5);
   imprime_4_(2,6);
   Delay_ms(300);
       Lcd_Cmd(_LCD_CLEAR);
   imprime_1_(1,9);
   imprime_2_(1,10);
   imprime_3_(2,9);
   imprime_4_(2,10);
   Delay_ms(300);
       Lcd_Cmd(_LCD_CLEAR);

   imprime_1();
   toca_som(466, 150);
   Delay_ms(40);
   imprime_2();
   toca_som(587, 150);
   Delay_ms(40);
   imprime_3();
   toca_som(698, 150);
   Delay_ms(40);
   imprime_4();
   toca_som(392, 150);
   Delay_ms(500);
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

    escreve(0,11);

    ADCON1 = 0x0F;

    //lcd
    Lcd_Init();                        // Initialize LCD
    delay_ms(250);
    LCD_Cmd(64);  //onde gravar
    for (i = 0; i<8; i++)
        LCD_Chr_Cp(superior_esquerdo[i]);

    for (i = 0; i<8; i++)
        LCD_Chr_Cp(superior_direito[i]);

    for (i = 0; i<8; i++)
        LCD_Chr_Cp(inferior_esquerdo[i]);

    for (i = 0; i<8; i++)
        LCD_Chr_Cp(inferior_direito[i]);
    LCD_Cmd(_LCD_RETURN_HOME);
    //lcd end
    
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Sound_Init(&PORTC, 2);//PORTC.F2

    animacao_inicio();
     
    Lcd_Out(1,13,"WELCOME TO");
    Lcd_Out(2,12,"GENIUS GAME");
    for(i=0; i<10; i++) {               // Move text to the right 4 times
      Lcd_Cmd(_LCD_SHIFT_LEFT);
      Delay_ms(300);
    }

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,2,"Press any key");

    carac = le_carac();
    while(carac == 255)
    {
       semente++;
       carac = le_carac();
    }
    Lcd_Cmd(_LCD_CLEAR);

    if (carac == '1')
    {
       escreve(127, 0);
       pontuacao_record = 0;
       Lcd_Out(1, 2, "Record erased");
       Delay_ms(1500);
       Lcd_Cmd(_LCD_CLEAR);
    }
    else
    {
       pontuacao_record = le(127);
       Lcd_Out(1,2,"Best score:");
       ShortToStr(pontuacao_record,TEMP_STR);
       Lcd_Out(2,6,TEMP_STR);
       Delay_ms(1500);
       Lcd_Cmd(_LCD_CLEAR);
    }
    
    srand(semente);
    Lcd_Out(1,5,"Ready?");
    Delay_ms(900);
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,7,"GO!");
    Delay_ms(1000);
    Lcd_Cmd(_LCD_CLEAR);
       
    while(!errou)
    {
       sequencia_num[nivel_atual] = rand() & 0b00000011;
       imprime_sequencia_num();
       errou = !le_sequencia_jogador();

       if (errou)
       {
         Lcd_Cmd(_LCD_CLEAR);
         Lcd_Out(1,5,"FAIL :(");
         toca_som(300, 450);
         toca_som(600, 450);
         Delay_ms(2000);
         Lcd_Cmd(_LCD_CLEAR);
         
         Lcd_Out(1,4,"Your score: ");
         IntToStr(nivel_atual,TEMP_STR);
         Lcd_Out(2,5,TEMP_STR);
         Delay_ms(1500);
         Lcd_Cmd(_LCD_CLEAR);
         
         if (nivel_atual > pontuacao_record)
         {
          escreve(127, nivel_atual);//novo record
          Lcd_Out(1,1,"Congrats!");
          Lcd_Out(2,3,"New record!");
          Delay_ms(1000);
          Lcd_Cmd(_LCD_CLEAR);
         }
       }
       else
       {
         nivel_atual++;
         Lcd_Cmd(_LCD_CLEAR);
         Lcd_Out(1,6,"GOOD!");
         Delay_ms(1000);
         Lcd_Cmd(_LCD_CLEAR);
       }
    }
}