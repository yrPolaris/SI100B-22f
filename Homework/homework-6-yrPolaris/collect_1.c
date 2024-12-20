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

The order in which lines are evicted is:
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
    for (uint32_t i = begin; i < end; i++)
    { // hit
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            memcpy(byte, cache->lines[i].data, cache->config.line_size); // let data in L1 miss load in L2 cache line
            cache->lines[i].last_access = get_timestamp();
            return;
        }
    }
    struct cache_line *evicted = NULL;
    for (uint32_t i = begin; i < end; i++)
    {
        if (!cache->lines[i].valid)
        {
            evicted = &cache->lines[i];
            break;
        }
    }
    if (evicted == NULL)
    { // perform LRU
        uint64_t lastacc = cache->lines[begin].last_access;
        evicted = &cache->lines[begin];
        for (uint32_t m = begin; m < end; m++)
        {
            if (cache->lines[m].last_access < lastacc)
            {
                lastacc = cache->lines[m].last_access;
                evicted = &cache->lines[m];
            }
        }
    }
    if (evicted->dirty && evicted->valid)
    {
        uint32_t saddr = (evicted->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        mem_store(evicted->data, saddr, cache->config.line_size);
    }
    uint32_t laddr = (addr >> log_bit(cache->config.line_size)) << log_bit(cache->config.line_size);
    mem_load(evicted->data, laddr, cache->config.line_size);
    memcpy(byte, evicted->data, cache->config.line_size);
    evicted->tag = tag;
    evicted->valid = true;
    evicted->dirty = false;
    evicted->last_access = get_timestamp();
}
void write_cacheline(struct cache *cache, uint32_t addr, uint8_t *byte)
{
    uint32_t offset, index, tag;
    get_component_of_addr(cache, addr, &tag, &index, &offset);
    uint32_t begin = index * cache->config.ways;
    uint32_t end = index * cache->config.ways + cache->config.ways;
    for (uint32_t i = begin; i < end; i++)
    { // hit
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            cache->lines[i].last_access = get_timestamp();
            if (cache->config.write_back)
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
                mem_store(cache->lines[i].data, addr - offset, cache->config.line_size);
            }
            return;
        }
    }
    struct cache_line *evicted = NULL;
    for (uint32_t i = begin; i < end; i++)
    {
        if (!cache->lines[i].valid)
        {
            evicted = &cache->lines[i];
            break;
        }
    }
    if (evicted == NULL)
    { // perform lru
        uint64_t lastacc = cache->lines[begin].last_access;
        evicted = &cache->lines[begin];
        for (uint32_t m = begin; m < end; m++)
        {
            if (cache->lines[m].last_access < lastacc)
            {
                lastacc = cache->lines[m].last_access;
                evicted = &cache->lines[m];
            }
        }
    }
    if (evicted->valid && evicted->dirty)
    {
        uint32_t saddr = (evicted->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        mem_store(evicted->data, saddr, cache->config.line_size);
    }
    uint32_t laddr = (addr >> log_bit(cache->config.line_size)) << log_bit(cache->config.line_size);
    mem_load(evicted->data, laddr, cache->config.line_size);
    memcpy(evicted->data, byte, cache->config.line_size);
    evicted->valid = true;
    if (cache->config.write_back)
    {
        evicted->dirty = true;
    }
    else
    {
        evicted->dirty = false;
    }
    evicted->tag = tag;
    evicted->last_access = get_timestamp();
    if (cache->config.write_back == false)
    {
        mem_store(evicted->data, laddr, cache->config.line_size);
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
    struct cache_line *evicted = NULL;
    for (uint32_t i = begin; i < end; i++)
    {
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            *byte = cache->lines[i].data[offset];
            cache->lines[i].last_access = get_timestamp();
            return true;
        }
    }
    for (uint32_t i = begin; i < end; i++)
    {
        if (!cache->lines[i].valid)
        {
            evicted = &cache->lines[i];
            break;
        }
    }
    if (evicted == NULL)
    {
        uint64_t lastacc = cache->lines[begin].last_access;
        evicted = &cache->lines[begin];
        for (uint32_t j = begin; j < end; j++)
        {
            if (cache->lines[j].last_access < lastacc)
            {
                lastacc = cache->lines[j].last_access;
                evicted = &cache->lines[j];
            }
        }
    }
    if (evicted->valid && evicted->dirty)
    {
        uint32_t saddr = (evicted->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        if (cache->lower_cache)
        {
            write_cacheline(cache->lower_cache, saddr, evicted->data);
        }
        else
        {
            mem_store(evicted->data, saddr, cache->config.line_size);
        }
    }
    uint32_t laddr = (addr >> log_bit(cache->config.line_size)) << log_bit(cache->config.line_size);
    if (cache->lower_cache)
    {
        read_cacheline(cache->lower_cache, laddr, evicted->data);
    }
    else
    {
        mem_load(evicted->data, laddr, cache->config.line_size);
    }
    evicted->tag = tag;
    evicted->valid = true;
    evicted->dirty = false;
    evicted->last_access = get_timestamp();
    *byte = evicted->data[offset];
    return false;
}
/* Write one byte into a specific address. return hit=true/miss=false*/
bool cache_write_byte(struct cache *cache, uint32_t addr, uint8_t byte)
{
    uint32_t offset, index, tag;
    get_component_of_addr(cache, addr, &tag, &index, &offset);
    uint32_t begin = index * cache->config.ways;
    uint32_t end = index * cache->config.ways + cache->config.ways;
    struct cache_line *evicted = NULL;
    for (uint32_t i = begin; i < end; i++)
    {
        if (cache->lines[i].valid && cache->lines[i].tag == tag)
        {
            cache->lines[i].data[offset] = byte;
            cache->lines[i].last_access = get_timestamp();
            if (cache->config.write_back)
            {
                cache->lines[i].dirty = true;
            }
            else
            {
                cache->lines[i].dirty = false;
            }
            if (!cache->config.write_back)
            {
                if (cache->lower_cache)
                {
                    write_cacheline(cache->lower_cache, addr - offset, cache->lines[i].data);
                }
                else
                {
                    mem_store(cache->lines[i].data, addr - offset, cache->config.line_size);
                }
            }

            return true;
        }
    }
    for (uint32_t i = begin; i < end; i++)
    {
        if (!cache->lines[i].valid)
        {
            evicted = &cache->lines[i];
            break;
        }
    }
    if (evicted == NULL)
    {
        uint64_t lastacc = cache->lines[begin].last_access;
        evicted = &cache->lines[begin];
        for (uint32_t j = begin; j < end; j++)
        {
            if (cache->lines[j].last_access < lastacc)
            {
                lastacc = cache->lines[j].last_access;
                evicted = &cache->lines[j];
            }
        }
    }
    if (evicted->valid && evicted->dirty)
    {
        uint32_t saddr = (evicted->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        if (cache->lower_cache)
        {
            write_cacheline(cache->lower_cache, saddr, evicted->data);
        }
        else
        {
            mem_store(evicted->data, saddr, cache->config.line_size);
        }
    }
    uint32_t laddr = (addr >> log_bit(cache->config.line_size)) << log_bit(cache->config.line_size);
    if (cache->lower_cache)
    {
        read_cacheline(cache->lower_cache, laddr, evicted->data);
    }
    else
    {
        mem_load(evicted->data, laddr, cache->config.line_size);
    }
    evicted->valid = true;
    if (cache->config.write_back)
    {
        evicted->dirty = true;
    }
    else
    {
        evicted->dirty = false;
    }
    evicted->last_access = get_timestamp();
    evicted->tag = tag;
    evicted->data[offset] = byte;
    if (cache->config.write_back == false)
    {
        if (cache->lower_cache)
        {
            write_cacheline(cache->lower_cache, laddr, evicted->data);
        }
        else
        {
            mem_store(evicted->data, laddr, cache->config.line_size);
        }
    }
    return false;
}