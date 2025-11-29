#include "physical_memory_manager.h"
#include "vga.h"     // para printf
#include <stddef.h>

void pmm_init(physical_memory_manager_t* pmm){
    for(int f=0; f<NUM_FRAMES; f++){
        for(int i=0; i<PAGE_SIZE; i++){
            pmm->mem[f][i] = 0;
        }
        pmm->in_use[f] = 0;
        pmm->frame_to_page[f] = -1;
    }
    pmm->next_victim = 0;
}

int pmm_allocate_frame(physical_memory_manager_t* pmm){
    // Buscar libre
    for(int f=0; f<NUM_FRAMES; f++){
        if(!pmm->in_use[f]){
            pmm->in_use[f] = 1;
            return f;
        }
    }

    int victim = (int)pmm->next_victim;
    pmm->next_victim = (uint8_t)((pmm->next_victim + 1) % NUM_FRAMES);

    return victim;
}

void pmm_free_frame(physical_memory_manager_t* pmm, size_t frame){
    if(frame >= (size_t)NUM_FRAMES) return;
    pmm->in_use[frame] = 0;
    pmm->frame_to_page[frame] = -1;
}

char pmm_read_byte(const physical_memory_manager_t* pmm, size_t frame, size_t offset){
    if(frame >= (size_t)NUM_FRAMES) return 0;
    if(offset >= (size_t)PAGE_SIZE) return 0;
    return pmm->mem[frame][offset];
}

void pmm_write_byte(physical_memory_manager_t* pmm, size_t frame, size_t offset, char value){
    if(frame >= (size_t)NUM_FRAMES) return;
    if(offset >= (size_t)PAGE_SIZE) return;
    pmm->mem[frame][offset] = value;
}

void pmm_read_frame(const physical_memory_manager_t* pmm, size_t frame, char* out_buf, size_t size){
    if(frame >= (size_t)NUM_FRAMES) return;
    if(size > (size_t)PAGE_SIZE) size = PAGE_SIZE;
    for(size_t i=0;i<size;i++) out_buf[i] = pmm->mem[frame][i];
}

void pmm_write_frame(physical_memory_manager_t* pmm, size_t frame, const char* in_buf, size_t size){
    if(frame >= (size_t)NUM_FRAMES) return;
    if(size > (size_t)PAGE_SIZE) size = PAGE_SIZE;
    for(size_t i=0;i<size;i++) pmm->mem[frame][i] = in_buf[i];
}

int pmm_get_page_in_frame(const physical_memory_manager_t* pmm, size_t frame){
    if(frame >= (size_t)NUM_FRAMES) return -1;
    return (int)pmm->frame_to_page[frame];
}

void pmm_set_frame_mapping(physical_memory_manager_t* pmm, size_t frame, int vpn){
    if(frame >= (size_t)NUM_FRAMES) return;
    pmm->frame_to_page[frame] = (int16_t)vpn;
}

void pmm_print_frame_table(const physical_memory_manager_t* pmm){
    printf("Frame Table:\n");
    printf("Idx | Use | VPN\n");
    for(int i=0;i<NUM_FRAMES;i++){
        int vpn = pmm->frame_to_page[i];
        printf("%d   | %d   | %d\n", i, (int)pmm->in_use[i], vpn);
    }
}
