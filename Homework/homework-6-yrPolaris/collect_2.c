#include "cache.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

uint32_t compute(uint32_t number)
{
    uint32_t count = 0;
    while (number > 1)
    {
        number >>= 1;
        count++;
    }
    return count;
}

/* Create a cache simulator according to the config */
struct cache *cache_create(struct cache_config config, struct cache *lower_level)
{
    /*YOUR CODE HERE*/
    struct cache *new_cache = (struct cache *)malloc(sizeof(struct cache));
    if (!new_cache)
        return NULL;
    new_cache->config = config;
    new_cache->lower_cache = lower_level;
    new_cache->offset_bits = compute(config.line_size);
    new_cache->index_bits = compute(config.lines / config.ways);
    new_cache->tag_bits = config.address_bits - new_cache->index_bits - new_cache->offset_bits;
    new_cache->offset_mask = (1 << new_cache->offset_bits) - 1;
    new_cache->index_mask = ((1 << new_cache->index_bits) - 1) << new_cache->offset_bits;
    new_cache->tag_mask = ((1 << new_cache->tag_bits) - 1) << (new_cache->index_bits + new_cache->offset_bits);

    new_cache->lines = (struct cache_line *)calloc(config.lines, sizeof(struct cache_line));
    if (!new_cache->lines)
        return NULL;
    for (uint32_t i = 0; i < config.lines; i++)
    {
        new_cache->lines[i].dirty = false;
        new_cache->lines[i].valid = false;
        new_cache->lines[i].last_access = 0;
        new_cache->lines[i].tag = 0;
        new_cache->lines[i].data = (uint8_t *)calloc(config.line_size, sizeof(uint8_t));
        if (!new_cache->lines[i].data)
            return NULL;
    }
    return new_cache;
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
void cache_destroy(struct cache *cache)
{
    /*YOUR CODE HERE*/
    if (!cache)
        return;
    uint32_t num_sets = cache->config.lines / cache->config.ways;
    for (uint32_t slot = 0; slot < cache->config.ways; slot++)
    {
        for (uint32_t set = 0; set < num_sets; set++)
        {
            uint32_t i = set * cache->config.ways + slot;
            struct cache_line *line = &cache->lines[i];
            if (line->dirty && line->valid && cache->config.write_back)
            {
                uint32_t addr = (line->tag << (cache->index_bits + cache->offset_bits)) + (set << cache->offset_bits);
                if (cache->lower_cache)
                {
                    for (uint32_t j = 0; j < cache->config.line_size; ++j)
                        cache_write_byte(cache, addr + j, line->data[j]);
                }
                else
                    mem_store(line->data, addr, cache->config.line_size);
            }
            free(line->data);
        }
    }
    free(cache->lines);
    free(cache);
}

/* Read one byte at a specific address. return hit=true/miss=false */
bool cache_read_byte(struct cache *cache, uint32_t addr, uint8_t *byte)
{
    /*YOUR CODE HERE*/
    uint32_t tag = (addr & cache->tag_mask) >> (cache->index_bits + cache->offset_bits);
    uint32_t index = (addr & cache->index_mask) >> cache->offset_bits;
    uint32_t offset = addr & cache->offset_mask;

    uint32_t start = index * cache->config.ways;
    // hit
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        if (cache->lines[start + i].tag == tag && cache->lines[start + i].valid)
        {
            *byte = cache->lines[start + i].data[offset];
            cache->lines[start + i].last_access = get_timestamp();
            return true;
        }
    }
    // miss
    struct cache_line *lru_line = NULL;
    uint64_t lru = 0xffffffff;
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        if (!cache->lines[start + i].valid)
        {
            lru_line = &cache->lines[start + i];
            break;
        }
        if (cache->lines[start + i].last_access < lru)
        {
            lru = cache->lines[start + i].last_access;
            lru_line = &cache->lines[start + i];
        }
    }
    uint32_t lru_addr = (lru_line->tag << (cache->index_bits + cache->offset_bits)) + ((index << cache->offset_bits));
    if (lru_line->valid && lru_line->dirty && cache->config.write_back)
    {
        if (cache->lower_cache)
        {
            for (uint32_t j = 0; j < cache->config.line_size; ++j)
                cache_write_byte(cache->lower_cache, lru_addr + j, lru_line->data[j]);
        }
        else
            mem_store(lru_line->data, lru_addr, cache->config.line_size);
    }
    uint32_t new_addr = (addr >> cache->offset_bits) << cache->offset_bits;
    if (cache->lower_cache)
    {
        for (uint32_t j = 0; j < cache->config.line_size; ++j)
            cache_read_byte(cache->lower_cache, new_addr + j, &lru_line->data[j]);
    }
    else
        mem_load(lru_line->data, new_addr, cache->config.line_size);

    lru_line->valid = true;
    lru_line->dirty = false;
    lru_line->tag = tag;
    lru_line->last_access = get_timestamp();
    *byte = lru_line->data[offset];
    return false;
}
/* Write one byte into a specific address. return hit=true/miss=false*/
bool cache_write_byte(struct cache *cache, uint32_t addr, uint8_t byte)
{
    /*YOUR CODE HERE*/
    uint32_t tag = (addr & cache->tag_mask) >> (cache->index_bits + cache->offset_bits);
    uint32_t index = (addr & cache->index_mask) >> cache->offset_bits;
    uint32_t offset = addr & cache->offset_mask;
    uint32_t start = index * cache->config.ways;

    uint32_t new_addr = (addr >> cache->offset_bits) << cache->offset_bits;
    // hit
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        if (cache->lines[start + i].tag == tag && cache->lines[start + i].valid)
        {
            cache->lines[start + i].data[offset] = byte;
            cache->lines[start + i].last_access = get_timestamp();
            cache->lines[start + i].dirty = true;
            if (!cache->config.write_back)
            {
                if (cache->lower_cache)
                    cache_write_byte(cache->lower_cache, addr, byte);
                else
                    mem_store(cache->lines[start + i].data, new_addr, cache->config.line_size);
            }
            return true;
        }
    }
    // miss
    struct cache_line *lru_line = NULL;
    uint64_t lru = 0xffffffff;
    for (uint32_t i = 0; i < cache->config.ways; ++i)
    {
        if (!cache->lines[start + i].valid)
        {
            lru_line = &cache->lines[start + i];
            break;
        }
        if (cache->lines[start + i].last_access < lru)
        {
            lru = cache->lines[start + i].last_access;
            lru_line = &cache->lines[start + i];
        }
    }
    uint32_t lru_addr = (lru_line->tag << (cache->index_bits + cache->offset_bits)) + (index << cache->offset_bits);
    if (lru_line->valid && lru_line->dirty && cache->config.write_back)
    {
        if (cache->lower_cache)
        {
            for (uint32_t i = 0; i < cache->config.line_size; ++i)
                cache_write_byte(cache->lower_cache, lru_addr + i, lru_line->data[i]);
        }
        else
            mem_store(lru_line->data, lru_addr, cache->config.line_size);
    }
    if (cache->lower_cache)
    {
        for (uint32_t i = 0; i < cache->config.line_size; ++i)
            cache_read_byte(cache->lower_cache, new_addr + i, &lru_line->data[i]);
    }
    else
        mem_load(lru_line->data, new_addr, cache->config.line_size);

    lru_line->valid = true;
    lru_line->dirty = cache->config.write_back;
    lru_line->tag = tag;
    lru_line->last_access = get_timestamp();
    lru_line->data[offset] = byte;
    if (!cache->config.write_back)
    {
        if (cache->lower_cache)
            cache_write_byte(cache->lower_cache, addr, byte);
        else
            mem_store(lru_line->data, new_addr, cache->config.line_size);
    }
    return false;
}
