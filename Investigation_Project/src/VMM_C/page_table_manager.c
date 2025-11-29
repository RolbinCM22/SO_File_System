#include "page_table_manager.h"


static inline uint32_t va_vpn(uint32_t va){ return (va >> 8) & 0xFFu; }
static inline uint32_t va_off(uint32_t va){ return va & 0xFFu; }

void ptm_init(page_table_manager_t* ptm, page_fault_handler_t* pfh){
    ptm->fault_handler = pfh;
    ptm->frame_size = 256;

    for(int i=0;i<PT_PAGES;i++){
        ptm->page_table[i].frame = 0;
        ptm->page_table[i].present = 0;
        ptm->page_table[i].dirty = 0;
        ptm->page_table[i].accessed = 0;     
        ptm->dirty[i] = 0;
    }
}

uint16_t ptm_get_frame_size(const page_table_manager_t* ptm){
    return ptm->frame_size;
}

int ptm_is_page_present(const page_table_manager_t* ptm, size_t vpn){
    if(vpn >= PT_PAGES) return 0;
    return ptm->page_table[vpn].present ? 1 : 0;
}

void ptm_set_frame(page_table_manager_t* ptm, size_t vpn, uint32_t frame){
    if(vpn >= PT_PAGES) return;
    ptm->page_table[vpn].frame = (uint8_t)frame;
    ptm->page_table[vpn].present = 1;
}

void ptm_mark_modified(page_table_manager_t* ptm, size_t vpn){
    if(vpn >= PT_PAGES) return;
    ptm->page_table[vpn].dirty = 1;
    ptm->dirty[vpn] = 1;
}

void ptm_get_dirty_pages(const page_table_manager_t* ptm, uint8_t out_dirty[PT_PAGES]){
    for(int i=0;i<PT_PAGES;i++) out_dirty[i] = ptm->dirty[i];
}

uint32_t ptm_translate(page_table_manager_t* ptm, uint32_t virtual_address){
    uint32_t vpn = va_vpn(virtual_address);
    uint32_t off = va_off(virtual_address);

    if(vpn >= PT_PAGES) return 0;

    pte_t* e = &ptm->page_table[vpn];
    e->accessed = 1;

    if(!e->present){
        int frame = pfh_handle_page_fault(ptm->fault_handler, ptm, (uint8_t)vpn);
        (void)frame; // opcional si no lo usás
        e = &ptm->page_table[vpn];
    }

    return ((uint32_t)e->frame * (uint32_t)ptm->frame_size) + off;
}

int ptm_is_dirty(const page_table_manager_t* ptm, uint32_t vpn){
    if(vpn >= PT_PAGES) return 0;
    return ptm->page_table[vpn].dirty ? 1 : 0;
}

void ptm_clear_dirty(page_table_manager_t* ptm, uint32_t vpn){
    if(vpn >= PT_PAGES) return;
    ptm->page_table[vpn].dirty = 0;
    ptm->dirty[vpn] = 0;
}

void ptm_invalidate(page_table_manager_t* ptm, uint32_t vpn){
    if(vpn >= PT_PAGES) return;
    ptm->page_table[vpn].present = 0;
    ptm->page_table[vpn].accessed = 0;
    ptm->page_table[vpn].frame = 0;
}
