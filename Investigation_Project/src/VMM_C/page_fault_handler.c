#include "page_fault_handler.h"
#include "page_table_manager.h"
#include "constants.h"
#include "physical_memory_manager.h"
#include "backing_store_manager.h"
#include "page_table_manager.h"
#include "vga.h"   // para printf es el del kernel


extern int  pmm_allocate_frame(physical_memory_manager_t* pmm);             


void pfh_init(page_fault_handler_t* pfh,
              physical_memory_manager_t* pmm,
              backing_store_manager_t* bsm)
{
    pfh->pmm = pmm;
    pfh->bsm = bsm;
}

int pfh_handle_page_fault(page_fault_handler_t* pfh,
                          page_table_manager_t* ptm,
                          uint8_t vpn)
{
    int frame = pmm_allocate_frame(pfh->pmm);
    if(frame < 0) return -1;

    // si el frame ya estaba ocupado, hay víctima (FIFO)
    int old_vpn = pmm_get_page_in_frame(pfh->pmm, (size_t)frame);
    if(old_vpn != -1){
        // writeback si estaba dirty
        if(ptm_is_dirty(ptm, (uint32_t)old_vpn)){
            char tmp[PAGE_SIZE];
            pmm_read_frame(pfh->pmm, (size_t)frame, tmp, PAGE_SIZE);
            bsm_save_page(pfh->bsm, (size_t)old_vpn, tmp);
            ptm_clear_dirty(ptm, (uint32_t)old_vpn);
        }
        // invalidar page table de la víctima
        ptm_invalidate(ptm, (uint32_t)old_vpn);
    }

    // cargar página solicitada desde backing store al frame
    char buffer[PAGE_SIZE];
    bsm_load_page(pfh->bsm, (size_t)vpn, buffer);
    pmm_write_frame(pfh->pmm, (size_t)frame, buffer, PAGE_SIZE);

    // actualizar mapping y PTE
    pmm_set_frame_mapping(pfh->pmm, (size_t)frame, (int)vpn);
    ptm_set_frame(ptm, (uint32_t)vpn, (uint32_t)frame);

#if 1
    printf("[PFH] vpn=%d -> frame=%d (old_vpn=%d)\n", (int)vpn, frame, old_vpn);
#endif

    return frame;
}
