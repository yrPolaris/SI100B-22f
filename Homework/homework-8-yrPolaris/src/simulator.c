#include "../inc/simulator.h"
#include "../inc/TLB.h"
#include <assert.h>
#include <stdlib.h>

status_t allocate_page(Process *process, addr_t address,
                       addr_t physical_address) {
  // This is guaranteed by the simulator
  assert(address >> OFFSET_BITS << OFFSET_BITS == address);
  assert(physical_address >> OFFSET_BITS << OFFSET_BITS == physical_address);
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }

  // TODO: Implement me!
  size_t l1_index = (address >> (OFFSET_BITS + L2_BITS)) & ((1 << L1_BITS) - 1);
  size_t l2_index = (address >> OFFSET_BITS) & ((1 << L2_BITS) - 1);
  size_t physical_index = (physical_address >> OFFSET_BITS);

  if (physical_index > main_memory->size) {
    return ERROR;
  }

  if (process->page_table.entries[l1_index].entries == NULL) {
    process->page_table.entries[l1_index].entries =
        (PTE *)calloc((1 << L2_BITS), sizeof(PTE));
    if (process->page_table.entries[l1_index].entries == NULL) {
      return ERROR;
    }
    process->page_table.entries[l1_index].valid_count = 0;
  }

  if (process->page_table.entries[l1_index].entries[l2_index].valid) {
    return ERROR;
  }

  process->page_table.entries[l1_index].entries[l2_index].frame =
      physical_index;
  process->page_table.entries[l1_index].entries[l2_index].valid = 1;
  process->page_table.entries[l1_index].valid_count++;

  return SUCCESS;
}

status_t deallocate_page(Process *process, addr_t address) {
  // This is guaranteed by the simulator
  assert(address >> OFFSET_BITS << OFFSET_BITS == address);
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }

  // TODO: Implement me!

  size_t l1_index = (address >> (OFFSET_BITS + L2_BITS)) & ((1 << L1_BITS) - 1);
  size_t l2_index = (address >> OFFSET_BITS) & ((1 << L2_BITS) - 1);

  if (process->page_table.entries[l1_index].entries == NULL) {
    return ERROR;
  }

  if (!process->page_table.entries[l1_index].entries[l2_index].valid) {
    return ERROR;
  }

  process->page_table.entries[l1_index].entries[l2_index].valid = 0;
  process->page_table.entries[l1_index].valid_count--;
  remove_TLB(process->pid, (address >> OFFSET_BITS));
  if (process->page_table.entries[l1_index].valid_count == 0) {
    free(process->page_table.entries[l1_index].entries);
    process->page_table.entries[l1_index].entries = NULL;
  }

  return SUCCESS;
}

status_t read_byte(Process *process, addr_t address, byte_t *byte) {
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }

  // TODO: Implement me!
  if (process->pid != global_tlb->pid) {
    flush_TLB();
    global_tlb->pid = process->pid;
  }

  unsigned ppn_of_TLB = read_TLB(process->pid, (address >> OFFSET_BITS));
  if (ppn_of_TLB != ERROR) {
    *byte = main_memory->pages[ppn_of_TLB]
                ->data[(address & ((1 << OFFSET_BITS) - 1))];
    return TLB_HIT;
  }

  size_t l1_index = (address >> (OFFSET_BITS + L2_BITS)) & ((1 << L1_BITS) - 1);
  size_t l2_index = (address >> OFFSET_BITS) & ((1 << L2_BITS) - 1);

  if (process->page_table.entries[l1_index].entries == NULL ||
      !process->page_table.entries[l1_index].entries[l2_index].valid) {
    return ERROR;
  }

  unsigned ppn = process->page_table.entries[l1_index].entries[l2_index].frame;
  *byte = main_memory->pages[ppn]->data[(address & ((1 << OFFSET_BITS) - 1))];
  write_TLB(process->pid, (address >> OFFSET_BITS), ppn);

  return SUCCESS;
}

status_t write_byte(Process *process, addr_t address, const byte_t *byte) {
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }

  // TODO: Implement me!
  if (global_tlb->pid != process->pid) {
    flush_TLB();
    global_tlb->pid = process->pid;
  }

  unsigned ppn_of_TLB = read_TLB(process->pid, (address >> OFFSET_BITS));
  if (ppn_of_TLB != ERROR) {
    main_memory->pages[ppn_of_TLB]->data[(address & ((1 << OFFSET_BITS) - 1))] =
        *byte;
    return TLB_HIT;
  }
  size_t l1_index = (address >> (OFFSET_BITS + L2_BITS)) & ((1 << L1_BITS) - 1);
  size_t l2_index = (address >> OFFSET_BITS) & ((1 << L2_BITS) - 1);

  if (process->page_table.entries[l1_index].entries == NULL ||
      !process->page_table.entries[l1_index].entries[l2_index].valid) {
    return ERROR;
  }

  unsigned ppn = process->page_table.entries[l1_index].entries[l2_index].frame;
  main_memory->pages[ppn]->data[(address & ((1 << OFFSET_BITS) - 1))] = *byte;
  write_TLB(process->pid, (address >> OFFSET_BITS), ppn);
  return SUCCESS;
}
