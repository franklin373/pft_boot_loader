#ifndef __FLASH_COMMON_H__
#define __FLASH_COMMON_H__
#include "global.h"

struct tag_flash_common_interface;
typedef struct tag_flash_common_interface FLASH_COMMON_INTERFACE;
typedef uint32 *(*FCI_FUNC_GET_SECTOR_ADDRESS)();
typedef int (*FCI_FUNC_GET_PROGRAM_UNIT_SIZE)();
typedef int (*FCI_FUNC_ERASE_SECTOR)(int sectorNumber);
typedef int (*FCI_FUNC_PROGRAM)(uint32 addrDest,uint8 *addrSrc);

#define FLASH_COMMON_INTERFACE_DECLARE                           \
    FCI_FUNC_GET_SECTOR_ADDRESS fci_get_sector_address;                       \
    FCI_FUNC_GET_PROGRAM_UNIT_SIZE fci_get_program_unit_size;                       \
    FCI_FUNC_ERASE_SECTOR fci_erase_sector;                       \
    FCI_FUNC_PROGRAM fci_program;                   \



struct tag_flash_common_interface{
    FLASH_COMMON_INTERFACE_DECLARE
};


#if 0
void iphone_conn_class_base_init_helper(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort,const char *ici_type);
void iphone_conn_class_windgo_init(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort);
void iphone_conn_class_never_init(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort);
void iphone_conn_class_cable_init(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort);
void iphone_conn_class_bt_init(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort);
void iphone_conn_class_bt_flaircomm_init(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort);
void iphone_conn_class_bt_rayson_init(IPHONE_CONN_INTERFACE *pIntf,uint8 whichPort);
#endif

void flash_common_flash1788_init(FLASH_COMMON_INTERFACE *pIntf);
void flash_common_flash_sst39vf6401b_init(FLASH_COMMON_INTERFACE *pIntf);
int flash_intf_init();
int flash_erase(uint32 addr,int size);
int flash_program(uint32 addr,uint8 *buf,int len);
void flash_disp_for_debug();


char *ICIF_type(int whichPort);
int ICIF_is_iphone_port(int whichPort);
int ICIF_is_physical_connected(int whichPort);
int ICIF_is_iphone_connected(int whichPort);
int ICIF_wrong_baud_curr(int whichPort);
int ICIF_rtrv_baud(int whichPort);
int ICIF_set_baud(int whichPort,int baud);
int*ICIF_get_baud_possible(int whichPort);
int ICIF_set_bt_name_nocache(int whichPort,const char *name);
int ICIF_get_bt_name_nocache(int whichPort,char *name);
int ICIF_get_bt_name_cache(int whichPort,char *name);
int ICIF_set_bt_pincode_nocache(int whichPort,const char *pincode);
int ICIF_get_bt_pincode_nocache(int whichPort,char *pincode);
int ICIF_bt_setting_init(int whichPort);
int ICIF_bt_initial_iPhone_connect(int whichPort,const char *bt_addr);
int ICIF_bt_initial_iPhone_disconnect(int whichPort);
void ICIF_may_triggle_by_tick(int whichPort);
void ICIF_may_triggle_by_data(int whichPort,const char *data,int len);
void ICIF_port_reset(int whichPort);
void ICIF_port_init(int whichPort);
void ICIF_dump_bt_settingt(int whichPort);
void ICIF_init(void);
void ICIF_delay_update(void);



#endif


