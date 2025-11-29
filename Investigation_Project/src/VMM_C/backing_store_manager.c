#include "backing_store_manager.h"

void bsm_init(backing_store_manager_t* bsm){
    for(int p=0; p<BACKING_PAGES; p++){
        for(int i=0; i<PAGE_SIZE; i++){
            bsm->store[p][i] = 0;
        }
    }
}

void bsm_load_page(const backing_store_manager_t* bsm, size_t vpn, char* out_buf){
    if(vpn >= (size_t)BACKING_PAGES) return;
    for(int i=0;i<PAGE_SIZE;i++) out_buf[i] = bsm->store[vpn][i];
}

void bsm_save_page(backing_store_manager_t* bsm, size_t vpn, const char* in_buf){
    if(vpn >= (size_t)BACKING_PAGES) return;
    for(int i=0;i<PAGE_SIZE;i++) bsm->store[vpn][i] = in_buf[i];
}

void bsm_init_test_patterns(backing_store_manager_t* bsm){
    for(int vpn=0; vpn<BACKING_PAGES; vpn++){
        for(int off=0; off<PAGE_SIZE; off++){
            bsm->store[vpn][off] = (char)((vpn + off) & 0xFF);
        }
    }
}

char bsm_read_byte(const backing_store_manager_t* bsm, size_t vpn, size_t offset){
    if(vpn >= (size_t)BACKING_PAGES) return 0;
    if(offset >= (size_t)PAGE_SIZE)  return 0;
    return bsm->store[vpn][offset];
}

void bsm_write_byte(backing_store_manager_t* bsm, size_t vpn, size_t offset, char value){
    if(vpn >= (size_t)BACKING_PAGES) return;
    if(offset >= (size_t)PAGE_SIZE)  return;
    bsm->store[vpn][offset] = value;
}
