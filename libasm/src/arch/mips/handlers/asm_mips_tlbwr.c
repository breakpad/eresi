/* Adam 'pi3' Zabrocki */
/* Manuel Martin - 2007 */

#include <libasm.h>

/* TLBWR */

int asm_mips_tlbwr(asm_instr *ins, unsigned char *buf, unsigned int len,
                  asm_processor *proc)
{
   ins->instr = ASM_MIPS_TLBWR;
   ins->type = ASM_TYPE_ARCH | ASM_TYPE_CONTROL;
   asm_mips_operand_fetch(&ins->op[0], buf, ASM_MIPS_OTYPE_NOOP, ins);

   /* Exceptions: Coprocessor Unusable */

   return 4;
}
