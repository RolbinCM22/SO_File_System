#include "virtual_memory_unit.h"

/* Si querés prints, usá tu printf VGA; si no, dejá esto apagado */
#ifndef VMU_DEBUG
#define VMU_DEBUG 0
#endif

#if VMU_DEBUG
#include "vga.h"
#endif

static inline vmu_status_t vmu_validate(size_t vpn, size_t offset){
  if(vpn >= (size_t)BACKING_PAGES) return VMU_ERR_VPN_RANGE;
  if(offset >= (size_t)PAGE_SIZE)  return VMU_ERR_OFF_RANGE;
  return VMU_OK;
}

static inline uint32_t build_va(size_t vpn, size_t offset){
  /* Layout 8-bit vpn + 8-bit offset */
  return (uint32_t)((vpn << 8) | (offset & 0xFFu));
}

void vmu_init(virtual_memory_unit_t* vmu,
              physical_memory_manager_t* pmm,
              backing_store_manager_t* bsm)
{
  vmu->pmm = pmm;
  vmu->bsm = bsm;

  pfh_init(&vmu->pfh, pmm, bsm);
  ptm_init(&vmu->ptm, &vmu->pfh);
}

/* Helpers: leer/escribir en frame */
static vmu_status_t read_from_frame(virtual_memory_unit_t* vmu,
                                    size_t frame, size_t off,
                                    char* out_value)
{
  if(frame >= (size_t)NUM_FRAMES) return VMU_ERR_FRAME_RANGE;
  if(off   >= (size_t)PAGE_SIZE)  return VMU_ERR_OFF_RANGE;

  *out_value = pmm_read_byte(vmu->pmm, frame, off);

#if VMU_DEBUG
  printf("[VMU] R frame=%d off=%d val=%d\n", (int)frame, (int)off, (int)(*out_value));
#endif
  return VMU_OK;
}

static vmu_status_t write_to_frame(virtual_memory_unit_t* vmu,
                                   size_t frame, size_t off,
                                   char value)
{
  if(frame >= (size_t)NUM_FRAMES) return VMU_ERR_FRAME_RANGE;
  if(off   >= (size_t)PAGE_SIZE)  return VMU_ERR_OFF_RANGE;

  pmm_write_byte(vmu->pmm, frame, off, value);

#if VMU_DEBUG
  printf("[VMU] W frame=%d off=%d val=%d\n", (int)frame, (int)off, (int)value);
#endif
  return VMU_OK;
}

vmu_status_t vmu_read_memory(virtual_memory_unit_t* vmu,
                             size_t vpn, size_t offset,
                             char* out_value)
{
  vmu_status_t st = vmu_validate(vpn, offset);
  if(st != VMU_OK) return st;

  uint32_t va = build_va(vpn, offset);
  uint32_t pa = ptm_translate(&vmu->ptm, va);

  uint32_t frame_size = (uint32_t)ptm_get_frame_size(&vmu->ptm); /* 256 */
  size_t frame = (size_t)(pa / frame_size);
  size_t off   = (size_t)(pa % frame_size);

  return read_from_frame(vmu, frame, off, out_value);
}

vmu_status_t vmu_write_memory(virtual_memory_unit_t* vmu,
                              size_t vpn, size_t offset,
                              char value)
{
  vmu_status_t st = vmu_validate(vpn, offset);
  if(st != VMU_OK) return st;

  uint32_t va = build_va(vpn, offset);
  uint32_t pa = ptm_translate(&vmu->ptm, va);

  uint32_t frame_size = (uint32_t)ptm_get_frame_size(&vmu->ptm);
  size_t frame = (size_t)(pa / frame_size);
  size_t off   = (size_t)(pa % frame_size);

  st = write_to_frame(vmu, frame, off, value);
  if(st != VMU_OK) return st;

  ptm_mark_modified(&vmu->ptm, (uint8_t)(vpn & 0xFFu));
  return VMU_OK;
}

void vmu_sync_dirty_pages(virtual_memory_unit_t* vmu){

}
