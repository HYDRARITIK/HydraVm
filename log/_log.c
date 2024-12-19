#include "_log.h"

void printf_binary(FILE *f, uint16_t num)
{
    // 4 = 00000000100 (16 bit)
    int c = 16;
    while (c--)
    {
        fprintf(f, "%d", (num >> c) & 1);
        num >> 1;
    }
}

void printf_inst(FILE *f, uint16_t instr)
{
    printf_binary(f, instr);
}

void fprintf_mem(FILE *f, uint16_t *mem, uint16_t from_addr, uint16_t to_addr)
{

    uint16_t *i;
    for (i = mem + from_addr; i <= mem + to_addr; i++)
    {
        printf_binary(f, *i);
    }
}
void fprintf_reg(FILE *f, uint16_t *reg, int idx)
{
    fprintf(stdout, "reg[%d]=0x%.04x\n", idx, reg[idx]);
}
void printf_reg_all(FILE *f, uint16_t *reg, int size)
{
    for (int i = 0; i < size; i++)
    {
        fprintf_reg(f, reg, i);
    }
}

int main(int argc, char *argv[])
{
    const char *pName = "sample.txt";

    FILE *_f;
    _f = fopen(pName, "w+");
    uint16_t _num = 6789;
    printf_binary(_f, _num);
}