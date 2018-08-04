/**********************************************************************
 * This file describes all methods that are instrumented on C programs *
 ***********************************************************************/
#ifndef Map2CheckFunctions_H
#define Map2CheckFunctions_H

#include "Map2CheckTypes.h"

/** Initializes variables used in map2check */
void map2check_init(int isSvComp);

/** Finalizes and exit program with error */
void map2check_error();

void map2check_track_bb(unsigned line, const char *function_name);

void map2check_nondet_int(unsigned line, unsigned scope, int value,
                          const char *function_name);
void map2check_nondet_unsigned(unsigned line, unsigned scope, unsigned value,
                               const char *function_name);
/**
 * Track pointer pointer for LIST_LOG (this function is to be used for
 * instrumentation)
 * @param x     Address of current pointer
 * @param scope Number of the scope
 * @param name  Name of the pointer
 * @param line  Line where pointer was declared
 */
void map2check_pointer(void *x, unsigned scope, const char *name, int line);

/**
 * Generates a non det integer by using an undirect NONDET call (this function
 * is
 * to be used for instrumentation)
 * @return An integer representing a non deterministic value
 */
int map2check_non_det_int();
unsigned int map2check_non_det_uint();

/**
 * Tracks execution of a specific function (this function is to be used for
 * instrumentation)
 * @param func_name   Name of the function where the operation occured.
 * @param scope       Number of the scope
 * @param line        Line where store operation was called
 */
void map2check_target_function(const char *func_name, int scope, int line);

/**
 * @brief Checks if address where the var will be stored is valid
 * @param ptr               Pointer to where value will be stored
 * @param size_of_destiny   Size of the value to be stored
 */
void map2check_store(void *ptr, unsigned size_of_destiny);

char map2check_non_det_char();
void map2check_nondet_char(unsigned line, unsigned scope, int value,
                           const char *function_name);

void *map2check_non_det_pointer();
long map2check_non_det_long();
unsigned short map2check_non_det_ushort();
void map2check_nondet_pointer(unsigned line, unsigned scope, int value,
                              const char *function_name);
void map2check_assume(int expr);
void __VERIFIER_error();

/**
 *
 * Generates TRUE result
 */
void map2check_success();

/**
 * Releases allocated resources and generates property violated
 */
void map2check_exit();

/**
 * Returns current step for map2check (used to generate wintess/example)
 */
unsigned map2check_get_current_step();
/**
 * Increments current step
 */
void map2check_next_current_step();
#endif
