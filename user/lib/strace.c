#include <lib.h>

void strace_barrier(u_int env_id) {
	int straced_bak = straced;
	straced = 0;
	while (envs[ENVX(env_id)].env_status == ENV_RUNNABLE) {
		syscall_yield();
	}
	straced = straced_bak;
}

void strace_send(int sysno) {
	if (!((SYS_putchar <= sysno && sysno <= SYS_set_tlb_mod_entry) ||
	      (SYS_exofork <= sysno && sysno <= SYS_panic)) ||
	    sysno == SYS_set_trapframe) {
		return;
	}

	// Your code here. (1/2)
	if (straced == 0) return;
	int straced_bak = straced;
	straced = 0;
	ipc_send(env->env_parent_id, sysno, NULL, 0);
	syscall_set_env_status(env->env_id, ENV_NOT_RUNNABLE);
	straced = straced_bak;
}

void strace_recv() {
	// Your code here. (2/2)
	int sysno;
	u_int whom;
	while ((sysno = ipc_recv(&whom, 0, 0))) {
		strace_barrier(whom);
		recv_sysno(whom, sysno);
		syscall_set_env_status(whom, ENV_RUNNABLE);
		if (sysno == SYS_env_destroy) break;
	}
}
