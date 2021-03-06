/**
* @file libasm/src/arch/ia32/operand_handlers/asm_operand_fetch.c
 *
 * @ingroup IA32_operands
 * @brief Implements top-level fetching handler for IA32 operands.
 *
 * $Id$
 */
#include <libasm.h>
#include <libasm-int.h>

/**
 * @brief Main function, dispatch processing to handler.
 *
 * @ingroup IA32_operands
 * @param op Pointer to operand to fill
 * @param opcode Pointer to operand data.
 * @param otype Type of operand to fetch : ASM_CONTENT_*
 * @param proc Pointer to processor structure.
 * @return Operand length or -1 on error (should currently never occur)
 */

int     asm_operand_fetch(asm_operand *operand, u_char *opcode, int otype, 
			  asm_instr *ins)
{ 
  vector_t      *vec;
  u_int         dim[1];
  int           to_ret;
  int           (*fetch)(asm_operand *, u_char *, int, asm_instr *);

  vec = aspect_vector_get(LIBASM_VECTOR_OPERAND_IA32);
  dim[0] = otype;
  
  fetch = aspect_vectors_select(vec, dim);
  to_ret = fetch(operand, opcode, otype, ins);
  if (to_ret == -1)
    {
      printf("%s:%i Unsupported operand type : %i\n", __FILE__, __LINE__, 
	     otype);
    }
  else
    {
      operand->sbaser = (operand->type == ASM_OPTYPE_REG) ||
			((operand->type == ASM_OPTYPE_MEM) && (operand->memtype & ASM_OP_BASE)) ? 
		    get_reg_intel(operand->baser, operand->regset) : "";
      operand->sindex = (operand->type == ASM_OPTYPE_MEM) && (operand->memtype & ASM_OP_INDEX) ? 
		    get_reg_intel(operand->indexr, operand->regset) : "";
    }
  return (to_ret);
}
