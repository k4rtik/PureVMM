// monitor.c -- Defines functions for writing to the monitor.
//             heavily based on Bran's kernel development tutorials,
//             but rewritten for JamesM's kernel tutorials.

#include "monitor.h"
#include "stdarg.h"

/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')


#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })


// The VGA framebuffer starts at 0xB8000.
u16int *video_memory = (u16int *)0xB8000;
// Stores the cursor position.
u8int cursor_x = 0;
u8int cursor_y = 0;

// Updates the hardware cursor.
static void move_cursor()
{
   // The screen is 80 characters wide...
   u16int cursorLocation = cursor_y * 80 + cursor_x;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{

   // Get a space character with the default colour attributes.
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(cursor_y >= 25)
   {
       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           video_memory[i] = video_memory[i+80];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = 24*80; i < 25*80; i++)
       {
           video_memory[i] = blank;
       }
       // The cursor should now be on the last line.
       cursor_y = 24;
   }
}

// Writes a single character out to the screen.
void monitor_put(char c)
{
   // The background colour is black (0), the foreground is white (15).
   u8int backColour = 0;
   u8int foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   u16int attribute = attributeByte << 8;
   u16int *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x)
   {
       cursor_x--;
   }

   // Handle a tab by increasing the cursor's X, but only to a point
   // where it is divisible by 8.
   else if (c == 0x09)
   {
       cursor_x = (cursor_x+8) & ~(8-1);
   }

   // Handle carriage return
   else if (c == '\r')
   {
       cursor_x = 0;
   }

   // Handle newline by moving cursor back to left and increasing the row
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   // Handle any other printable character.
   else if(c >= ' ')
   {
       location = video_memory + (cursor_y*80 + cursor_x);
       *location = c | attribute;
       cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= 80)
   {
       cursor_x = 0;
       cursor_y ++;
   }

   // Scroll the screen if needed.
   scroll();
   // Move the hardware cursor.
   move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
   // Make an attribute byte for the default colours
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   int i;
   for (i = 0; i < 80*25; i++)
   {
       video_memory[i] = blank;
   }

   // Move the hardware cursor back to the start.
   cursor_x = 0;
   cursor_y = 0;
   move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
   int i = 0;
   while (c[i])
   {
       monitor_put(c[i++]);
   }
}

void monitor_write_hex(u32int n)
{
    s32int tmp;

    monitor_write("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            monitor_put (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            monitor_put( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        monitor_put (tmp-0xA+'a');
    }
    else
    {
        monitor_put (tmp+'0');
    }

}

void monitor_write_dec(u32int n)
{

    if (n == 0)
    {
        monitor_put('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    monitor_write(c2);

}

/* For vsprintf like functionality */

/*
 *  linux/kernel/vsprintf.c
 *
 *  (C) 1991  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */

static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

static char * number(char * str, int num, int base, int size, int precision
	,int type)
{
	char c,sign,tmp[36];
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
	if (type&LEFT) type &= ~ZEROPAD;
	if (base<2 || base>36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;
	if (type&SIGN && num<0) {
		sign='-';
		num = -num;
	} else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	if (sign) size--;
	if (type&SPECIAL)
		if (base==16) size -= 2;
		else if (base==8) size--;
	i=0;
	if (num==0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];
	if (i>precision) precision=i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type&SPECIAL)
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type&LEFT))
		while(size-->0)
			*str++ = c;
	while(i<precision--)
		*str++ = '0';
	while(i-->0)
		*str++ = tmp[i];
	while(size-->0)
		*str++ = ' ';
	return str;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	int len;
	int i;
	char * str;
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}
		
		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			break;

		case 's':
			s = va_arg(args, char *);
			len = strlen(s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o':
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(args, int *);
			*ip = (str - buf);
			break;

		default:
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';
	return str-buf;
}

int printf(char *fmt, ...)
{
	char *p;	
	int count = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	count += vsprintf(p, fmt, arg_ptr);
	va_end(arg_ptr);
	monitor_write(p);

	return count;
}
