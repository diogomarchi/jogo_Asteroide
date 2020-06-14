/* 
 * File:   lcd_hd_44780.c
 * Author: 
 * Diogo Agenor Marchi 
 * George de Borba Nardes
 *          
 *  
 */
#include "lcd.h"

void pulso(t_display_port *lcd){        
    /* habilita display */
    lcd->E = 1;
    __delay_ms(1);
    
    /* desabilita display */
    lcd->E = 0;                
    __delay_ms(1);    
}

void function_set(t_display_port *lcd, char data_lenght, 
        char num_lines, char char_font){        
    lcd->E = 0;
    lcd->RS = 0;
    lcd->R_W = 0;
    
    /* function set habilitado */
    lcd->data = 0x02;        
    __delay_ms(1);
    
    pulso(lcd);
    
    /* tamanho de dados */
    lcd->data = 0x02|data_lenght;    
    __delay_ms(1);
    
    pulso(lcd);
             
    /* -- segunda parte -- */            
    /* numero de linhas e fonte */
    lcd->data = lcd->data|(num_lines<<3)|(char_font<<2);    
    __delay_ms(1);
    
    pulso(lcd);    
}

void display_onoff_control(t_display_port *lcd, char display_on, 
        char cursor_on, char blink){
    lcd->E = 0;
    lcd->RS = 0;
    lcd->R_W = 0;
        
    /* primeira parte */
    lcd->data = 0x00;
    __delay_ms(1);
    
    pulso(lcd);
    
    /* -- segunda parte -- */            
    /* ativa display - cursor_on - blink */
    lcd->data = (0x08)|(display_on<<2)|(cursor_on<<1)|(blink);    
    __delay_ms(1);
    
    pulso(lcd);              
}

void lcd_cmd(t_display_port *lcd, char a){
    lcd->RS = 0;
    lcd->data = (a);
    __delay_ms(1);
    
    pulso(lcd);        
}

//fun��o para posicionar o cursor
void goto_XY(t_display_port *lcd, unsigned char x, unsigned char y){
    unsigned char temp,z,a;
    if(x == 1)              //verifica se � primeira linha
    {
       temp = 0x80 + y - 1; //posi��o inicial da primeira linha mais coluna, menos 1
       z = temp>>4;         //pegando bits mais significativos
       a = temp;            //pegando bit menos significativos
       lcd_cmd(lcd, z);
       __delay_ms(1);
       lcd_cmd(lcd, a);
    }
    else if(x == 2)         //verifica se � segunda linha
    {
       temp = 0xC0 + y - 1; //posi��o inicial da segunda linha mais coluna, menos 1
       z = temp>>4;         //pegando bits mais significativos
       a = temp;            ////pegando bit menos significativos
       lcd_cmd(lcd, z);
       __delay_ms(1);
       lcd_cmd(lcd, a);
    } 
    else if(x == 3)         //verifica se � segunda linha
    {
       temp = 0x90 + y - 1; //posi��o inicial da segunda linha mais coluna, menos 1
       z = temp>>4;         //pegando bits mais significativos
       a = temp;            ////pegando bit menos significativos
       lcd_cmd(lcd, z);
       __delay_ms(1);
       lcd_cmd(lcd, a);
    } 
    else if(x == 4)         //verifica se � segunda linha
    {
       temp = 0xD0 + y - 1; //posi��o inicial da segunda linha mais coluna, menos 1
       z = temp>>4;         //pegando bits mais significativos
       a = temp;            ////pegando bit menos significativos
       lcd_cmd(lcd, z);
       __delay_ms(1);
       lcd_cmd(lcd, a);
    } 
}

void entry_mode_set(t_display_port *lcd, char move_direction, char display_shift){
    lcd->E = 0;
    lcd->RS = 0;
    lcd->R_W = 0;
    
    lcd->data = 0x00;
    __delay_ms(1);
    
    pulso(lcd);
    
    /* -- segunda parte -- */ 
    lcd->data = 0x04|(move_direction<<1)|display_shift;
    __delay_ms(1);
    
    pulso(lcd);            
}


//Le o status da busy flag
void waitFlag(t_display_port *lcd){
    
    TRISD = 0xF0;//D0 a D3 como entrada
    lcd->RS = 0;
    lcd->R_W = 1;//Operacao de leitura
    
    
    do{        
       pulso(lcd);
    } while(lcd->data&0x08);
        
    
    lcd->R_W = 0;
    TRISD = 0x00;//Todos como saida 
}

void write_char(t_display_port *lcd, char c){
    lcd->E = 0;    
    lcd->RS = 1;
    lcd->R_W = 0;    
    
    /* primeira parte do char */
    lcd->data = (c>>4);        
    lcd->E = 1;    
    lcd->E = 0;   
    
    /* segunda parte do char */
    lcd->data = (c);    
    lcd->E = 1;     
    lcd->E = 0; 
    
////////// check busy flag ///////////////
    TRISD = 0xF0;//D7 a D4 como entrada
    lcd->RS = 0;
    lcd->R_W = 1;//Operacao de leitura        
    do{        
       lcd->E = 1;
       lcd->E = 0;
       lcd->E = 1;
       lcd->E = 0;
    } while(lcd->data&0x08); //             
    lcd->R_W = 0;
    TRISD = 0x00;//Todos como saida 
////////////////////////
}

void clear_display(t_display_port *lcd){
    lcd->E = 0;
    lcd->RS = 0;
    lcd->R_W = 0;
    
    /* primeira parte */    
    lcd->data = 0x0;
    __delay_ms(1);
    
    pulso(lcd);
    
    /* segunda parte */
    lcd->data = 0x1;
    __delay_ms(1);
    
    pulso(lcd);
}

void return_home(t_display_port *lcd){
    lcd->E = 0;
    lcd->RS = 0;
    lcd->R_W = 0;
    
    /* primeira parte */    
    lcd->data = 0x0;
    __delay_ms(1);
    
    pulso(lcd);
    
    /* segunda parte */
    lcd->data = 0x2;
    __delay_ms(1);
    
    pulso(lcd);    
}

void print_mat(t_display_port *lcd){
    for(int i = 0; i<4; i++){
        goto_XY(lcd, i+1, 1);
        for(int j=0; j<TAM; j++)
            write_char(lcd, mat_disp[i][j]);
    }        
}


