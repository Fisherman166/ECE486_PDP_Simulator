/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** branch_trace.h
*/

#ifndef BRANCH_TRACE_H
#define BRANCH_TRACE_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static FILE* branch_file;

int branch_trace_init(void);
void write_branch_trace(uint16_t, uint16_t, const char*, uint8_t);
int close_branch_trace(void);

#endif

