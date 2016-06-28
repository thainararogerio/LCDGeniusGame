unsigned short mapaDigitos[11];
unsigned short sequencia_num[100];
unsigned short displays[4];
unsigned short displayAtual = 0;
unsigned short botao = 0;
unsigned short tecla = 0;
unsigned short pontuacao_record = 0;

unsigned int nivel_atual = 0;
unsigned int semente = 0;
unsigned int errou = 0;

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

void toca_som(unsigned int a,unsigned int b)
{
    TMR0IE_BIT = 0;
    Sound_Play(a, b);
    TMR0IE_BIT = 1;
}

void limpa_display()
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

void escreve_display(unsigned short d, unsigned short numero) {
    PORTE = 0;
    PORTA.F5 = 0;

    if (numero == 255) {
       PORTD = 0x00;
    } else {
       PORTD = mapaDigitos[numero];
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

    escreve_display(displayAtual+1, displays[displayAtual]);
    displayAtual++;

    if (displayAtual > 3)  {
                displayAtual = 0;
    }
}

void popula_displays_num(unsigned int num) {
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

void popula_displays_palavra(char *palavra)
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

void preenche_mapa_digitos()
{
    mapaDigitos[0] = S_A + S_B + S_C + S_D + S_E + S_F;
    mapaDigitos[1] = S_B + S_C;
    mapaDigitos[2] = S_A + S_B + S_D + S_E + S_G;
    mapaDigitos[3] = S_A + S_B + S_C + S_D + S_G;
    mapaDigitos[4] = S_B + S_C + S_F + S_G;
    mapaDigitos[5] = S_A + S_C + S_D + S_F + S_G;
    mapaDigitos[6] = S_A + S_C + S_D + S_E + S_F + S_G;
    mapaDigitos[7] = S_A + S_B + S_C;
    mapaDigitos[8] = S_A + S_B + S_C + S_D + S_E + S_F + S_G;
    mapaDigitos[9] = S_A + S_B + S_C + S_F + S_G;
    mapaDigitos['A'] = S_A + S_B + S_C + S_E + S_F + S_G;
    mapaDigitos['B'] = S_C + S_D + S_E + S_F + S_G;
    mapaDigitos['C'] = S_A + S_D + S_E + S_F;
    mapaDigitos['D'] = S_B + S_C + S_D + S_E + S_G;
    mapaDigitos['E'] = S_A + S_E + S_F + S_G + S_D;
    mapaDigitos['F'] = S_A + S_E + S_F + S_G;
    mapaDigitos['G'] = S_A + S_D + S_E + S_C + S_F + S_G;
    mapaDigitos['H'] = S_B + S_C + S_E + S_F + S_G;
    mapaDigitos['I'] = S_B + S_C;
    mapaDigitos['L'] = S_D + S_E + S_F;
    mapaDigitos['N'] = S_E + S_G + S_C;
    mapaDigitos['O'] = S_A + S_B + S_C + S_D + S_E + S_F;
    mapaDigitos['P'] = S_A + S_B + S_E + S_F + S_G;
    mapaDigitos['R'] = S_E + S_G;
    mapaDigitos['S'] = S_A + S_C + S_D + S_F + S_G;
    mapaDigitos['T'] = S_B + S_C + S_G;
    mapaDigitos['U'] = S_B + S_C + S_D + S_E + S_F;
    mapaDigitos['V'] = S_B + S_C + S_D + S_E + S_F;
    mapaDigitos['Y'] = S_B + S_C + S_D + S_F + S_G;
    mapaDigitos['Z'] = S_A + S_B + S_D + S_E + S_G;
}

void imprime_sequencia_num()
{
   int i;
   for (i = 0; i <= nivel_atual; ++i)
   {
      switch(sequencia_num[i])
      {
         case 0:
           popula_displays_num(1111);
           toca_som(466, 150);
           break;
         case 1:
           popula_displays_num(2222);
           toca_som(587, 150);
           break;
         case 2:
           popula_displays_num(3333);
           toca_som(698, 150);
           break;
         case 3:
           popula_displays_num(4444);
           toca_som(392, 150);
           break;
      }
      Delay_ms(300);
      limpa_display();
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
           if (acertou) { popula_displays_num(1111);toca_som(466, 150); }
           break;
      case '2':
           acertou = (sequencia_num[i] == 1);
           if (acertou) { popula_displays_num(2222);toca_som(587, 150); }
           break;
      case '3':
           acertou = (sequencia_num[i] == 2);
           if (acertou) { popula_displays_num(3333);toca_som(698, 150); }
           break;
      case '4':
           acertou = (sequencia_num[i] == 3);
           if (acertou) { popula_displays_num(4444);toca_som(392, 150); }
           break;
      case 255:
           acertou = 0;
           break;
     }
     Delay_ms(100);
     limpa_display();
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

    init_timer();//começa a escutar as teclas

    adcon1 = 0x0F;

    preenche_mapa_digitos();
    limpa_display();
        
    Sound_Init(&PORTC, 2);//PORTC.F2

    limpa_display();
    popula_displays_palavra("CLIC");
    carac = le_carac();
    while(carac == 255)
    {
       semente++;
       carac = le_carac();
    }
    limpa_display();

    if (carac == '1')
    {
       escreve(127, 0);
       pontuacao_record = 0;
       popula_displays_palavra("NO");
       Delay_ms(1000);
       popula_displays_palavra("RECO");
       Delay_ms(1500);
    }
    else
    {
       pontuacao_record = le(127);
       popula_displays_palavra("RECO");
       Delay_ms(1000);
       limpa_display();
       Delay_ms(500);
       popula_displays_num(pontuacao_record);
       Delay_ms(1500);
    }
    
    srand(semente);
    limpa_display();
    Delay_ms(500);
    popula_displays_palavra("PLAY");
    Delay_ms(1500);
       
    while(!errou)
    {
       sequencia_num[nivel_atual] = rand() & 0b00000011;
       imprime_sequencia_num();
       errou = !le_sequencia_jogador();

       if (errou)
       {
         limpa_display();
         popula_displays_palavra("FAIL");
         toca_som(300, 450);
         toca_som(600, 450);
         Delay_ms(2000);
         
         popula_displays_palavra("SCOR");
         Delay_ms(1500);
         popula_displays_num(nivel_atual);
         Delay_ms(1500);
         limpa_display();
         
         if (nivel_atual > pontuacao_record)
         {
          escreve(127, nivel_atual);//novo record
          popula_displays_palavra("UHUL");
          Delay_ms(1000);
          popula_displays_palavra("NOVO");
          Delay_ms(1000);
          popula_displays_palavra("RECO");
          Delay_ms(1000);
          limpa_display();
         }
       }
       else
       {
         nivel_atual++;
         limpa_display();
         popula_displays_palavra("GOOD");
         Delay_ms(1000);
       }
    }
}