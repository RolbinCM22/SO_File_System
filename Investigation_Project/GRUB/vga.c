#include "vga.h"
#include <stdarg.h>
#include <stdint.h>

#define VGA_W 80
#define VGA_H 25
static volatile uint16_t* const VGA = (volatile uint16_t*)0xB8000;

static int cur_row = 0, cur_col = 0;
static uint8_t cur_color = 0x0F;

void console_set_color(uint8_t color){ cur_color = color; }

static void scroll_if_needed(void){
    if(cur_row < VGA_H) return;

    // scroll up by 1 line
    for(int r=1; r<VGA_H; r++){
        for(int c=0; c<VGA_W; c++){
            VGA[(r-1)*VGA_W + c] = VGA[r*VGA_W + c];
        }
    }
    // clear last line
    for(int c=0; c<VGA_W; c++){
        VGA[(VGA_H-1)*VGA_W + c] = (uint16_t)' ' | ((uint16_t)0x07<<8);
    }
    cur_row = VGA_H-1;
    cur_col = 0;
}

void console_putc(char c){
    if(c=='\n'){
        cur_row++; cur_col=0;
        scroll_if_needed();
        return;
    }
    if(c=='\r'){ cur_col=0; return; }
    if(c=='\t'){ cur_col = (cur_col + 4) & ~3; return; }

    if(c=='\b'){ // backspace
        if(cur_col>0) cur_col--;
        VGA[cur_row*VGA_W + cur_col] = (uint16_t)' ' | ((uint16_t)cur_color<<8);
        return;
    }

    if(cur_col >= VGA_W){ cur_row++; cur_col=0; scroll_if_needed(); }
    VGA[cur_row*VGA_W + cur_col] = (uint16_t)c | ((uint16_t)cur_color<<8);
    cur_col++;
}

void console_write(const char* s){
    while(s && *s) console_putc(*s++);
}

void console_writeln(const char* s){
    console_write(s);
    console_putc('\n');
}

void cls(void){
    for(int i=0;i<VGA_W*VGA_H;i++) VGA[i] = (uint16_t)' ' | ((uint16_t)0x07<<8);
    cur_row = 0; cur_col = 0; cur_color = 0x0F;
}

void puts_at(int row, int col, const char* s, uint8_t color){
    int r=row, c=col;
    while(*s){
        if(c>=VGA_W){ r++; c=0; }
        if(r>=VGA_H) break;
        VGA[r*VGA_W + c] = (uint16_t)(*s) | ((uint16_t)color<<8);
        s++; c++;
    }
}

/* printf minimal */
static void print_dec(int x){
    char buf[12]; int i=0;
    if(x==0){ console_putc('0'); return; }
    if(x<0){ console_putc('-'); x=-x; }
    while(x>0 && i<11){ buf[i++] = '0' + (x%10); x/=10; }
    while(i--) console_putc(buf[i]);
}

int printf(const char* fmt, ...){
    va_list ap; va_start(ap, fmt);
    for(; *fmt; fmt++){
        if(*fmt!='%'){ console_putc(*fmt); continue; }
        fmt++;
        if(*fmt=='d'){ int v=va_arg(ap,int); print_dec(v); }
        else if(*fmt=='s'){ const char* s=va_arg(ap,const char*); while(s && *s) console_putc(*s++); }
        else if(*fmt=='c'){ int ch=va_arg(ap,int); console_putc((char)ch); }
        else { console_putc('%'); console_putc(*fmt); }
    }
    va_end(ap);
    return 0;
}
