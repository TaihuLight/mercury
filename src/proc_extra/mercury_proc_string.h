/*
 * Copyright (C) 2013 Argonne National Laboratory, Department of Energy,
 *                    UChicago Argonne, LLC and The HDF Group.
 * All rights reserved.
 *
 * The full copyright notice, including terms governing use, modification,
 * and redistribution, is contained in the COPYING file that can be
 * found at the root of the source code distribution tree.
 */

#ifndef MERCURY_PROC_STRING_H
#define MERCURY_PROC_STRING_H

#include "mercury_proc.h"
#include "mercury_string_object.h"

#include <string.h>

typedef const char * hg_const_string_t;
typedef char * hg_string_t;

#ifndef HG_PROC_STRING_INLINE
  #if defined(__GNUC__) && !defined(__GNUC_STDC_INLINE__)
    #define HG_PROC_STRING_INLINE extern HG_INLINE
  #else
    #define HG_PROC_STRING_INLINE HG_INLINE
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Inline prototypes (do not remove)
 */
HG_EXPORT HG_PROC_STRING_INLINE int hg_proc_hg_const_string_t(hg_proc_t proc, hg_const_string_t *data);
HG_EXPORT HG_PROC_STRING_INLINE int hg_proc_hg_string_t(hg_proc_t proc, hg_string_t *data);
HG_EXPORT HG_PROC_STRING_INLINE int hg_proc_hg_string_object_t(hg_proc_t proc, hg_string_object_t *data);

/**
 * Generic processing routine.
 *
 * \param proc [IN/OUT]         abstract processor object
 * \param data [IN/OUT]         pointer to data
 *
 * \return Non-negative on success or negative on failure
 */
HG_PROC_STRING_INLINE int
hg_proc_hg_const_string_t(hg_proc_t proc, hg_const_string_t *data)
{
    hg_string_object_t string;
    int ret = HG_FAIL;

    switch (hg_proc_get_op(proc)) {
        case HG_ENCODE:
            hg_string_object_init_const_char(&string, *data, 0);
            ret = hg_proc_hg_string_object_t(proc, &string);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
            }
            hg_string_object_free(&string);
            break;
        case HG_DECODE:
            ret = hg_proc_hg_string_object_t(proc, &string);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
            }
            *data = hg_string_object_swap(&string, 0);
            hg_string_object_free(&string);
            break;
        case HG_FREE:
            hg_string_object_init_const_char(&string, *data, 1);
            ret = hg_proc_hg_string_object_t(proc, &string);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
            }
            break;
        default:
            break;
    }

    return ret;
}

/**
 * Generic processing routine.
 *
 * \param proc [IN/OUT]         abstract processor object
 * \param data [IN/OUT]         pointer to data
 *
 * \return Non-negative on success or negative on failure
 */
HG_PROC_STRING_INLINE int
hg_proc_hg_string_t(hg_proc_t proc, hg_string_t *data)
{
    hg_string_object_t string;
    int ret = HG_FAIL;

    switch (hg_proc_get_op(proc)) {
        case HG_ENCODE:
            hg_string_object_init_char(&string, *data, 0);
            ret = hg_proc_hg_string_object_t(proc, &string);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
            }
            hg_string_object_free(&string);
            break;
        case HG_DECODE:
            ret = hg_proc_hg_string_object_t(proc, &string);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
            }
            *data = hg_string_object_swap(&string, 0);
            hg_string_object_free(&string);
            break;
        case HG_FREE:
            hg_string_object_init_char(&string, *data, 1);
            ret = hg_proc_hg_string_object_t(proc, &string);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
            }
            break;
        default:
            break;
    }

    return ret;
}

/**
 * Generic processing routine.
 *
 * \param proc [IN/OUT]         abstract processor object
 * \param string [IN/OUT]       pointer to string
 *
 * \return Non-negative on success or negative on failure
 */
HG_PROC_STRING_INLINE int
hg_proc_hg_string_object_t(hg_proc_t proc, hg_string_object_t *string)
{
    hg_uint64_t string_len = 0;
    int ret = HG_FAIL;

    switch (hg_proc_get_op(proc)) {
        case HG_ENCODE:
            string_len = strlen(string->data) + 1;
            ret = hg_proc_uint64_t(proc, &string_len);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            ret = hg_proc_raw(proc, string->data, string_len);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            ret = hg_proc_hg_uint8_t(proc, (hg_uint8_t*) &string->is_const);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            ret = hg_proc_hg_uint8_t(proc, (hg_uint8_t*) &string->is_owned);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            break;
        case HG_DECODE:
            ret = hg_proc_uint64_t(proc, &string_len);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            string->data = (char*) malloc(string_len);
            ret = hg_proc_raw(proc, string->data, string_len);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            ret = hg_proc_hg_uint8_t(proc, (hg_uint8_t*) &string->is_const);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            ret = hg_proc_hg_uint8_t(proc, (hg_uint8_t*) &string->is_owned);
            if (ret != HG_SUCCESS) {
                HG_ERROR_DEFAULT("Proc error");
                ret = HG_FAIL;
                return ret;
            }
            break;
        case HG_FREE:
            if (!string->data) {
                HG_ERROR_DEFAULT("Already freed");
                ret = HG_FAIL;
                return ret;
            }
            hg_string_object_free(string);
            ret = HG_SUCCESS;
            break;
        default:
            break;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* MERCURY_PROC_STRING_H */
