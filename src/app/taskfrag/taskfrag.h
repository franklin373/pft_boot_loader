#ifndef __TASKFRAG_H
#define __TASKFRAG_H

typedef void (*TF_FUNC)();
struct tagTF_Define{
	uint32 period;
	TF_FUNC func;
};
struct tagTF_Data{
	uint32 period;
	TF_FUNC func;
	uint32 trigger_tick;
};
int TF_init(struct tagTF_Define tf_define[],int tf_cnt);
void TF_dispatch();
void TF_schedule(int tf_id,int run_after);

#endif
