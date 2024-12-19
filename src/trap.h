
extern trp_ex_f trp_ex[8];
typedef void (*trp_ex_f)();

// Trap Routines
typedef enum __trapCode
{
    TRAP_GETC = 0x20,  /* get character from keyboard, not echoed onto the terminal */
    TRAP_OUT = 0x21,   /* output a character */
    TRAP_PUTS = 0x22,  /* output a word string */
    TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
    TRAP_PUTSP = 0x24, /* output a byte string */
    TRAP_HALT = 0x25   /* halt the program */
} trap_code;


void TRAP(uint16_t instr);


void tgetch();
void tout();
void tputs();
void tin();
void tputsp();
void thalt();
void tinu16();
void toutu16();