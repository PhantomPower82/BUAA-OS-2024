#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <trap.h>

extern void handle_int(void);
extern void handle_tlb(void);
extern void handle_sys(void);
extern void handle_mod(void);
extern void handle_reserved(void);
extern void handle_ri(void);

void (*exception_handlers[32])(void) = {
    [0 ... 31] = handle_reserved,
    [0] = handle_int,
    [2 ... 3] = handle_tlb,
    [10] = handle_ri,
#if !defined(LAB) || LAB >= 4
    [1] = handle_mod,
    [8] = handle_sys,
#endif
};

/* Overview:
 *   The fallback handler when an unknown exception code is encountered.
 *   'genex.S' wraps this function in 'handle_reserved'.
 */
void do_reserved(struct Trapframe *tf) {
	print_tf(tf);
	panic("Unknown ExcCode %2d", (tf->cp0_cause >> 2) & 0x1f);
}

void do_ri(struct Trapframe *tf) {
	u_long va = tf->cp0_epc;
	Pte *pte;
	page_lookup(curenv->env_pgdir, va, &pte);
	u_long pa = PTE_ADDR(*pte) | (va & 0xfff);
	u_long kva = KADDR(pa);
	u_int *instr = (int *)kva;
	u_int code = (*instr) & (0x3ff);
	u_int *rs = &tf->regs[(*instr) >> 21 & (0x1f)];
	u_int *rt = &tf->regs[(*instr) >> 16 & (0x1f)];
	u_int *rd = &tf->regs[(*instr) >> 11 & (0x1f)];
	if (((*instr) >> 26) == 0) {
	if (code == 0x3f) {
		*rd = 0;
		for (int i = 0; i < 32; i += 8) {
		   u_int rs_i = *rs & (0xff << i);
		   u_int rt_i = *rt & (0xff << i);
		   if (rs_i < rt_i) {
		      *rd = *rd | rt_i;
		   } else {
		      *rd = *rd | rs_i;
		   }
		}
        } else if (code == 0x3e) {
		u_int tmp = *((u_int *)*rs);
		if (*((u_int *)*rs) == *rt) {
		   *((u_int *)*rs) = *rd;
		}
		*rd = tmp;
	}
	}
        tf->cp0_epc += 4;
}

