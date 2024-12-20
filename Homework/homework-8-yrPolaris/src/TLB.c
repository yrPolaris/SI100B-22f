#include "../inc/TLB.h"

#include <stdlib.h>

unsigned global_TLB_init(void) {
  global_tlb = calloc(1, sizeof(TLB));
  return 0;
}

void global_TLB_free(void) {
  if (global_tlb == NULL) {
    return;
  }
  free(global_tlb);
  global_tlb = NULL;
}

void flush_TLB() {
  if (global_tlb == NULL) {
    return;
  }

  for (int i = 0; i < TLB_SIZE; i++) {
    global_tlb->entries[i].valid = 0;
  }
}

unsigned read_TLB(proc_id_t pid, unsigned vpn) {
  if (global_tlb == NULL || global_tlb->pid != pid) {
    return ERROR;
  }

  for (int i = 0; i < TLB_SIZE; i++) {
    if (global_tlb->entries[i].valid && global_tlb->entries[i].vpn == vpn) {
      global_tlb->entries[i].lut = global_tlb->clock;
      global_tlb->clock++;
      return global_tlb->entries[i].ppn;
    }
  }
  return ERROR;
}

void write_TLB(proc_id_t pid, unsigned vpn, unsigned ppn) {
  if (global_tlb == NULL) {
    return;
  }

  if (global_tlb->pid != pid) {
    flush_TLB();
    global_tlb->pid = pid;
  }

  int lru_index = 0;
  for (int i = 1; i < TLB_SIZE; i++) {
    if (!global_tlb->entries[i].valid ||
        global_tlb->entries[i].lut < global_tlb->entries[lru_index].lut) {
      lru_index = i;
    }
  }

  global_tlb->entries[lru_index].lut = global_tlb->clock;
  global_tlb->entries[lru_index].ppn = ppn;
  global_tlb->entries[lru_index].vpn = vpn;
  global_tlb->entries[lru_index].valid = 1;
  global_tlb->clock++;
}

void remove_TLB(proc_id_t pid, unsigned vpn) {
  if (global_tlb == NULL || global_tlb->pid != pid) {
    return;
  }

  for (int i = 0; i < TLB_SIZE; i++) {
    if (global_tlb->entries[i].valid && global_tlb->entries[i].vpn == vpn) {
      global_tlb->entries->valid = 0;
      break;
    }
  }
}
