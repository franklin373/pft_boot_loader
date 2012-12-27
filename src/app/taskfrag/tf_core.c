#include "global.h"
#include "taskfrag.h"

/*
typedef void (*TF_FUNC)();
struct tagTF_Define{
	uint32 period;
	TF_FUNC func;
}
struct tagTF_Data{
	uint32 period;
	TF_FUNC func;
	uint32 trigger_tick;
}
*/
static struct tagTF_Data TF_DataArray[10];
static int TF_DataArrayLen;
#define UINT32_ADD_SAFE_NO_OVERFLOW(sum,a,b) do{									\
    CPU_SR_ALLOC();																	\
    OS_ENTER_CRITICAL();                                   							\
	sum=((uint32)(a)+(uint32)(b)>=(uint32)(a)?(uint32)(a)+(uint32)(b):0xFFFFFFFF);	\
    OS_EXIT_CRITICAL();																\
}while(0)

int TF_init(struct tagTF_Define tf_define[],int tf_cnt)
{
	int i;
	
	if(tf_cnt>ARR_SIZE(TF_DataArray)){
		ASSERT_MINE(0);
	}
	TF_DataArrayLen=tf_cnt;
	for(i=0;i<tf_cnt;++i){
		TF_DataArray[i].period=tf_define[i].period;
		TF_DataArray[i].func=tf_define[i].func;
		UINT32_ADD_SAFE_NO_OVERFLOW(TF_DataArray[i].trigger_tick,OSTime,tf_define[i].period);
	}
	return 0;
}
void TF_dispatch()
{
	int i;

	for(i=0;i<TF_DataArrayLen;i++){
		if(TF_DataArray[i].trigger_tick<=OSTime){
			TF_DataArray[i].func();
			UINT32_ADD_SAFE_NO_OVERFLOW(TF_DataArray[i].trigger_tick,OSTime,TF_DataArray[i].period);
		}
	}
}
void TF_schedule(int tf_id,int run_after)
{
	UINT32_ADD_SAFE_NO_OVERFLOW(TF_DataArray[tf_id].trigger_tick,OSTime,run_after);
}
