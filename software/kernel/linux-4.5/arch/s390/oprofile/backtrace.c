/*
 * S390 Version
 *   Copyright IBM Corp. 2005
 *   Author(s): Andreas Krebbel <Andreas.Krebbel@de.ibm.com>
 */

#include <linux/oprofile.h>

#include <asm/processor.h> /* for struct stack_frame */

static unsigned long
__show_trace(unsigned int *depth, unsigned long sp,
	     unsigned long low, unsigned long high)
{
	struct stack_frame *sf;
	struct pt_regs *regs;

	while (*depth) {
		if (sp < low || sp > high - sizeof(*sf))
			return sp;
		sf = (struct stack_frame *) sp;
		(*depth)--;
		oprofile_add_trace(sf->gprs[8]);

		/* Follow the backchain.  */
		while (*depth) {
			low = sp;
			sp = sf->back_chain;
			if (!sp)
				break;
			if (sp <= low || sp > high - sizeof(*sf))
				return sp;
			sf = (struct stack_frame *) sp;
			(*depth)--;
			oprofile_add_trace(sf->gprs[8]);

		}

		if (*depth == 0)
			break;

		/* Zero backchain detected, check for interrupt frame.  */
		sp = (unsigned long) (sf + 1);
		if (sp <= low || sp > high - sizeof(*regs))
			return sp;
		regs = (struct pt_regs *) sp;
		(*depth)--;
		oprofile_add_trace(sf->gprs[8]);
		low = sp;
		sp = regs->gprs[15];
	}
	return sp;
}

void s390_backtrace(struct pt_regs * const regs, unsigned int depth)
{
	unsigned long head, frame_size;
	struct stack_frame* head_sf;

	if (user_mode(regs))
		return;

	frame_size = STACK_FRAME_OVERHEAD + sizeof(struct pt_regs);
	head = regs->gprs[15];
	head_sf = (struct stack_frame*)head;

	if (!head_sf->back_chain)
		return;

	head = head_sf->back_chain;

	head = __show_trace(&depth, head,
			    S390_lowcore.async_stack + frame_size - ASYNC_SIZE,
			    S390_lowcore.async_stack + frame_size);

	__show_trace(&depth, head, S390_lowcore.thread_info,
		     S390_lowcore.thread_info + THREAD_SIZE);
}