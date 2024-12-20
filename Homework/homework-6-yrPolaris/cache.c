#include "cache.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

uint32_t log_bit(uint32_t num)
{
    uint32_t count = 0;
    while (num >>= 1)
    {
        count++;
    }
    return count;
}

void get_component_of_addr(struct cache *cache, uint32_t addr, uint32_t *tag, uint32_t *index, uint32_t *offset)
{
    *tag = (cache->tag_mask & addr) >> (cache->index_bits + cache->offset_bits);
    *index = (cache->index_mask & addr) >> (cache->offset_bits);
    *offset = (cache->offset_mask & addr);
}

/* Create a cache simulator according to the config */
struct cache *cache_create(struct cache_config config, struct cache *lower_level)
{
    /*YOUR CODE HERE*/
    struct cache *cache = (struct cache *)malloc(sizeof(struct cache));
    if (cache == NULL)
    {
        return NULL;
    }

    cache->config = config;
    cache->index_bits = log_bit(config.size / (config.line_size * config.ways));
    cache->offset_bits = log_bit(config.line_size);
    cache->tag_bits = config.address_bits - cache->index_bits - cache->offset_bits;
    cache->offset_mask = (1 << cache->offset_bits) - 1;
    cache->index_mask = ((1 << cache->index_bits) - 1) << cache->offset_bits;
    cache->tag_mask = ((1 << cache->tag_bits) - 1) << (cache->index_bits + cache->offset_bits);

    cache->lines = calloc(config.lines, sizeof(struct cache_line));
    for (uint32_t i = 0; i < config.lines; i++)
    {
        cache->lines[i].data = calloc(config.line_size, sizeof(uint8_t));
    }

    cache->lower_cache = lower_level;
    return cache;
}

/*
Release the resources allocated for the cache simulator.
Also writeback dirty lines

The order in which lines are victim_cacheline is:
set0-slot0, set1-slot0, set2-slot0, (the 0th way)
set0-slot1, set1-slot1, set2-slot1, (the 1st way)
set0-slot2, set1-slot2, set2-slot2, (the 2nd way)
and so on.
*/
void read_cacheline(struct cache *cache, uint32_t addr, uint8_t *byte)
{
    uint32_t offset, index, tag;
    get_component_of_addr(cache, addr, &tag, &index, &offset);
    uint32_t begin = index * cache->config.ways;
    uint32_t end = index * cache->config.ways + cache->config.ways;
    uint32_t last_access = cache->lines[begin].last_access;
    uint32_t addr_without_offset = (addr >> cache->offset_bits) << cache->offset_bits;

    struct cache_line *victim_cacheline = &cache->lines[begin];
    uint32_t victim_cacheline_addr = 0;

    // hit
    for (uint32_t i = begin; i < end; i++)
    {
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            memcpy(byte, cache->lines[i].data, cache->config.line_size); // let data in L1 miss load in L2 cache line
            cache->lines[i].last_access = get_timestamp();
            return;
        }
    }

    // miss
    for (uint32_t i = begin; i < end; i++)
    {
        // look for invalid cacheline or the last access valid cacheline
        if (!cache->lines[i].valid)
        {
            victim_cacheline = &cache->lines[i];
            break;
        }

        if (last_access > cache->lines[i].last_access)
        {
            last_access = cache->lines[i].last_access;
            victim_cacheline = &cache->lines[i];
        }
    }

    // if valid and dirty, first evict
    if (victim_cacheline->dirty && victim_cacheline->valid)
    {
        victim_cacheline_addr = (victim_cacheline->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        mem_store(victim_cacheline->data, victim_cacheline_addr, cache->config.line_size);
    }

    // load from lower-level
    mem_load(victim_cacheline->data, addr_without_offset, cache->config.line_size);
    memcpy(byte, victim_cacheline->data, cache->config.line_size);
    victim_cacheline->tag = tag;
    victim_cacheline->valid = true;
    victim_cacheline->dirty = false;
    victim_cacheline->last_access = get_timestamp();
}

void write_cacheline(struct cache *cache, uint32_t addr, uint8_t *byte)
{
    uint32_t offset, index, tag;
    get_component_of_addr(cache, addr, &tag, &index, &offset);
    uint32_t begin = index * cache->config.ways;
    uint32_t end = index * cache->config.ways + cache->config.ways;
    uint32_t last_access = cache->lines[begin].last_access;
    uint32_t addr_without_offset = (addr >> cache->offset_bits) << cache->offset_bits;

    struct cache_line *victim_cacheline = &cache->lines[begin];
    uint32_t victim_cacheline_addr = 0;

    // hit
    for (uint32_t i = begin; i < end; i++)
    { // hit
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            cache->lines[i].last_access = get_timestamp();
            if (cache->config.write_back == true)
            {
                cache->lines[i].dirty = true;
            }
            else if (cache->config.write_back == false)
            {
                cache->lines[i].dirty = false;
            }
            memcpy(cache->lines[i].data, byte, cache->config.line_size); // load data in to L2
            cache->lines[i].valid = true;
            if (cache->config.write_back == false)
            {
                mem_store(cache->lines[i].data, addr_without_offset, cache->config.line_size);
            }
            return;
        }
    }

    // miss
    for (uint32_t i = begin; i < end; i++)
    {
        // look for invalid cacheline or the last access valid cacheline
        if (!cache->lines[i].valid)
        {
            victim_cacheline = &cache->lines[i];
            break;
        }

        if (cache->lines[i].last_access < last_access)
        {
            last_access = cache->lines[i].last_access;
            victim_cacheline = &cache->lines[i];
        }
    }

    // if valid and dirty, first evict
    if (victim_cacheline->valid && victim_cacheline->dirty)
    {
        victim_cacheline_addr = (victim_cacheline->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        mem_store(victim_cacheline->data, victim_cacheline_addr, cache->config.line_size);
    }

    // load from lower-level
    mem_load(victim_cacheline->data, addr_without_offset, cache->config.line_size);
    memcpy(victim_cacheline->data, byte, cache->config.line_size);
    victim_cacheline->valid = true;
    victim_cacheline->tag = tag;
    victim_cacheline->last_access = get_timestamp();
    if (cache->config.write_back == true)
    {
        victim_cacheline->dirty = true;
    }
    else
    {
        victim_cacheline->dirty = false;
        mem_store(victim_cacheline->data, addr_without_offset, cache->config.line_size);
    }
}
void cache_destroy(struct cache *cache)
{
    if (cache == NULL)
    {
        return;
    }
    for (uint32_t i = 0; i < cache->config.ways; i++)
    {
        for (uint32_t set = 0; set < cache->config.lines / cache->config.ways; set++)
        {
            uint32_t location = set * cache->config.ways + i;
            if (cache->lines[location].valid && cache->lines[location].dirty)
            {
                uint32_t addr = ((cache->lines[location].tag << (cache->index_bits + cache->offset_bits))) | (set << cache->offset_bits);
                if (cache->lower_cache)
                {
                    write_cacheline(cache->lower_cache, addr, cache->lines[location].data);
                }
                else
                {
                    mem_store(cache->lines[location].data, addr, cache->config.line_size);
                }
            }
            free(cache->lines[location].data);
        }
    }
    free(cache->lines);
    free(cache);
}

/* Read one byte at a specific address. return hit=true/miss=false */
bool cache_read_byte(struct cache *cache, uint32_t addr, uint8_t *byte)
{
    /*YOUR CODE HERE*/
    uint32_t offset, index, tag;
    get_component_of_addr(cache, addr, &tag, &index, &offset);
    uint32_t begin = index * cache->config.ways;
    uint32_t end = index * cache->config.ways + cache->config.ways;
    uint32_t last_access = cache->lines[begin].last_access;
    uint32_t addr_without_offset = (addr >> cache->offset_bits) << cache->offset_bits;

    struct cache_line *victim_cacheline = &cache->lines[begin];
    uint32_t victim_cacheline_addr = 0;

    // hit
    for (uint32_t i = begin; i < end; i++)
    {
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            *byte = cache->lines[i].data[offset];
            cache->lines[i].last_access = get_timestamp();
            return true;
        }
    }

    // miss
    for (uint32_t i = begin; i < end; i++)
    {
        // look for invalid cacheline or the last access valid cacheline
        if (!cache->lines[i].valid)
        {
            victim_cacheline = &cache->lines[i];
            break;
        }

        if (last_access > cache->lines[i].last_access)
        {
            last_access = cache->lines[i].last_access;
            victim_cacheline = &cache->lines[i];
        }
    }

    // if valid and dirty, first evict
    if (victim_cacheline->valid && victim_cacheline->dirty)
    {
        victim_cacheline_addr = (victim_cacheline->tag << (cache->index_bits + cache->offset_bits)) + ((index << cache->offset_bits));
        if (cache->lower_cache == NULL)
        {
            mem_store(victim_cacheline->data, victim_cacheline_addr, cache->config.line_size);
        }
        else
        {
            write_cacheline(cache->lower_cache, victim_cacheline_addr, victim_cacheline->data);
        }
    }

    // load from lower-level
    if (cache->lower_cache == NULL)
    {
        mem_load(victim_cacheline->data, addr_without_offset, cache->config.line_size);
    }
    else
    {
        read_cacheline(cache->lower_cache, addr_without_offset, victim_cacheline->data);
    }
    victim_cacheline->tag = tag;
    victim_cacheline->valid = true;
    victim_cacheline->dirty = false;
    victim_cacheline->last_access = get_timestamp();
    *byte = victim_cacheline->data[offset];
    return false;
}
/* Write one byte into a specific address. return hit=true/miss=false*/
bool cache_write_byte(struct cache *cache, uint32_t addr, uint8_t byte)
{
    uint32_t offset, index, tag;
    get_component_of_addr(cache, addr, &tag, &index, &offset);
    uint32_t begin = index * cache->config.ways;
    uint32_t end = index * cache->config.ways + cache->config.ways;
    uint32_t last_access = cache->lines[begin].last_access;
    uint32_t addr_without_offset = (addr >> cache->offset_bits) << cache->offset_bits;

    struct cache_line *victim_cacheline = &cache->lines[begin];
    uint32_t victim_cacheline_addr = 0;

    // hit
    for (uint32_t i = begin; i < end; i++)
    {
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            cache->lines[i].data[offset] = byte;
            cache->lines[i].last_access = get_timestamp();
            if (cache->config.write_back == true)
            {
                cache->lines[i].dirty = true;
            }
            else
            {
                cache->lines[i].dirty = false;
                if (cache->lower_cache)
                {
                    write_cacheline(cache->lower_cache, addr_without_offset, cache->lines[i].data);
                }
                else
                {
                    mem_store(cache->lines[i].data, addr_without_offset, cache->config.line_size);
                }
            }
            return true;
        }
    }

    // look for invalid cacheline or the last access valid cacheline
    for (uint32_t i = begin; i < end; i++)
    {
        if (!cache->lines[i].valid)
        {
            victim_cacheline = &cache->lines[i];
            break;
        }

        if (cache->lines[i].last_access < last_access)
        {
            last_access = cache->lines[i].last_access;
            victim_cacheline = &cache->lines[i];
        }
    }

    // if valid and dirty, first evict
    if (victim_cacheline->valid && victim_cacheline->dirty)
    {
        victim_cacheline_addr = (victim_cacheline->tag << (cache->index_bits + cache->offset_bits)) + ((index << cache->offset_bits));
        if (cache->lower_cache == NULL)
        {
            mem_store(victim_cacheline->data, victim_cacheline_addr, cache->config.line_size);
        }
        else
        {
            write_cacheline(cache->lower_cache, victim_cacheline_addr, victim_cacheline->data);
        }
    }

    // load from lower-level
    if (cache->lower_cache == NULL)
    {
        mem_load(victim_cacheline->data, addr_without_offset, cache->config.line_size);
    }
    else
    {
        read_cacheline(cache->lower_cache, addr_without_offset, victim_cacheline->data);
    }
    victim_cacheline->valid = true;
    victim_cacheline->last_access = get_timestamp();
    victim_cacheline->tag = tag;
    victim_cacheline->data[offset] = byte;
    if (cache->config.write_back == true)
    {
        victim_cacheline->dirty = true;
    }
    else
    {
        victim_cacheline->dirty = false;
        if (cache->lower_cache == NULL)
        {
            mem_store(victim_cacheline->data, addr_without_offset, cache->config.line_size);
        }

        else
        {
            write_cacheline(cache->lower_cache, addr_without_offset, victim_cacheline->data);
        }
    }
    return false;
}