#include "vmm.h"
#include "output_utility.h"

#include <stdio.h>

/**
 * Initialized a statistics object to zero.
 * @param stats A pointer to an uninitialized statistics object.
 */
static void statistics_initialize(Statistics *stats) {
    stats->tlb_hits = 0;
    stats->pagetable_hits = 0;
    stats->total_memory_accesses = 0;
}

Statistics simulate_virtual_memory_accesses(FILE *fd_addresses, FILE *fd_backing) {
    // Initialize statistics
    Statistics stats;
    statistics_initialize(&stats);

    // TODO: Implement your solution

    return stats;
}
