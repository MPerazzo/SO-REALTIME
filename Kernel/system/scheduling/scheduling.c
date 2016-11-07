#include <stdint.h>

// structure definitions

struct StackFrame {

	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;

};

typedef struct {

	process_t * process;
	ProcessSlot * next;

} ProcessNode;


// global variables

int last_pid_given = 0;

ProcessNode * currentProcessNode = NULL;
ProcessNode * lastProcessNodeAdded = NULL;


// functions

void * switchUserToKernel(void * esp) {

	currentProcessNode->process->userStack = esp;

	return currentProcessNode->process->kernelStack;

}


void * switchKernelToUser() {

	return getNextProcess();

}

// returns RSP to next process
uint64_t getNextProcess() {

	ProcessNode current = currentProcessNode;

	current = current->next;

	while(current->process->processState != READY) {

		current = current->next;

	}

	return current->process->userStack;

}

pid_t getNewPid() {

	return ++last_pid_given;

}

void addProcess(void * entryPoint, char * name) {

	process_t * newProcess;
	processNode * newNode;

	newProcess = (process_t *) kmalloc(sizeof(process_t));
	newProcess = (ProcessNode *) kmalloc(sizeof(ProcessNode));

	newProcess->pid = getNewPid();
	newProcess->name = name;

	newProcess->stack_start = get_memblock(STACKSIZE);
	newProcess->stack_size = STACKSIZE;

	newProcess->heap_start = get_memblock(HEAPSIZE);
	newProcess->heap_size = HEAPSIZE;

	init_process_heap(process);

	newProcess->entryPoint = entryPoint;

	buildStackFrame(newProcess->stack_start);

	newProcess->state = READY;

	newProcess->signal_handler = defaultSignalHandler;

	newNode->next = lastProcessNodeAdded->next;
	lastProcessNodeAdded->next = newNode;
	lastProcessNodeAdded = newNode;


	return ;
}

void removeProcess() {



}

void initializeScheduling(void * entryPoint) {



}

void * buildStackFrame(void * entryPoint, void * userStack) {

	StackFrame stackFrame = (StackFrame *) userStack - 1;

	stackFrame->gs = 0x001;
	stackFrame->fs = 0x002;
	stackFrame->r15 = 0x003;
	stackFrame->r14 = 0x004;
	stackFrame->r13 = 0x005;
	stackFrame->r12 = 0x006;
	stackFrame->r11 = 0x007;
	stackFrame->r10 = 0x008;
	stackFrame->r9 =	0x009;
	stackFrame->r8 =	0x00A;
	stackFrame->rsi = 0x00B;
	stackFrame->rdi = 0x00C;
	stackFrame->rbp = 0x00D;
	stackFrame->rdx = 0x00E;
	stackFrame->rcx = 0x00F;
	stackFrame->rbx = 0x010;
	stackFrame->rax = 0x011;
	stackFrame->rip = (uint64_t) entry_point;
	stackFrame->cs =	0x008;
	stackFrame->eflags = 0x202;
	stackFrame->rsp = (uint64_t)&(registers->base);
	stackFrame->ss = 0x000;
	stackFrame->base = 0x000;

	return stackFrame;
}

void defaultSignalHandler() {



}

/*int kill(pid_t pid) {

	ProcessNode * queue = processQueue;

	do {

		if (queue->process->pid == pid) {

			queue->process->state = ENDED;
			queue->process->signalHandler(SIGKILL);
			return 0;

		}

		queue = queue->next;

	} while (queue != processQueue);

	return 1;

}*/