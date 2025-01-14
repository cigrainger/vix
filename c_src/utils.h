#ifndef VIX_UTILS_H
#define VIX_UTILS_H

#include "erl_nif.h"
#include <stdbool.h>

/* #define DEBUG */

#ifdef DEBUG
#define debug(...)                                                             \
  do {                                                                         \
    enif_fprintf(stderr, "%s:%d\t(fn \"%s\")  - ", __FILE__, __LINE__,         \
                 __func__);                                                    \
    enif_fprintf(stderr, __VA_ARGS__);                                         \
    enif_fprintf(stderr, "\n");                                                \
  } while (0)
#define start_timing() ErlNifTime __start = enif_monotonic_time(ERL_NIF_USEC)
#define elapsed_microseconds() (enif_monotonic_time(ERL_NIF_USEC) - __start)
#else
#define debug(...)
#define start_timing()
#define elapsed_microseconds() 0
#endif

#define error(...)                                                             \
  do {                                                                         \
    enif_fprintf(stderr, "%s:%d\t(fn: \"%s\")  - ", __FILE__, __LINE__,        \
                 __func__);                                                    \
    enif_fprintf(stderr, __VA_ARGS__);                                         \
    enif_fprintf(stderr, "\n");                                                \
  } while (0)

#define ASSERT_ARGC(argc, count)                                               \
  if (argc != count) {                                                         \
    error("number of arguments must be %d", count);                            \
    return enif_make_badarg(env);                                              \
  }

// Using macro to preserve file and line number metadata in the error log
#define SET_ERROR_RESULT(env, reason, res)                                     \
  do {                                                                         \
    res.is_success = false;                                                    \
    res.result = make_binary(env, reason);                                     \
    error(reason);                                                             \
  } while (0)

#define SET_RESULT_FROM_VIPS_ERROR(env, label, res)                            \
  do {                                                                         \
    res.is_success = false;                                                    \
    res.result = enif_make_tuple2(env, make_binary(env, label),                \
                                  make_binary(env, vips_error_buffer()));      \
    error("%s: %s", label, vips_error_buffer());                               \
    vips_error_clear();                                                        \
  } while (0)

#define SET_VIX_RESULT(res, term)                                              \
  do {                                                                         \
    res.is_success = true;                                                     \
    res.result = term;                                                         \
  } while (0)

typedef struct _VixResult {
  bool is_success;
  ERL_NIF_TERM result;
} VixResult;

extern int MAX_G_TYPE_NAME_LENGTH;

extern ERL_NIF_TERM ATOM_OK;

extern ERL_NIF_TERM ATOM_ERROR;

extern ERL_NIF_TERM ATOM_NIL;

extern ERL_NIF_TERM ATOM_TRUE;

extern ERL_NIF_TERM ATOM_FALSE;

ERL_NIF_TERM raise_exception(ErlNifEnv *env, const char *msg);

ERL_NIF_TERM raise_badarg(ErlNifEnv *env, const char *reason);

ERL_NIF_TERM make_ok(ErlNifEnv *env, ERL_NIF_TERM term);

ERL_NIF_TERM make_error(ErlNifEnv *env, const char *reason);

ERL_NIF_TERM make_atom(ErlNifEnv *env, const char *name);

ERL_NIF_TERM make_binary(ErlNifEnv *env, const char *str);

bool get_binary(ErlNifEnv *env, ERL_NIF_TERM bin_term, char *str, ssize_t size);

VixResult vix_result(ERL_NIF_TERM term);

int utils_init(ErlNifEnv *env);

void notify_consumed_timeslice(ErlNifEnv *env, ErlNifTime start,
                               ErlNifTime stop);
#endif
