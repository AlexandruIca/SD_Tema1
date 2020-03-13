#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

/*
libfort

MIT License

Copyright (c) 2017 - 2020 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <sstream>
#include <stdexcept>
#include <string>

/*
libfort

MIT License

Copyright (c) 2017 - 2020 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @file fort.h
 * @brief Main header file describing libfort API.
 *
 * This file contains declarations of all libfort functions and macro
 * definitions.
 */

#ifndef LIBFORT_H
#define LIBFORT_H

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*****************************************************************************
 *               VERSION
 *****************************************************************************/

#define LIBFORT_MAJOR_VERSION 0
#define LIBFORT_MINOR_VERSION 4
#define LIBFORT_REVISION 1
#define LIBFORT_VERSION_STR "0.4.1"

/*****************************************************************************
 *               Configuration
 *****************************************************************************/

/**
 * libfort configuration macros
 * (to disable wchar_t/UTF-8 support this macros should be defined)
 */
/** #define FT_CONGIG_DISABLE_WCHAR */
/** #define FT_CONGIG_DISABLE_UTF8 */

#if !defined(FT_CONGIG_DISABLE_WCHAR)
#define FT_HAVE_WCHAR
#endif

#if !defined(FT_CONGIG_DISABLE_UTF8)
#define FT_HAVE_UTF8
#endif

/*****************************************************************************
 *               RETURN CODES
 *****************************************************************************/

/**
 * Operation successfully ended.
 */
#define FT_SUCCESS 0

/**
 * Memory allocation failed.
 */
#define FT_MEMORY_ERROR -1

/**
 * Invalid argument.
 */
#define FT_EINVAL -2

/**
 *  Libfort internal logic error.
 *
 *  Usually such errors mean that something is wrong in
 *  libfort internal logic and in most of cases cause of
 *  these errors is a library bug.
 */
#define FT_INTERN_ERROR -3

/**
 * General error.
 *
 * Different errors that do not belong to the group of errors
 * mentioned above.
 */
#define FT_GEN_ERROR -4

#define FT_IS_SUCCESS(arg) ((arg) >= 0)
#define FT_IS_ERROR(arg) ((arg) < 0)

/**
 * @cond HELPER_MACROS
 */

/*****************************************************************************
 *               Determine compiler
 *****************************************************************************/

#if defined(__clang__)
#define FT_CLANG_COMPILER
#elif defined(__GNUC__)
#define FT_GCC_COMPILER
#elif defined(_MSC_VER)
#define FT_MICROSOFT_COMPILER
#else
#define FT_UNDEFINED_COMPILER
#endif

/*****************************************************************************
 *               Declare inline
 *****************************************************************************/

#if defined(__cplusplus)
#define FT_INLINE inline
#else
#define FT_INLINE __inline
#endif /* if defined(__cplusplus) */

/*****************************************************************************
 *    C++ needs to know that types and declarations are C, not C++.
 *****************************************************************************/

#ifdef __cplusplus
#define FT_BEGIN_DECLS extern "C" {
#define FT_END_DECLS }
#else
#define FT_BEGIN_DECLS
#define FT_END_DECLS
#endif

/*****************************************************************************
 *               Helper macros
 *****************************************************************************/

#define FT_STR_2_CAT_(arg1, arg2) arg1##arg2
#define FT_STR_2_CAT(arg1, arg2) FT_STR_2_CAT_(arg1, arg2)

/**
 * @interanl
 */
static FT_INLINE int ft_check_if_string_helper(const char* str)
{
    (void)str;
    return 0;
}

/**
 * @interanl
 */
static FT_INLINE int ft_check_if_wstring_helper(const wchar_t* str)
{
    (void)str;
    return 0;
}

#define FT_NARGS_IMPL_(                                                        \
    x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, N, ...)  \
    N
#define FT_EXPAND_(x) x
#define FT_PP_NARG_(...)                                                       \
    FT_EXPAND_(FT_NARGS_IMPL_(                                                 \
        __VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define FT_CHECK_IF_STR_32(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_31(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_31(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_30(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_30(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_29(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_29(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_28(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_28(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_27(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_27(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_26(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_26(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_25(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_25(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_24(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_24(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_23(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_23(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_22(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_22(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_21(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_21(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_20(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_20(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_19(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_19(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_18(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_18(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_17(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_17(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_16(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_16(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_15(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_15(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_14(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_14(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_13(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_13(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_12(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_12(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_11(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_11(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_10(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_10(checker, arg, ...)                                  \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_9(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_9(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_8(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_8(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_7(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_7(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_6(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_6(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_5(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_5(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_4(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_4(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_3(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_3(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_2(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_2(checker, arg, ...)                                   \
    (checker(arg), FT_EXPAND_(FT_CHECK_IF_STR_1(checker, __VA_ARGS__)))
#define FT_CHECK_IF_STR_1(checker, arg) (checker(arg))

#define FT_CHECK_IF_ARGS_ARE_STRINGS__(checker, func, ...)                     \
    FT_EXPAND_(func(checker, __VA_ARGS__))
#define FT_CHECK_IF_ARGS_ARE_STRINGS_(checker, basis, n, ...)                  \
    FT_CHECK_IF_ARGS_ARE_STRINGS__(                                            \
        checker, FT_STR_2_CAT_(basis, n), __VA_ARGS__)
#define FT_CHECK_IF_ARGS_ARE_STRINGS(...)                                      \
    FT_CHECK_IF_ARGS_ARE_STRINGS_(ft_check_if_string_helper,                   \
                                  FT_CHECK_IF_STR_,                            \
                                  FT_PP_NARG_(__VA_ARGS__),                    \
                                  __VA_ARGS__)

#ifdef FT_HAVE_WCHAR
#define CHECK_IF_ARGS_ARE_WSTRINGS(...)                                        \
    FT_CHECK_IF_ARGS_ARE_STRINGS_(ft_check_if_wstring_helper,                  \
                                  FT_CHECK_IF_STR_,                            \
                                  FT_PP_NARG_(__VA_ARGS__),                    \
                                  __VA_ARGS__)
#endif

/**
 * @endcond
 */

/*****************************************************************************
 *               Attribute format for argument checking
 *****************************************************************************/

#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
#define FT_PRINTF_ATTRIBUTE_FORMAT(string_index, first_to_check)               \
    __attribute__((format(printf, string_index, first_to_check)))
#else
#define FT_PRINTF_ATTRIBUTE_FORMAT(string_index, first_to_check)
#endif /* defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER) */

/*****************************************************************************
 *                   libfort API
 *****************************************************************************/

FT_BEGIN_DECLS

/**
 * The main structure of libfort containing information about formatted table.
 */
struct ft_table;

/**
 * The main structure of libfort containing information about formatted table.
 *
 * ft_table_t objects should be created by a call to ft_create_table and
 * destroyed with ft_destroy_table.
 */
typedef struct ft_table ft_table_t;

/**
 * Create formatted table.
 *
 * @return
 *   The pointer to the new allocated ft_table_t, on success. NULL on error.
 */
ft_table_t* ft_create_table(void);

/**
 * Destroy formatted table.
 *
 * Destroy formatted table and free all resources allocated during table
 * creation and work with it.
 *
 * @param table
 *   Pointer to formatted table previousley created with ft_create_table. If
 *   table is a null pointer, the function does nothing.
 */
void ft_destroy_table(ft_table_t* table);

/**
 * Copy formatted table.
 *
 * @param table
 *   Pointer to formatted table previousley created with ft_create_table. If
 *   table is a null pointer, the function returns null.
 * @return
 *   The pointer to the new allocated ft_table_t, on success. NULL on error.
 */
ft_table_t* ft_copy_table(ft_table_t* table);

/**
 * Move current position to the first cell of the next line(row).
 *
 * @param table
 *   Pointer to formatted table.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error.
 * @note
 *   This function can fail only in case FT_STRATEGY_INSERT adding strategy
 *   was set for the table.
 */
int ft_ln(ft_table_t* table);

/**
 * Get row number of the current cell.
 *
 * @param table
 *   Pointer to formatted table.
 * @return
 *   Row number of the current cell.
 */
size_t ft_cur_row(const ft_table_t* table);

/**
 * Get column number of the current cell.
 *
 * @param table
 *   Pointer to formatted table.
 * @return
 *   Column number of the current cell.
 */
size_t ft_cur_col(const ft_table_t* table);

/**
 * Set current cell position.
 *
 * Current cell - cell that will be edited with all modifiing functions
 * (ft_printf, ft_write ...).
 *
 * @param table
 *   Pointer to formatted table.
 * @param row
 *   New row number for the current cell.
 * @param col
 *   New row number for the current cell.
 */
void ft_set_cur_cell(ft_table_t* table, size_t row, size_t col);

/**
 * Check if table is empty.
 *
 * @param table
 *   Pointer to  the table.
 * @return
 *   1 - table is empty
 *   0 - some data has been inserted
 */
int ft_is_empty(const ft_table_t* table);

/**
 * Get number of rows in the table.
 *
 * @param table
 *   Pointer to formatted table.
 * @return
 *   Number of rows in the table.
 */
size_t ft_row_count(const ft_table_t* table);

/**
 *  Erase range of cells.
 *
 *  Range of cells is determined by 2 points (top-left and bottom-right) (both
 *  ends are included).
 *
 * @param table
 *   Pointer to formatted table.
 * @param top_left_row
 *   Row number of the top left cell in the range.
 * @param top_left_col
 *   Column number of the top left cell in the range.
 * @param bottom_right_row
 *   Row number of the bottom right cell in the range.
 * @param bottom_right_col
 *   Column number of the bottom right cell in the range.
 * @return
 *   - 0 - Operation was successfully implemented
 *   - (<0): In case of error
 */
int ft_erase_range(ft_table_t* table,
                   size_t top_left_row,
                   size_t top_left_col,
                   size_t bottom_right_row,
                   size_t bottom_right_col);

#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)

/**
 * Write data formatted acording to the format string to a variety of table
 * cells.
 *
 * @param table
 *   Pointer to formatted table.
 * @param fmt
 *   Pointer to a null-terminated multibyte string specifying how to interpret
 *   the data. The format string consists of ordinary characters (except % and
 * |), which are copied unchanged into the output stream, and conversion
 *   specifications. Conversion specifications are the same as for standard
 *   printf function. Character '|' (wich can be changed with
 *   {@link ft_set_default_printf_field_separator}) in the format string is
 * treated as a cell separator.
 * @param ...
 *   Arguments specifying data to print. Similarly to standard printf-like
 *   functions if any argument after default conversions is not the type
 *   expected by the corresponding conversion specifier, or if there are fewer
 *   arguments than required by format, the behavior is undefined. If there are
 *   more arguments than required by format, the extraneous arguments are
 *   evaluated and ignored.
 * @return
 *   - Number of printed cells
 *   - (<0): In case of error
 */
int ft_printf(ft_table_t* table, const char* fmt, ...)
    FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

/**
 * Write data formatted acording to the format string to a variety of table
 * cells and move current position to the first cell of the next line(row).
 *
 * @param table
 *   Pointer to formatted table.
 * @param fmt
 *   Pointer to a null-terminated multibyte string specifying how to interpret
 *   the data. The format string consists of ordinary characters (except % and
 * |), which are copied unchanged into the output stream, and conversion
 *   specifications. Conversion specifications are the same as for standard
 *   printf function. Character '|' (wich can be changed with
 *   {@link ft_set_default_printf_field_separator}) in the format string is
 * treated as a cell separator.
 * @param ...
 *   Arguments specifying data to print. Similarly to standard printf-like
 *   functions if any argument after default conversions is not the type
 *   expected by the corresponding conversion specifier, or if there are fewer
 *   arguments than required by format, the behavior is undefined. If there are
 *   more arguments than required by format, the extraneous arguments are
 *   evaluated and ignored.
 * @return
 *   - Number of printed cells.
 *   - (<0): In case of error.
 */
int ft_printf_ln(ft_table_t* table, const char* fmt, ...)
    FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#else

/**
 * @cond IGNORE_DOC
 */

int ft_printf_impl(ft_table_t* table, const char* fmt, ...)
    FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
int ft_printf_ln_impl(ft_table_t* table, const char* fmt, ...)
    FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#define ft_printf(table, ...)                                                  \
    ((0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_impl(table, __VA_ARGS__))
#define ft_printf_ln(table, ...)                                               \
    ((0 ? fprintf(stderr, __VA_ARGS__) : 1),                                   \
     ft_printf_ln_impl(table, __VA_ARGS__))

/**
 * @endcond
 */
#endif

/**
 * Set field separator for {@link ft_printf}, {@link ft_printf_ln}
 * (default separator is '|').
 *
 * @param separator
 *   New separator.
 */
void ft_set_default_printf_field_separator(char separator);

/**
 * Write strings to the table.
 *
 * Write specified strings to the same number of consecutive cells in the
 * current row.
 *
 * @param table
 *   Pointer to formatted table.
 * @param ...
 *   Strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
#define ft_write(table, ...)                                                   \
    (0 ? FT_CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__)                             \
       : ft_nwrite(table, FT_PP_NARG_(__VA_ARGS__), __VA_ARGS__))

/**
 * Write strings to the table and go to the next line.
 *
 * Write specified strings to the same number of consecutive cells in the
 * current row and move current position to the first cell of the next
 * line(row).
 *
 * @param table
 *   Pointer to formatted table.
 * @param ...
 *   Strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
#define ft_write_ln(table, ...)                                                \
    (0 ? FT_CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__)                             \
       : ft_nwrite_ln(table, FT_PP_NARG_(__VA_ARGS__), __VA_ARGS__))

/**
 * Write specified number of strings to the table.
 *
 * Write specified number of strings to the same number of consecutive cells in
 * the current row.
 *
 * @note In most cases it is more preferable to use MACRO @ref ft_write instead
 * of @ref ft_nwrite, which is more safe (@ref ft_write automatically counts the
 * number of string arguments and at compile check that all passed arguments are
 * strings).
 *
 * @param table
 *   Pointer to formatted table.
 * @param count
 *   Number of strings to write.
 * @param cell_content
 *   First string to write.
 * @param ...
 *   Other strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
int ft_nwrite(ft_table_t* table, size_t count, const char* cell_content, ...);

/**
 * Write specified number of strings to the table and go to the next line.
 *
 * Write specified number of strings to the same number of consecutive cells
 * in the current row and move current position to the first cell of the next
 * line(row).
 *
 * @note In most cases it is more preferable to use MACRO @ref ft_write instead
 * of @ref ft_nwrite, which is more safe (@ref ft_write automatically counts the
 * number of string arguments and at compile check that all passed arguments are
 * strings).
 *
 * @param table
 *   Pointer to formatted table.
 * @param count
 *   Number of strings to write.
 * @param cell_content
 *   First string to write.
 * @param ...
 *   Other strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
int ft_nwrite_ln(ft_table_t* table,
                 size_t count,
                 const char* cell_content,
                 ...);

/**
 * Write strings from the array to the table.
 *
 * Write specified number of strings from the array to the same number of
 * consecutive cells in the current row.
 *
 * @param table
 *   Pointer to formatted table.
 * @param cols
 *   Number of elements in row_cells.
 * @param row_cells
 *   Array of strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
int ft_row_write(ft_table_t* table, size_t cols, const char* row_cells[]);

/**
 * Write strings from the array to the table and go to the next line.
 *
 * Write specified number of strings from the array to the same number of
 * consecutive cells in the current row and move current position to the first
 * cell of the next line(row).
 *
 * @param table
 *   Pointer to formatted table.
 * @param cols
 *   Number of elements in row_cells.
 * @param row_cells
 *   Array of strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
int ft_row_write_ln(ft_table_t* table, size_t cols, const char* row_cells[]);

/**
 * Write strings from the 2D array to the table.
 *
 * Write specified number of strings from the 2D array to the formatted table.
 *
 * @param table
 *   Pointer to formatted table.
 * @param rows
 *   Number of rows in the 2D array.
 * @param cols
 *   Number of columns in the 2D array.
 * @param table_cells
 *   2D array of strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
int ft_table_write(ft_table_t* table,
                   size_t rows,
                   size_t cols,
                   const char* table_cells[]);

/**
 * Write strings from the 2D array to the table and go to the next line.
 *
 * Write specified number of strings from the 2D array to the formatted table
 * and move current position to the first cell of the next line(row).
 *
 * @param table
 *   Pointer to formatted table.
 * @param rows
 *   Number of rows in the 2D array.
 * @param cols
 *   Number of columns in the 2D array.
 * @param table_cells
 *   2D array of strings to write.
 * @return
 *   - 0: Success; data were written
 *   - (<0): In case of error
 */
int ft_table_write_ln(ft_table_t* table,
                      size_t rows,
                      size_t cols,
                      const char* table_cells[]);

/**
 * Add separator after the current row.
 *
 * @param table
 *   Formatted table.
 * @return
 *   - 0: Success; separator was added.
 *   - (<0): In case of error
 */
int ft_add_separator(ft_table_t* table);

/**
 * Convert table to string representation.
 *
 * ft_table_t has ownership of the returned pointer. So there is no need to
 * free it. To take ownership user should explicitly copy the returned
 * string with strdup or similar functions.
 *
 * Returned pointer may be later invalidated by:
 * - Calling ft_destroy_table;
 * - Other invocations of ft_to_string.
 *
 * @param table
 *   Formatted table.
 * @return
 *   - The pointer to the string representation of formatted table, on success.
 *   - NULL on error.
 */
const char* ft_to_string(const ft_table_t* table);

/**
 * Structure describing border appearance.
 */
struct ft_border_chars
{
    const char* top_border_ch;
    const char* separator_ch;
    const char* bottom_border_ch;
    const char* side_border_ch;
    const char* out_intersect_ch;
    const char* in_intersect_ch;
};

/**
 * Structure describing border style.
 */
struct ft_border_style
{
    struct ft_border_chars border_chs;
    struct ft_border_chars header_border_chs;
    const char* hor_separator_char;
};

/**
 * @defgroup BasicStyles
 * @name Built-in table border styles.
 * @note Built-in border styles (FT_BASIC_STYLE, FT_BASIC2_STYLE ...) can be
 * used as arguments for @ref ft_set_border_style and
 * @ref ft_set_default_border_style, but their fields shouldn't be accessed
 * directly because implementation doesn't guarantee that these objects are
 * properly initialized.
 * @{
 */
extern const struct ft_border_style* const FT_BASIC_STYLE;
extern const struct ft_border_style* const FT_BASIC2_STYLE;
extern const struct ft_border_style* const FT_SIMPLE_STYLE;
extern const struct ft_border_style* const FT_PLAIN_STYLE;
extern const struct ft_border_style* const FT_DOT_STYLE;
extern const struct ft_border_style* const FT_EMPTY_STYLE;
extern const struct ft_border_style* const FT_EMPTY2_STYLE;
extern const struct ft_border_style* const FT_SOLID_STYLE;
extern const struct ft_border_style* const FT_SOLID_ROUND_STYLE;
extern const struct ft_border_style* const FT_NICE_STYLE;
extern const struct ft_border_style* const FT_DOUBLE_STYLE;
extern const struct ft_border_style* const FT_DOUBLE2_STYLE;
extern const struct ft_border_style* const FT_BOLD_STYLE;
extern const struct ft_border_style* const FT_BOLD2_STYLE;
extern const struct ft_border_style* const FT_FRAME_STYLE;
/** @} */

/**
 * Set default border style for all new formatted tables.
 *
 * @param style
 *   Pointer to border style.
 * @return
 *   - 0: Success; default border style was changed.
 *   - (<0): In case of error
 */
int ft_set_default_border_style(const struct ft_border_style* style);

/**
 * Set border style for the table.
 *
 * @param table
 *   A pointer to the ft_table_t structure.
 * @param style
 *   Pointer to border style.
 * @return
 *   - 0: Success; table border style was changed.
 *   - (<0): In case of error
 */
int ft_set_border_style(ft_table_t* table, const struct ft_border_style* style);

/**
 * @name Special macros to define cell position (row and column).
 * @{
 */
#define FT_ANY_COLUMN                                                          \
    (UINT_MAX) /**< Any column (can be used to refer to all cells in a row)*/
#define FT_CUR_COLUMN (UINT_MAX - 1) /**< Current column */
#define FT_ANY_ROW                                                             \
    (UINT_MAX) /**< Any row (can be used to refer to all cells in a column)*/
#define FT_CUR_ROW (UINT_MAX - 1) /**< Current row */
/** @} */

#define FT_MAX_ROW_INDEX (UINT_MAX - 2)
#define FT_MAX_COL_INDEX (UINT_MAX - 2)

/**
 * @name Cell properties identifiers.
 * @{
 */
#define FT_CPROP_MIN_WIDTH (0x01U << 0)   /**< Minimum width */
#define FT_CPROP_TEXT_ALIGN (0x01U << 1)  /**< Text alignment */
#define FT_CPROP_TOP_PADDING (0x01U << 2) /**< Top padding for cell content */
#define FT_CPROP_BOTTOM_PADDING                                                \
    (0x01U << 3) /**< Bottom padding for cell content */
#define FT_CPROP_LEFT_PADDING                                                  \
    (0x01U << 4) /**< Left padding for cell content                            \
                  */
#define FT_CPROP_RIGHT_PADDING                                                 \
    (0x01U << 5) /**< Right padding for cell content */
#define FT_CPROP_EMPTY_STR_HEIGHT (0x01U << 6) /**< Height of empty cell */
#define FT_CPROP_ROW_TYPE (0x01U << 7)         /**< Row type */
#define FT_CPROP_CONT_FG_COLOR                                                 \
    (0x01U << 8) /**< Cell content foreground text color */
#define FT_CPROP_CELL_BG_COLOR (0x01U << 9) /**< Cell background color */
#define FT_CPROP_CONT_BG_COLOR                                                 \
    (0x01U << 10) /**< Cell content background color */
#define FT_CPROP_CELL_TEXT_STYLE (0x01U << 11) /**< Cell text style */
#define FT_CPROP_CONT_TEXT_STYLE (0x01U << 12) /**< Cell content text style */
/** @} */

/**
 * Colors.
 */
enum ft_color
{
    FT_COLOR_DEFAULT = 0,        /**< Default color */
    FT_COLOR_BLACK = 1,          /**< Black color*/
    FT_COLOR_RED = 2,            /**< Red color */
    FT_COLOR_GREEN = 3,          /**< Green color */
    FT_COLOR_YELLOW = 4,         /**< Yellow color */
    FT_COLOR_BLUE = 5,           /**< Blue color */
    FT_COLOR_MAGENTA = 6,        /**< Magenta color */
    FT_COLOR_CYAN = 7,           /**< Cyan color */
    FT_COLOR_LIGHT_GRAY = 8,     /**< Light gray color */
    FT_COLOR_DARK_GRAY = 9,      /**< Dark gray color */
    FT_COLOR_LIGHT_RED = 10,     /**< Light red color */
    FT_COLOR_LIGHT_GREEN = 11,   /**< Light green color */
    FT_COLOR_LIGHT_YELLOW = 12,  /**< Light yellow color */
    FT_COLOR_LIGHT_BLUE = 13,    /**< Light blue color */
    FT_COLOR_LIGHT_MAGENTA = 15, /**< Light magenta color */
    FT_COLOR_LIGHT_CYAN = 16,    /**< Light cyan color */
    FT_COLOR_LIGHT_WHYTE = 17    /**< Light whyte color */
};

/**
 * Text styles.
 */
enum ft_text_style
{
    FT_TSTYLE_DEFAULT = (1U << 0),    /**< Default style */
    FT_TSTYLE_BOLD = (1U << 1),       /**< Bold */
    FT_TSTYLE_DIM = (1U << 2),        /**< Dim */
    FT_TSTYLE_ITALIC = (1U << 3),     /**< Italic */
    FT_TSTYLE_UNDERLINED = (1U << 4), /**< Underlined */
    FT_TSTYLE_BLINK = (1U << 5),      /**< Blink */
    FT_TSTYLE_INVERTED =
        (1U << 6), /**< Reverse (invert the foreground and background colors) */
    FT_TSTYLE_HIDDEN = (1U << 7) /**< Hidden (useful for passwords)  */
};

/**
 * Alignment of cell content.
 */
enum ft_text_alignment
{
    FT_ALIGNED_LEFT = 0, /**< Align left */
    FT_ALIGNED_CENTER,   /**< Align center */
    FT_ALIGNED_RIGHT     /**< Align right */
};

/**
 * Type of table row. Determines appearance of row.
 */
enum ft_row_type
{
    FT_ROW_COMMON = 0, /**< Common row */
    FT_ROW_HEADER      /**< Header row */
};

/**
 * Set default cell property for all new formatted tables.
 *
 * @param property
 *   Cell property identifier.
 * @param value
 *   Cell property value.
 * @return
 *   - 0: Success; default cell property was changed.
 *   - (<0): In case of error
 */
int ft_set_default_cell_prop(uint32_t property, int value);

/**
 * Set property for the specified cell of the table.
 *
 * @param table
 *   A pointer to the ft_table_t structure.
 * @param row
 *   Cell row.
 * @param col
 *   Cell column.
 * @param property
 *   Cell property identifier.
 * @param value
 *   Cell property value.
 * @return
 *   - 0: Success; cell property was changed.
 *   - (<0): In case of error
 */
int ft_set_cell_prop(
    ft_table_t* table, size_t row, size_t col, uint32_t property, int value);

/**
 * @name Table properties identifiers.
 * @{
 */
#define FT_TPROP_LEFT_MARGIN (0x01U << 0)
#define FT_TPROP_TOP_MARGIN (0x01U << 1)
#define FT_TPROP_RIGHT_MARGIN (0x01U << 2)
#define FT_TPROP_BOTTOM_MARGIN (0x01U << 3)
#define FT_TPROP_ADDING_STRATEGY (0x01U << 4)
/** @} */

/**
 * Adding strategy.
 *
 * Determines what happens with old content if current cell is not empty after
 * adding data to it. Default strategy is FT_STRATEGY_REPLACE.
 */
enum ft_adding_strategy
{
    FT_STRATEGY_REPLACE = 0, /**< Replace old content. */
    FT_STRATEGY_INSERT       /**< Insert new conten. Old content is shifted. */
};

/**
 * Set default table property.
 *
 * @param property
 *   Table property identifier.
 * @param value
 *   Table property value.
 * @return
 *   - 0: Success; default table property was changed.
 *   - (<0): In case of error
 */
int ft_set_default_tbl_prop(uint32_t property, int value);

/**
 * Set table property.
 *
 * @param table
 *   A pointer to the ft_table_t structure.
 * @param property
 *   Table property identifier.
 * @param value
 *   Table property value.
 * @return
 *   - 0: Success; default table property was changed.
 *   - (<0): In case of error
 */
int ft_set_tbl_prop(ft_table_t* table, uint32_t property, int value);

/**
 * Set column span for the specified cell of the table.
 *
 * @param table
 *   A pointer to the ft_table_t structure.
 * @param row
 *   Cell row.
 * @param col
 *   Cell column.
 * @param hor_span
 *   Column span.
 * @return
 *   - 0: Success; cell span was changed.
 *   - (<0): In case of error
 */
int ft_set_cell_span(ft_table_t* table,
                     size_t row,
                     size_t col,
                     size_t hor_span);

/**
 * Set functions for memory allocation and deallocation to be used instead of
 * standard ones.
 *
 * @param f_malloc
 *   Pointer to a function for memory allocation that should be used instead of
 *   malloc.
 * @param f_free
 *   Pointer to a function for memory deallocation that should be used instead
 *   of free.
 * @note
 *   To return memory allocation/deallocation functions to their standard values
 *   set f_malloc and f_free to NULL.
 */
void ft_set_memory_funcs(void* (*f_malloc)(size_t size),
                         void (*f_free)(void* ptr));

/**
 * Return string describing the `error_code`.
 *
 * @param error_code
 *   Error code returned by the library.
 * @return
 *   String describing the error.
 */
const char* ft_strerror(int error_code);

#ifdef FT_HAVE_WCHAR

int ft_wprintf(ft_table_t* table, const wchar_t* fmt, ...);
int ft_wprintf_ln(ft_table_t* table, const wchar_t* fmt, ...);

#define ft_wwrite(table, ...)                                                  \
    (0 ? CHECK_IF_ARGS_ARE_WSTRINGS(__VA_ARGS__)                               \
       : ft_nwwrite(table, FT_PP_NARG_(__VA_ARGS__), __VA_ARGS__))
#define ft_wwrite_ln(table, ...)                                               \
    (0 ? CHECK_IF_ARGS_ARE_WSTRINGS(__VA_ARGS__)                               \
       : ft_nwwrite_ln(table, FT_PP_NARG_(__VA_ARGS__), __VA_ARGS__))
int ft_nwwrite(ft_table_t* table, size_t n, const wchar_t* cell_content, ...);
int ft_nwwrite_ln(ft_table_t* table,
                  size_t n,
                  const wchar_t* cell_content,
                  ...);

int ft_row_wwrite(ft_table_t* table, size_t cols, const wchar_t* row_cells[]);
int ft_row_wwrite_ln(ft_table_t* table,
                     size_t cols,
                     const wchar_t* row_cells[]);

int ft_table_wwrite(ft_table_t* table,
                    size_t rows,
                    size_t cols,
                    const wchar_t* table_cells[]);
int ft_table_wwrite_ln(ft_table_t* table,
                       size_t rows,
                       size_t cols,
                       const wchar_t* table_cells[]);

const wchar_t* ft_to_wstring(const ft_table_t* table);
#endif

#ifdef FT_HAVE_UTF8
#define ft_u8write(table, ...)                                                 \
    (ft_u8nwrite(table, FT_PP_NARG_(__VA_ARGS__), __VA_ARGS__))
#define ft_u8write_ln(table, ...)                                              \
    (ft_u8nwrite_ln(table, FT_PP_NARG_(__VA_ARGS__), __VA_ARGS__))
int ft_u8nwrite(ft_table_t* table, size_t n, const void* cell_content, ...);
int ft_u8nwrite_ln(ft_table_t* table, size_t n, const void* cell_content, ...);

int ft_u8printf(ft_table_t* table, const char* fmt, ...)
    FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
int ft_u8printf_ln(ft_table_t* table, const char* fmt, ...)
    FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

const void* ft_to_u8string(const ft_table_t* table);

/**
 * Set custom function to compute visible width of UTF-8 string.
 *
 * libfort internally has a very simple logic to compute visible width of UTF-8
 * strings. It considers that each codepoint will occupy one position on the
 * terminal in case of monowidth font (some east asians wide and fullwidth
 * characters (see http://www.unicode.org/reports/tr11/tr11-33.html) will occupy
 * 2 positions). This logic is very simple and covers wide range of cases. But
 * obviously there a lot of cases when it is not sufficient. In such cases user
 * should use some external libraries and provide an appropriate function to
 * libfort.
 *
 * @param u8strwid
 *   User provided function to evaluate width of UTF-8 string ( beg - start of
 *   UTF-8 string, end - end of UTF-8 string (not included), width - pointer to
 *   the result). If function succeed it should return 0, otherwise some non-
 *   zero value. If function returns nonzero value libfort fallbacks to default
 *   internal algorithm.
 */
void ft_set_u8strwid_func(int (*u8strwid)(const void* beg,
                                          const void* end,
                                          size_t* width));

#endif /* FT_HAVE_UTF8 */

FT_END_DECLS

#endif /* LIBFORT_H */

namespace fort {

/**
 * Alignment of cell content.
 */
enum class text_align
{
    left = FT_ALIGNED_LEFT,
    center = FT_ALIGNED_CENTER,
    right = FT_ALIGNED_RIGHT
};

/**
 * Type of table row. Determines appearance of row.
 */
enum class row_type
{
    common = FT_ROW_COMMON,
    header = FT_ROW_HEADER
};

/**
 * Adding strategy.
 *
 * Determines what happens with old content if current cell is not empty after
 * adding data to it. Default strategy is 'replace'.
 */
enum class add_strategy
{
    replace = FT_STRATEGY_REPLACE,
    insert = FT_STRATEGY_INSERT
};

/**
 * Colors.
 */
enum class color
{
    default_color = FT_COLOR_DEFAULT,
    black = FT_COLOR_BLACK,
    red = FT_COLOR_RED,
    green = FT_COLOR_GREEN,
    yellow = FT_COLOR_YELLOW,
    blue = FT_COLOR_BLUE,
    magenta = FT_COLOR_MAGENTA,
    cyan = FT_COLOR_CYAN,
    light_gray = FT_COLOR_LIGHT_GRAY,
    dark_gray = FT_COLOR_DARK_GRAY,
    light_red = FT_COLOR_LIGHT_RED,
    light_green = FT_COLOR_LIGHT_GREEN,
    light_yellow = FT_COLOR_LIGHT_YELLOW,
    light_blue = FT_COLOR_LIGHT_BLUE,
    light_magenta = FT_COLOR_LIGHT_MAGENTA,
    light_cyan = FT_COLOR_LIGHT_CYAN,
    light_whyte = FT_COLOR_LIGHT_WHYTE
};

/**
 * Text styles.
 */
enum class text_style
{
    default_style = FT_TSTYLE_DEFAULT,
    bold = FT_TSTYLE_BOLD,
    dim = FT_TSTYLE_DIM,
    italic = FT_TSTYLE_ITALIC,
    underlined = FT_TSTYLE_UNDERLINED,
    blink = FT_TSTYLE_BLINK,
    inverted = FT_TSTYLE_INVERTED,
    hidden = FT_TSTYLE_HIDDEN
};

enum class table_type
{
    character,
#ifdef FT_HAVE_UTF8
    utf8
#endif /* FT_HAVE_UTF8 */
};

/**
 * Table manipulator.
 *
 * Table manipulators can be used to change current cell and change appearance
 * of cells.
 */
class table_manipulator
{
public:
    explicit table_manipulator(int i)
        : value(i)
    {
    }
    template<table_type TT>
    friend class table;

private:
    int value;
};

/**
 * Table manipulator to set current row as a header.
 */
const table_manipulator header(0);

/**
 * Table manipulator to move current cell to the first cell of the next row.
 */
const table_manipulator endr(1);

/**
 * Table manipulator to add separator to the table.
 */
const table_manipulator separator(2);

/**
 * Property owner.
 *
 * property_owner is a base class for all objects (table, row, column, cell) for
 * which user can specify properties.
 */
template<typename table>
class property_owner
{
public:
    property_owner(std::size_t row_idx,
                   std::size_t coll_idx,
                   table* tbl,
                   bool def = false)
        : ps_row_idx_(row_idx)
        , ps_coll_idx_(coll_idx)
        , ps_table_(tbl)
        , set_default_properties_(def)
    {
    }

    /**
     * Set min width for the specified cell of the table.
     *
     * @param value
     *   Value of the min width.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error
     */
    bool set_cell_min_width(unsigned value)
    {
        return set_property(FT_CPROP_MIN_WIDTH, value);
    }

    /**
     * Set text alignment for the specified cell of the table.
     *
     * @param value
     *   Value of the text alignment.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error
     */
    bool set_cell_text_align(fort::text_align value)
    {
        return set_property(FT_CPROP_TEXT_ALIGN, static_cast<int>(value));
    }

    /**
     * Set top padding for the specified cell of the table.
     *
     * @param value
     *   Value of the top padding.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_top_padding(unsigned value)
    {
        return set_property(FT_CPROP_TOP_PADDING, value);
    }

    /**
     * Set bottom padding for the specified cell of the table.
     *
     * @param value
     *   Value of the bottom padding.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_bottom_padding(unsigned value)
    {
        return set_property(FT_CPROP_BOTTOM_PADDING, value);
    }

    /**
     * Set left padding for the specified cell of the table.
     *
     * @param value
     *   Value of the left padding.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_left_padding(unsigned value)
    {
        return set_property(FT_CPROP_LEFT_PADDING, value);
    }

    /**
     * Set right padding for the specified cell of the table.
     *
     * @param value
     *   Value of the left padding.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_right_padding(unsigned value)
    {
        return set_property(FT_CPROP_RIGHT_PADDING, value);
    }

    /**
     * Set row type for the specified cell of the table.
     *
     * @param value
     *   Value of the row type.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_row_type(fort::row_type value)
    {
        return set_property(FT_CPROP_ROW_TYPE, static_cast<int>(value));
    }

    /**
     * Set empty string height for the specified cell of the table.
     *
     * @param value
     *   Value of the empty string height.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_empty_str_height(unsigned value)
    {
        return set_property(FT_CPROP_EMPTY_STR_HEIGHT, value);
    }

    /**
     * Set content foreground color for the specified cell of the table.
     *
     * @param value
     *   Color.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_content_fg_color(fort::color value)
    {
        return set_property(FT_CPROP_CONT_FG_COLOR, static_cast<int>(value));
    }

    /**
     * Set cell background color for the specified cell of the table.
     *
     * @param value
     *   Color.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_bg_color(fort::color value)
    {
        return set_property(FT_CPROP_CELL_BG_COLOR, static_cast<int>(value));
    }

    /**
     * Set content background color for the specified cell of the table.
     *
     * @param value
     *   Color.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_content_bg_color(fort::color value)
    {
        return set_property(FT_CPROP_CONT_BG_COLOR, static_cast<int>(value));
    }

    /**
     * Set cell text style for the specified cell of the table.
     *
     * @param value
     *   Text style.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_text_style(fort::text_style value)
    {
        return set_property(FT_CPROP_CELL_TEXT_STYLE, static_cast<int>(value));
    }

    /**
     * Set content text style for the specified cell of the table.
     *
     * @param value
     *   Text style.
     * @return
     *   - true: Success; cell property was changed.
     *   - false: In case of error.
     */
    bool set_cell_content_text_style(fort::text_style value)
    {
        return set_property(FT_CPROP_CONT_TEXT_STYLE, static_cast<int>(value));
    }

protected:
    std::size_t ps_row_idx_;
    std::size_t ps_coll_idx_;
    table* ps_table_;
    bool set_default_properties_;

    bool set_property(uint32_t property, int value)
    {
        int status;
        if(set_default_properties_) {
            status = ft_set_default_cell_prop(property, value);
        }
        else {
            status = ft_set_cell_prop(
                ps_table_->table_, ps_row_idx_, ps_coll_idx_, property, value);
        }
        return FT_IS_SUCCESS(status);
    }
};

/**
 * Formatted table.
 *
 * Table template class is a C++ wrapper around struct {@link ft_table}.
 * Template parameter is {@link table_type}. Useful instantiations of table
 * template class are {@link char_table} and {@link utf8_table}.
 */
template<table_type TT = table_type::character>
class table : public property_owner<table<TT>>
{
    /**
     *  Utility types.
     */
    using table_t = table<TT>;
    using property_owner_t = property_owner<table_t>;

public:
    /**
     * Default constructor.
     */
    table()
        : property_owner_t(FT_ANY_ROW, FT_ANY_COLUMN, this)
        , table_(ft_create_table())
    {
        if(table_ == NULL)
            throw std::bad_alloc();
    }

    /**
     * Destructor.
     */
    ~table()
    {
        ft_destroy_table(table_);
    }

    /**
     * Copy contstructor.
     */
    table(const table& tbl)
        : property_owner_t(FT_ANY_ROW, FT_ANY_COLUMN, this)
        , table_(NULL)
    {
        if(tbl.table_) {
            ft_table_t* table_copy = ft_copy_table(tbl.table_);
            if(table_copy == NULL)
                throw std::runtime_error("Error during table copy");

            stream_.str(std::string());
            if(tbl.stream_.tellp() >= 0) {
                stream_ << tbl.stream_.str();
            }
            table_ = table_copy;
        }
    }

    /**
     * Move contstructor.
     */
    table(table&& tbl)
        : property_owner_t(FT_ANY_ROW, FT_ANY_COLUMN, this)
        , table_(tbl.table_)
    {
        if(tbl.stream_.tellp() >= 0) {
            stream_ << tbl.stream_.str();
            tbl.stream_.str(std::string());
        }
        tbl.table_ = 0;
    }

    /**
     * Copy assignment operator.
     */
    table& operator=(const table& tbl)
    {
        if(&tbl == this)
            return *this;

        if(tbl.table_) {
            ft_table_t* table_copy = ft_copy_table(tbl.table_);
            if(table_copy == NULL)
                throw std::runtime_error("Error during table copy");

            stream_.str(std::string());
            if(tbl.stream_.tellp() >= 0) {
                stream_ << tbl.stream_.str();
            }
            ft_destroy_table(table_);
            table_ = table_copy;
        }
        return *this;
    }

    /**
     * Move assignment operator.
     */
    table& operator=(table&& tbl)
    {
        if(&tbl == this)
            return *this;

        if(tbl.table_) {
            stream_.str(std::string());
            if(tbl.stream_.tellp() >= 0) {
                stream_ << tbl.stream_.str();
                tbl.stream_.str(std::string());
            }
            ft_destroy_table(table_);
            table_ = tbl.table_;
            tbl.table_ = NULL;
        }
        return *this;
    }

    /**
     * Convert table to string representation.
     *
     * @return
     *   - String representation of formatted table, on success.
     *   - In case of error std::runtime_error is thrown.
     */
    std::string to_string() const
    {
        const char* str = c_str();
        if(str == NULL)
            throw std::runtime_error("Error during table to string conversion");
        return str;
    }

    /**
     * Convert table to string representation.
     *
     * Table object has ownership of the returned pointer. So there is no need
     * to free it. To take ownership user should explicitly copy the returned
     * string with strdup or similar functions.
     *
     * Returned pointer may be later invalidated by:
     * - Calling destroying the table;
     * - Other invocations of c_str or to_string.
     *
     * @return
     *   - The pointer to the string representation of formatted table, on
     * success.
     *   - NULL on error.
     */
    const char* c_str() const
    {
#ifdef FT_HAVE_UTF8
        return (TT == table_type::character)
                   ? ft_to_string(table_)
                   : static_cast<const char*>(ft_to_u8string(table_));
#else
        return ft_to_string(table_);
#endif
    }

    /**
     * Write provided object to the table.
     *
     * To convert object to the string representation conversion for
     * std::ostream is used.
     *
     * @param arg
     *   Obect that would be inserted in the current cell.
     * @return
     *   - Reference to the current table.
     */
    template<typename T>
    table& operator<<(const T& arg)
    {
        stream_ << arg;
        if(stream_.tellp() >= 0) {
#ifdef FT_HAVE_UTF8
            if(TT == table_type::character) {
                ft_nwrite(table_, 1, stream_.str().c_str());
            }
            else {
                ft_u8nwrite(
                    table_, 1, static_cast<const void*>(stream_.str().c_str()));
            }
#else
            ft_nwrite(table_, 1, stream_.str().c_str());
#endif

            stream_.str(std::string());
        }
        return *this;
    }

    table& operator<<(const table_manipulator& arg)
    {
        if(arg.value == header.value)
            ft_set_cell_prop(table_,
                             FT_CUR_ROW,
                             FT_ANY_ROW,
                             FT_CPROP_ROW_TYPE,
                             FT_ROW_HEADER);
        else if(arg.value == endr.value)
            ft_ln(table_);
        else if(arg.value == separator.value)
            ft_add_separator(table_);
        return *this;
    }

    /**
     * Write string to the the table.
     *
     * Write specified string to the current cell.
     *
     * @param str
     *   String to write.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    bool write(const char* str)
    {
#ifdef FT_HAVE_UTF8
        if(TT == table_type::character) {
            return FT_IS_SUCCESS(ft_write(table_, str));
        }
        else {
            return FT_IS_SUCCESS(
                ft_u8write(table_, static_cast<const void*>(str)));
        }
#else
        return FT_IS_SUCCESS(ft_write(table_, str));
#endif
    }

    /**
     * Write string to the the table and go to the next line.
     *
     * Write specified string to the current cell and move current position to
     * the first cell of the next line(row).
     *
     * @param str
     *   String to write.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    bool write_ln(const char* str)
    {
#ifdef FT_HAVE_UTF8
        if(TT == table_type::character) {
            return FT_IS_SUCCESS(ft_write_ln(table_, str));
        }
        else {
            return FT_IS_SUCCESS(ft_u8write_ln(table_, str));
        }
#else
        return FT_IS_SUCCESS(ft_write_ln(table_, str));
#endif
    }

    /**
     * Write string to the the table.
     *
     * Write specified string to the current cell.
     *
     * @param str
     *   String to write.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    bool write(const std::string& str)
    {
        return write(str.c_str());
    }

    /**
     * Write string to the the table and go to the next line.
     *
     * Write specified string to the current cell and move current position to
     * the first cell of the next line(row).
     *
     * @param str
     *   String to write.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    bool write_ln(const std::string& str)
    {
        return write_ln(str.c_str());
    }

#ifdef __cpp_variadic_templates
    /**
     * Write strings to the table.
     *
     * Write specified strings to the same number of consecutive cells in the
     * current row.
     *
     * @param str
     *   String to write.
     * @param strings
     *   Strings to write.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    template<typename T, typename... Ts>
    bool write(const T& str, const Ts&... strings)
    {
        return write(str) && write(strings...);
    }

    /**
     * Write strings to the table and go to the next line.
     *
     * Write specified strings to the same number of consecutive cells in the
     * current row  and move current position to the first cell of the next
     * line(row).
     *
     * @param str
     *   String to write.
     * @param strings
     *   Strings to write.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    template<typename T, typename... Ts>
    bool write_ln(const T& str, const Ts&... strings)
    {
        return write(str) && write_ln(strings...);
    }
#else /* __cpp_variadic_templates */

    template<typename T_0, typename T_1>
    bool write(const T_0& arg_0, const T_1& arg_1)
    {
        return write(arg_0) && write(arg_1);
    }

    template<typename T_0, typename T_1, typename T_2>
    bool write(const T_0& arg_0, const T_1& arg_1, const T_2& arg_2)
    {
        return write(arg_0) && write(arg_1, arg_2);
    }

    template<typename T_0, typename T_1, typename T_2, typename T_3>
    bool write(const T_0& arg_0,
               const T_1& arg_1,
               const T_2& arg_2,
               const T_3& arg_3)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4>
    bool write(const T_0& arg_0,
               const T_1& arg_1,
               const T_2& arg_2,
               const T_3& arg_3,
               const T_4& arg_4)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4,
             typename T_5>
    bool write(const T_0& arg_0,
               const T_1& arg_1,
               const T_2& arg_2,
               const T_3& arg_3,
               const T_4& arg_4,
               const T_5& arg_5)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4, arg_5);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4,
             typename T_5,
             typename T_6>
    bool write(const T_0& arg_0,
               const T_1& arg_1,
               const T_2& arg_2,
               const T_3& arg_3,
               const T_4& arg_4,
               const T_5& arg_5,
               const T_6& arg_6)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4,
             typename T_5,
             typename T_6,
             typename T_7>
    bool write(const T_0& arg_0,
               const T_1& arg_1,
               const T_2& arg_2,
               const T_3& arg_3,
               const T_4& arg_4,
               const T_5& arg_5,
               const T_6& arg_6,
               const T_7& arg_7)
    {
        return write(arg_0) &&
               write(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7);
    }

    template<typename T_0, typename T_1>
    bool write_ln(const T_0& arg_0, const T_1& arg_1)
    {
        return write(arg_0) && write_ln(arg_1);
    }

    template<typename T_0, typename T_1, typename T_2>
    bool write_ln(const T_0& arg_0, const T_1& arg_1, const T_2& arg_2)
    {
        return write(arg_0) && write_ln(arg_1, arg_2);
    }

    template<typename T_0, typename T_1, typename T_2, typename T_3>
    bool write_ln(const T_0& arg_0,
                  const T_1& arg_1,
                  const T_2& arg_2,
                  const T_3& arg_3)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4>
    bool write_ln(const T_0& arg_0,
                  const T_1& arg_1,
                  const T_2& arg_2,
                  const T_3& arg_3,
                  const T_4& arg_4)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3, arg_4);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4,
             typename T_5>
    bool write_ln(const T_0& arg_0,
                  const T_1& arg_1,
                  const T_2& arg_2,
                  const T_3& arg_3,
                  const T_4& arg_4,
                  const T_5& arg_5)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3, arg_4, arg_5);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4,
             typename T_5,
             typename T_6>
    bool write_ln(const T_0& arg_0,
                  const T_1& arg_1,
                  const T_2& arg_2,
                  const T_3& arg_3,
                  const T_4& arg_4,
                  const T_5& arg_5,
                  const T_6& arg_6)
    {
        return write(arg_0) &&
               write_ln(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);
    }

    template<typename T_0,
             typename T_1,
             typename T_2,
             typename T_3,
             typename T_4,
             typename T_5,
             typename T_6,
             typename T_7>
    bool write_ln(const T_0& arg_0,
                  const T_1& arg_1,
                  const T_2& arg_2,
                  const T_3& arg_3,
                  const T_4& arg_4,
                  const T_5& arg_5,
                  const T_6& arg_6,
                  const T_7& arg_7)
    {
        return write(arg_0) &&
               write_ln(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7);
    }

#endif /* __cpp_variadic_templates */

    /**
     * Write elements from range to the table.
     *
     * Write objects from range to consecutive cells in the current row.
     *
     * @param first, last
     *   Range of elements.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    template<typename InputIt>
    bool range_write(InputIt first, InputIt last)
    {
        while(first != last) {
            *this << *first;
            ++first;
        }
        return true;
    }

    /**
     * Write elements from range to the table and go to the next line.
     *
     * Write objects from range to consecutive cells in the current row and
     * move current position to the first cell of the next line(row).
     *
     * @param first, last
     *   Range of elements.
     * @return
     *   - 0: Success; data were written
     *   - (<0): In case of error
     */
    template<typename InputIt>
    bool range_write_ln(InputIt first, InputIt last)
    {
        while(first != last) {
            *this << *first;
            ++first;
        }
        ft_ln(table_);
        return true;
    }

    /**
     * Set border style for the table.
     *
     * @param style
     *   Pointer to border style.
     * @return
     *   - True: Success; table border style was changed.
     *   - False: Error
     */
    bool set_border_style(const struct ft_border_style* style)
    {
        return FT_IS_SUCCESS(ft_set_border_style(table_, style));
    }

    /**
     * Set current cell position.
     *
     * Current cell - cell that will be edited with all modifiing functions.
     *
     * @param row_i
     *   New row number for the current cell.
     * @param col_i
     *   New row number for the current cell.
     */
    void set_cur_cell(size_t row_i, size_t col_i)
    {
        ft_set_cur_cell(table_, row_i, col_i);
    }

    /**
     * Set table left margin.
     *
     * @param value
     *   Left margin.
     * @return
     *   - true: Success; table property was changed.
     *   - false: In case of error.
     */
    bool set_left_margin(unsigned value)
    {
        return FT_IS_SUCCESS(
            ft_set_tbl_prop(table_, FT_TPROP_LEFT_MARGIN, value));
    }

    /**
     * Set table top margin.
     *
     * @param value
     *   Top margin.
     * @return
     *   - true: Success; table property was changed.
     *   - false: In case of error.
     */
    bool set_top_margin(unsigned value)
    {
        return FT_IS_SUCCESS(
            ft_set_tbl_prop(table_, FT_TPROP_TOP_MARGIN, value));
    }

    /**
     * Set table right margin.
     *
     * @param value
     *   Right margin.
     * @return
     *   - true: Success; table property was changed.
     *   - false: In case of error.
     */
    bool set_right_margin(unsigned value)
    {
        return FT_IS_SUCCESS(
            ft_set_tbl_prop(table_, FT_TPROP_RIGHT_MARGIN, value));
    }

    /**
     * Set table bottom margin.
     *
     * @param value
     *   Bottom margin.
     * @return
     *   - true: Success; table property was changed.
     *   - false: In case of error.
     */
    bool set_bottom_margin(unsigned value)
    {
        return FT_IS_SUCCESS(
            ft_set_tbl_prop(table_, FT_TPROP_BOTTOM_MARGIN, value));
    }

    /**
     * Set table adding strategy.
     *
     * @param value
     *   Adding strategy.
     * @return
     *   - true: Success; table property was changed.
     *   - false: In case of error.
     */
    bool set_adding_strategy(fort::add_strategy value)
    {
        return FT_IS_SUCCESS(ft_set_tbl_prop(
            table_, FT_TPROP_ADDING_STRATEGY, static_cast<int>(value)));
    }

private:
    ft_table_t* table_;
    mutable std::stringstream stream_;
    friend class property_owner<table>;

public:
    /* Iterators */
    /* todo: implement chains like table[0][0] = table [0][1] = "somethings" */

    /**
     * Table cell.
     */
    class table_cell : public property_owner_t
    {
        using property_owner_t::ps_coll_idx_;
        using property_owner_t::ps_row_idx_;
        using property_owner_t::ps_table_;
        using property_owner_t::set_default_properties_;

    public:
        table_cell(std::size_t row_idx, std::size_t coll_idx, table& tbl)
            : property_owner_t(row_idx, coll_idx, &tbl)
        {
        }

        table_cell& operator=(const char* str)
        {
            ft_set_cur_cell(ps_table_->table_, ps_row_idx_, ps_coll_idx_);
            ps_table_->write(str);
            return *this;
        }

        table_cell& operator=(const std::string& str)
        {
            return operator=(str.c_str());
        }

        /**
         * Set column span for the specified cell of the table.
         *
         * @param hor_span
         *   Column span.
         * @return
         *   - true: Success; cell span was changed.
         *   - false: In case of error.
         */
        bool set_cell_span(size_t hor_span)
        {
            if(set_default_properties_)
                return false;

            return FT_IS_SUCCESS(ft_set_cell_span(
                ps_table_->table_, ps_row_idx_, ps_coll_idx_, hor_span));
        }
    };

    /**
     * Table row.
     */
    class table_row : public property_owner_t
    {
        using property_owner_t::ps_row_idx_;
        using property_owner_t::ps_table_;

    public:
        table_row(std::size_t row_idx, table& tbl)
            : property_owner_t(row_idx, FT_ANY_COLUMN, &tbl)
        {
        }

        class table_cell operator[](std::size_t coll_idx)
        {
            return table_cell(ps_row_idx_, coll_idx, *ps_table_);
        }

        void erase()
        {
            if(FT_IS_ERROR(ft_erase_range(ps_table_->table_,
                                          property_owner_t::ps_row_idx_,
                                          0,
                                          property_owner_t::ps_row_idx_,
                                          FT_MAX_COL_INDEX))) {
                throw std::runtime_error("Failed to erase row");
            }
        }
    };

    /**
     * Table column.
     */
    class table_column : public property_owner_t
    {
        using property_owner_t::ps_coll_idx_;
        using property_owner_t::ps_table_;

    public:
        table_column(std::size_t col_idx, table& tbl)
            : property_owner_t(FT_ANY_ROW, col_idx, &tbl)
        {
        }

        void erase()
        {
            if(FT_IS_ERROR(ft_erase_range(ps_table_->table_,
                                          0,
                                          ps_coll_idx_,
                                          FT_MAX_ROW_INDEX,
                                          ps_coll_idx_))) {
                throw std::runtime_error("Failed to erase column");
            }
        }
    };

    /**
     * Range of cells.
     *
     * @note: at the moment function of propery owener will work only on the
     * top left cell.
     * @todo: Implement their work on the whole range.
     */
    class cell_range : public property_owner_t
    {
        using property_owner_t::ps_coll_idx_;
        using property_owner_t::ps_row_idx_;
        using property_owner_t::ps_table_;

    public:
        cell_range(size_t top_left_row,
                   size_t top_left_col,
                   size_t bottom_right_row,
                   size_t bottom_right_col,
                   table& tbl)
            : property_owner_t(top_left_row, top_left_col, &tbl)
            , bottom_right_row_(bottom_right_row)
            , bottom_right_col_(bottom_right_col)
        {
        }

        void erase()
        {
            if(FT_IS_ERROR(ft_erase_range(ps_table_->table_,
                                          ps_row_idx_,
                                          ps_coll_idx_,
                                          bottom_right_row_,
                                          bottom_right_col_))) {
                throw std::runtime_error("Failed to erase column");
            }
        }

    private:
        std::size_t bottom_right_row_;
        std::size_t bottom_right_col_;
    };

    class default_properties : public property_owner_t
    {
    public:
        default_properties(table* tbl)
            : property_owner_t(FT_ANY_ROW, FT_ANY_COLUMN, tbl, true)
        {
        }
    };

    class table_row operator[](std::size_t row_idx)
    {
        return table_row(row_idx, *this);
    }

    /**
     * Get cell.
     *
     * @param row_idx
     *   Row index.
     * @param col_idx
     *   Column index.
     * @return
     *   table_cell object.
     */
    class table_cell cell(std::size_t row_idx, std::size_t col_idx)
    {
        return (*this)[row_idx][col_idx];
    }

    /**
     * Get column number of the current cell.
     *
     * @return
     *   Column number of the current cell.
     */
    size_t cur_col() const noexcept
    {
        return ft_cur_col(table_);
    }

    /**
     * Get row number of the current cell.
     *
     * @return
     *   Row number of the current cell.
     */
    size_t cur_row() const noexcept
    {
        return ft_cur_row(table_);
    }

    /**
     * Check if table is empty.
     *
     * @return
     *   true - table is empty
     *   false - some data has been inserted
     */
    bool is_empty() const noexcept
    {
        return ft_is_empty(table_);
    }

    /**
     * Get number of rows in the table.
     *
     * @return
     *   Number of rows in the table.
     */
    std::size_t row_count() const noexcept
    {
        return ft_row_count(table_);
    }

    /**
     * Get current cell.
     *
     * @return
     *   Current cell.
     */
    class table_cell cur_cell()
    {
        return cell(cur_row(), cur_col());
    }

    /**
     * Get row.
     *
     * @param row_idx
     *   Row index.
     * @return
     *   table_row object.
     */
    class table_row row(std::size_t row_idx)
    {
        return table_row(row_idx, *this);
    }

    /**
     * Get column.
     *
     * @param col_idx
     *   Column index.
     * @return
     *   table_column object.
     */
    class table_column column(std::size_t col_idx)
    {
        return table_column(col_idx, *this);
    }

    /**
     * Get range of cells.
     *
     * @param col_idx
     *   Column index.
     * @return
     *   table_column object.
     */
    class cell_range range(std::size_t top_left_row,
                           std::size_t top_left_col,
                           std::size_t bottom_right_row,
                           std::size_t bottom_right_col)
    {
        return cell_range(top_left_row,
                          top_left_col,
                          bottom_right_row,
                          bottom_right_col,
                          *this);
    }

    static class default_properties default_props()
    {
        return default_properties(NULL);
    }
};

/**
 * Formatted table containing common char content.
 *
 * Content of the table is treated as a string where each byte represesents a
 * character. Should work for ascii characters. In case of usage of different
 * international symbols it is recommended to use {@link utf8_table}.
 */
using char_table = table<table_type::character>;

#ifdef FT_HAVE_UTF8
/**
 * Formatted table containing utf-8 content.
 */
using utf8_table = table<table_type::utf8>;
#endif

/**
 * Set default border style for all new formatted tables.
 *
 * @param style
 *   Pointer to border style.
 * @return
 *   - True: Success; table border style was changed.
 *   - False: Error
 */
inline bool set_default_border_style(struct ft_border_style* style)
{
    return FT_IS_SUCCESS(ft_set_default_border_style(style));
}

} // namespace fort

template<typename T>
auto operator<<(std::ostream& os, std::vector<T> const& v) -> std::ostream&
{
    os << '[';
    if(v.size() >= 1) {
        os << v.front();

        if(v.size() >= 2) {
            for(int i = 1; i < v.size(); ++i) {
                os << ", " << v[i];
            }
        }
    }
    os << ']';
    return os;
}

enum class sort_type
{
    empty = 0,
    sorted,
    sorted_repeat,
    reversed,
    reversed_repeat,
    shuffled,
    shuffled_repeat,
    random
};

class sort;

auto get_tests() -> std::vector<sort*>&
{
    static std::vector<sort*> tests;
    return tests;
}

class sort
{
private:
    std::string m_name{};
    fort::utf8_table m_table{};

protected:
    sort(std::string const name)
        : m_name{ std::move(name) }
    {
        get_tests().push_back(this);

        m_table.set_border_style(FT_NICE_STYLE);

        m_table << fort::header << m_name << ""
                << ""
                << ""
                << ""
                << "";
        m_table << fort::endr;
        m_table << "TYPE"
                << "SUCCESSFUL?"
                << "INPUT_SIZE"
                << "MAX_INPUT"
                << "DURATION(ms)"
                << "DURATION(std::sort)" << fort::endr;
    }

public:
    sort() = delete;
    sort(sort const&) = default;
    virtual ~sort() noexcept = default;

    virtual auto do_sort(std::vector<int> const& input,
                         std::vector<int>& output) -> void = 0;

    auto benchmark(std::vector<int>& input,
                   std::vector<int>& output,
                   std::string const& type,
                   int const array_size,
                   int const biggest_num) -> void
    {
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        this->do_sort(input, output);
        auto end = high_resolution_clock::now();

        auto sort_duration = duration_cast<milliseconds>(end - start).count();

        auto start_std = high_resolution_clock::now();
        std::sort(input.begin(), input.end());
        auto end_std = high_resolution_clock::now();

        auto std_sort_duration =
            duration_cast<milliseconds>(end_std - start_std).count();

        bool successful{ true };
        if(input != output) {
            successful = false;
        }

        m_table << type << (successful ? "YES" : "NO") << array_size
                << biggest_num << sort_duration << std_sort_duration
                << fort::endr;
    }

    [[nodiscard]] auto format() -> std::string
    {
        m_table.row(0)[0].set_cell_text_align(fort::text_align::center);
        m_table.row(0)[0].set_cell_bg_color(fort::color::red);
        m_table.row(0)[0].set_cell_span(6);

        m_table.row(1)[0].set_cell_content_fg_color(fort::color::yellow);
        m_table.row(1)[1].set_cell_content_fg_color(fort::color::red);
        m_table.row(1)[2].set_cell_content_fg_color(fort::color::light_blue);
        m_table.row(1)[3].set_cell_content_fg_color(fort::color::light_blue);
        m_table.row(1)[4].set_cell_content_fg_color(fort::color::green);
        m_table.row(1)[5].set_cell_content_fg_color(fort::color::green);

        for(int i = 2; i < m_table.row_count(); ++i) {
            m_table[i][0].set_cell_text_align(fort::text_align::left);
            m_table[i][1].set_cell_text_align(fort::text_align::center);
            m_table[i][2].set_cell_text_align(fort::text_align::center);
            m_table[i][3].set_cell_text_align(fort::text_align::center);
            m_table[i][4].set_cell_text_align(fort::text_align::right);
            m_table[i][4].set_cell_content_fg_color(fort::color::magenta);
            m_table[i][5].set_cell_text_align(fort::text_align::right);
            m_table[i][5].set_cell_content_fg_color(fort::color::magenta);
        }

        return m_table.to_string();
    }

    [[nodiscard]] inline auto name() const noexcept -> std::string const&
    {
        return m_name;
    }
};

#define PASTE(a, b) a##b
#define PASTE_2(a, b) PASTE(a, b)
#define RAND(name) PASTE_2(name, __LINE__)

#define NEW_SORT(sort_name, input_name, output_name)                           \
    class RAND(sort__)                                                         \
        : public sort                                                          \
    {                                                                          \
    public:                                                                    \
        RAND(sort__)                                                           \
        ()                                                                     \
            : sort{ sort_name }                                                \
        {                                                                      \
        }                                                                      \
        virtual ~RAND(sort__)() noexcept override = default;                   \
                                                                               \
        auto do_sort(std::vector<int> const&, std::vector<int>&)               \
            -> void override;                                                  \
    };                                                                         \
    static RAND(sort__) RAND(sort_tmp_var__);                                  \
                                                                               \
    auto RAND(sort__)::do_sort(                                                \
        [[maybe_unused]] std::vector<int> const& input_name,                   \
        std::vector<int>& output_name)                                         \
        ->void

// Use like this:
//
// NEW_SORT("Countsort", input, output) {
//      std::vector<int> frecv(find_max(input), 0);
//      output.reserve(input.size());
//      // sort...
// }

auto find_max(std::vector<int> const& v) -> int
{
    int max{ 0 };

    for(int const elem : v) {
        if(elem > max) {
            max = elem;
        }
    }

    return max;
}

class config
{
private:
    int m_max_array_size{ 10'000'000 };
    int m_max_biggest_number{ std::numeric_limits<int>::max() };

public:
    constexpr config() noexcept = default;
    constexpr config(int const max_arr_size, int const max_big_num)
        : m_max_array_size{ max_arr_size }
        , m_max_biggest_number{ max_big_num }
    {
    }
    ~config() noexcept = default;

    [[nodiscard]] constexpr auto max_array_size() const noexcept -> int
    {
        return m_max_array_size;
    }

    [[nodiscard]] constexpr auto max_biggest_number() const noexcept -> int
    {
        return m_max_biggest_number;
    }
};

class sort_generator
{
protected:
    static std::mt19937_64 m_rng;
    static std::random_device m_rd;
    static std::uniform_int_distribution<int> m_dist;

    int m_array_size{ 0 };
    int m_biggest_number{ 0 };

    sort_generator() noexcept = default;
    sort_generator(int const array_size, int const max_biggest_number) noexcept
        : m_array_size{ array_size }
        , m_biggest_number{ max_biggest_number - 1 }
    {
        m_rng.seed(m_rd());
    }

    auto rnd()
    {
        return m_dist(m_rng) % m_biggest_number;
    }

public:
    virtual ~sort_generator() noexcept = default;

    virtual auto generate(std::vector<int>&) -> void = 0;
};

std::mt19937_64 sort_generator::m_rng{};
std::random_device sort_generator::m_rd{};
std::uniform_int_distribution<int> sort_generator::m_dist{
    0, std::numeric_limits<int>::max()
};

class empty_generator final : public sort_generator
{
public:
    empty_generator()
        : sort_generator{ 0, 0 }
    {
    }
    ~empty_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        v.clear();
    }
};

class sorted_generator final : public sort_generator
{
private:
    bool m_repeat{ false };

public:
    sorted_generator() = delete;
    sorted_generator(int const array_size, bool repeat = false)
        : sort_generator{ array_size, array_size }
        , m_repeat{ repeat }
    {
    }
    ~sorted_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        v.resize(m_array_size);

        if(m_repeat) {
            std::generate(v.begin(), v.end(), [this, x = 0]() mutable {
                return (this->rnd() % 2 == 0) ? x++ : x;
            });
        }
        else {
            std::generate(
                v.begin(), v.end(), [x = 0]() mutable { return x++; });
        }
    }
};

class reversed_generator final : public sort_generator
{
private:
    bool m_repeat{ false };

public:
    reversed_generator() = delete;
    reversed_generator(int const array_size, bool repeat = false)
        : sort_generator{ array_size, array_size }
        , m_repeat{ repeat }
    {
    }
    ~reversed_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        sorted_generator g{ m_array_size, m_repeat };
        g.generate(v);
        std::reverse(v.begin(), v.end());
    }
};

class shuffled_generator final : public sort_generator
{
private:
    bool m_repeat{ false };

public:
    shuffled_generator() = delete;
    shuffled_generator(int const array_size, bool repeat = false)
        : sort_generator{ array_size, array_size }
        , m_repeat{ repeat }
    {
    }
    ~shuffled_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        sorted_generator g{ m_array_size, m_repeat };
        g.generate(v);
        std::shuffle(v.begin(), v.end(), m_rng);
    }
};

class random_generator final : public sort_generator
{
public:
    random_generator() = delete;
    random_generator(int const array_size, int const biggest_num)
        : sort_generator{ array_size, biggest_num }
    {
    }
    ~random_generator() noexcept override = default;

    auto generate(std::vector<int>& v) -> void override
    {
        v.resize(m_array_size);
        std::generate(v.begin(), v.end(), [this]() { return this->rnd(); });
        std::shuffle(v.begin(), v.end(), m_rng);
    }
};

auto to_lower(std::string const& str) -> std::string
{
    std::string result;

    for(char const ch : str) {
        result.push_back(std::tolower(ch));
    }

    return result;
}

auto to_sort_type(std::string const& str) -> sort_type
{
    if(to_lower(str) == "empty") {
        return sort_type::empty;
    }
    else if(to_lower(str) == "sorted") {
        return sort_type::sorted;
    }
    else if(to_lower(str) == "sorted_rep") {
        return sort_type::sorted_repeat;
    }
    else if(to_lower(str) == "reversed") {
        return sort_type::reversed;
    }
    else if(to_lower(str) == "reversed_rep") {
        return sort_type::reversed_repeat;
    }
    else if(to_lower(str) == "shuffled") {
        return sort_type::shuffled;
    }
    else if(to_lower(str) == "shuffled_rep") {
        return sort_type::shuffled_repeat;
    }
    else if(to_lower(str) == "random") {
        return sort_type::random;
    }
    else {
        throw "Unknown sort type!";
    }
}

auto make_generator(sort_type const type,
                    int const array_size,
                    int const biggest_num) -> std::unique_ptr<sort_generator>
{
    switch(type) {
    case sort_type::empty:
        return std::make_unique<empty_generator>();
    case sort_type::sorted:
        return std::make_unique<sorted_generator>(array_size);
    case sort_type::sorted_repeat:
        return std::make_unique<sorted_generator>(array_size, true);
    case sort_type::reversed:
        return std::make_unique<reversed_generator>(array_size);
    case sort_type::reversed_repeat:
        return std::make_unique<reversed_generator>(array_size, true);
    case sort_type::shuffled:
        return std::make_unique<shuffled_generator>(array_size);
    case sort_type::shuffled_repeat:
        return std::make_unique<shuffled_generator>(array_size, true);
    case sort_type::random:
        return std::make_unique<shuffled_generator>(array_size, biggest_num);
    default:
        throw "Unknown sort type given!";
    }
}

auto split(std::string const& str, std::string const& pattern)
    -> std::vector<std::string>
{
    std::size_t pos_start = 0;
    std::size_t pos_end;
    std::size_t delim_len = pattern.length();
    std::string token;
    std::vector<std::string> res;

    while((pos_end = str.find(pattern, pos_start)) != std::string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if(!token.empty()) {
            res.push_back(token);
        }
    }

    auto s = str.substr(pos_start);

    if(!s.empty()) {
        res.push_back(s);
    }
    return res;
}

[[nodiscard]] auto check_input(std::string const& config_path,
                               std::map<std::string, config> const& config)
    -> std::vector<std::tuple<sort_type, int, int>>
{
    std::vector<std::tuple<sort_type, int, int>> params;
    std::ifstream f{ config_path };
    std::string line{};

    while(std::getline(f, line)) {
        sort_type type;
        int array_size{ 0 };
        int biggest_num{ 0 };
        std::stringstream ss;

        auto words = split(line, " ");
        auto [sort_name, cfg] = *config.find(
            std::string{ config_path.begin(), config_path.end() - 4 });
        type = to_sort_type(words[0]);

        if(words.size() > 1) {
            array_size = std::stoi(words[1]);

            if(array_size > cfg.max_array_size()) {
                std::cout << sort_name
                          << " array size too large: " << array_size
                          << std::endl;
                std::cout << "Max: " << cfg.max_array_size() << std::endl;
                throw "array_size too large";
            }
        }

        if(words.size() > 2) {
            biggest_num = std::stoi(words[2]);

            if(biggest_num > cfg.max_biggest_number()) {
                std::cout << sort_name
                          << " does not accept numbers larger than "
                          << cfg.max_biggest_number() << std::endl;
                std::cout << "Given: " << biggest_num << std::endl;
                throw "biggest_num too large";
            }
        }
        else {
            biggest_num = array_size;
        }

        params.emplace_back(type, array_size, biggest_num);
    }

    return params;
}

auto to_str(sort_type const type) -> std::string
{
    switch(type) {
    case sort_type::empty:
        return "empty";
    case sort_type::random:
        return "random";
    case sort_type::reversed:
        return "reversed";
    case sort_type::reversed_repeat:
        return "reversed_repeat";
    case sort_type::shuffled:
        return "shuffled";
    case sort_type::shuffled_repeat:
        return "shuffled_repeat";
    case sort_type::sorted:
        return "sorted";
    case sort_type::sorted_repeat:
        return "sorted_repeat";
    default:
        return "";
    }
}

auto main() noexcept -> int
{
    std::map<std::string, config> sort_config{};

    sort_config.emplace(
        std::make_pair("CountSort", config{ 10'000'000, 1'000'000 }));
    sort_config.emplace(std::make_pair(
        "BubbleSort", config{ 7'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(std::make_pair(
        "InsertionSort", config{ 20'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(
        std::make_pair("InsertionSortSTL",
                       config{ 100'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(
        std::make_pair("RadixSort10", config{ 10'000'000, 10'000'000 }));
    sort_config.emplace(
        std::make_pair("RadixSort2", config{ 10'000'000, 10'000'000 }));
    sort_config.emplace(std::make_pair(
        "MergeSort", config{ 10'000'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(std::make_pair(
        "QuickSort", config{ 10'000'000, std::numeric_limits<int>::max() }));
    sort_config.emplace(std::make_pair(
        "QuickSortSTL", config{ 10'000'000, std::numeric_limits<int>::max() }));

    for(auto& test : get_tests()) {
        std::vector<std::tuple<sort_type, int, int>> params{};

        try {
            using namespace std::string_literals;
            params = check_input(test->name() + ".txt"s, sort_config);
        }
        catch(char const* text) {
            std::cout << "Error: " << text << std::endl;
        }

        for(auto const [type, array_size, biggest_num] : params) {
            std::vector<int> input;
            std::vector<int> output;

            make_generator(type, array_size, biggest_num)->generate(input);
            output = input;
            test->benchmark(
                input, output, to_str(type), array_size, biggest_num);
        }

        std::cout << std::endl << test->format() << std::endl << std::endl;
    }
}
#include <array>
#include <cstddef>
#include <iterator>
#include <queue>

NEW_SORT("CountSort", input, output)
{
    std::vector<int> frecv(find_max(input) + 1, 0);

    for(auto const elem : input) {
        ++frecv[elem];
    }

    int index{ 0 };
    for(int i = 0; i < frecv.size(); ++i) {
        for(int j = 0; j < frecv[i]; ++j) {
            output[index++] = i;
        }
    }
}

NEW_SORT("BubbleSort", input, output)
{
    bool sorted{ true };

    do {
        sorted = true;
        for(int i = 1; i < output.size(); ++i) {
            if(output[i] < output[i - 1]) {
                std::swap(output[i], output[i - 1]);
                sorted = false;
            }
        }
    } while(!sorted);
}

NEW_SORT("InsertionSort", input, output)
{
    if(input.empty()) {
        return;
    }

    for(int i = 1; i < output.size(); ++i) {
        int j = i - 1;
        while(j >= 0 && output[j] > output[j + 1]) {
            std::swap(output[j], output[j + 1]);
            --j;
        }
    }
}

NEW_SORT("InsertionSortSTL", input, output)
{
    for(auto it = output.begin(); it != output.end(); ++it) {
        std::rotate(std::upper_bound(output.begin(), it, *it), it, it + 1);
    }
}

template<int Base>
auto radix_sort(std::vector<int>& v) -> void
{
    std::array<std::queue<int>, Base> digits;
    int max = find_max(v);
    int pow{ 1 };

    while(max / pow > 0) {
        for(int const num : v) {
            digits[(num / pow) % Base].push(num);
        }

        pow *= Base;
        v.clear();

        for(int i = 0; i < Base; ++i) {
            while(!digits[i].empty()) {
                v.push_back(digits[i].front());
                digits[i].pop();
            }
        }
    }
}

NEW_SORT("RadixSort10", input, output)
{
    radix_sort<10>(output);
}

NEW_SORT("RadixSort2", input, output)
{
    radix_sort<2>(output);
}

auto merge(std::vector<int>& buf,
           std::vector<int>& v,
           int const left,
           int const mid,
           int const right) -> void
{
    int i{ left };
    int j{ mid + 1 };
    int index{ left };

    while(i <= mid && j <= right) {
        if(buf[j] < buf[i]) {
            v[index++] = buf[j++];
        }
        else {
            v[index++] = buf[i++];
        }
    }

    while(i <= mid) {
        v[index++] = buf[i++];
    }
    while(j <= right) {
        v[index++] = buf[j++];
    }

    for(int k = left; k < index; ++k) {
        buf[k] = v[k];
    }
}

auto merge_sort(std::vector<int>& buf,
                std::vector<int>& v,
                int const left,
                int const right) -> void
{
    if(right - left <= 0) {
        return;
    }
    if(right - left == 1) {
        if(buf[left] > buf[right]) {
            return std::swap(buf[left], buf[right]);
        }

        return;
    }

    int const mid = left + (right - left) / 2;
    merge_sort(buf, v, left, mid);
    merge_sort(buf, v, mid + 1, right);
    merge(buf, v, left, mid, right);
}

NEW_SORT("MergeSort", input, output)
{
    std::vector<int> buf = input;
    merge_sort(buf, output, 0, input.size() - 1);
}

auto median_of_three(std::vector<int>& v, int const left, int const right)
    -> int
{
    int const mid = left + (right - left) / 2;

    if(v[right] < v[left]) {
        std::swap(v[right], v[left]);
    }
    if(v[mid] < v[left]) {
        std::swap(v[mid], v[left]);
    }
    if(v[right] < v[mid]) {
        std::swap(v[right], v[mid]);
    }

    return mid;
}

auto quicksort(std::vector<int>& v, int const left, int const right) -> void
{
    if(right - left <= 0) {
        return;
    }
    if(right - left == 1) {
        if(v[right] < v[left]) {
            return std::swap(v[right], v[left]);
        }
        return;
    }

    int i{ left };
    int j{ right };
    int pivot = v[median_of_three(v, left, right)];

    while(i <= j) {
        while(v[i] < pivot) {
            ++i;
        }
        while(v[j] > pivot) {
            --j;
        }

        if(i <= j) {
            std::swap(v[i++], v[j--]);
        }
    }

    if(j > left) {
        quicksort(v, left, j);
    }
    if(i < right) {
        quicksort(v, i, right);
    }
}

NEW_SORT("QuickSort", input, output)
{
    quicksort(output, 0, input.size() - 1);
}

template<typename InputIt>
auto median_of_threeSTL(InputIt first, InputIt last) -> InputIt
{
    InputIt mid = first;
    std::advance(mid, std::distance(first, last) / 2);

    if(*last < *first) {
        std::swap(*last, *first);
    }
    if(*mid < *first) {
        std::swap(*mid, *first);
    }
    if(*last < *mid) {
        std::swap(*last, *mid);
    }

    return mid;
}

template<typename InputIt>
auto quicksortSTL(InputIt first, InputIt last) -> void
{
    if(first == last) {
        return;
    }

    auto pivot = *median_of_threeSTL(first, last - 1);
    InputIt middle1 = std::partition(
        first, last, [pivot](auto const& elem) { return elem < pivot; });
    InputIt middle2 = std::partition(
        middle1, last, [pivot](auto const& elem) { return !(pivot < elem); });
    quicksortSTL(first, middle1);
    quicksortSTL(middle2, last);
}

NEW_SORT("QuickSortSTL", input, output)
{
    quicksortSTL(output.begin(), output.end());
}

/*
libfort

MIT License

Copyright (c) 2017 - 2020 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* The file was GENERATED by an amalgamation script.*/
/* DO NOT EDIT BY HAND!!! */

#define FT_AMALGAMED_SOURCE /* Macros to make internal libfort functions       \
                               static */

/********************************************************
   Begin of file "fort_utils.h"
 ********************************************************/

#ifndef FORT_IMPL_H
#define FORT_IMPL_H

#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS /* To disable warnings for unsafe functions */
#endif

//#include "fort.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define FT_INTERNAL to make internal libfort functions static
 * in the result amalgamed source file.
 */
#ifdef FT_AMALGAMED_SOURCE
#define FT_INTERNAL static
#else
#define FT_INTERNAL
#endif /* FT_AMALGAMED_SORCE */

#define FORT_DEFAULT_COL_SEPARATOR '|'
extern char g_col_separator;

#define FORT_COL_SEPARATOR_LENGTH 1

#define FORT_UNUSED __attribute__((unused))

#define F_MALLOC fort_malloc
#define F_FREE fort_free
#define F_CALLOC fort_calloc
#define F_REALLOC fort_realloc
#define F_STRDUP fort_strdup
#define F_WCSDUP fort_wcsdup
/* @todo: replace with custom impl !!!*/
#define F_UTF8DUP utf8dup

#define F_CREATE(type) ((type*)F_CALLOC(sizeof(type), 1))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define FT_NEWLINE "\n"
#define FT_SPACE " "

/*****************************************************************************
 *               DEFAULT_SIZES
 * ***************************************************************************/
#define DEFAULT_STR_BUF_SIZE 1024
#define DEFAULT_VECTOR_CAPACITY 10

/*****************************************************************************
 *               DATA TYPES
 * ***************************************************************************/

enum f_get_policy
{
    CREATE_ON_NULL,
    DONT_CREATE_ON_NULL
};

enum f_bool
{
    F_FALSE = 0,
    F_TRUE = 1
};

enum f_cell_type
{
    COMMON_CELL,
    GROUP_MASTER_CELL,
    GROUP_SLAVE_CELL
};

enum f_geometry_type
{
    VISIBLE_GEOMETRY,
    INTERN_REPR_GEOMETRY
};

enum f_string_type
{
    CHAR_BUF,
#ifdef FT_HAVE_WCHAR
    W_CHAR_BUF,
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    UTF8_BUF,
#endif /* FT_HAVE_WCHAR */
};

struct f_string_view
{
    union
    {
        const char* cstr;
#ifdef FT_HAVE_WCHAR
        const wchar_t* wstr;
#endif
#ifdef FT_HAVE_UTF8
        const void* u8str;
#endif
        const void* data;
    } u;
    enum f_string_type type;
};
typedef struct f_string_view f_string_view_t;

#define FT_STR_2_CAT_(arg1, arg2) arg1##arg2
#define FT_STR_2_CAT(arg1, arg2) FT_STR_2_CAT_(arg1, arg2)

#define UNIQUE_NAME_(prefix) FT_STR_2_CAT(prefix, __COUNTER__)
#define UNIQUE_NAME(prefix) UNIQUE_NAME_(prefix)

typedef int f_status;

struct f_table_properties;
struct f_row;
struct f_vector;
struct f_cell;
struct f_string_buffer;
struct f_separator
{
    int enabled;
};

typedef struct f_table_properties f_table_properties_t;
typedef struct f_vector f_vector_t;
typedef struct f_cell f_cell_t;
typedef struct f_string_buffer f_string_buffer_t;
typedef struct f_row f_row_t;
typedef struct f_separator f_separator_t;

struct f_context
{
    f_table_properties_t* table_properties;
    size_t row;
    size_t column;
};
typedef struct f_context f_context_t;

struct f_conv_context
{
    union
    {
        char* buf;
#ifdef FT_HAVE_WCHAR
        wchar_t* wbuf;
#endif
#ifdef FT_HAVE_UTF8
        const void* u8str;
#endif
    } u;
    size_t raw_avail;
    struct f_context* cntx;
    enum f_string_type b_type;
};
typedef struct f_conv_context f_conv_context_t;

/*****************************************************************************
 *               LIBFORT helpers
 *****************************************************************************/

extern void* (*fort_malloc)(size_t size);
extern void (*fort_free)(void* ptr);
extern void* (*fort_calloc)(size_t nmemb, size_t size);
extern void* (*fort_realloc)(void* ptr, size_t size);

FT_INTERNAL
void set_memory_funcs(void* (*f_malloc)(size_t size),
                      void (*f_free)(void* ptr));

FT_INTERNAL
char* fort_strdup(const char* str);

FT_INTERNAL
size_t number_of_columns_in_format_string(const f_string_view_t* fmt);

FT_INTERNAL
size_t number_of_columns_in_format_buffer(const f_string_buffer_t* fmt);

#if defined(FT_HAVE_WCHAR)
FT_INTERNAL
wchar_t* fort_wcsdup(const wchar_t* str);
#endif

FT_INTERNAL
int print_n_strings(f_conv_context_t* cntx, size_t n, const char* str);

FT_INTERNAL
int ft_nprint(f_conv_context_t* cntx, const char* str, size_t strlen);
#ifdef FT_HAVE_WCHAR
FT_INTERNAL
int ft_nwprint(f_conv_context_t* cntx, const wchar_t* str, size_t strlen);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
FT_INTERNAL
int ft_nu8print(f_conv_context_t* cntx, const void* beg, const void* end);
#endif /* FT_HAVE_UTF8 */

/*#define PRINT_DEBUG_INFO fprintf(stderr, "error in %s(%s:%d)\n", __FUNCTION__,
 * __FILE__, __LINE__);*/
#define PRINT_DEBUG_INFO

#define FT_CHECK(statement)                                                    \
    do {                                                                       \
        tmp = statement;                                                       \
        if(tmp < 0) {                                                          \
            PRINT_DEBUG_INFO                                                   \
            goto clear;                                                        \
        }                                                                      \
    } while(0)

#define CHCK_RSLT_ADD_TO_WRITTEN(statement)                                    \
    do {                                                                       \
        tmp = statement;                                                       \
        if(tmp < 0) {                                                          \
            PRINT_DEBUG_INFO                                                   \
            goto clear;                                                        \
        }                                                                      \
        written += (size_t)tmp;                                                \
    } while(0)

#define CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(statement)                          \
    do {                                                                       \
        tmp = statement;                                                       \
        if(tmp < 0) {                                                          \
            PRINT_DEBUG_INFO                                                   \
            goto clear;                                                        \
        }                                                                      \
        invisible_written += (size_t)tmp;                                      \
    } while(0)

#define CHECK_NOT_NEGATIVE(x)                                                  \
    do {                                                                       \
        if((x) < 0)                                                            \
            goto fort_fail;                                                    \
    } while(0)

#endif /* FORT_IMPL_H */

/********************************************************
   End of file "fort_utils.h"
 ********************************************************/

/********************************************************
   Begin of file "vector.h"
 ********************************************************/

#ifndef VECTOR_H
#define VECTOR_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */

#define INVALID_VEC_INDEX ((size_t)-1)

FT_INTERNAL
f_vector_t* create_vector(size_t item_size, size_t capacity);

FT_INTERNAL
void destroy_vector(f_vector_t*);

FT_INTERNAL
size_t vector_size(const f_vector_t*);

FT_INTERNAL
size_t vector_capacity(const f_vector_t*);

FT_INTERNAL
int vector_push(f_vector_t*, const void* item);

FT_INTERNAL
int vector_insert(f_vector_t*, const void* item, size_t pos);

FT_INTERNAL
f_vector_t* vector_split(f_vector_t*, size_t pos);

FT_INTERNAL
const void* vector_at_c(const f_vector_t* vector, size_t index);

FT_INTERNAL
void* vector_at(f_vector_t*, size_t index);

FT_INTERNAL
f_status vector_swap(f_vector_t* cur_vec, f_vector_t* mv_vec, size_t pos);

FT_INTERNAL
void vector_clear(f_vector_t*);

FT_INTERNAL
int vector_erase(f_vector_t*, size_t index);

#ifdef FT_TEST_BUILD
f_vector_t* copy_vector(f_vector_t*);
size_t vector_index_of(const f_vector_t*, const void* item);
#endif

#define VECTOR_AT(vector, pos, data_type)                                      \
    *(data_type*)vector_at((vector), (pos))

#define VECTOR_AT_C(vector, pos, const_data_type)                              \
    *(const_data_type*)vector_at_c((vector), (pos))

#endif /* VECTOR_H */

/********************************************************
   End of file "vector.h"
 ********************************************************/

/********************************************************
   Begin of file "wcwidth.h"
 ********************************************************/

#ifndef WCWIDTH_H
#define WCWIDTH_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */

#ifdef FT_HAVE_WCHAR
#include <wchar.h>

FT_INTERNAL
int mk_wcswidth(const wchar_t* pwcs, size_t n);

#endif /* FT_HAVE_WCHAR */

#endif /* WCWIDTH_H */

/********************************************************
   End of file "wcwidth.h"
 ********************************************************/

/********************************************************
   Begin of file "utf8.h"
 ********************************************************/

// The latest version of this library is available on GitHub;
// https://github.com/sheredom/utf8.h

// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>

#ifndef SHEREDOM_UTF8_H_INCLUDED
#define SHEREDOM_UTF8_H_INCLUDED

#if defined(_MSC_VER)
#pragma warning(push)

// disable 'bytes padding added after construct' warning
#pragma warning(disable : 4820)
#endif

#include <stddef.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(_MSC_VER)
typedef __int32 utf8_int32_t;
#else
#include <stdint.h>
typedef int32_t utf8_int32_t;
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wcast-qual"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__clang__) || defined(__GNUC__)
#define utf8_nonnull __attribute__((nonnull))
#define utf8_pure __attribute__((pure))
#define utf8_restrict __restrict__
#define utf8_weak __attribute__((weak))
#elif defined(_MSC_VER)
#define utf8_nonnull
#define utf8_pure
#define utf8_restrict __restrict
#define utf8_weak __inline
#else
#define utf8_nonnull
#define utf8_pure
#define utf8_restrict
#define utf8_weak inline
#endif

#ifdef __cplusplus
#define utf8_null NULL
#else
#define utf8_null 0
#endif

// Return less than 0, 0, greater than 0 if src1 < src2, src1 == src2, src1 >
// src2 respectively, case insensitive.
utf8_nonnull utf8_pure utf8_weak int utf8casecmp(const void* src1,
                                                 const void* src2);

// Append the utf8 string src onto the utf8 string dst.
utf8_nonnull utf8_weak void* utf8cat(void* utf8_restrict dst,
                                     const void* utf8_restrict src);

// Find the first match of the utf8 codepoint chr in the utf8 string src.
utf8_nonnull utf8_pure utf8_weak void* utf8chr(const void* src,
                                               utf8_int32_t chr);

// Return less than 0, 0, greater than 0 if src1 < src2,
// src1 == src2, src1 > src2 respectively.
utf8_nonnull utf8_pure utf8_weak int utf8cmp(const void* src1,
                                             const void* src2);

// Copy the utf8 string src onto the memory allocated in dst.
utf8_nonnull utf8_weak void* utf8cpy(void* utf8_restrict dst,
                                     const void* utf8_restrict src);

// Number of utf8 codepoints in the utf8 string src that consists entirely
// of utf8 codepoints not from the utf8 string reject.
utf8_nonnull utf8_pure utf8_weak size_t utf8cspn(const void* src,
                                                 const void* reject);

// Duplicate the utf8 string src by getting its size, malloc'ing a new buffer
// copying over the data, and returning that. Or 0 if malloc failed.
utf8_nonnull utf8_weak void* utf8dup(const void* src);

// Number of utf8 codepoints in the utf8 string str,
// excluding the null terminating byte.
utf8_nonnull utf8_pure utf8_weak size_t utf8len(const void* str);

// Visible width of utf8string.
utf8_nonnull utf8_pure utf8_weak size_t utf8width(const void* str);

// Visible width of codepoint.
utf8_nonnull utf8_pure utf8_weak int utf8cwidth(utf8_int32_t c);

// Return less than 0, 0, greater than 0 if src1 < src2, src1 == src2, src1 >
// src2 respectively, case insensitive. Checking at most n bytes of each utf8
// string.
utf8_nonnull utf8_pure utf8_weak int
utf8ncasecmp(const void* src1, const void* src2, size_t n);

// Append the utf8 string src onto the utf8 string dst,
// writing at most n+1 bytes. Can produce an invalid utf8
// string if n falls partway through a utf8 codepoint.
utf8_nonnull utf8_weak void*
utf8ncat(void* utf8_restrict dst, const void* utf8_restrict src, size_t n);

// Return less than 0, 0, greater than 0 if src1 < src2,
// src1 == src2, src1 > src2 respectively. Checking at most n
// bytes of each utf8 string.
utf8_nonnull utf8_pure utf8_weak int
utf8ncmp(const void* src1, const void* src2, size_t n);

// Copy the utf8 string src onto the memory allocated in dst.
// Copies at most n bytes. If there is no terminating null byte in
// the first n bytes of src, the string placed into dst will not be
// null-terminated. If the size (in bytes) of src is less than n,
// extra null terminating bytes are appended to dst such that at
// total of n bytes are written. Can produce an invalid utf8
// string if n falls partway through a utf8 codepoint.
utf8_nonnull utf8_weak void*
utf8ncpy(void* utf8_restrict dst, const void* utf8_restrict src, size_t n);

// Similar to utf8dup, except that at most n bytes of src are copied. If src is
// longer than n, only n bytes are copied and a null byte is added.
//
// Returns a new string if successful, 0 otherwise
utf8_nonnull utf8_weak void* utf8ndup(const void* src, size_t n);

// Locates the first occurence in the utf8 string str of any byte in the
// utf8 string accept, or 0 if no match was found.
utf8_nonnull utf8_pure utf8_weak void* utf8pbrk(const void* str,
                                                const void* accept);

// Find the last match of the utf8 codepoint chr in the utf8 string src.
utf8_nonnull utf8_pure utf8_weak void* utf8rchr(const void* src, int chr);

// Number of bytes in the utf8 string str,
// including the null terminating byte.
utf8_nonnull utf8_pure utf8_weak size_t utf8size(const void* str);

// Number of utf8 codepoints in the utf8 string src that consists entirely
// of utf8 codepoints from the utf8 string accept.
utf8_nonnull utf8_pure utf8_weak size_t utf8spn(const void* src,
                                                const void* accept);

// The position of the utf8 string needle in the utf8 string haystack.
utf8_nonnull utf8_pure utf8_weak void* utf8str(const void* haystack,
                                               const void* needle);

// The position of the utf8 string needle in the utf8 string haystack, case
// insensitive.
utf8_nonnull utf8_pure utf8_weak void* utf8casestr(const void* haystack,
                                                   const void* needle);

// Return 0 on success, or the position of the invalid
// utf8 codepoint on failure.
utf8_nonnull utf8_pure utf8_weak void* utf8valid(const void* str);

// Sets out_codepoint to the next utf8 codepoint in str, and returns the address
// of the utf8 codepoint after the current one in str.
utf8_nonnull utf8_weak void*
utf8codepoint(const void* utf8_restrict str,
              utf8_int32_t* utf8_restrict out_codepoint);

// Returns the size of the given codepoint in bytes.
utf8_weak size_t utf8codepointsize(utf8_int32_t chr);

// Write a codepoint to the given string, and return the address to the next
// place after the written codepoint. Pass how many bytes left in the buffer to
// n. If there is not enough space for the codepoint, this function returns
// null.
utf8_nonnull utf8_weak void*
utf8catcodepoint(void* utf8_restrict str, utf8_int32_t chr, size_t n);

// Returns 1 if the given character is lowercase, or 0 if it is not.
utf8_weak int utf8islower(utf8_int32_t chr);

// Returns 1 if the given character is uppercase, or 0 if it is not.
utf8_weak int utf8isupper(utf8_int32_t chr);

// Transform the given string into all lowercase codepoints.
utf8_nonnull utf8_weak void utf8lwr(void* utf8_restrict str);

// Transform the given string into all uppercase codepoints.
utf8_nonnull utf8_weak void utf8upr(void* utf8_restrict str);

// Make a codepoint lower case if possible.
utf8_weak utf8_int32_t utf8lwrcodepoint(utf8_int32_t cp);

// Make a codepoint upper case if possible.
utf8_weak utf8_int32_t utf8uprcodepoint(utf8_int32_t cp);

#undef utf8_weak
#undef utf8_pure
#undef utf8_nonnull

int utf8casecmp(const void* src1, const void* src2)
{
    utf8_int32_t src1_cp, src2_cp, src1_orig_cp, src2_orig_cp;

    for(;;) {
        src1 = utf8codepoint(src1, &src1_cp);
        src2 = utf8codepoint(src2, &src2_cp);

        // Take a copy of src1 & src2
        src1_orig_cp = src1_cp;
        src2_orig_cp = src2_cp;

        // Lower the srcs if required
        src1_cp = utf8lwrcodepoint(src1_cp);
        src2_cp = utf8lwrcodepoint(src2_cp);

        // Check if the lowered codepoints match
        if((0 == src1_orig_cp) && (0 == src2_orig_cp)) {
            return 0;
        }
        else if(src1_cp == src2_cp) {
            continue;
        }

        // If they don't match, then we return which of the original's are less
        if(src1_orig_cp < src2_orig_cp) {
            return -1;
        }
        else if(src1_orig_cp > src2_orig_cp) {
            return 1;
        }
    }
}

void* utf8cat(void* utf8_restrict dst, const void* utf8_restrict src)
{
    char* d = (char*)dst;
    const char* s = (const char*)src;

    // find the null terminating byte in dst
    while('\0' != *d) {
        d++;
    }

    // overwriting the null terminating byte in dst, append src byte-by-byte
    while('\0' != *s) {
        *d++ = *s++;
    }

    // write out a new null terminating byte into dst
    *d = '\0';

    return dst;
}

void* utf8chr(const void* src, utf8_int32_t chr)
{
    char c[5] = { '\0', '\0', '\0', '\0', '\0' };

    if(0 == chr) {
        // being asked to return position of null terminating byte, so
        // just run s to the end, and return!
        const char* s = (const char*)src;
        while('\0' != *s) {
            s++;
        }
        return (void*)s;
    }
    else if(0 == ((utf8_int32_t)0xffffff80 & chr)) {
        // 1-byte/7-bit ascii
        // (0b0xxxxxxx)
        c[0] = (char)chr;
    }
    else if(0 == ((utf8_int32_t)0xfffff800 & chr)) {
        // 2-byte/11-bit utf8 code point
        // (0b110xxxxx 0b10xxxxxx)
        c[0] = 0xc0 | (char)(chr >> 6);
        c[1] = 0x80 | (char)(chr & 0x3f);
    }
    else if(0 == ((utf8_int32_t)0xffff0000 & chr)) {
        // 3-byte/16-bit utf8 code point
        // (0b1110xxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xe0 | (char)(chr >> 12);
        c[1] = 0x80 | (char)((chr >> 6) & 0x3f);
        c[2] = 0x80 | (char)(chr & 0x3f);
    }
    else { // if (0 == ((int)0xffe00000 & chr)) {
        // 4-byte/21-bit utf8 code point
        // (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xf0 | (char)(chr >> 18);
        c[1] = 0x80 | (char)((chr >> 12) & 0x3f);
        c[2] = 0x80 | (char)((chr >> 6) & 0x3f);
        c[3] = 0x80 | (char)(chr & 0x3f);
    }

    // we've made c into a 2 utf8 codepoint string, one for the chr we are
    // seeking, another for the null terminating byte. Now use utf8str to
    // search
    return utf8str(src, c);
}

int utf8cmp(const void* src1, const void* src2)
{
    const unsigned char* s1 = (const unsigned char*)src1;
    const unsigned char* s2 = (const unsigned char*)src2;

    while(('\0' != *s1) || ('\0' != *s2)) {
        if(*s1 < *s2) {
            return -1;
        }
        else if(*s1 > *s2) {
            return 1;
        }

        s1++;
        s2++;
    }

    // both utf8 strings matched
    return 0;
}

int utf8coll(const void* src1, const void* src2);

void* utf8cpy(void* utf8_restrict dst, const void* utf8_restrict src)
{
    char* d = (char*)dst;
    const char* s = (const char*)src;

    // overwriting anything previously in dst, write byte-by-byte
    // from src
    while('\0' != *s) {
        *d++ = *s++;
    }

    // append null terminating byte
    *d = '\0';

    return dst;
}

size_t utf8cspn(const void* src, const void* reject)
{
    const char* s = (const char*)src;
    size_t chars = 0;

    while('\0' != *s) {
        const char* r = (const char*)reject;
        size_t offset = 0;

        while('\0' != *r) {
            // checking that if *r is the start of a utf8 codepoint
            // (it is not 0b10xxxxxx) and we have successfully matched
            // a previous character (0 < offset) - we found a match
            if((0x80 != (0xc0 & *r)) && (0 < offset)) {
                return chars;
            }
            else {
                if(*r == s[offset]) {
                    // part of a utf8 codepoint matched, so move our checking
                    // onwards to the next byte
                    offset++;
                    r++;
                }
                else {
                    // r could be in the middle of an unmatching utf8 code
                    // point, so we need to march it on to the next character
                    // beginning,

                    do {
                        r++;
                    } while(0x80 == (0xc0 & *r));

                    // reset offset too as we found a mismatch
                    offset = 0;
                }
            }
        }

        // the current utf8 codepoint in src did not match reject, but src
        // could have been partway through a utf8 codepoint, so we need to
        // march it onto the next utf8 codepoint starting byte
        do {
            s++;
        } while((0x80 == (0xc0 & *s)));
        chars++;
    }

    return chars;
}

size_t utf8size(const void* str);

void* utf8dup(const void* src)
{
    const char* s = (const char*)src;
    char* n = utf8_null;

    // figure out how many bytes (including the terminator) we need to copy
    // first
    size_t bytes = utf8size(src);

    n = (char*)malloc(bytes);

    if(utf8_null == n) {
        // out of memory so we bail
        return utf8_null;
    }
    else {
        bytes = 0;

        // copy src byte-by-byte into our new utf8 string
        while('\0' != s[bytes]) {
            n[bytes] = s[bytes];
            bytes++;
        }

        // append null terminating byte
        n[bytes] = '\0';
        return n;
    }
}

void* utf8fry(const void* str);

size_t utf8len(const void* str)
{
    const unsigned char* s = (const unsigned char*)str;
    size_t length = 0;

    while('\0' != *s) {
        if(0xf0 == (0xf8 & *s)) {
            // 4-byte utf8 code point (began with 0b11110xxx)
            s += 4;
        }
        else if(0xe0 == (0xf0 & *s)) {
            // 3-byte utf8 code point (began with 0b1110xxxx)
            s += 3;
        }
        else if(0xc0 == (0xe0 & *s)) {
            // 2-byte utf8 code point (began with 0b110xxxxx)
            s += 2;
        }
        else { // if (0x00 == (0x80 & *s)) {
            // 1-byte ascii (began with 0b0xxxxxxx)
            s += 1;
        }

        // no matter the bytes we marched s forward by, it was
        // only 1 utf8 codepoint
        length++;
    }

    return length;
}

// See
// https://unicode.org/Public/UNIDATA/EastAsianWidth.txt
// http://www.unicode.org/reports/tr11/tr11-33.html
int utf8cwidth(utf8_int32_t c)
{
    // TODO: add non printable characters check
    if(c == 0)
        return 0;

    if(c < 0x1100)
        return 1;

    // Fullwidth
    if((0x3000 == c) || (0xFF01 <= c && c <= 0xFF60) ||
       (0xFFE0 <= c && c <= 0xFFE6)) {
        return 2;
    }

    // Wide
    if((0x1100 <= c && c <= 0x115F) || (0x11A3 <= c && c <= 0x11A7) ||
       (0x11FA <= c && c <= 0x11FF) || (0x2329 <= c && c <= 0x232A) ||
       (0x2E80 <= c && c <= 0x2E99) || (0x2E9B <= c && c <= 0x2EF3) ||
       (0x2F00 <= c && c <= 0x2FD5) || (0x2FF0 <= c && c <= 0x2FFB) ||
       (0x3001 <= c && c <= 0x303E) || (0x3041 <= c && c <= 0x3096) ||
       (0x3099 <= c && c <= 0x30FF) || (0x3105 <= c && c <= 0x312D) ||
       (0x3131 <= c && c <= 0x318E) || (0x3190 <= c && c <= 0x31BA) ||
       (0x31C0 <= c && c <= 0x31E3) || (0x31F0 <= c && c <= 0x321E) ||
       (0x3220 <= c && c <= 0x3247) || (0x3250 <= c && c <= 0x32FE) ||
       (0x3300 <= c && c <= 0x4DBF) || (0x4E00 <= c && c <= 0xA48C) ||
       (0xA490 <= c && c <= 0xA4C6) || (0xA960 <= c && c <= 0xA97C) ||
       (0xAC00 <= c && c <= 0xD7A3) || (0xD7B0 <= c && c <= 0xD7C6) ||
       (0xD7CB <= c && c <= 0xD7FB) || (0xF900 <= c && c <= 0xFAFF) ||
       (0xFE10 <= c && c <= 0xFE19) || (0xFE30 <= c && c <= 0xFE52) ||
       (0xFE54 <= c && c <= 0xFE66) || (0xFE68 <= c && c <= 0xFE6B) ||
       (0x1B000 <= c && c <= 0x1B001) || (0x1F200 <= c && c <= 0x1F202) ||
       (0x1F210 <= c && c <= 0x1F23A) || (0x1F240 <= c && c <= 0x1F248) ||
       (0x1F250 <= c && c <= 0x1F251) || (0x20000 <= c && c <= 0x2F73F) ||
       (0x2B740 <= c && c <= 0x2FFFD) || (0x30000 <= c && c <= 0x3FFFD)) {
        return 2;
    }

    return 1;
}

size_t utf8width(const void* str)
{
    size_t length = 0;
    utf8_int32_t c = 0;

    str = utf8codepoint(str, &c);
    while(c != 0) {
        length += utf8cwidth(c);
        str = utf8codepoint(str, &c);
    }
    return length;
}

int utf8ncasecmp(const void* src1, const void* src2, size_t n)
{
    utf8_int32_t src1_cp, src2_cp, src1_orig_cp, src2_orig_cp;

    do {
        const unsigned char* const s1 = (const unsigned char*)src1;
        const unsigned char* const s2 = (const unsigned char*)src2;

        // first check that we have enough bytes left in n to contain an entire
        // codepoint
        if(0 == n) {
            return 0;
        }

        if((1 == n) && ((0xc0 == (0xe0 & *s1)) || (0xc0 == (0xe0 & *s2)))) {
            const utf8_int32_t c1 = (0xe0 & *s1);
            const utf8_int32_t c2 = (0xe0 & *s2);

            if(c1 < c2) {
                return -1;
            }
            else if(c1 > c2) {
                return 1;
            }
            else {
                return 0;
            }
        }

        if((2 >= n) && ((0xe0 == (0xf0 & *s1)) || (0xe0 == (0xf0 & *s2)))) {
            const utf8_int32_t c1 = (0xf0 & *s1);
            const utf8_int32_t c2 = (0xf0 & *s2);

            if(c1 < c2) {
                return -1;
            }
            else if(c1 > c2) {
                return 1;
            }
            else {
                return 0;
            }
        }

        if((3 >= n) && ((0xf0 == (0xf8 & *s1)) || (0xf0 == (0xf8 & *s2)))) {
            const utf8_int32_t c1 = (0xf8 & *s1);
            const utf8_int32_t c2 = (0xf8 & *s2);

            if(c1 < c2) {
                return -1;
            }
            else if(c1 > c2) {
                return 1;
            }
            else {
                return 0;
            }
        }

        src1 = utf8codepoint(src1, &src1_cp);
        src2 = utf8codepoint(src2, &src2_cp);
        n -= utf8codepointsize(src1_cp);

        // Take a copy of src1 & src2
        src1_orig_cp = src1_cp;
        src2_orig_cp = src2_cp;

        // Lower srcs if required
        src1_cp = utf8lwrcodepoint(src1_cp);
        src2_cp = utf8lwrcodepoint(src2_cp);

        // Check if the lowered codepoints match
        if((0 == src1_orig_cp) && (0 == src2_orig_cp)) {
            return 0;
        }
        else if(src1_cp == src2_cp) {
            continue;
        }

        // If they don't match, then we return which of the original's are less
        if(src1_orig_cp < src2_orig_cp) {
            return -1;
        }
        else if(src1_orig_cp > src2_orig_cp) {
            return 1;
        }
    } while(0 < n);

    // both utf8 strings matched
    return 0;
}

void* utf8ncat(void* utf8_restrict dst, const void* utf8_restrict src, size_t n)
{
    char* d = (char*)dst;
    const char* s = (const char*)src;

    // find the null terminating byte in dst
    while('\0' != *d) {
        d++;
    }

    // overwriting the null terminating byte in dst, append src byte-by-byte
    // stopping if we run out of space
    do {
        *d++ = *s++;
    } while(('\0' != *s) && (0 != --n));

    // write out a new null terminating byte into dst
    *d = '\0';

    return dst;
}

int utf8ncmp(const void* src1, const void* src2, size_t n)
{
    const unsigned char* s1 = (const unsigned char*)src1;
    const unsigned char* s2 = (const unsigned char*)src2;

    while((0 != n--) && (('\0' != *s1) || ('\0' != *s2))) {
        if(*s1 < *s2) {
            return -1;
        }
        else if(*s1 > *s2) {
            return 1;
        }

        s1++;
        s2++;
    }

    // both utf8 strings matched
    return 0;
}

void* utf8ncpy(void* utf8_restrict dst, const void* utf8_restrict src, size_t n)
{
    char* d = (char*)dst;
    const char* s = (const char*)src;
    size_t index;

    // overwriting anything previously in dst, write byte-by-byte
    // from src
    for(index = 0; index < n; index++) {
        d[index] = s[index];
        if('\0' == s[index]) {
            break;
        }
    }

    // append null terminating byte
    for(; index < n; index++) {
        d[index] = 0;
    }

    return dst;
}

void* utf8ndup(const void* src, size_t n)
{
    const char* s = (const char*)src;
    char* c = utf8_null;
    size_t bytes = 0;

    // Find the end of the string or stop when n is reached
    while('\0' != s[bytes] && bytes < n) {
        bytes++;
    }

    // In case bytes is actually less than n, we need to set it
    // to be used later in the copy byte by byte.
    n = bytes;

    c = (char*)malloc(bytes + 1);
    if(utf8_null == c) {
        // out of memory so we bail
        return utf8_null;
    }

    bytes = 0;

    // copy src byte-by-byte into our new utf8 string
    while('\0' != s[bytes] && bytes < n) {
        c[bytes] = s[bytes];
        bytes++;
    }

    // append null terminating byte
    c[bytes] = '\0';
    return c;
}

void* utf8rchr(const void* src, int chr)
{
    const char* s = (const char*)src;
    const char* match = utf8_null;
    char c[5] = { '\0', '\0', '\0', '\0', '\0' };

    if(0 == chr) {
        // being asked to return position of null terminating byte, so
        // just run s to the end, and return!
        while('\0' != *s) {
            s++;
        }
        return (void*)s;
    }
    else if(0 == ((int)0xffffff80 & chr)) {
        // 1-byte/7-bit ascii
        // (0b0xxxxxxx)
        c[0] = (char)chr;
    }
    else if(0 == ((int)0xfffff800 & chr)) {
        // 2-byte/11-bit utf8 code point
        // (0b110xxxxx 0b10xxxxxx)
        c[0] = 0xc0 | (char)(chr >> 6);
        c[1] = 0x80 | (char)(chr & 0x3f);
    }
    else if(0 == ((int)0xffff0000 & chr)) {
        // 3-byte/16-bit utf8 code point
        // (0b1110xxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xe0 | (char)(chr >> 12);
        c[1] = 0x80 | (char)((chr >> 6) & 0x3f);
        c[2] = 0x80 | (char)(chr & 0x3f);
    }
    else { // if (0 == ((int)0xffe00000 & chr)) {
        // 4-byte/21-bit utf8 code point
        // (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx)
        c[0] = 0xf0 | (char)(chr >> 18);
        c[1] = 0x80 | (char)((chr >> 12) & 0x3f);
        c[2] = 0x80 | (char)((chr >> 6) & 0x3f);
        c[3] = 0x80 | (char)(chr & 0x3f);
    }

    // we've created a 2 utf8 codepoint string in c that is
    // the utf8 character asked for by chr, and a null
    // terminating byte

    while('\0' != *s) {
        size_t offset = 0;

        while(s[offset] == c[offset]) {
            offset++;
        }

        if('\0' == c[offset]) {
            // we found a matching utf8 code point
            match = s;
            s += offset;
        }
        else {
            s += offset;

            // need to march s along to next utf8 codepoint start
            // (the next byte that doesn't match 0b10xxxxxx)
            if('\0' != *s) {
                do {
                    s++;
                } while(0x80 == (0xc0 & *s));
            }
        }
    }

    // return the last match we found (or 0 if no match was found)
    return (void*)match;
}

void* utf8pbrk(const void* str, const void* accept)
{
    const char* s = (const char*)str;

    while('\0' != *s) {
        const char* a = (const char*)accept;
        size_t offset = 0;

        while('\0' != *a) {
            // checking that if *a is the start of a utf8 codepoint
            // (it is not 0b10xxxxxx) and we have successfully matched
            // a previous character (0 < offset) - we found a match
            if((0x80 != (0xc0 & *a)) && (0 < offset)) {
                return (void*)s;
            }
            else {
                if(*a == s[offset]) {
                    // part of a utf8 codepoint matched, so move our checking
                    // onwards to the next byte
                    offset++;
                    a++;
                }
                else {
                    // r could be in the middle of an unmatching utf8 code
                    // point, so we need to march it on to the next character
                    // beginning,

                    do {
                        a++;
                    } while(0x80 == (0xc0 & *a));

                    // reset offset too as we found a mismatch
                    offset = 0;
                }
            }
        }

        // we found a match on the last utf8 codepoint
        if(0 < offset) {
            return (void*)s;
        }

        // the current utf8 codepoint in src did not match accept, but src
        // could have been partway through a utf8 codepoint, so we need to
        // march it onto the next utf8 codepoint starting byte
        do {
            s++;
        } while((0x80 == (0xc0 & *s)));
    }

    return utf8_null;
}

size_t utf8size(const void* str)
{
    const char* s = (const char*)str;
    size_t size = 0;
    while('\0' != s[size]) {
        size++;
    }

    // we are including the null terminating byte in the size calculation
    size++;
    return size;
}

size_t utf8spn(const void* src, const void* accept)
{
    const char* s = (const char*)src;
    size_t chars = 0;

    while('\0' != *s) {
        const char* a = (const char*)accept;
        size_t offset = 0;

        while('\0' != *a) {
            // checking that if *r is the start of a utf8 codepoint
            // (it is not 0b10xxxxxx) and we have successfully matched
            // a previous character (0 < offset) - we found a match
            if((0x80 != (0xc0 & *a)) && (0 < offset)) {
                // found a match, so increment the number of utf8 codepoints
                // that have matched and stop checking whether any other utf8
                // codepoints in a match
                chars++;
                s += offset;
                break;
            }
            else {
                if(*a == s[offset]) {
                    offset++;
                    a++;
                }
                else {
                    // a could be in the middle of an unmatching utf8 codepoint,
                    // so we need to march it on to the next character
                    // beginning,
                    do {
                        a++;
                    } while(0x80 == (0xc0 & *a));

                    // reset offset too as we found a mismatch
                    offset = 0;
                }
            }
        }

        // if a got to its terminating null byte, then we didn't find a match.
        // Return the current number of matched utf8 codepoints
        if('\0' == *a) {
            return chars;
        }
    }

    return chars;
}

void* utf8str(const void* haystack, const void* needle)
{
    const char* h = (const char*)haystack;
    utf8_int32_t throwaway_codepoint;

    // if needle has no utf8 codepoints before the null terminating
    // byte then return haystack
    if('\0' == *((const char*)needle)) {
        return (void*)haystack;
    }

    while('\0' != *h) {
        const char* maybeMatch = h;
        const char* n = (const char*)needle;

        while(*h == *n && (*h != '\0' && *n != '\0')) {
            n++;
            h++;
        }

        if('\0' == *n) {
            // we found the whole utf8 string for needle in haystack at
            // maybeMatch, so return it
            return (void*)maybeMatch;
        }
        else {
            // h could be in the middle of an unmatching utf8 codepoint,
            // so we need to march it on to the next character beginning
            // starting from the current character
            h = (const char*)utf8codepoint(maybeMatch, &throwaway_codepoint);
        }
    }

    // no match
    return utf8_null;
}

void* utf8casestr(const void* haystack, const void* needle)
{
    const void* h = haystack;

    // if needle has no utf8 codepoints before the null terminating
    // byte then return haystack
    if('\0' == *((const char*)needle)) {
        return (void*)haystack;
    }

    for(;;) {
        const void* maybeMatch = h;
        const void* n = needle;
        utf8_int32_t h_cp, n_cp;

        // Get the next code point and track it
        const void* nextH = h = utf8codepoint(h, &h_cp);
        n = utf8codepoint(n, &n_cp);

        while((0 != h_cp) && (0 != n_cp)) {
            h_cp = utf8lwrcodepoint(h_cp);
            n_cp = utf8lwrcodepoint(n_cp);

            // if we find a mismatch, bail out!
            if(h_cp != n_cp) {
                break;
            }

            h = utf8codepoint(h, &h_cp);
            n = utf8codepoint(n, &n_cp);
        }

        if(0 == n_cp) {
            // we found the whole utf8 string for needle in haystack at
            // maybeMatch, so return it
            return (void*)maybeMatch;
        }

        if(0 == h_cp) {
            // no match
            return utf8_null;
        }

        // Roll back to the next code point in the haystack to test
        h = nextH;
    }
}

void* utf8valid(const void* str)
{
    const char* s = (const char*)str;

    while('\0' != *s) {
        if(0xf0 == (0xf8 & *s)) {
            // ensure each of the 3 following bytes in this 4-byte
            // utf8 codepoint began with 0b10xxxxxx
            if((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])) ||
               (0x80 != (0xc0 & s[3]))) {
                return (void*)s;
            }

            // ensure that our utf8 codepoint ended after 4 bytes
            if(0x80 == (0xc0 & s[4])) {
                return (void*)s;
            }

            // ensure that the top 5 bits of this 4-byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if((0 == (0x07 & s[0])) && (0 == (0x30 & s[1]))) {
                return (void*)s;
            }

            // 4-byte utf8 code point (began with 0b11110xxx)
            s += 4;
        }
        else if(0xe0 == (0xf0 & *s)) {
            // ensure each of the 2 following bytes in this 3-byte
            // utf8 codepoint began with 0b10xxxxxx
            if((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2]))) {
                return (void*)s;
            }

            // ensure that our utf8 codepoint ended after 3 bytes
            if(0x80 == (0xc0 & s[3])) {
                return (void*)s;
            }

            // ensure that the top 5 bits of this 3-byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if((0 == (0x0f & s[0])) && (0 == (0x20 & s[1]))) {
                return (void*)s;
            }

            // 3-byte utf8 code point (began with 0b1110xxxx)
            s += 3;
        }
        else if(0xc0 == (0xe0 & *s)) {
            // ensure the 1 following byte in this 2-byte
            // utf8 codepoint began with 0b10xxxxxx
            if(0x80 != (0xc0 & s[1])) {
                return (void*)s;
            }

            // ensure that our utf8 codepoint ended after 2 bytes
            if(0x80 == (0xc0 & s[2])) {
                return (void*)s;
            }

            // ensure that the top 4 bits of this 2-byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if(0 == (0x1e & s[0])) {
                return (void*)s;
            }

            // 2-byte utf8 code point (began with 0b110xxxxx)
            s += 2;
        }
        else if(0x00 == (0x80 & *s)) {
            // 1-byte ascii (began with 0b0xxxxxxx)
            s += 1;
        }
        else {
            // we have an invalid 0b1xxxxxxx utf8 code point entry
            return (void*)s;
        }
    }

    return utf8_null;
}

void* utf8codepoint(const void* utf8_restrict str,
                    utf8_int32_t* utf8_restrict out_codepoint)
{
    const char* s = (const char*)str;

    if(0xf0 == (0xf8 & s[0])) {
        // 4 byte utf8 codepoint
        *out_codepoint = ((0x07 & s[0]) << 18) | ((0x3f & s[1]) << 12) |
                         ((0x3f & s[2]) << 6) | (0x3f & s[3]);
        s += 4;
    }
    else if(0xe0 == (0xf0 & s[0])) {
        // 3 byte utf8 codepoint
        *out_codepoint =
            ((0x0f & s[0]) << 12) | ((0x3f & s[1]) << 6) | (0x3f & s[2]);
        s += 3;
    }
    else if(0xc0 == (0xe0 & s[0])) {
        // 2 byte utf8 codepoint
        *out_codepoint = ((0x1f & s[0]) << 6) | (0x3f & s[1]);
        s += 2;
    }
    else {
        // 1 byte utf8 codepoint otherwise
        *out_codepoint = s[0];
        s += 1;
    }

    return (void*)s;
}

size_t utf8codepointsize(utf8_int32_t chr)
{
    if(0 == ((utf8_int32_t)0xffffff80 & chr)) {
        return 1;
    }
    else if(0 == ((utf8_int32_t)0xfffff800 & chr)) {
        return 2;
    }
    else if(0 == ((utf8_int32_t)0xffff0000 & chr)) {
        return 3;
    }
    else { // if (0 == ((int)0xffe00000 & chr)) {
        return 4;
    }
}

void* utf8catcodepoint(void* utf8_restrict str, utf8_int32_t chr, size_t n)
{
    char* s = (char*)str;

    if(0 == ((utf8_int32_t)0xffffff80 & chr)) {
        // 1-byte/7-bit ascii
        // (0b0xxxxxxx)
        if(n < 1) {
            return utf8_null;
        }
        s[0] = (char)chr;
        s += 1;
    }
    else if(0 == ((utf8_int32_t)0xfffff800 & chr)) {
        // 2-byte/11-bit utf8 code point
        // (0b110xxxxx 0b10xxxxxx)
        if(n < 2) {
            return utf8_null;
        }
        s[0] = 0xc0 | (char)(chr >> 6);
        s[1] = 0x80 | (char)(chr & 0x3f);
        s += 2;
    }
    else if(0 == ((utf8_int32_t)0xffff0000 & chr)) {
        // 3-byte/16-bit utf8 code point
        // (0b1110xxxx 0b10xxxxxx 0b10xxxxxx)
        if(n < 3) {
            return utf8_null;
        }
        s[0] = 0xe0 | (char)(chr >> 12);
        s[1] = 0x80 | (char)((chr >> 6) & 0x3f);
        s[2] = 0x80 | (char)(chr & 0x3f);
        s += 3;
    }
    else { // if (0 == ((int)0xffe00000 & chr)) {
        // 4-byte/21-bit utf8 code point
        // (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx)
        if(n < 4) {
            return utf8_null;
        }
        s[0] = 0xf0 | (char)(chr >> 18);
        s[1] = 0x80 | (char)((chr >> 12) & 0x3f);
        s[2] = 0x80 | (char)((chr >> 6) & 0x3f);
        s[3] = 0x80 | (char)(chr & 0x3f);
        s += 4;
    }

    return s;
}

int utf8islower(utf8_int32_t chr)
{
    return chr != utf8uprcodepoint(chr);
}

int utf8isupper(utf8_int32_t chr)
{
    return chr != utf8lwrcodepoint(chr);
}

void utf8lwr(void* utf8_restrict str)
{
    void *p, *pn;
    utf8_int32_t cp;

    p = (char*)str;
    pn = utf8codepoint(p, &cp);

    while(cp != 0) {
        const utf8_int32_t lwr_cp = utf8lwrcodepoint(cp);
        const size_t size = utf8codepointsize(lwr_cp);

        if(lwr_cp != cp) {
            utf8catcodepoint(p, lwr_cp, size);
        }

        p = pn;
        pn = utf8codepoint(p, &cp);
    }
}

void utf8upr(void* utf8_restrict str)
{
    void *p, *pn;
    utf8_int32_t cp;

    p = (char*)str;
    pn = utf8codepoint(p, &cp);

    while(cp != 0) {
        const utf8_int32_t lwr_cp = utf8uprcodepoint(cp);
        const size_t size = utf8codepointsize(lwr_cp);

        if(lwr_cp != cp) {
            utf8catcodepoint(p, lwr_cp, size);
        }

        p = pn;
        pn = utf8codepoint(p, &cp);
    }
}

utf8_int32_t utf8lwrcodepoint(utf8_int32_t cp)
{
    if(((0x0041 <= cp) && (0x005a >= cp)) ||
       ((0x00c0 <= cp) && (0x00d6 >= cp)) ||
       ((0x00d8 <= cp) && (0x00de >= cp)) ||
       ((0x0391 <= cp) && (0x03a1 >= cp)) ||
       ((0x03a3 <= cp) && (0x03ab >= cp))) {
        cp += 32;
    }
    else if(((0x0100 <= cp) && (0x012f >= cp)) ||
            ((0x0132 <= cp) && (0x0137 >= cp)) ||
            ((0x014a <= cp) && (0x0177 >= cp)) ||
            ((0x0182 <= cp) && (0x0185 >= cp)) ||
            ((0x01a0 <= cp) && (0x01a5 >= cp)) ||
            ((0x01de <= cp) && (0x01ef >= cp)) ||
            ((0x01f8 <= cp) && (0x021f >= cp)) ||
            ((0x0222 <= cp) && (0x0233 >= cp)) ||
            ((0x0246 <= cp) && (0x024f >= cp)) ||
            ((0x03d8 <= cp) && (0x03ef >= cp))) {
        cp |= 0x1;
    }
    else if(((0x0139 <= cp) && (0x0148 >= cp)) ||
            ((0x0179 <= cp) && (0x017e >= cp)) ||
            ((0x01af <= cp) && (0x01b0 >= cp)) ||
            ((0x01b3 <= cp) && (0x01b6 >= cp)) ||
            ((0x01cd <= cp) && (0x01dc >= cp))) {
        cp += 1;
        cp &= ~0x1;
    }
    else {
        switch(cp) {
        default:
            break;
        case 0x0178:
            cp = 0x00ff;
            break;
        case 0x0243:
            cp = 0x0180;
            break;
        case 0x018e:
            cp = 0x01dd;
            break;
        case 0x023d:
            cp = 0x019a;
            break;
        case 0x0220:
            cp = 0x019e;
            break;
        case 0x01b7:
            cp = 0x0292;
            break;
        case 0x01c4:
            cp = 0x01c6;
            break;
        case 0x01c7:
            cp = 0x01c9;
            break;
        case 0x01ca:
            cp = 0x01cc;
            break;
        case 0x01f1:
            cp = 0x01f3;
            break;
        case 0x01f7:
            cp = 0x01bf;
            break;
        case 0x0187:
            cp = 0x0188;
            break;
        case 0x018b:
            cp = 0x018c;
            break;
        case 0x0191:
            cp = 0x0192;
            break;
        case 0x0198:
            cp = 0x0199;
            break;
        case 0x01a7:
            cp = 0x01a8;
            break;
        case 0x01ac:
            cp = 0x01ad;
            break;
        case 0x01af:
            cp = 0x01b0;
            break;
        case 0x01b8:
            cp = 0x01b9;
            break;
        case 0x01bc:
            cp = 0x01bd;
            break;
        case 0x01f4:
            cp = 0x01f5;
            break;
        case 0x023b:
            cp = 0x023c;
            break;
        case 0x0241:
            cp = 0x0242;
            break;
        case 0x03fd:
            cp = 0x037b;
            break;
        case 0x03fe:
            cp = 0x037c;
            break;
        case 0x03ff:
            cp = 0x037d;
            break;
        case 0x037f:
            cp = 0x03f3;
            break;
        case 0x0386:
            cp = 0x03ac;
            break;
        case 0x0388:
            cp = 0x03ad;
            break;
        case 0x0389:
            cp = 0x03ae;
            break;
        case 0x038a:
            cp = 0x03af;
            break;
        case 0x038c:
            cp = 0x03cc;
            break;
        case 0x038e:
            cp = 0x03cd;
            break;
        case 0x038f:
            cp = 0x03ce;
            break;
        case 0x0370:
            cp = 0x0371;
            break;
        case 0x0372:
            cp = 0x0373;
            break;
        case 0x0376:
            cp = 0x0377;
            break;
        case 0x03f4:
            cp = 0x03d1;
            break;
        case 0x03cf:
            cp = 0x03d7;
            break;
        case 0x03f9:
            cp = 0x03f2;
            break;
        case 0x03f7:
            cp = 0x03f8;
            break;
        case 0x03fa:
            cp = 0x03fb;
            break;
        };
    }

    return cp;
}

utf8_int32_t utf8uprcodepoint(utf8_int32_t cp)
{
    if(((0x0061 <= cp) && (0x007a >= cp)) ||
       ((0x00e0 <= cp) && (0x00f6 >= cp)) ||
       ((0x00f8 <= cp) && (0x00fe >= cp)) ||
       ((0x03b1 <= cp) && (0x03c1 >= cp)) ||
       ((0x03c3 <= cp) && (0x03cb >= cp))) {
        cp -= 32;
    }
    else if(((0x0100 <= cp) && (0x012f >= cp)) ||
            ((0x0132 <= cp) && (0x0137 >= cp)) ||
            ((0x014a <= cp) && (0x0177 >= cp)) ||
            ((0x0182 <= cp) && (0x0185 >= cp)) ||
            ((0x01a0 <= cp) && (0x01a5 >= cp)) ||
            ((0x01de <= cp) && (0x01ef >= cp)) ||
            ((0x01f8 <= cp) && (0x021f >= cp)) ||
            ((0x0222 <= cp) && (0x0233 >= cp)) ||
            ((0x0246 <= cp) && (0x024f >= cp)) ||
            ((0x03d8 <= cp) && (0x03ef >= cp))) {
        cp &= ~0x1;
    }
    else if(((0x0139 <= cp) && (0x0148 >= cp)) ||
            ((0x0179 <= cp) && (0x017e >= cp)) ||
            ((0x01af <= cp) && (0x01b0 >= cp)) ||
            ((0x01b3 <= cp) && (0x01b6 >= cp)) ||
            ((0x01cd <= cp) && (0x01dc >= cp))) {
        cp -= 1;
        cp |= 0x1;
    }
    else {
        switch(cp) {
        default:
            break;
        case 0x00ff:
            cp = 0x0178;
            break;
        case 0x0180:
            cp = 0x0243;
            break;
        case 0x01dd:
            cp = 0x018e;
            break;
        case 0x019a:
            cp = 0x023d;
            break;
        case 0x019e:
            cp = 0x0220;
            break;
        case 0x0292:
            cp = 0x01b7;
            break;
        case 0x01c6:
            cp = 0x01c4;
            break;
        case 0x01c9:
            cp = 0x01c7;
            break;
        case 0x01cc:
            cp = 0x01ca;
            break;
        case 0x01f3:
            cp = 0x01f1;
            break;
        case 0x01bf:
            cp = 0x01f7;
            break;
        case 0x0188:
            cp = 0x0187;
            break;
        case 0x018c:
            cp = 0x018b;
            break;
        case 0x0192:
            cp = 0x0191;
            break;
        case 0x0199:
            cp = 0x0198;
            break;
        case 0x01a8:
            cp = 0x01a7;
            break;
        case 0x01ad:
            cp = 0x01ac;
            break;
        case 0x01b0:
            cp = 0x01af;
            break;
        case 0x01b9:
            cp = 0x01b8;
            break;
        case 0x01bd:
            cp = 0x01bc;
            break;
        case 0x01f5:
            cp = 0x01f4;
            break;
        case 0x023c:
            cp = 0x023b;
            break;
        case 0x0242:
            cp = 0x0241;
            break;
        case 0x037b:
            cp = 0x03fd;
            break;
        case 0x037c:
            cp = 0x03fe;
            break;
        case 0x037d:
            cp = 0x03ff;
            break;
        case 0x03f3:
            cp = 0x037f;
            break;
        case 0x03ac:
            cp = 0x0386;
            break;
        case 0x03ad:
            cp = 0x0388;
            break;
        case 0x03ae:
            cp = 0x0389;
            break;
        case 0x03af:
            cp = 0x038a;
            break;
        case 0x03cc:
            cp = 0x038c;
            break;
        case 0x03cd:
            cp = 0x038e;
            break;
        case 0x03ce:
            cp = 0x038f;
            break;
        case 0x0371:
            cp = 0x0370;
            break;
        case 0x0373:
            cp = 0x0372;
            break;
        case 0x0377:
            cp = 0x0376;
            break;
        case 0x03d1:
            cp = 0x03f4;
            break;
        case 0x03d7:
            cp = 0x03cf;
            break;
        case 0x03f2:
            cp = 0x03f9;
            break;
        case 0x03f8:
            cp = 0x03f7;
            break;
        case 0x03fb:
            cp = 0x03fa;
            break;
        };
    }

    return cp;
}

#undef utf8_restrict
#undef utf8_null

#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // SHEREDOM_UTF8_H_INCLUDED

/********************************************************
   End of file "utf8.h"
 ********************************************************/

/********************************************************
   Begin of file "string_buffer.h"
 ********************************************************/

#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */

/*****************************************************************************
 *               STRING BUFFER
 * ***************************************************************************/

struct f_string_buffer
{
    union
    {
        char* cstr;
#ifdef FT_HAVE_WCHAR
        wchar_t* wstr;
#endif
#ifdef FT_HAVE_UTF8
        void* u8str;
#endif
        void* data;
    } str;
    size_t data_sz;
    enum f_string_type type;
};

FT_INTERNAL
f_string_buffer_t* create_string_buffer(size_t number_of_chars,
                                        enum f_string_type type);

FT_INTERNAL
void destroy_string_buffer(f_string_buffer_t* buffer);

FT_INTERNAL
f_string_buffer_t* copy_string_buffer(const f_string_buffer_t* buffer);

FT_INTERNAL
f_status realloc_string_buffer_without_copy(f_string_buffer_t* buffer);

FT_INTERNAL
f_status fill_buffer_from_string(f_string_buffer_t* buffer, const char* str);

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
f_status fill_buffer_from_wstring(f_string_buffer_t* buffer,
                                  const wchar_t* str);
#endif /* FT_HAVE_WCHAR */

#ifdef FT_HAVE_UTF8
FT_INTERNAL
f_status fill_buffer_from_u8string(f_string_buffer_t* buffer, const void* str);
#endif /* FT_HAVE_UTF8 */

FT_INTERNAL
size_t buffer_text_visible_width(const f_string_buffer_t* buffer);

FT_INTERNAL
size_t buffer_text_visible_height(const f_string_buffer_t* buffer);

FT_INTERNAL
size_t string_buffer_cod_width_capacity(const f_string_buffer_t* buffer);

FT_INTERNAL
size_t string_buffer_raw_capacity(const f_string_buffer_t* buffer);

FT_INTERNAL
size_t string_buffer_width_capacity(const f_string_buffer_t* buffer);

FT_INTERNAL
void* buffer_get_data(f_string_buffer_t* buffer);

FT_INTERNAL
int buffer_check_align(f_string_buffer_t* buffer);

FT_INTERNAL
int buffer_printf(f_string_buffer_t* buffer,
                  size_t buffer_row,
                  f_conv_context_t* cntx,
                  size_t cod_width,
                  const char* content_style_tag,
                  const char* reset_content_style_tag);

#ifdef FT_HAVE_UTF8
FT_INTERNAL
void buffer_set_u8strwid_func(int (*u8strwid)(const void* beg,
                                              const void* end,
                                              size_t* width));
#endif /* FT_HAVE_UTF8 */

#endif /* STRING_BUFFER_H */

/********************************************************
   End of file "string_buffer.h"
 ********************************************************/

/********************************************************
   Begin of file "properties.h"
 ********************************************************/

#ifndef PROPERTIES_H
#define PROPERTIES_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */
#include <limits.h>
#include <stdint.h>

#define PROP_IS_SET(ft_props, property) ((ft_props) & (property))
#define PROP_SET(ft_props, property) ((ft_props) |= (property))
#define PROP_UNSET(ft_props, property) ((ft_props) &= ~((uint32_t)(property)))

#define TEXT_STYLE_TAG_MAX_SIZE (64 * 2)

FT_INTERNAL
void get_style_tag_for_cell(const f_table_properties_t* props,
                            size_t row,
                            size_t col,
                            char* style_tag,
                            size_t sz);

FT_INTERNAL
void get_reset_style_tag_for_cell(const f_table_properties_t* props,
                                  size_t row,
                                  size_t col,
                                  char* style_tag,
                                  size_t sz);

FT_INTERNAL
void get_style_tag_for_content(const f_table_properties_t* props,
                               size_t row,
                               size_t col,
                               char* style_tag,
                               size_t sz);

FT_INTERNAL
void get_reset_style_tag_for_content(const f_table_properties_t* props,
                                     size_t row,
                                     size_t col,
                                     char* style_tag,
                                     size_t sz);

struct f_cell_props
{
    size_t cell_row;
    size_t cell_col;
    uint32_t properties_flags;

    unsigned int col_min_width;
    enum ft_text_alignment align;
    unsigned int cell_padding_top;
    unsigned int cell_padding_bottom;
    unsigned int cell_padding_left;
    unsigned int cell_padding_right;
    unsigned int cell_empty_string_height;
    enum ft_row_type row_type;
    unsigned int content_fg_color_number;
    unsigned int content_bg_color_number;
    unsigned int cell_bg_color_number;
    enum ft_text_style cell_text_style;
    enum ft_text_style content_text_style;
};

typedef struct f_cell_props f_cell_props_t;
typedef f_vector_t f_cell_prop_container_t;

FT_INTERNAL
f_cell_prop_container_t* create_cell_prop_container(void);

FT_INTERNAL
void destroy_cell_prop_container(f_cell_prop_container_t* cont);

FT_INTERNAL
const f_cell_props_t*
cget_cell_prop(const f_cell_prop_container_t* cont, size_t row, size_t col);

FT_INTERNAL
f_cell_props_t* get_cell_prop_and_create_if_not_exists(
    f_cell_prop_container_t* cont, size_t row, size_t col);

FT_INTERNAL
f_status set_cell_property(f_cell_prop_container_t* cont,
                           size_t row,
                           size_t col,
                           uint32_t property,
                           int value);

FT_INTERNAL
int get_cell_property_hierarchically(const f_table_properties_t* properties,
                                     size_t row,
                                     size_t column,
                                     uint32_t property);

FT_INTERNAL
f_status set_default_cell_property(uint32_t property, int value);

/*         TABLE BORDER DESСRIPTION
 *
 *
 *   TL TT TT TT TV TT TT TT TT TT TT TT TR
 *   LL          IV                      RR
 *   LL          IV                      RR
 *   LH IH IH IH II IH IH IH TI IH IH IH RH
 *   LL          IV          IV          RR
 *   LL          IV          IV          RR
 *   LL          LI IH IH IH RI          RH
 *   LL          IV          IV          RR
 *   LL          IV          IV          RR
 *   LH IH IH IH BI IH IH IH II IH IH IH RH
 *   LL                      IV          RR
 *   LL                      IV          RR
 *   BL BB BB BB BV BB BB BB BV BB BB BB BR
 */

/*      HORIZONTAL SEPARATOR DESCRIPTION
 *
 *
 *   TL TT TT TT TV TT TT TT TV TT TT TT TR        <----- TOP_SEPARATOR
 *   LL          IV          IV          RR
 *   LH IH IH IH II IH IH IH II IH IH IH RH        <----- INSIDE_SEPARATOR
 *   LL          IV          IV          RR
 *   BL BB BB BB BV BB BB BB BV BB BB BB BR        <----- BOTTOM_SEPARATOR
 */

enum f_hor_separator_pos
{
    TOP_SEPARATOR,
    INSIDE_SEPARATOR,
    BOTTOM_SEPARATOR
};

enum f_border_item_pos
{
    TL_bip = 0,
    TT_bip = 1,
    TV_bip = 2,
    TR_bip = 3,

    LL_bip = 4,
    IV_bip = 5,
    RR_bip = 6,

    LH_bip = 7,
    IH_bip = 8,
    II_bip = 9,
    RH_bip = 10,

    BL_bip = 11,
    BB_bip = 12,
    BV_bip = 13,
    BR_bip = 14,

    LI_bip = 15,
    TI_bip = 16,
    RI_bip = 17,
    BI_bip = 18,

    BORDER_ITEM_POS_SIZE
};

enum f_separator_item_pos
{
    LH_sip = 0,
    IH_sip = 1,
    II_sip = 2,
    RH_sip = 3,

    TI_sip = 4,
    BI_sip = 5,

    SEPARATOR_ITEM_POS_SIZE
};

struct fort_border_style
{
    const char* border_chars[BORDER_ITEM_POS_SIZE];
    const char* header_border_chars[BORDER_ITEM_POS_SIZE];
    const char* separator_chars[SEPARATOR_ITEM_POS_SIZE];
};
extern struct fort_border_style FORT_BASIC_STYLE;
extern struct fort_border_style FORT_BASIC2_STYLE;
extern struct fort_border_style FORT_SIMPLE_STYLE;
extern struct fort_border_style FORT_PLAIN_STYLE;
extern struct fort_border_style FORT_DOT_STYLE;
extern struct fort_border_style FORT_EMPTY_STYLE;
extern struct fort_border_style FORT_EMPTY2_STYLE;
extern struct fort_border_style FORT_SOLID_STYLE;
extern struct fort_border_style FORT_SOLID_ROUND_STYLE;
extern struct fort_border_style FORT_NICE_STYLE;
extern struct fort_border_style FORT_DOUBLE_STYLE;
extern struct fort_border_style FORT_DOUBLE2_STYLE;
extern struct fort_border_style FORT_BOLD_STYLE;
extern struct fort_border_style FORT_BOLD2_STYLE;
extern struct fort_border_style FORT_FRAME_STYLE;

struct fort_entire_table_properties
{
    unsigned int left_margin;
    unsigned int top_margin;
    unsigned int right_margin;
    unsigned int bottom_margin;
    enum ft_adding_strategy add_strategy;
};
typedef struct fort_entire_table_properties fort_entire_table_properties_t;
extern fort_entire_table_properties_t g_entire_table_properties;

FT_INTERNAL
f_status set_entire_table_property(f_table_properties_t* table_properties,
                                   uint32_t property,
                                   int value);

FT_INTERNAL
f_status set_default_entire_table_property(uint32_t property, int value);

struct f_table_properties
{
    struct fort_border_style border_style;
    f_cell_prop_container_t* cell_properties;
    fort_entire_table_properties_t entire_table_properties;
};
extern f_table_properties_t g_table_properties;

FT_INTERNAL
size_t max_border_elem_strlen(struct f_table_properties*);

FT_INTERNAL
f_table_properties_t* create_table_properties(void);

FT_INTERNAL
void destroy_table_properties(f_table_properties_t* properties);

FT_INTERNAL
f_table_properties_t*
copy_table_properties(const f_table_properties_t* property);

#endif /* PROPERTIES_H */

/********************************************************
   End of file "properties.h"
 ********************************************************/

/********************************************************
   Begin of file "cell.h"
 ********************************************************/

#ifndef CELL_H
#define CELL_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */

FT_INTERNAL
f_cell_t* create_cell(void);

FT_INTERNAL
void destroy_cell(f_cell_t* cell);

FT_INTERNAL
f_cell_t* copy_cell(f_cell_t* cell);

FT_INTERNAL
size_t cell_vis_width(const f_cell_t* cell, const f_context_t* context);

FT_INTERNAL
size_t cell_invis_codes_width(const f_cell_t* cell, const f_context_t* context);

FT_INTERNAL
size_t hint_height_cell(const f_cell_t* cell, const f_context_t* context);

FT_INTERNAL
void set_cell_type(f_cell_t* cell, enum f_cell_type type);

FT_INTERNAL
enum f_cell_type get_cell_type(const f_cell_t* cell);

FT_INTERNAL
int cell_printf(f_cell_t* cell,
                size_t row,
                f_conv_context_t* cntx,
                size_t cod_width);

FT_INTERNAL
f_status fill_cell_from_string(f_cell_t* cell, const char* str);

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
f_status fill_cell_from_wstring(f_cell_t* cell, const wchar_t* str);
#endif

FT_INTERNAL
f_status fill_cell_from_buffer(f_cell_t* cell, const f_string_buffer_t* buf);

FT_INTERNAL
f_string_buffer_t* cell_get_string_buffer(f_cell_t* cell);

#endif /* CELL_H */

/********************************************************
   End of file "cell.h"
 ********************************************************/

/********************************************************
   Begin of file "row.h"
 ********************************************************/

#ifndef ROW_H
#define ROW_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */
//#include "fort.h"
#include <stdarg.h>
/* #include "properties.h" */ /* Commented by amalgamation script */
#ifdef FT_HAVE_WCHAR
#include <wchar.h>
#endif

FT_INTERNAL
f_row_t* create_row(void);

FT_INTERNAL
void destroy_row(f_row_t* row);

FT_INTERNAL
f_row_t* copy_row(f_row_t* row);

FT_INTERNAL
f_row_t* split_row(f_row_t* row, size_t pos);

// Delete range [left; right] of cells (both ends included)
FT_INTERNAL
int ft_row_erase_range(f_row_t* row, size_t left, size_t right);

FT_INTERNAL
f_row_t* create_row_from_string(const char* str);

FT_INTERNAL
f_row_t* create_row_from_fmt_string(const struct f_string_view* fmt,
                                    va_list* va_args);

FT_INTERNAL
size_t columns_in_row(const f_row_t* row);

FT_INTERNAL
f_cell_t* get_cell(f_row_t* row, size_t col);

FT_INTERNAL
const f_cell_t* get_cell_c(const f_row_t* row, size_t col);

FT_INTERNAL
f_cell_t* get_cell_and_create_if_not_exists(f_row_t* row, size_t col);

FT_INTERNAL
f_cell_t* create_cell_in_position(f_row_t* row, size_t col);

FT_INTERNAL
f_status swap_row(f_row_t* cur_row, f_row_t* ins_row, size_t pos);

FT_INTERNAL
f_status insert_row(f_row_t* cur_row, f_row_t* ins_row, size_t pos);

FT_INTERNAL
size_t group_cell_number(const f_row_t* row, size_t master_cell_col);

FT_INTERNAL
int get_row_cell_types(const f_row_t* row,
                       enum f_cell_type* types,
                       size_t types_sz);

FT_INTERNAL
f_status row_set_cell_span(f_row_t* row, size_t cell_column, size_t hor_span);

FT_INTERNAL
int print_row_separator(f_conv_context_t* cntx,
                        const size_t* col_width_arr,
                        size_t cols,
                        const f_row_t* upper_row,
                        const f_row_t* lower_row,
                        enum f_hor_separator_pos separatorPos,
                        const f_separator_t* sep);

FT_INTERNAL
int snprintf_row(const f_row_t* row,
                 f_conv_context_t* cntx,
                 size_t* col_width_arr,
                 size_t col_width_arr_sz,
                 size_t row_height);

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
f_row_t* create_row_from_wstring(const wchar_t* str);
#endif

#endif /* ROW_H */

/********************************************************
   End of file "row.h"
 ********************************************************/

/********************************************************
   Begin of file "table.h"
 ********************************************************/

#ifndef TABLE_H
#define TABLE_H

/* #include "fort_utils.h" */ /* Commented by amalgamation script */

struct ft_table
{
    f_vector_t* rows;
    f_table_properties_t* properties;
    f_string_buffer_t* conv_buffer;
    size_t cur_row;
    size_t cur_col;
    f_vector_t* separators;
};

FT_INTERNAL
f_separator_t* create_separator(int enabled);

FT_INTERNAL
void destroy_separator(f_separator_t* sep);

FT_INTERNAL
f_separator_t* copy_separator(f_separator_t* sep);

FT_INTERNAL
f_status get_table_sizes(const ft_table_t* table, size_t* rows, size_t* cols);

FT_INTERNAL
f_row_t* get_row(ft_table_t* table, size_t row);

FT_INTERNAL
const f_row_t* get_row_c(const ft_table_t* table, size_t row);

FT_INTERNAL
f_row_t* get_row_and_create_if_not_exists(ft_table_t* table, size_t row);

FT_INTERNAL
f_string_buffer_t*
get_cur_str_buffer_and_create_if_not_exists(ft_table_t* table);

FT_INTERNAL
f_status table_rows_and_cols_geometry(const ft_table_t* table,
                                      size_t** col_width_arr_p,
                                      size_t* col_width_arr_sz,
                                      size_t** row_height_arr_p,
                                      size_t* row_height_arr_sz,
                                      enum f_geometry_type geom);

FT_INTERNAL
f_status table_geometry(const ft_table_t* table, size_t* height, size_t* width);

/*
 * Returns geometry in codepoints(characters) (include codepoints of invisible
 * elements: e.g. styles tags).
 */
FT_INTERNAL
f_status table_internal_codepoints_geometry(const ft_table_t* table,
                                            size_t* height,
                                            size_t* width);

#endif /* TABLE_H */

/********************************************************
   End of file "table.h"
 ********************************************************/

/********************************************************
   Begin of file "cell.c"
 ********************************************************/

/* #include "cell.h" */          /* Commented by amalgamation script */
/* #include "properties.h" */    /* Commented by amalgamation script */
/* #include "string_buffer.h" */ /* Commented by amalgamation script */
#include <assert.h>

struct f_cell
{
    f_string_buffer_t* str_buffer;
    enum f_cell_type cell_type;
};

FT_INTERNAL
f_cell_t* create_cell(void)
{
    f_cell_t* cell = static_cast<f_cell_t*>(F_CALLOC(sizeof(f_cell_t), 1));
    if(cell == NULL)
        return NULL;
    cell->str_buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, CHAR_BUF);
    if(cell->str_buffer == NULL) {
        F_FREE(cell);
        return NULL;
    }
    cell->cell_type = COMMON_CELL;
    return cell;
}

FT_INTERNAL
void destroy_cell(f_cell_t* cell)
{
    if(cell == NULL)
        return;
    destroy_string_buffer(cell->str_buffer);
    F_FREE(cell);
}

FT_INTERNAL
f_cell_t* copy_cell(f_cell_t* cell)
{
    assert(cell);

    f_cell_t* result = create_cell();
    if(result == NULL)
        return NULL;
    destroy_string_buffer(result->str_buffer);
    result->str_buffer = copy_string_buffer(cell->str_buffer);
    if(result->str_buffer == NULL) {
        destroy_cell(result);
        return NULL;
    }
    result->cell_type = cell->cell_type;
    return result;
}

FT_INTERNAL
void set_cell_type(f_cell_t* cell, enum f_cell_type type)
{
    assert(cell);
    cell->cell_type = type;
}

FT_INTERNAL
enum f_cell_type get_cell_type(const f_cell_t* cell)
{
    assert(cell);
    return cell->cell_type;
}

FT_INTERNAL
size_t cell_vis_width(const f_cell_t* cell, const f_context_t* context)
{
    /* todo:
     * At the moment min width includes paddings. Maybe it is better that min
     * width weren't include paddings but be min width of the cell content
     * without padding
     */

    assert(cell);
    assert(context);

    f_table_properties_t* properties = context->table_properties;
    size_t row = context->row;
    size_t column = context->column;

    size_t padding_left = get_cell_property_hierarchically(
        properties, row, column, FT_CPROP_LEFT_PADDING);
    size_t padding_right = get_cell_property_hierarchically(
        properties, row, column, FT_CPROP_RIGHT_PADDING);
    size_t result = padding_left + padding_right;
    if(cell->str_buffer && cell->str_buffer->str.data) {
        result += buffer_text_visible_width(cell->str_buffer);
    }
    result = MAX(result,
                 (size_t)get_cell_property_hierarchically(
                     properties, row, column, FT_CPROP_MIN_WIDTH));
    return result;
}

FT_INTERNAL
size_t cell_invis_codes_width([[maybe_unused]] const f_cell_t* cell,
                              const f_context_t* context)
{
    assert(cell);
    assert(context);

    f_table_properties_t* properties = context->table_properties;
    size_t row = context->row;
    size_t column = context->column;

    size_t result = 0;
    char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_cell(
        properties, row, column, cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    result += strlen(cell_style_tag);

    char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_cell(
        properties, row, column, reset_cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    result += strlen(reset_cell_style_tag);

    char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_content(
        properties, row, column, content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    result += strlen(content_style_tag);

    char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_content(properties,
                                    row,
                                    column,
                                    reset_content_style_tag,
                                    TEXT_STYLE_TAG_MAX_SIZE);
    result += strlen(reset_content_style_tag);
    return result;
}

FT_INTERNAL
size_t hint_height_cell(const f_cell_t* cell, const f_context_t* context)
{
    assert(cell);
    assert(context);
    f_table_properties_t* properties = context->table_properties;
    size_t row = context->row;
    size_t column = context->column;

    size_t padding_top = get_cell_property_hierarchically(
        properties, row, column, FT_CPROP_TOP_PADDING);
    size_t padding_bottom = get_cell_property_hierarchically(
        properties, row, column, FT_CPROP_BOTTOM_PADDING);
    size_t empty_string_height = get_cell_property_hierarchically(
        properties, row, column, FT_CPROP_EMPTY_STR_HEIGHT);

    size_t result = padding_top + padding_bottom;
    if(cell->str_buffer && cell->str_buffer->str.data) {
        size_t text_height = buffer_text_visible_height(cell->str_buffer);
        result += text_height == 0 ? empty_string_height : text_height;
    }
    return result;
}

FT_INTERNAL
int cell_printf(f_cell_t* cell,
                size_t row,
                f_conv_context_t* cntx,
                size_t vis_width)
{
    const f_context_t* context = cntx->cntx;
    size_t buf_len = vis_width;

    if(cell == NULL || (vis_width < cell_vis_width(cell, context))) {
        return -1;
    }

    f_table_properties_t* properties = context->table_properties;
    unsigned int padding_top = get_cell_property_hierarchically(
        properties, context->row, context->column, FT_CPROP_TOP_PADDING);
    unsigned int padding_left = get_cell_property_hierarchically(
        properties, context->row, context->column, FT_CPROP_LEFT_PADDING);
    unsigned int padding_right = get_cell_property_hierarchically(
        properties, context->row, context->column, FT_CPROP_RIGHT_PADDING);

    size_t written = 0;
    size_t invisible_written = 0;
    int tmp = 0;

    /* todo: Dirty hack with changing buf_len! need refactoring. */
    /* Also maybe it is better to move all struff with colors to buffers? */
    char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_cell(properties,
                           context->row,
                           context->column,
                           cell_style_tag,
                           TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(cell_style_tag);

    char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_cell(properties,
                                 context->row,
                                 context->column,
                                 reset_cell_style_tag,
                                 TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(reset_cell_style_tag);

    char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_content(properties,
                              context->row,
                              context->column,
                              content_style_tag,
                              TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(content_style_tag);

    char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_content(properties,
                                    context->row,
                                    context->column,
                                    reset_content_style_tag,
                                    TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(reset_content_style_tag);

    /*    CELL_STYLE_T   LEFT_PADDING   CONTENT_STYLE_T  CONTENT
     * RESET_CONTENT_STYLE_T    RIGHT_PADDING   RESET_CELL_STYLE_T | | | | | |
     * |                    | L1 R1 L2 R2 L3                               R3
     */

    size_t L2 = padding_left;

    size_t R2 = padding_right;
    size_t R3 = strlen(reset_cell_style_tag);

#define TOTAL_WRITTEN (written + invisible_written)
#define RIGHT (padding_right + extra_right)

#define WRITE_CELL_STYLE_TAG                                                   \
    CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(print_n_strings(cntx, 1, cell_style_tag))
#define WRITE_RESET_CELL_STYLE_TAG                                             \
    CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(                                        \
        print_n_strings(cntx, 1, reset_cell_style_tag))
#define WRITE_CONTENT_STYLE_TAG                                                \
    CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(                                        \
        print_n_strings(cntx, 1, content_style_tag))
#define WRITE_RESET_CONTENT_STYLE_TAG                                          \
    CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(                                        \
        print_n_strings(cntx, 1, reset_content_style_tag))

    if(row >= hint_height_cell(cell, context) || row < padding_top ||
       row >= (padding_top + buffer_text_visible_height(cell->str_buffer))) {
        WRITE_CELL_STYLE_TAG;
        WRITE_CONTENT_STYLE_TAG;
        WRITE_RESET_CONTENT_STYLE_TAG;
        CHCK_RSLT_ADD_TO_WRITTEN(
            print_n_strings(cntx, buf_len - TOTAL_WRITTEN - R3, FT_SPACE));
        WRITE_RESET_CELL_STYLE_TAG;

        return static_cast<int>(TOTAL_WRITTEN);
    }

    WRITE_CELL_STYLE_TAG;
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, L2, FT_SPACE));
    if(cell->str_buffer) {
        CHCK_RSLT_ADD_TO_WRITTEN(buffer_printf(cell->str_buffer,
                                               row - padding_top,
                                               cntx,
                                               vis_width - L2 - R2,
                                               content_style_tag,
                                               reset_content_style_tag));
    }
    else {
        WRITE_CONTENT_STYLE_TAG;
        WRITE_RESET_CONTENT_STYLE_TAG;
        CHCK_RSLT_ADD_TO_WRITTEN(
            print_n_strings(cntx, vis_width - L2 - R2, FT_SPACE));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, R2, FT_SPACE));
    WRITE_RESET_CELL_STYLE_TAG;

    return static_cast<int>(TOTAL_WRITTEN);

clear:
    return -1;
#undef WRITE_CELL_STYLE_TAG
#undef WRITE_RESET_CELL_STYLE_TAG
#undef WRITE_CONTENT_STYLE_TAG
#undef WRITE_RESET_CONTENT_STYLE_TAG
#undef TOTAL_WRITTEN
#undef RIGHT
}

FT_INTERNAL
f_status fill_cell_from_string(f_cell_t* cell, const char* str)
{
    assert(str);
    assert(cell);

    return fill_buffer_from_string(cell->str_buffer, str);
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
f_status fill_cell_from_wstring(f_cell_t* cell, const wchar_t* str)
{
    assert(str);
    assert(cell);

    return fill_buffer_from_wstring(cell->str_buffer, str);
}
#endif

#ifdef FT_HAVE_UTF8
static f_status fill_cell_from_u8string(f_cell_t* cell, const void* str)
{
    assert(str);
    assert(cell);
    return fill_buffer_from_u8string(cell->str_buffer, str);
}
#endif /* FT_HAVE_UTF8 */

FT_INTERNAL
f_string_buffer_t* cell_get_string_buffer(f_cell_t* cell)
{
    assert(cell);
    assert(cell->str_buffer);
    return cell->str_buffer;
}

FT_INTERNAL
f_status fill_cell_from_buffer(f_cell_t* cell, const f_string_buffer_t* buffer)
{
    assert(cell);
    assert(buffer);
    switch(buffer->type) {
    case CHAR_BUF:
        return fill_cell_from_string(cell, buffer->str.cstr);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return fill_cell_from_wstring(cell, buffer->str.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return fill_cell_from_u8string(cell, buffer->str.u8str);
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
        return FT_GEN_ERROR;
    }
}

/********************************************************
   End of file "cell.c"
 ********************************************************/

/********************************************************
   Begin of file "fort_impl.c"
 ********************************************************/

/*
libfort

MIT License

Copyright (c) 2017 - 2018 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//#include "fort.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* #include "vector.h" */        /* Commented by amalgamation script */
/* #include "fort_utils.h" */    /* Commented by amalgamation script */
/* #include "string_buffer.h" */ /* Commented by amalgamation script */
/* #include "table.h" */         /* Commented by amalgamation script */
/* #include "row.h" */           /* Commented by amalgamation script */
/* #include "properties.h" */    /* Commented by amalgamation script */

ft_table_t* ft_create_table(void)
{
    ft_table_t* result =
        static_cast<ft_table_t*>(F_CALLOC(1, sizeof(ft_table_t)));
    if(result == NULL)
        return NULL;

    result->rows = create_vector(sizeof(f_row_t*), DEFAULT_VECTOR_CAPACITY);
    if(result->rows == NULL) {
        F_FREE(result);
        return NULL;
    }
    result->separators =
        create_vector(sizeof(f_separator_t*), DEFAULT_VECTOR_CAPACITY);
    if(result->separators == NULL) {
        destroy_vector(result->rows);
        F_FREE(result);
        return NULL;
    }

    result->properties = create_table_properties();
    if(result->properties == NULL) {
        destroy_vector(result->separators);
        destroy_vector(result->rows);
        F_FREE(result);
        return NULL;
    }
    result->conv_buffer = NULL;
    result->cur_row = 0;
    result->cur_col = 0;
    return result;
}

void ft_destroy_table(ft_table_t* table)
{
    size_t i = 0;

    if(table == NULL)
        return;

    if(table->rows) {
        size_t row_n = vector_size(table->rows);
        for(i = 0; i < row_n; ++i) {
            destroy_row(VECTOR_AT(table->rows, i, f_row_t*));
        }
        destroy_vector(table->rows);
    }
    if(table->separators) {
        size_t row_n = vector_size(table->separators);
        for(i = 0; i < row_n; ++i) {
            destroy_separator(VECTOR_AT(table->separators, i, f_separator_t*));
        }
        destroy_vector(table->separators);
    }
    destroy_table_properties(table->properties);
    destroy_string_buffer(table->conv_buffer);
    F_FREE(table);
}

ft_table_t* ft_copy_table(ft_table_t* table)
{
    if(table == NULL)
        return NULL;

    ft_table_t* result = ft_create_table();
    if(result == NULL)
        return NULL;

    size_t i = 0;
    size_t rows_n = vector_size(table->rows);
    for(i = 0; i < rows_n; ++i) {
        f_row_t* row = VECTOR_AT(table->rows, i, f_row_t*);
        f_row_t* new_row = copy_row(row);
        if(new_row == NULL) {
            ft_destroy_table(result);
            return NULL;
        }
        vector_push(result->rows, &new_row);
    }

    size_t sep_sz = vector_size(table->separators);
    for(i = 0; i < sep_sz; ++i) {
        f_separator_t* sep = VECTOR_AT(table->separators, i, f_separator_t*);
        f_separator_t* new_sep = copy_separator(sep);
        if(new_sep == NULL) {
            ft_destroy_table(result);
            return NULL;
        }
        vector_push(result->separators, &new_sep);
    }

    /* note: by default new table has allocated default properties, so we
     * have to destroy them first.
     */
    if(result->properties) {
        destroy_table_properties(result->properties);
    }
    result->properties = copy_table_properties(table->properties);
    if(result->properties == NULL) {
        ft_destroy_table(result);
        return NULL;
    }

    /* todo: copy conv_buffer  ??  */

    result->cur_row = table->cur_row;
    result->cur_col = table->cur_col;
    return result;
}

static int split_cur_row(ft_table_t* table, f_row_t** tail_of_cur_row)
{
    if(table->cur_row >= vector_size(table->rows)) {
        tail_of_cur_row = NULL;
        return 0;
    }

    f_row_t* row = VECTOR_AT(table->rows, table->cur_row, f_row_t*);
    if(table->cur_col >= columns_in_row(row)) {
        tail_of_cur_row = NULL;
        return 0;
    }

    f_row_t* tail = split_row(row, table->cur_col);
    if(!tail) {
        tail_of_cur_row = NULL;
        return FT_GEN_ERROR;
    }

    *tail_of_cur_row = tail;
    return 0;
}

int ft_ln(ft_table_t* table)
{
    assert(table);
    fort_entire_table_properties_t* table_props =
        &table->properties->entire_table_properties;
    switch(table_props->add_strategy) {
    case FT_STRATEGY_INSERT: {
        f_row_t* new_row = NULL;
        if(FT_IS_ERROR(split_cur_row(table, &new_row))) {
            return FT_GEN_ERROR;
        }
        if(new_row) {
            if(FT_IS_ERROR(
                   vector_insert(table->rows, &new_row, table->cur_row + 1))) {
                destroy_row(new_row);
                return FT_GEN_ERROR;
            }
        }
        break;
    }
    case FT_STRATEGY_REPLACE:
        // do nothing
        break;
    default:
        assert(0 && "Unexpected situation inside libfort");
        break;
    }
    table->cur_col = 0;
    table->cur_row++;
    return FT_SUCCESS;
}

size_t ft_cur_row(const ft_table_t* table)
{
    assert(table);
    return table->cur_row;
}

size_t ft_cur_col(const ft_table_t* table)
{
    assert(table);
    return table->cur_col;
}

void ft_set_cur_cell(ft_table_t* table, size_t row, size_t col)
{
    assert(table);
    table->cur_row = row;
    table->cur_col = col;
}

int ft_is_empty(const ft_table_t* table)
{
    assert(table);
    return ft_row_count(table) == 0;
}

size_t ft_row_count(const ft_table_t* table)
{
    assert(table && table->rows);
    return vector_size(table->rows);
}

int ft_erase_range(ft_table_t* table,
                   size_t top_left_row,
                   size_t top_left_col,
                   size_t bottom_right_row,
                   size_t bottom_right_col)
{
    assert(table && table->rows);
    int status = FT_SUCCESS;

    size_t rows_n = vector_size(table->rows);

    if(top_left_row == FT_CUR_ROW)
        top_left_row = table->cur_row;
    if(bottom_right_row == FT_CUR_ROW)
        bottom_right_row = table->cur_row;

    if(top_left_col == FT_CUR_COLUMN)
        top_left_col = table->cur_row;
    if(bottom_right_col == FT_CUR_COLUMN)
        bottom_right_col = table->cur_row;

    if(top_left_row > bottom_right_row || top_left_col > bottom_right_col)
        return FT_EINVAL;

    f_row_t* row = NULL;
    size_t i = top_left_row;
    while(i < rows_n && i <= bottom_right_row) {
        row = VECTOR_AT(table->rows, i, f_row_t*);
        status = ft_row_erase_range(row, top_left_col, bottom_right_col);
        if(FT_IS_ERROR(status))
            return status;
        ++i;
    }

    f_separator_t* separator = NULL;

    size_t n_iterations = MIN(rows_n - 1, bottom_right_row) - top_left_row + 1;
    size_t j = 0;
    i = top_left_row;
    for(j = 0; j < n_iterations; ++j) {
        row = VECTOR_AT(table->rows, i, f_row_t*);
        if(columns_in_row(row)) {
            ++i;
        }
        else {
            destroy_row(row);
            status = vector_erase(table->rows, i);
            if(FT_IS_ERROR(status))
                return status;
            if(i < vector_size(table->separators)) {
                separator = VECTOR_AT(table->separators, i, f_separator_t*);
                destroy_separator(separator);
                vector_erase(table->separators, i);
            }
        }
    }

    return FT_SUCCESS;
}

static int ft_row_printf_impl_(ft_table_t* table,
                               size_t row,
                               const struct f_string_view* fmt,
                               va_list* va)
{
    size_t i = 0;
    size_t new_cols = 0;

    if(table == NULL)
        return -1;

    f_row_t* new_row = create_row_from_fmt_string(fmt, va);

    if(new_row == NULL) {
        return -1;
    }

    f_row_t** cur_row_p = NULL;
    size_t sz = vector_size(table->rows);
    if(row >= sz) {
        size_t push_n = row - sz + 1;
        for(i = 0; i < push_n; ++i) {
            f_row_t* padding_row = create_row();
            if(padding_row == NULL)
                goto clear;

            if(FT_IS_ERROR(vector_push(table->rows, &padding_row))) {
                destroy_row(padding_row);
                goto clear;
            }
        }
    }
    /* todo: clearing pushed items in case of error ?? */

    new_cols = columns_in_row(new_row);
    cur_row_p = &VECTOR_AT(table->rows, row, f_row_t*);

    switch(table->properties->entire_table_properties.add_strategy) {
    case FT_STRATEGY_INSERT: {
        if(FT_IS_ERROR(insert_row(*cur_row_p, new_row, table->cur_col)))
            goto clear;
        break;
    }
    case FT_STRATEGY_REPLACE: {
        if(FT_IS_ERROR(swap_row(*cur_row_p, new_row, table->cur_col)))
            goto clear;
        break;
    }
    default:
        assert(0 && "Unexpected situation inside libfort");
        break;
    }

    table->cur_col += new_cols;
    destroy_row(new_row);
    return (int)new_cols;

clear:
    destroy_row(new_row);
    return -1;
}

#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
#define FT_PRINTF ft_printf
#define FT_PRINTF_LN ft_printf_ln
#else
#define FT_PRINTF ft_printf_impl
#define FT_PRINTF_LN ft_printf_ln_impl
#endif

int FT_PRINTF(ft_table_t* table, const char* fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);

    struct f_string_view fmt_str;
    fmt_str.type = CHAR_BUF;
    fmt_str.u.cstr = fmt;
    int result = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
    va_end(va);
    return result;
}

int FT_PRINTF_LN(ft_table_t* table, const char* fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);

    struct f_string_view fmt_str;
    fmt_str.type = CHAR_BUF;
    fmt_str.u.cstr = fmt;
    int result = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
    if(result >= 0) {
        ft_ln(table);
    }
    va_end(va);
    return result;
}

#undef FT_PRINTF
#undef FT_PRINTF_LN

#ifdef FT_HAVE_WCHAR
int ft_wprintf(ft_table_t* table, const wchar_t* fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);

    struct f_string_view fmt_str;
    fmt_str.type = W_CHAR_BUF;
    fmt_str.u.wstr = fmt;
    int result = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
    va_end(va);
    return result;
}

int ft_wprintf_ln(ft_table_t* table, const wchar_t* fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);

    struct f_string_view fmt_str;
    fmt_str.type = W_CHAR_BUF;
    fmt_str.u.wstr = fmt;
    int result = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
    if(result >= 0) {
        ft_ln(table);
    }
    va_end(va);
    return result;
}

#endif

void ft_set_default_printf_field_separator(char separator)
{
    g_col_separator = separator;
}

static int ft_write_impl_(ft_table_t* table,
                          const f_string_view_t* cell_content)
{
    assert(table);
    f_string_buffer_t* buf = get_cur_str_buffer_and_create_if_not_exists(table);
    if(buf == NULL)
        return FT_GEN_ERROR;

    int status = FT_SUCCESS;
    switch(cell_content->type) {
    case CHAR_BUF:
        status = fill_buffer_from_string(buf, cell_content->u.cstr);
        break;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        status = fill_buffer_from_wstring(buf, cell_content->u.wstr);
        break;
#endif
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        status = fill_buffer_from_u8string(buf, cell_content->u.u8str);
        break;
#endif
    default:
        status = FT_GEN_ERROR;
    }
    if(FT_IS_SUCCESS(status)) {
        table->cur_col++;
    }
    return status;
}

static int ft_write_impl(ft_table_t* table, const char* cell_content)
{
    f_string_view_t content;
    content.type = CHAR_BUF;
    content.u.cstr = cell_content;
    return ft_write_impl_(table, &content);
}

#ifdef FT_HAVE_UTF8
static int ft_u8write_impl(ft_table_t* table, const void* cell_content)
{
    f_string_view_t content;
    content.type = UTF8_BUF;
    content.u.u8str = cell_content;
    return ft_write_impl_(table, &content);
}
#endif /* FT_HAVE_UTF8 */

#ifdef FT_HAVE_WCHAR
static int ft_wwrite_impl(ft_table_t* table, const wchar_t* cell_content)
{
    f_string_view_t content;
    content.type = W_CHAR_BUF;
    content.u.wstr = cell_content;
    return ft_write_impl_(table, &content);
}
#endif

int ft_nwrite(ft_table_t* table, size_t count, const char* cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_write_impl(table, cell_content);
    if(FT_IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --count;
    for(i = 0; i < count; ++i) {
        const char* cell = va_arg(va, const char*);
        status = ft_write_impl(table, cell);
        if(FT_IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);
    return status;
}

int ft_nwrite_ln(ft_table_t* table, size_t count, const char* cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_write_impl(table, cell_content);
    if(FT_IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --count;
    for(i = 0; i < count; ++i) {
        const char* cell = va_arg(va, const char*);
        status = ft_write_impl(table, cell);
        if(FT_IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);

    ft_ln(table);
    return status;
}

#ifdef FT_HAVE_WCHAR

int ft_nwwrite(ft_table_t* table, size_t n, const wchar_t* cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_wwrite_impl(table, cell_content);
    if(FT_IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for(i = 0; i < n; ++i) {
        const wchar_t* cell = va_arg(va, const wchar_t*);
        status = ft_wwrite_impl(table, cell);
        if(FT_IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);
    return status;
}

int ft_nwwrite_ln(ft_table_t* table, size_t n, const wchar_t* cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_wwrite_impl(table, cell_content);
    if(FT_IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for(i = 0; i < n; ++i) {
        const wchar_t* cell = va_arg(va, const wchar_t*);
        status = ft_wwrite_impl(table, cell);
        if(FT_IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);

    ft_ln(table);
    return status;
}
#endif

int ft_row_write(ft_table_t* table, size_t cols, const char* cells[])
{
    size_t i = 0;
    assert(table);
    for(i = 0; i < cols; ++i) {
        int status = ft_write_impl(table, cells[i]);
        if(FT_IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal to the
             * one before function call? */
            return status;
        }
    }
    return FT_SUCCESS;
}

int ft_row_write_ln(ft_table_t* table, size_t cols, const char* cells[])
{
    assert(table);
    int status = ft_row_write(table, cols, cells);
    if(FT_IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}

#ifdef FT_HAVE_WCHAR
int ft_row_wwrite(ft_table_t* table, size_t cols, const wchar_t* cells[])
{
    size_t i = 0;
    assert(table);
    for(i = 0; i < cols; ++i) {
        int status = ft_wwrite_impl(table, cells[i]);
        if(FT_IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal
             * to the one before function call?
             */
            return status;
        }
    }
    return FT_SUCCESS;
}

int ft_row_wwrite_ln(ft_table_t* table, size_t cols, const wchar_t* cells[])
{
    assert(table);
    int status = ft_row_wwrite(table, cols, cells);
    if(FT_IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}
#endif

int ft_table_write(ft_table_t* table,
                   size_t rows,
                   size_t cols,
                   const char* table_cells[])
{
    size_t i = 0;
    assert(table);
    for(i = 0; i < rows; ++i) {
        int status =
            ft_row_write(table, cols, (const char**)&table_cells[i * cols]);
        if(FT_IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal
             * to the one before function call?
             */
            return status;
        }
        if(i != rows - 1)
            ft_ln(table);
    }
    return FT_SUCCESS;
}

int ft_table_write_ln(ft_table_t* table,
                      size_t rows,
                      size_t cols,
                      const char* table_cells[])
{
    assert(table);
    int status = ft_table_write(table, rows, cols, table_cells);
    if(FT_IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}

#ifdef FT_HAVE_WCHAR
int ft_table_wwrite(ft_table_t* table,
                    size_t rows,
                    size_t cols,
                    const wchar_t* table_cells[])
{
    size_t i = 0;
    assert(table);
    for(i = 0; i < rows; ++i) {
        int status =
            ft_row_wwrite(table, cols, (const wchar_t**)&table_cells[i * cols]);
        if(FT_IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal
             * to the one before function call?
             */
            return status;
        }
        if(i != rows - 1)
            ft_ln(table);
    }
    return FT_SUCCESS;
}

int ft_table_wwrite_ln(ft_table_t* table,
                       size_t rows,
                       size_t cols,
                       const wchar_t* table_cells[])
{
    assert(table);
    int status = ft_table_wwrite(table, rows, cols, table_cells);
    if(FT_IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}
#endif

static const char* empty_str_arr[] = { "", (const char*)L"", "" };

static const void* ft_to_string_impl(const ft_table_t* table,
                                     enum f_string_type b_type)
{
    assert(table);

    const char* result = NULL;

    /* Determine size of table string representation */
    size_t cod_height = 0;
    size_t cod_width = 0;
    int status =
        table_internal_codepoints_geometry(table, &cod_height, &cod_width);
    if(FT_IS_ERROR(status)) {
        return NULL;
    }
    size_t n_codepoints = cod_height * cod_width + 1;

    /* Allocate string buffer for string representation */
    if(table->conv_buffer == NULL) {
        ((ft_table_t*)table)->conv_buffer =
            create_string_buffer(n_codepoints, b_type);
        if(table->conv_buffer == NULL)
            return NULL;
    }
    while(string_buffer_cod_width_capacity(table->conv_buffer) < n_codepoints) {
        if(FT_IS_ERROR(
               realloc_string_buffer_without_copy(table->conv_buffer))) {
            return NULL;
        }
    }
    if(!buffer_check_align(table->conv_buffer))
        return NULL;
    char* buffer = (char*)buffer_get_data(table->conv_buffer);

    size_t cols = 0;
    size_t rows = 0;
    size_t* col_vis_width_arr = NULL;
    size_t* row_vis_height_arr = NULL;
    status = table_rows_and_cols_geometry(table,
                                          &col_vis_width_arr,
                                          &cols,
                                          &row_vis_height_arr,
                                          &rows,
                                          VISIBLE_GEOMETRY);
    if(FT_IS_ERROR(status))
        return NULL;

    if(rows == 0) {
        F_FREE(col_vis_width_arr);
        F_FREE(row_vis_height_arr);
        return empty_str_arr[b_type];
    }

    int tmp = 0;
    size_t i = 0;
    f_context_t context;
    context.table_properties =
        (table->properties ? table->properties : &g_table_properties);
    f_row_t* prev_row = NULL;
    f_row_t* cur_row = NULL;
    f_separator_t* cur_sep = NULL;
    size_t sep_size = vector_size(table->separators);

    f_conv_context_t cntx;
    cntx.u.buf = buffer;
    cntx.raw_avail = string_buffer_raw_capacity(table->conv_buffer);
    cntx.cntx = &context;
    cntx.b_type = b_type;

    /* Print top margin */
    for(i = 0; i < context.table_properties->entire_table_properties.top_margin;
        ++i) {
        FT_CHECK(print_n_strings(
            &cntx, cod_width - 1 /* minus new_line*/, FT_SPACE));
        FT_CHECK(print_n_strings(&cntx, 1, FT_NEWLINE));
    }

    for(i = 0; i < rows; ++i) {
        cur_sep = (i < sep_size)
                      ? VECTOR_AT(table->separators, i, f_separator_t*)
                      : NULL;
        cur_row = VECTOR_AT(table->rows, i, f_row_t*);
        enum f_hor_separator_pos separatorPos =
            (i == 0) ? TOP_SEPARATOR : INSIDE_SEPARATOR;
        context.row = i;
        FT_CHECK(print_row_separator(&cntx,
                                     col_vis_width_arr,
                                     cols,
                                     prev_row,
                                     cur_row,
                                     separatorPos,
                                     cur_sep));
        FT_CHECK(snprintf_row(
            cur_row, &cntx, col_vis_width_arr, cols, row_vis_height_arr[i]));
        prev_row = cur_row;
    }
    cur_row = NULL;
    cur_sep =
        (i < sep_size) ? VECTOR_AT(table->separators, i, f_separator_t*) : NULL;
    context.row = i;
    FT_CHECK(print_row_separator(&cntx,
                                 col_vis_width_arr,
                                 cols,
                                 prev_row,
                                 cur_row,
                                 BOTTOM_SEPARATOR,
                                 cur_sep));

    /* Print bottom margin */
    for(i = 0;
        i < context.table_properties->entire_table_properties.bottom_margin;
        ++i) {
        FT_CHECK(print_n_strings(
            &cntx, cod_width - 1 /* minus new_line*/, FT_SPACE));
        FT_CHECK(print_n_strings(&cntx, 1, FT_NEWLINE));
    }

    result = buffer;

clear:
    F_FREE(col_vis_width_arr);
    F_FREE(row_vis_height_arr);
    return result;
}

const char* ft_to_string(const ft_table_t* table)
{
    return (const char*)ft_to_string_impl(table, CHAR_BUF);
}

#ifdef FT_HAVE_WCHAR
const wchar_t* ft_to_wstring(const ft_table_t* table)
{
    return (const wchar_t*)ft_to_string_impl(table, W_CHAR_BUF);
}
#endif

int ft_add_separator(ft_table_t* table)
{
    assert(table);
    assert(table->separators);

    while(vector_size(table->separators) <= table->cur_row) {
        f_separator_t* sep_p = create_separator(F_FALSE);
        if(sep_p == NULL)
            return FT_MEMORY_ERROR;
        int status = vector_push(table->separators, &sep_p);
        if(FT_IS_ERROR(status))
            return status;
    }

    f_separator_t** sep_p =
        &VECTOR_AT(table->separators, table->cur_row, f_separator_t*);
    if(*sep_p == NULL)
        *sep_p = create_separator(F_TRUE);
    else
        (*sep_p)->enabled = F_TRUE;

    if(*sep_p == NULL)
        return FT_GEN_ERROR;
    return FT_SUCCESS;
}

static const struct fort_border_style* built_in_styles[] = {
    &FORT_BASIC_STYLE,  &FORT_BASIC2_STYLE, &FORT_SIMPLE_STYLE,
    &FORT_PLAIN_STYLE,  &FORT_DOT_STYLE,    &FORT_EMPTY_STYLE,
    &FORT_EMPTY2_STYLE, &FORT_SOLID_STYLE,  &FORT_SOLID_ROUND_STYLE,
    &FORT_NICE_STYLE,   &FORT_DOUBLE_STYLE, &FORT_DOUBLE2_STYLE,
    &FORT_BOLD_STYLE,   &FORT_BOLD2_STYLE,  &FORT_FRAME_STYLE,
};
#define BUILT_IN_STYLES_SZ                                                     \
    (sizeof(built_in_styles) / sizeof(built_in_styles[0]))

/* todo: remove this stupid and dangerous code */
static const struct ft_border_style
    built_in_external_styles[BUILT_IN_STYLES_SZ] = {
        { { "", "", "", "", "", "" }, { "", "", "", "", "", "" }, "" }
    };

const struct ft_border_style* const FT_BASIC_STYLE =
    &built_in_external_styles[0];
const struct ft_border_style* const FT_BASIC2_STYLE =
    &built_in_external_styles[1];
const struct ft_border_style* const FT_SIMPLE_STYLE =
    &built_in_external_styles[2];
const struct ft_border_style* const FT_PLAIN_STYLE =
    &built_in_external_styles[3];
const struct ft_border_style* const FT_DOT_STYLE = &built_in_external_styles[4];
const struct ft_border_style* const FT_EMPTY_STYLE =
    &built_in_external_styles[5];
const struct ft_border_style* const FT_EMPTY2_STYLE =
    &built_in_external_styles[6];
const struct ft_border_style* const FT_SOLID_STYLE =
    &built_in_external_styles[7];
const struct ft_border_style* const FT_SOLID_ROUND_STYLE =
    &built_in_external_styles[8];
const struct ft_border_style* const FT_NICE_STYLE =
    &built_in_external_styles[9];
const struct ft_border_style* const FT_DOUBLE_STYLE =
    &built_in_external_styles[10];
const struct ft_border_style* const FT_DOUBLE2_STYLE =
    &built_in_external_styles[11];
const struct ft_border_style* const FT_BOLD_STYLE =
    &built_in_external_styles[12];
const struct ft_border_style* const FT_BOLD2_STYLE =
    &built_in_external_styles[13];
const struct ft_border_style* const FT_FRAME_STYLE =
    &built_in_external_styles[14];

static void set_border_props_for_props(f_table_properties_t* properties,
                                       const struct ft_border_style* style)
{
    if(style >= built_in_external_styles &&
       style < (built_in_external_styles + BUILT_IN_STYLES_SZ)) {
        size_t pos = (size_t)(style - built_in_external_styles);
        memcpy(&(properties->border_style),
               built_in_styles[pos],
               sizeof(struct fort_border_style));
        return;
    }

    const struct ft_border_chars* border_chs = &(style->border_chs);
    const struct ft_border_chars* header_border_chs =
        &(style->header_border_chs);

#define BOR_CHARS properties->border_style.border_chars
#define H_BOR_CHARS properties->border_style.header_border_chars
#define SEP_CHARS properties->border_style.separator_chars

    BOR_CHARS[TT_bip] = border_chs->top_border_ch;
    BOR_CHARS[IH_bip] = border_chs->separator_ch;
    BOR_CHARS[BB_bip] = border_chs->bottom_border_ch;
    BOR_CHARS[LL_bip] = BOR_CHARS[IV_bip] = BOR_CHARS[RR_bip] =
        border_chs->side_border_ch;

    BOR_CHARS[TL_bip] = BOR_CHARS[TV_bip] = BOR_CHARS[TR_bip] =
        border_chs->out_intersect_ch;
    BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = border_chs->out_intersect_ch;
    BOR_CHARS[BL_bip] = BOR_CHARS[BV_bip] = BOR_CHARS[BR_bip] =
        border_chs->out_intersect_ch;
    BOR_CHARS[II_bip] = border_chs->in_intersect_ch;

    BOR_CHARS[LI_bip] = BOR_CHARS[TI_bip] = BOR_CHARS[RI_bip] =
        BOR_CHARS[BI_bip] = border_chs->in_intersect_ch;

    if(strlen(border_chs->separator_ch) == 0 &&
       strlen(border_chs->in_intersect_ch) == 0) {
        BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = "\0";
    }

    H_BOR_CHARS[TT_bip] = header_border_chs->top_border_ch;
    H_BOR_CHARS[IH_bip] = header_border_chs->separator_ch;
    H_BOR_CHARS[BB_bip] = header_border_chs->bottom_border_ch;
    H_BOR_CHARS[LL_bip] = H_BOR_CHARS[IV_bip] = H_BOR_CHARS[RR_bip] =
        header_border_chs->side_border_ch;

    H_BOR_CHARS[TL_bip] = H_BOR_CHARS[TV_bip] = H_BOR_CHARS[TR_bip] =
        header_border_chs->out_intersect_ch;
    H_BOR_CHARS[LH_bip] = H_BOR_CHARS[RH_bip] =
        header_border_chs->out_intersect_ch;
    H_BOR_CHARS[BL_bip] = H_BOR_CHARS[BV_bip] = H_BOR_CHARS[BR_bip] =
        header_border_chs->out_intersect_ch;
    H_BOR_CHARS[II_bip] = header_border_chs->in_intersect_ch;

    H_BOR_CHARS[LI_bip] = H_BOR_CHARS[TI_bip] = H_BOR_CHARS[RI_bip] =
        H_BOR_CHARS[BI_bip] = header_border_chs->in_intersect_ch;

    if(strlen(header_border_chs->separator_ch) == 0 &&
       strlen(header_border_chs->in_intersect_ch) == 0) {
        BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = "\0";
    }

    SEP_CHARS[LH_sip] = SEP_CHARS[RH_sip] = SEP_CHARS[II_sip] =
        header_border_chs->out_intersect_ch;
    SEP_CHARS[TI_sip] = SEP_CHARS[BI_sip] = header_border_chs->out_intersect_ch;
    SEP_CHARS[IH_sip] = style->hor_separator_char;

#undef BOR_CHARS
#undef H_BOR_CHARS
#undef SEP_CHARS
}

int ft_set_default_border_style(const struct ft_border_style* style)
{
    set_border_props_for_props(&g_table_properties, style);
    return FT_SUCCESS;
}

int ft_set_border_style(ft_table_t* table, const struct ft_border_style* style)
{
    assert(table);
    if(table->properties == NULL) {
        table->properties = create_table_properties();
        if(table->properties == NULL)
            return FT_MEMORY_ERROR;
    }
    set_border_props_for_props(table->properties, style);
    return FT_SUCCESS;
}

int ft_set_cell_prop(
    ft_table_t* table, size_t row, size_t col, uint32_t property, int value)
{
    assert(table);

    if(table->properties == NULL) {
        table->properties = create_table_properties();
        if(table->properties == NULL)
            return FT_MEMORY_ERROR;
    }
    if(table->properties->cell_properties == NULL) {
        table->properties->cell_properties = create_cell_prop_container();
        if(table->properties->cell_properties == NULL) {
            return FT_GEN_ERROR;
        }
    }

    if(row == FT_CUR_ROW)
        row = table->cur_row;
    if(col == FT_CUR_COLUMN)
        col = table->cur_col;

    return set_cell_property(
        table->properties->cell_properties, row, col, property, value);
}

int ft_set_default_cell_prop(uint32_t property, int value)
{
    return set_default_cell_property(property, value);
}

int ft_set_default_tbl_prop(uint32_t property, int value)
{
    return set_default_entire_table_property(property, value);
}

int ft_set_tbl_prop(ft_table_t* table, uint32_t property, int value)
{
    assert(table);

    if(table->properties == NULL) {
        table->properties = create_table_properties();
        if(table->properties == NULL)
            return FT_MEMORY_ERROR;
    }
    return set_entire_table_property(table->properties, property, value);
}

void ft_set_memory_funcs(void* (*f_malloc)(size_t size),
                         void (*f_free)(void* ptr))
{
    set_memory_funcs(f_malloc, f_free);
}

const char* ft_strerror(int error_code)
{
    switch(error_code) {
    case FT_MEMORY_ERROR:
        return "Out of memory";
    case FT_GEN_ERROR:
        return "General error";
    case FT_EINVAL:
        return "Invalid argument";
    case FT_INTERN_ERROR:
        return "Internal libfort error";
    default:
        if(error_code < 0)
            return "Unknown error code";
        else
            return "Success";
    }
}

int ft_set_cell_span(ft_table_t* table, size_t row, size_t col, size_t hor_span)
{
    assert(table);
    if(hor_span < 2)
        return FT_EINVAL;

    if(row == FT_CUR_ROW)
        row = table->cur_row;
    if(row == FT_CUR_COLUMN)
        col = table->cur_col;

    f_row_t* row_p = get_row_and_create_if_not_exists(table, row);
    if(row_p == NULL)
        return FT_GEN_ERROR;

    return row_set_cell_span(row_p, col, hor_span);
}

#ifdef FT_HAVE_UTF8

int ft_u8nwrite(ft_table_t* table, size_t n, const void* cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_u8write_impl(table, cell_content);
    if(FT_IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for(i = 0; i < n; ++i) {
        const void* cell = va_arg(va, const void*);
        status = ft_u8write_impl(table, cell);
        if(FT_IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);

    return status;
}

int ft_u8nwrite_ln(ft_table_t* table, size_t n, const void* cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_u8write_impl(table, cell_content);
    if(FT_IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for(i = 0; i < n; ++i) {
        const void* cell = va_arg(va, const void*);
        status = ft_u8write_impl(table, cell);
        if(FT_IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);

    ft_ln(table);
    return status;
}

FT_PRINTF_ATTRIBUTE_FORMAT(2, 3)
int ft_u8printf(ft_table_t* table, const char* fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);

    struct f_string_view fmt_str;
    fmt_str.type = UTF8_BUF;
    fmt_str.u.cstr = fmt;
    int result = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
    va_end(va);
    return result;
}

FT_PRINTF_ATTRIBUTE_FORMAT(2, 3)
int ft_u8printf_ln(ft_table_t* table, const char* fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);

    struct f_string_view fmt_str;
    fmt_str.type = UTF8_BUF;
    fmt_str.u.cstr = fmt;
    int result = ft_row_printf_impl_(table, table->cur_row, &fmt_str, &va);
    if(result >= 0) {
        ft_ln(table);
    }
    va_end(va);
    return result;
}

const void* ft_to_u8string(const ft_table_t* table)
{
    return (const void*)ft_to_string_impl(table, UTF8_BUF);
}

void ft_set_u8strwid_func(int (*u8strwid)(const void* beg,
                                          const void* end,
                                          size_t* width))
{
    buffer_set_u8strwid_func(u8strwid);
}

#endif /* FT_HAVE_UTF8 */

/********************************************************
   End of file "fort_impl.c"
 ********************************************************/

/********************************************************
   Begin of file "fort_utils.c"
 ********************************************************/

/* #include "fort_utils.h" */ /* Commented by amalgamation script */
#ifdef FT_HAVE_WCHAR
#include <wchar.h>
#endif
#if defined(FT_HAVE_UTF8)
/* #include "utf8.h" */ /* Commented by amalgamation script */
#endif
/* #include "string_buffer.h" */ /* Commented by amalgamation script */

char g_col_separator = FORT_DEFAULT_COL_SEPARATOR;

/*****************************************************************************
 *               LIBFORT helpers
 *****************************************************************************/

#if defined(FT_GCC_COMPILER) || defined(FT_CLANG_COMPILER)
void* (*fort_malloc)(size_t size) = &malloc;
void (*fort_free)(void* ptr) = &free;
void* (*fort_calloc)(size_t nmemb, size_t size) = &calloc;
void* (*fort_realloc)(void* ptr, size_t size) = &realloc;
#else
static void* local_malloc(size_t size)
{
    return malloc(size);
}

static void local_free(void* ptr)
{
    free(ptr);
}

static void* local_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

static void* local_realloc(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

void* (*fort_malloc)(size_t size) = &local_malloc;
void (*fort_free)(void* ptr) = &local_free;
void* (*fort_calloc)(size_t nmemb, size_t size) = &local_calloc;
void* (*fort_realloc)(void* ptr, size_t size) = &local_realloc;
#endif

static void* custom_fort_calloc(size_t nmemb, size_t size)
{
    size_t total_size = nmemb * size;
    void* result = F_MALLOC(total_size);
    if(result != NULL)
        memset(result, 0, total_size);
    return result;
}

static void* custom_fort_realloc(void* ptr, size_t size)
{
    if(ptr == NULL)
        return F_MALLOC(size);
    if(size == 0) {
        F_FREE(ptr);
        return NULL;
    }

    void* new_chunk = F_MALLOC(size);
    if(new_chunk == NULL)
        return NULL;

    /*
     * In theory we should copy MIN(size, size allocated for ptr) bytes,
     * but this is rather dummy implementation so we don't care about it
     */
    memcpy(new_chunk, ptr, size);
    F_FREE(ptr);
    return new_chunk;
}

FT_INTERNAL
void set_memory_funcs(void* (*f_malloc)(size_t size), void (*f_free)(void* ptr))
{
    assert((f_malloc == NULL && f_free == NULL) /* Use std functions */
           || (f_malloc != NULL && f_free != NULL) /* Use custom functions */);

    if(f_malloc == NULL && f_free == NULL) {
#if defined(FT_GCC_COMPILER) || defined(FT_CLANG_COMPILER)
        fort_malloc = &malloc;
        fort_free = &free;
        fort_calloc = &calloc;
        fort_realloc = &realloc;
#else
        fort_malloc = &local_malloc;
        fort_free = &local_free;
        fort_calloc = &local_calloc;
        fort_realloc = &local_realloc;
#endif
    }
    else {
        fort_malloc = f_malloc;
        fort_free = f_free;
        fort_calloc = &custom_fort_calloc;
        fort_realloc = &custom_fort_realloc;
    }
}

FT_INTERNAL
char* fort_strdup(const char* str)
{
    if(str == NULL)
        return NULL;

    size_t sz = strlen(str);
    char* str_copy = (char*)F_MALLOC((sz + 1) * sizeof(char));
    if(str_copy == NULL)
        return NULL;

    strcpy(str_copy, str);
    return str_copy;
}

#if defined(FT_HAVE_WCHAR)
FT_INTERNAL
wchar_t* fort_wcsdup(const wchar_t* str)
{
    if(str == NULL)
        return NULL;

    size_t sz = wcslen(str);
    wchar_t* str_copy = (wchar_t*)F_MALLOC((sz + 1) * sizeof(wchar_t));
    if(str_copy == NULL)
        return NULL;

    wcscpy(str_copy, str);
    return str_copy;
}
#endif

static size_t columns_number_in_fmt_string(const char* fmt)
{
    size_t separator_counter = 0;
    const char* pos = fmt;
    while(1) {
        pos = strchr(pos, g_col_separator);
        if(pos == NULL)
            break;

        separator_counter++;
        ++pos;
    }
    return separator_counter + 1;
}

#if defined(FT_HAVE_WCHAR)
static size_t columns_number_in_fmt_wstring(const wchar_t* fmt)
{
    size_t separator_counter = 0;
    const wchar_t* pos = fmt;
    while(1) {
        pos = wcschr(pos, g_col_separator);
        if(pos == NULL)
            break;

        separator_counter++;
        ++pos;
    }
    return separator_counter + 1;
}
#endif

#if defined(FT_HAVE_UTF8)
static size_t columns_number_in_fmt_u8string(const void* fmt)
{
    size_t separator_counter = 0;
    const char* pos = (const char*)fmt;
    while(1) {
        pos = (const char*)utf8chr(pos, g_col_separator);
        if(pos == NULL)
            break;

        separator_counter++;
        ++pos;
    }
    return separator_counter + 1;
}
#endif

FT_INTERNAL
size_t number_of_columns_in_format_string(const f_string_view_t* fmt)
{
    switch(fmt->type) {
    case CHAR_BUF:
        return columns_number_in_fmt_string(fmt->u.cstr);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return columns_number_in_fmt_wstring(fmt->u.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return columns_number_in_fmt_u8string(fmt->u.u8str);
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
    }
    return 0;
}

FT_INTERNAL
size_t number_of_columns_in_format_buffer(const f_string_buffer_t* fmt)
{
    switch(fmt->type) {
    case CHAR_BUF:
        return columns_number_in_fmt_string(fmt->str.cstr);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return columns_number_in_fmt_wstring(fmt->str.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return columns_number_in_fmt_u8string(fmt->str.u8str);
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
    }
    return 0;
}

static int
snprint_n_strings_impl(char* buf, size_t length, size_t n, const char* str)
{
    size_t str_len = strlen(str);
    if(length <= n * str_len)
        return -1;

    if(n == 0)
        return 0;

    /* To ensure valid return value it is safely not print such big strings */
    if(n * str_len > INT_MAX)
        return -1;

    if(str_len == 0)
        return 0;

    int status = snprintf(buf, length, "%0*d", (int)(n * str_len), 0);
    if(status < 0)
        return status;

    size_t i = 0;
    for(i = 0; i < n; ++i) {
        const char* str_p = str;
        while(*str_p)
            *(buf++) = *(str_p++);
    }
    return (int)(n * str_len);
}

static int snprint_n_strings(f_conv_context_t* cntx, size_t n, const char* str)
{
    int w = snprint_n_strings_impl(cntx->u.buf, cntx->raw_avail, n, str);
    if(w >= 0) {
        cntx->u.buf += w;
        cntx->raw_avail -= w;
    }
    return w;
}

#if defined(FT_HAVE_WCHAR)
static int
wsnprint_n_string(wchar_t* buf, size_t length, size_t n, const char* str);
#endif

#if defined(FT_HAVE_UTF8)
static int
u8nprint_n_strings(void* buf, size_t length, size_t n, const void* str);
#endif

FT_INTERNAL
int print_n_strings(f_conv_context_t* cntx, size_t n, const char* str)
{
    int cod_w;
    int raw_written;

    switch(cntx->b_type) {
    case CHAR_BUF:
        raw_written = snprint_n_strings(cntx, n, str);
        cod_w = raw_written;
        return cod_w;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        cod_w = wsnprint_n_string(cntx->u.wbuf, cntx->raw_avail, n, str);
        if(cod_w < 0)
            return cod_w;
        raw_written = sizeof(wchar_t) * cod_w;

        cntx->u.buf += raw_written;
        cntx->raw_avail -= raw_written;
        return cod_w;
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        /* Everying is very strange and differs with W_CHAR_BUF */
        raw_written = u8nprint_n_strings(cntx->u.buf, cntx->raw_avail, n, str);
        if(raw_written < 0) {
            fprintf(stderr, " raw_written = %d\n", raw_written);
            return raw_written;
        }

        cntx->u.buf += raw_written;
        cntx->raw_avail -= raw_written;
        return utf8len(str) * n;
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
        return -1;
    }
}

FT_INTERNAL
int ft_nprint(f_conv_context_t* cntx, const char* str, size_t strlen)
{
    if(cntx->raw_avail + 1 /* for 0 */ < strlen)
        return -1;

    memcpy(cntx->u.buf, str, strlen);
    cntx->u.buf += strlen;
    cntx->raw_avail -= strlen;
    *cntx->u.buf = '\0'; /* Do we need this ? */
    return strlen;
}

#ifdef FT_HAVE_WCHAR
int ft_nwprint(f_conv_context_t* cntx, const wchar_t* str, size_t strlen)
{
    if(cntx->raw_avail + 1 /* for 0 */ < strlen)
        return -1;

    size_t raw_len = strlen * sizeof(wchar_t);

    memcpy(cntx->u.buf, str, raw_len);
    cntx->u.buf += raw_len;
    cntx->raw_avail -= raw_len;

    /* Do we need this ? */
    wchar_t end_of_string = L'\0';
    memcpy(cntx->u.buf, &end_of_string, sizeof(wchar_t));
    return strlen;
}
#endif /* FT_HAVE_WCHAR */

#ifdef FT_HAVE_UTF8
FT_INTERNAL
int ft_nu8print(f_conv_context_t* cntx, const void* beg, const void* end)
{
    const char* bc = (const char*)beg;
    const char* ec = (const char*)end;
    size_t raw_len = ec - bc;
    if(cntx->raw_avail + 1 < raw_len)
        return -1;

    memcpy(cntx->u.buf, beg, raw_len);
    cntx->u.buf += raw_len;
    cntx->raw_avail -= raw_len;
    *(cntx->u.buf) = '\0'; /* Do we need this ? */
    return raw_len;        /* what return here ? */
}
#endif /* FT_HAVE_UTF8 */

#if defined(FT_HAVE_WCHAR)
#define WCS_SIZE 64

static int
wsnprint_n_string(wchar_t* buf, size_t length, size_t n, const char* str)
{
    size_t str_len = strlen(str);

    /* note: maybe it's, better to return -1 in case of multibyte character
     * strings (not sure this case is done correctly).
     */
    if(str_len > 1) {
        const unsigned char* p = (const unsigned char*)str;
        while(*p) {
            if(*p <= 127)
                p++;
            else {
                wchar_t wcs[WCS_SIZE];
                const char* ptr = str;
                size_t wcs_len;
                mbstate_t mbst;
                memset(&mbst, 0, sizeof(mbst));
                wcs_len = mbsrtowcs(wcs, (const char**)&ptr, WCS_SIZE, &mbst);
                /* for simplicity */
                if((wcs_len == (size_t)-1) || wcs_len > 1) {
                    return -1;
                }
                else {
                    wcs[wcs_len] = L'\0';
                    size_t k = n;
                    while(k) {
                        *buf = *wcs;
                        ++buf;
                        --k;
                    }
                    buf[n] = L'\0';
                    return (int)n;
                }
            }
        }
    }

    if(length <= n * str_len)
        return -1;

    if(n == 0)
        return 0;

    /* To ensure valid return value it is safely not print such big strings */
    if(n * str_len > INT_MAX)
        return -1;

    if(str_len == 0)
        return 0;

    int status = swprintf(buf, length, L"%0*d", (int)(n * str_len), 0);
    if(status < 0)
        return status;

    size_t i = 0;
    for(i = 0; i < n; ++i) {
        const char* str_p = str;
        while(*str_p)
            *(buf++) = (wchar_t) * (str_p++);
    }
    return (int)(n * str_len);
}
#endif

#if defined(FT_HAVE_UTF8)
static int
u8nprint_n_strings(void* buf, size_t length, size_t n, const void* str)
{
    size_t str_size =
        utf8size(str) - 1; /* str_size - raw size in bytes, excluding \0 */
    if(length <= n * str_size)
        return -1;

    if(n == 0)
        return 0;

    /* To ensure valid return value it is safely not print such big strings */
    if(n * str_size > INT_MAX)
        return -1;

    if(str_size == 0)
        return 0;

    size_t i = n;
    while(i) {
        memcpy(buf, str, str_size);
        buf = (char*)buf + str_size;
        --i;
    }
    *(char*)buf = '\0';
    return (int)(n * str_size);
}
#endif

/********************************************************
   End of file "fort_utils.c"
 ********************************************************/

/********************************************************
   Begin of file "properties.c"
 ********************************************************/

/* #include "fort_utils.h" */ /* Commented by amalgamation script */
#include <assert.h>
/* #include "properties.h" */ /* Commented by amalgamation script */
/* #include "vector.h" */     /* Commented by amalgamation script */

#define FT_RESET_COLOR "\033[0m"

static const char* fg_colors[] = {
    "",         "\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m",
    "\033[35m", "\033[36m", "\033[37m", "\033[90m", "\033[91m", "\033[92m",
    "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[97m",
};

static const char* bg_colors[] = {
    "",          "\033[40m",  "\033[41m",  "\033[42m",  "\033[43m",
    "\033[44m",  "\033[45m",  "\033[46m",  "\033[47m",  "\033[100m",
    "\033[101m", "\033[102m", "\033[103m", "\033[104m", "\033[105m",
    "\033[106m", "\033[107m",
};

static const char* text_styles[] = {
    "",        "\033[1m", "\033[2m", "\033[3m",
    "\033[4m", "\033[5m", "\033[7m", "\033[8m",
};

#define UNIVERSAL_RESET_TAG "\033[0m"

static const size_t n_fg_colors = sizeof(fg_colors) / sizeof(fg_colors[0]);
static const size_t n_bg_colors = sizeof(bg_colors) / sizeof(bg_colors[0]);
static const size_t n_styles = sizeof(text_styles) / sizeof(text_styles[0]);

void get_style_tag_for_cell(const f_table_properties_t* props,
                            size_t row,
                            size_t col,
                            char* style_tag,
                            size_t sz)
{
    (void)sz;
    size_t i = 0;

    unsigned bg_color_number = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CELL_BG_COLOR);
    unsigned text_style = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CELL_TEXT_STYLE);

    style_tag[0] = '\0';

    if(text_style < (1U << n_styles)) {
        for(i = 0; i < n_styles; ++i) {
            if(text_style & (1 << i)) {
                strcat(style_tag, text_styles[i]);
            }
        }
    }
    else {
        goto error;
    }

    if(bg_color_number < n_bg_colors) {
        strcat(style_tag, bg_colors[bg_color_number]);
    }
    else {
        goto error;
    }

    return;

error:
    /* shouldn't be here */
    assert(0);
    style_tag[0] = '\0';
    return;
}

void get_reset_style_tag_for_cell(const f_table_properties_t* props,
                                  size_t row,
                                  size_t col,
                                  char* reset_style_tag,
                                  size_t sz)
{
    (void)sz;
    size_t i = 0;

    unsigned bg_color_number = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CELL_BG_COLOR);
    unsigned text_style = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CELL_TEXT_STYLE);

    reset_style_tag[0] = '\0';

    if(text_style < (1U << n_styles)) {
        for(i = 0; i < n_styles; ++i) {
            if(text_style & (1 << i)) {
                if(i != 0) // FT_TSTYLE_DEFAULT
                    goto reset_style;
            }
        }
    }
    else {
        goto error;
    }

    if(bg_color_number < n_bg_colors) {
        if(bg_color_number)
            goto reset_style;
    }
    else {
        goto error;
    }

    return;

reset_style:
    strcat(reset_style_tag, UNIVERSAL_RESET_TAG);
    return;

error:
    /* shouldn't be here */
    assert(0);
    reset_style_tag[0] = '\0';
    return;
}

void get_style_tag_for_content(const f_table_properties_t* props,
                               size_t row,
                               size_t col,
                               char* style_tag,
                               size_t sz)
{
    (void)sz;
    size_t i = 0;

    unsigned text_style = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CONT_TEXT_STYLE);
    unsigned fg_color_number = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CONT_FG_COLOR);
    unsigned bg_color_number = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CONT_BG_COLOR);

    style_tag[0] = '\0';

    if(text_style < (1U << n_styles)) {
        for(i = 0; i < n_styles; ++i) {
            if(text_style & (1 << i)) {
                strcat(style_tag, text_styles[i]);
            }
        }
    }
    else {
        goto error;
    }

    if(fg_color_number < n_fg_colors) {
        if(fg_color_number)
            strcat(style_tag, fg_colors[fg_color_number]);
    }
    else {
        goto error;
    }

    if(bg_color_number < n_bg_colors) {
        strcat(style_tag, bg_colors[bg_color_number]);
    }
    else {
        goto error;
    }

    return;

error:
    /* shouldn't be here */
    assert(0);
    style_tag[0] = '\0';
    return;
}

void get_reset_style_tag_for_content(const f_table_properties_t* props,
                                     size_t row,
                                     size_t col,
                                     char* reset_style_tag,
                                     size_t sz)
{
    (void)sz;
    size_t i = 0;
    size_t len = 0;

    unsigned text_style = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CONT_TEXT_STYLE);
    unsigned fg_color_number = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CONT_FG_COLOR);
    unsigned bg_color_number = get_cell_property_hierarchically(
        props, row, col, FT_CPROP_CONT_BG_COLOR);

    reset_style_tag[0] = '\0';

    if(text_style < (1U << n_styles)) {
        for(i = 0; i < n_styles; ++i) {
            if(text_style & (1 << i)) {
                if(i != 0) // FT_TSTYLE_DEFAULT
                    goto reset_style;
            }
        }
    }
    else {
        goto error;
    }

    if(fg_color_number < n_fg_colors) {
        if(fg_color_number)
            goto reset_style;
    }
    else {
        goto error;
    }

    if(bg_color_number < n_bg_colors) {
        if(bg_color_number)
            goto reset_style;
    }
    else {
        goto error;
    }

    return;

reset_style:
    strcat(reset_style_tag, UNIVERSAL_RESET_TAG);
    len = strlen(reset_style_tag);
    get_style_tag_for_cell(props, row, col, reset_style_tag + len, sz - len);
    return;

error:
    /* shouldn't be here */
    assert(0);
    reset_style_tag[0] = '\0';
    return;
}

static struct f_cell_props g_default_cell_properties = {
    FT_ANY_ROW,    /* cell_row */
    FT_ANY_COLUMN, /* cell_col */

    /* properties_flags */
    FT_CPROP_MIN_WIDTH | FT_CPROP_TEXT_ALIGN | FT_CPROP_TOP_PADDING |
        FT_CPROP_BOTTOM_PADDING | FT_CPROP_LEFT_PADDING |
        FT_CPROP_RIGHT_PADDING | FT_CPROP_EMPTY_STR_HEIGHT |
        FT_CPROP_CONT_FG_COLOR | FT_CPROP_CELL_BG_COLOR |
        FT_CPROP_CONT_BG_COLOR | FT_CPROP_CELL_TEXT_STYLE |
        FT_CPROP_CONT_TEXT_STYLE,

    0,               /* col_min_width */
    FT_ALIGNED_LEFT, /* align */
    0,               /* cell_padding_top         */
    0,               /* cell_padding_bottom      */
    1,               /* cell_padding_left        */
    1,               /* cell_padding_right       */
    1,               /* cell_empty_string_height */

    FT_ROW_COMMON,     /* row_type */
    FT_COLOR_DEFAULT,  /* content_fg_color_number */
    FT_COLOR_DEFAULT,  /* content_bg_color_number */
    FT_COLOR_DEFAULT,  /* cell_bg_color_number */
    FT_TSTYLE_DEFAULT, /* cell_text_style */
    FT_TSTYLE_DEFAULT, /* content_text_style */
};

static int
get_prop_value_if_exists_otherwise_default(const struct f_cell_props* cell_opts,
                                           uint32_t property)
{
    if(cell_opts == NULL ||
       !PROP_IS_SET(cell_opts->properties_flags, property)) {
        cell_opts = &g_default_cell_properties;
    }

    switch(property) {
    case FT_CPROP_MIN_WIDTH:
        return cell_opts->col_min_width;
    case FT_CPROP_TEXT_ALIGN:
        return cell_opts->align;
    case FT_CPROP_TOP_PADDING:
        return cell_opts->cell_padding_top;
    case FT_CPROP_BOTTOM_PADDING:
        return cell_opts->cell_padding_bottom;
    case FT_CPROP_LEFT_PADDING:
        return cell_opts->cell_padding_left;
    case FT_CPROP_RIGHT_PADDING:
        return cell_opts->cell_padding_right;
    case FT_CPROP_EMPTY_STR_HEIGHT:
        return cell_opts->cell_empty_string_height;
    case FT_CPROP_ROW_TYPE:
        return cell_opts->row_type;
    case FT_CPROP_CONT_FG_COLOR:
        return cell_opts->content_fg_color_number;
    case FT_CPROP_CONT_BG_COLOR:
        return cell_opts->content_bg_color_number;
    case FT_CPROP_CELL_BG_COLOR:
        return cell_opts->cell_bg_color_number;
    case FT_CPROP_CELL_TEXT_STYLE:
        return cell_opts->cell_text_style;
    case FT_CPROP_CONT_TEXT_STYLE:
        return cell_opts->content_text_style;
    default:
        /* todo: implement later */
        exit(333);
    }
}

FT_INTERNAL
f_cell_prop_container_t* create_cell_prop_container(void)
{
    f_cell_prop_container_t* ret =
        create_vector(sizeof(f_cell_props_t), DEFAULT_VECTOR_CAPACITY);
    return ret;
}

FT_INTERNAL
void destroy_cell_prop_container(f_cell_prop_container_t* cont)
{
    if(cont)
        destroy_vector(cont);
}

FT_INTERNAL
const f_cell_props_t*
cget_cell_prop(const f_cell_prop_container_t* cont, size_t row, size_t col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    size_t i = 0;
    for(i = 0; i < sz; ++i) {
        const f_cell_props_t* opt = &VECTOR_AT_C(cont, i, const f_cell_props_t);
        if(opt->cell_row == row && opt->cell_col == col)
            return opt;
    }
    return NULL;
}

FT_INTERNAL
f_cell_props_t* get_cell_prop_and_create_if_not_exists(
    f_cell_prop_container_t* cont, size_t row, size_t col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    size_t i = 0;
    for(i = 0; i < sz; ++i) {
        f_cell_props_t* opt = &VECTOR_AT(cont, i, f_cell_props_t);
        if(opt->cell_row == row && opt->cell_col == col)
            return opt;
    }

    f_cell_props_t opt;
    if(row == FT_ANY_ROW && col == FT_ANY_COLUMN)
        memcpy(&opt, &g_default_cell_properties, sizeof(f_cell_props_t));
    else
        memset(&opt, 0, sizeof(f_cell_props_t));

    opt.cell_row = row;
    opt.cell_col = col;
    if(FT_IS_SUCCESS(vector_push(cont, &opt))) {
        return &VECTOR_AT(cont, sz, f_cell_props_t);
    }

    return NULL;
}

FT_INTERNAL
int get_cell_property_hierarchically(const f_table_properties_t* propertiess,
                                     size_t row,
                                     size_t column,
                                     uint32_t property)
{
    assert(propertiess);
    size_t row_origin = row;

    const f_cell_props_t* opt = NULL;
    if(propertiess->cell_properties != NULL) {
        while(1) {
            opt = cget_cell_prop(propertiess->cell_properties, row, column);
            if(opt != NULL && PROP_IS_SET(opt->properties_flags, property))
                break;

            if(row != FT_ANY_ROW && column != FT_ANY_COLUMN) {
                row = FT_ANY_ROW;
                continue;
            }
            else if(row == FT_ANY_ROW && column != FT_ANY_COLUMN) {
                row = row_origin;
                column = FT_ANY_COLUMN;
                continue;
            }
            else if(row != FT_ANY_ROW && column == FT_ANY_COLUMN) {
                row = FT_ANY_ROW;
                column = FT_ANY_COLUMN;
                continue;
            }

            opt = NULL;
            break;
        }
    }

    return get_prop_value_if_exists_otherwise_default(opt, property);
}

static f_status
set_cell_property_impl(f_cell_props_t* opt, uint32_t property, int value)
{
    assert(opt);

    PROP_SET(opt->properties_flags, property);
    if(PROP_IS_SET(property, FT_CPROP_MIN_WIDTH)) {
        CHECK_NOT_NEGATIVE(value);
        opt->col_min_width = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_TEXT_ALIGN)) {
        opt->align = (enum ft_text_alignment)value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_TOP_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_top = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_BOTTOM_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_bottom = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_LEFT_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_left = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_RIGHT_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_right = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_EMPTY_STR_HEIGHT)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_empty_string_height = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_ROW_TYPE)) {
        opt->row_type = (enum ft_row_type)value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_CONT_FG_COLOR)) {
        opt->content_fg_color_number = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_CONT_BG_COLOR)) {
        opt->content_bg_color_number = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_CELL_BG_COLOR)) {
        opt->cell_bg_color_number = value;
    }
    else if(PROP_IS_SET(property, FT_CPROP_CELL_TEXT_STYLE)) {
        enum ft_text_style v = (enum ft_text_style)value;
        if(v == FT_TSTYLE_DEFAULT) {
            opt->cell_text_style = FT_TSTYLE_DEFAULT;
        }
        else {
            opt->cell_text_style =
                (enum ft_text_style)(opt->cell_text_style | v);
        }
    }
    else if(PROP_IS_SET(property, FT_CPROP_CONT_TEXT_STYLE)) {
        enum ft_text_style v = (enum ft_text_style)value;
        if(v == FT_TSTYLE_DEFAULT) {
            opt->content_text_style = v;
        }
        else {
            opt->content_text_style =
                (enum ft_text_style)(opt->content_text_style | v);
        }
    }

    return FT_SUCCESS;

fort_fail:
    return FT_EINVAL;
}

FT_INTERNAL
f_status set_cell_property(f_cell_prop_container_t* cont,
                           size_t row,
                           size_t col,
                           uint32_t property,
                           int value)
{
    f_cell_props_t* opt =
        get_cell_prop_and_create_if_not_exists(cont, row, col);
    if(opt == NULL)
        return FT_GEN_ERROR;

    return set_cell_property_impl(opt, property, value);
    /*
    PROP_SET(opt->propertiess, property);
    if (PROP_IS_SET(property, FT_CPROP_MIN_WIDTH)) {
        opt->col_min_width = value;
    } else if (PROP_IS_SET(property, FT_CPROP_TEXT_ALIGN)) {
        opt->align = value;
    }

    return FT_SUCCESS;
    */
}

FT_INTERNAL
f_status set_default_cell_property(uint32_t property, int value)
{
    return set_cell_property_impl(&g_default_cell_properties, property, value);
}

#define BASIC_STYLE                                                            \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "+",  "-", "+", "+", "|", "|", "|", "\0", "\0", "\0",              \
            "\0", "+", "-", "+", "+", "+", "+", "+",  "+",                     \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "+", "-", "+", "+", "|", "|", "|", "+", "-", "+",              \
                "+", "+", "-", "+", "+", "+", "+", "+", "+",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "+", "-", "+", "+", "+", "+",                                  \
            },                                                                 \
    }

#define BASIC2_STYLE                                                           \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "+", "-", "+", "+", "|", "|", "|", "+", "-", "+",                  \
            "+", "+", "-", "+", "+", "+", "+", "+", "+",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "+", "-", "+", "+", "|", "|", "|", "+", "-", "+",              \
                "+", "+", "-", "+", "+", "+", "+", "+", "+",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "+", "-", "+", "+", "+", "+",                                  \
            },                                                                 \
    }

#define SIMPLE_STYLE                                                           \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "\0", "\0", "\0", "\0", "\0", " ",  "\0", "\0", "\0", "\0",        \
            "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",              \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "\0", "\0", "\0", "\0", "\0", " ", "\0", "\0", "-", " ",       \
                "\0", "\0", " ",  " ",  "\0", " ", "-",  " ",  "-",            \
            }, /* separator_chars */                                           \
            {                                                                  \
                "\0", "-", " ", "\0", " ", " ",                                \
            },                                                                 \
    }

#define PLAIN_STYLE                                                            \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "\0", "\0", "\0", "\0", "\0", " ",  "\0", "\0", "\0", "\0",        \
            "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",              \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "\0", "-",  "-", "\0", "\0", " ", "\0", "\0", "-", "-",        \
                "\0", "\0", "-", "-",  "\0", " ", "-",  " ",  "-",             \
            }, /* separator_chars */                                           \
            {                                                                  \
                "\0", "-", "-", "\0", "-", "-",                                \
            },                                                                 \
    }

#define DOT_STYLE                                                              \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            ".",  ".", ".", ".", ":", ":", ":", "\0", "\0", "\0",              \
            "\0", ":", ".", ":", ":", "+", ":", "+",  ":",                     \
        }, /* header_border_chars */                                           \
            {                                                                  \
                ".", ".", ".", ".", ":", ":", ":", ":", ".", ":",              \
                ":", ":", ".", ":", ":", "+", ".", "+", ".",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                ":", ".", ":", ":", ":", ":",                                  \
            },                                                                 \
    }

#define EMPTY_STYLE                                                            \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",        \
            "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",              \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",    \
                "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",          \
            }, /* separator_chars */                                           \
            {                                                                  \
                "\0", " ", "\0 ", "\0", "\0", "\0",                            \
            },                                                                 \
    }

#define EMPTY2_STYLE                                                           \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            " ",  " ", " ", " ", " ", " ", " ", "\0", "\0", "\0",              \
            "\0", " ", " ", " ", " ", " ", " ", " ",  " ",                     \
        }, /* header_border_chars */                                           \
            {                                                                  \
                " ",  " ", " ", " ", " ", " ", " ", "\0", "\0", "\0",          \
                "\0", " ", " ", " ", " ", " ", " ", " ",  " ",                 \
            }, /* separator_chars */                                           \
            {                                                                  \
                " ", " ", " ", " ", " ", " ",                                  \
            },                                                                 \
    }

#define SOLID_STYLE                                                            \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "┌", "─", "┬", "┐", "│", "│", "│", "",  "",  "",                   \
            "",  "└", "─", "┴", "┘", "│", "─", "│", "─",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "┌", "─", "┬", "┐", "│", "│", "│", "├", "─", "┼",              \
                "┤", "└", "─", "┴", "┘", "┼", "┬", "┼", "┴",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "├", "─", "┼", "┤", "┬", "┴",                                  \
            },                                                                 \
    }

#define SOLID_ROUND_STYLE                                                      \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "╭", "─", "┬", "╮", "│", "│", "│", "",  "",  "",                   \
            "",  "╰", "─", "┴", "╯", "│", "─", "│", "─",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "╭", "─", "┬", "╮", "│", "│", "│", "├", "─", "┼",              \
                "┤", "╰", "─", "┴", "╯", "┼", "┬", "┼", "┴",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "├", "─", "┼", "┤", "┬", "┴",                                  \
            },                                                                 \
    }

#define NICE_STYLE                                                             \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "╔", "═", "╦", "╗", "║", "║", "║", "",  "",  "",                   \
            "",  "╚", "═", "╩", "╝", "┣", "┻", "┣", "┳",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "╔", "═", "╦", "╗", "║", "║", "║", "╠", "═", "╬",              \
                "╣", "╚", "═", "╩", "╝", "┣", "╦", "┣", "╩",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "╟", "─", "╫", "╢", "╥", "╨",                                  \
            },                                                                 \
    }

#define DOUBLE_STYLE                                                           \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "╔", "═", "╦", "╗", "║", "║", "║", "",  "",  "",                   \
            "",  "╚", "═", "╩", "╝", "┣", "┻", "┣", "┳",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "╔", "═", "╦", "╗", "║", "║", "║", "╠", "═", "╬",              \
                "╣", "╚", "═", "╩", "╝", "┣", "╦", "┣", "╩",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "╠", "═", "╬", "╣", "╦", "╩",                                  \
            },                                                                 \
    }

#define DOUBLE2_STYLE                                                          \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "╔", "═", "╤", "╗", "║", "│", "║", "╟", "─", "┼",                  \
            "╢", "╚", "═", "╧", "╝", "├", "┬", "┤", "┴",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "╔", "═", "╤", "╗", "║", "│", "║", "╠", "═", "╪",              \
                "╣", "╚", "═", "╧", "╝", "├", "╤", "┤", "╧",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "╠", "═", "╪", "╣", "╤", "╧",                                  \
            },                                                                 \
    }

#define BOLD_STYLE                                                             \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "┏", "━", "┳", "┓", "┃", "┃", "┃", "",  "",  "",                   \
            "",  "┗", "━", "┻", "┛", "┣", "┻", "┣", "┳",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "┏", "━", "┳", "┓", "┃", "┃", "┃", "┣", "━", "╋",              \
                "┫", "┗", "━", "┻", "┛", "┣", "┳", "┣", "┻",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "┣", "━", "╋", "┫", "┳", "┻",                                  \
            },                                                                 \
    }

#define BOLD2_STYLE                                                            \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "┏", "━", "┯", "┓", "┃", "│", "┃", "┠", "─", "┼",                  \
            "┨", "┗", "━", "┷", "┛", "┣", "┬", "┣", "┴",                       \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "┏", "━", "┯", "┓", "┃", "│", "┃", "┣", "━", "┿",              \
                "┫", "┗", "━", "┷", "┛", "┣", "┯", "┣", "┷",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "┣", "━", "┿", "┫", "┯", "┷",                                  \
            },                                                                 \
    }

#define FRAME_STYLE                                                            \
    {                                                                          \
        /* border_chars */                                                     \
        {                                                                      \
            "▛", "▀", "▀", "▜", "▌", "┃", "▐", "",  "", "",                    \
            "",  "▙", "▄", "▄", "▟", "┣", "━", "┣", "━"                      \
        }, /* header_border_chars */                                           \
            {                                                                  \
                "▛", "▀", "▀", "▜", "▌", "┃", "▐", "▌", "━", "╋",              \
                "▐", "▙", "▄", "▄", "▟", "┣", "━", "┣", "━",                   \
            }, /* separator_chars */                                           \
            {                                                                  \
                "▌", "━", "╋", "▐", "╋", "╋",                                  \
            },                                                                 \
    }

struct fort_border_style FORT_BASIC_STYLE = BASIC_STYLE;
struct fort_border_style FORT_BASIC2_STYLE = BASIC2_STYLE;
struct fort_border_style FORT_SIMPLE_STYLE = SIMPLE_STYLE;
struct fort_border_style FORT_PLAIN_STYLE = PLAIN_STYLE;
struct fort_border_style FORT_DOT_STYLE = DOT_STYLE;
struct fort_border_style FORT_EMPTY_STYLE = EMPTY_STYLE;
struct fort_border_style FORT_EMPTY2_STYLE = EMPTY2_STYLE;
struct fort_border_style FORT_SOLID_STYLE = SOLID_STYLE;
struct fort_border_style FORT_SOLID_ROUND_STYLE = SOLID_ROUND_STYLE;
struct fort_border_style FORT_NICE_STYLE = NICE_STYLE;
struct fort_border_style FORT_DOUBLE_STYLE = DOUBLE_STYLE;
struct fort_border_style FORT_DOUBLE2_STYLE = DOUBLE2_STYLE;
struct fort_border_style FORT_BOLD_STYLE = BOLD_STYLE;
struct fort_border_style FORT_BOLD2_STYLE = BOLD2_STYLE;
struct fort_border_style FORT_FRAME_STYLE = FRAME_STYLE;

fort_entire_table_properties_t g_entire_table_properties = {
    0,                   /* left_margin */
    0,                   /* top_margin */
    0,                   /* right_margin */
    0,                   /* bottom_margin */
    FT_STRATEGY_REPLACE, /* add_strategy */
};

static f_status set_entire_table_property_internal(
    fort_entire_table_properties_t* properties, uint32_t property, int value)
{
    assert(properties);
    CHECK_NOT_NEGATIVE(value);
    if(PROP_IS_SET(property, FT_TPROP_LEFT_MARGIN)) {
        properties->left_margin = value;
    }
    else if(PROP_IS_SET(property, FT_TPROP_TOP_MARGIN)) {
        properties->top_margin = value;
    }
    else if(PROP_IS_SET(property, FT_TPROP_RIGHT_MARGIN)) {
        properties->right_margin = value;
    }
    else if(PROP_IS_SET(property, FT_TPROP_BOTTOM_MARGIN)) {
        properties->bottom_margin = value;
    }
    else if(PROP_IS_SET(property, FT_TPROP_ADDING_STRATEGY)) {
        properties->add_strategy = (enum ft_adding_strategy)value;
    }
    else {
        return FT_EINVAL;
    }
    return FT_SUCCESS;

fort_fail:
    return FT_EINVAL;
}

FT_INTERNAL
f_status set_entire_table_property(f_table_properties_t* table_properties,
                                   uint32_t property,
                                   int value)
{
    assert(table_properties);
    return set_entire_table_property_internal(
        &table_properties->entire_table_properties, property, value);
}

FT_INTERNAL
f_status set_default_entire_table_property(uint32_t property, int value)
{
    return set_entire_table_property_internal(
        &g_entire_table_properties, property, value);
}

FT_INTERNAL
size_t max_border_elem_strlen(struct f_table_properties* properties)
{
    assert(properties);
    size_t result = 1;
    int i = 0;
    for(i = 0; i < BORDER_ITEM_POS_SIZE; ++i) {
        result = MAX(result, strlen(properties->border_style.border_chars[i]));
    }

    for(i = 0; i < BORDER_ITEM_POS_SIZE; ++i) {
        result = MAX(result,
                     strlen(properties->border_style.header_border_chars[i]));
    }

    for(i = 0; i < SEPARATOR_ITEM_POS_SIZE; ++i) {
        result =
            MAX(result, strlen(properties->border_style.separator_chars[i]));
    }
    return result;
}

f_table_properties_t g_table_properties = {
    /* border_style */
    BASIC_STYLE,
    NULL, /* cell_properties */
    /* entire_table_properties */
    {
        0,                   /* left_margin */
        0,                   /* top_margin */
        0,                   /* right_margin */
        0,                   /* bottom_margin */
        FT_STRATEGY_REPLACE, /* add_strategy */
    }
};

FT_INTERNAL
f_table_properties_t* create_table_properties(void)
{
    f_table_properties_t* properties =
        (f_table_properties_t*)F_CALLOC(sizeof(f_table_properties_t), 1);
    if(properties == NULL) {
        return NULL;
    }
    memcpy(properties, &g_table_properties, sizeof(f_table_properties_t));
    properties->cell_properties = create_cell_prop_container();
    if(properties->cell_properties == NULL) {
        destroy_table_properties(properties);
        return NULL;
    }
    memcpy(&properties->entire_table_properties,
           &g_entire_table_properties,
           sizeof(fort_entire_table_properties_t));
    return properties;
}

FT_INTERNAL
void destroy_table_properties(f_table_properties_t* properties)
{
    if(properties == NULL)
        return;

    if(properties->cell_properties != NULL) {
        destroy_cell_prop_container(properties->cell_properties);
    }
    F_FREE(properties);
}

static f_cell_prop_container_t*
copy_cell_properties(f_cell_prop_container_t* cont)
{
    f_cell_prop_container_t* result = create_cell_prop_container();
    if(result == NULL)
        return NULL;

    size_t i = 0;
    size_t sz = vector_size(cont);
    for(i = 0; i < sz; ++i) {
        f_cell_props_t* opt = (f_cell_props_t*)vector_at(cont, i);
        if(FT_IS_ERROR(vector_push(result, opt))) {
            destroy_cell_prop_container(result);
            return NULL;
        }
    }
    return result;
}

FT_INTERNAL
f_table_properties_t*
copy_table_properties(const f_table_properties_t* properties)
{
    f_table_properties_t* new_opt = create_table_properties();
    if(new_opt == NULL)
        return NULL;

    destroy_vector(new_opt->cell_properties);
    new_opt->cell_properties =
        copy_cell_properties(properties->cell_properties);
    if(new_opt->cell_properties == NULL) {
        destroy_table_properties(new_opt);
        return NULL;
    }

    memcpy(&new_opt->border_style,
           &properties->border_style,
           sizeof(struct fort_border_style));
    memcpy(&new_opt->entire_table_properties,
           &properties->entire_table_properties,
           sizeof(fort_entire_table_properties_t));

    return new_opt;
}

/********************************************************
   End of file "properties.c"
 ********************************************************/

/********************************************************
   Begin of file "row.c"
 ********************************************************/

#include <assert.h>
#include <ctype.h>
/* #include "row.h" */           /* Commented by amalgamation script */
/* #include "cell.h" */          /* Commented by amalgamation script */
/* #include "string_buffer.h" */ /* Commented by amalgamation script */
/* #include "vector.h" */        /* Commented by amalgamation script */

struct f_row
{
    f_vector_t* cells;
};

static f_row_t* create_row_impl(f_vector_t* cells)
{
    f_row_t* row = (f_row_t*)F_CALLOC(1, sizeof(f_row_t));
    if(row == NULL)
        return NULL;
    if(cells) {
        row->cells = cells;
    }
    else {
        row->cells = create_vector(sizeof(f_cell_t*), DEFAULT_VECTOR_CAPACITY);
        if(row->cells == NULL) {
            F_FREE(row);
            return NULL;
        }
    }
    return row;
}

FT_INTERNAL
f_row_t* create_row(void)
{
    return create_row_impl(NULL);
}

static void destroy_each_cell(f_vector_t* cells)
{
    size_t i = 0;
    size_t cells_n = vector_size(cells);
    for(i = 0; i < cells_n; ++i) {
        f_cell_t* cell = VECTOR_AT(cells, i, f_cell_t*);
        destroy_cell(cell);
    }
}

FT_INTERNAL
void destroy_row(f_row_t* row)
{
    if(row == NULL)
        return;

    if(row->cells) {
        destroy_each_cell(row->cells);
        destroy_vector(row->cells);
    }

    F_FREE(row);
}

FT_INTERNAL
f_row_t* copy_row(f_row_t* row)
{
    assert(row);
    f_row_t* result = create_row();
    if(result == NULL)
        return NULL;

    size_t i = 0;
    size_t cols_n = vector_size(row->cells);
    for(i = 0; i < cols_n; ++i) {
        f_cell_t* cell = VECTOR_AT(row->cells, i, f_cell_t*);
        f_cell_t* new_cell = copy_cell(cell);
        if(new_cell == NULL) {
            destroy_row(result);
            return NULL;
        }
        vector_push(result->cells, &new_cell);
    }

    return result;
}

FT_INTERNAL
f_row_t* split_row(f_row_t* row, size_t pos)
{
    assert(row);

    f_vector_t* cells = vector_split(row->cells, pos);
    if(!cells)
        return NULL;
    f_row_t* tail = create_row_impl(cells);
    if(!tail) {
        destroy_each_cell(cells);
        destroy_vector(cells);
    }
    return tail;
}

FT_INTERNAL
int ft_row_erase_range(f_row_t* row, size_t left, size_t right)
{
    assert(row);
    size_t cols_n = vector_size(row->cells);
    if(cols_n == 0 || (right < left))
        return FT_SUCCESS;

    f_cell_t* cell = NULL;
    size_t i = left;
    while(i < cols_n && i <= right) {
        cell = VECTOR_AT(row->cells, i, f_cell_t*);
        destroy_cell(cell);
        ++i;
    }
    size_t n_destroy = MIN(cols_n - 1, right) - left + 1;
    while(n_destroy--) {
        vector_erase(row->cells, left);
    }
    return FT_SUCCESS;
}

FT_INTERNAL
size_t columns_in_row(const f_row_t* row)
{
    if(row == NULL || row->cells == NULL)
        return 0;

    return vector_size(row->cells);
}

static f_cell_t*
get_cell_impl(f_row_t* row, size_t col, enum f_get_policy policy)
{
    if(row == NULL || row->cells == NULL) {
        return NULL;
    }

    switch(policy) {
    case DONT_CREATE_ON_NULL:
        if(col < columns_in_row(row)) {
            return VECTOR_AT(row->cells, col, f_cell_t*);
        }
        return NULL;
    case CREATE_ON_NULL:
        while(col >= columns_in_row(row)) {
            f_cell_t* new_cell = create_cell();
            if(new_cell == NULL)
                return NULL;
            if(FT_IS_ERROR(vector_push(row->cells, &new_cell))) {
                destroy_cell(new_cell);
                return NULL;
            }
        }
        return VECTOR_AT(row->cells, col, f_cell_t*);
    }

    assert(0 && "Shouldn't be here!");
    return NULL;
}

FT_INTERNAL
f_cell_t* get_cell(f_row_t* row, size_t col)
{
    return get_cell_impl(row, col, DONT_CREATE_ON_NULL);
}

FT_INTERNAL
const f_cell_t* get_cell_c(const f_row_t* row, size_t col)
{
    return get_cell((f_row_t*)row, col);
}

FT_INTERNAL
f_cell_t* get_cell_and_create_if_not_exists(f_row_t* row, size_t col)
{
    return get_cell_impl(row, col, CREATE_ON_NULL);
}

FT_INTERNAL
f_cell_t* create_cell_in_position(f_row_t* row, size_t col)
{
    if(row == NULL || row->cells == NULL) {
        return NULL;
    }

    f_cell_t* new_cell = create_cell();
    if(new_cell == NULL)
        return NULL;
    if(FT_IS_ERROR(vector_insert(row->cells, &new_cell, col))) {
        destroy_cell(new_cell);
        return NULL;
    }
    return VECTOR_AT(row->cells, col, f_cell_t*);
}

FT_INTERNAL
f_status swap_row(f_row_t* cur_row, f_row_t* ins_row, size_t pos)
{
    assert(cur_row);
    assert(ins_row);
    size_t cur_sz = vector_size(cur_row->cells);
    if(cur_sz == 0 && pos == 0) {
        f_row_t tmp;
        memcpy(&tmp, cur_row, sizeof(f_row_t));
        memcpy(cur_row, ins_row, sizeof(f_row_t));
        memcpy(ins_row, &tmp, sizeof(f_row_t));
        return FT_SUCCESS;
    }

    return vector_swap(cur_row->cells, ins_row->cells, pos);
}

/* Ownership of cells of `ins_row` is passed to `cur_row`. */
FT_INTERNAL
f_status insert_row(f_row_t* cur_row, f_row_t* ins_row, size_t pos)
{
    assert(cur_row);
    assert(ins_row);

    while(vector_size(cur_row->cells) < pos) {
        f_cell_t* new_cell = create_cell();
        if(!new_cell)
            return FT_GEN_ERROR;
        vector_push(cur_row->cells, &new_cell);
    }

    size_t sz = vector_size(ins_row->cells);
    size_t i = 0;
    for(i = 0; i < sz; ++i) {
        f_cell_t* cell = VECTOR_AT(ins_row->cells, i, f_cell_t*);
        if(FT_IS_ERROR(vector_insert(cur_row->cells, &cell, pos + i))) {
            /* clean up what we have inserted */
            while(i--) {
                vector_erase(cur_row->cells, pos);
            }
            return FT_GEN_ERROR;
        }
    }
    /* Clear cells so that it will be safe to destroy this row */
    vector_clear(ins_row->cells);
    return FT_SUCCESS;
}

FT_INTERNAL
size_t group_cell_number(const f_row_t* row, size_t master_cell_col)
{
    assert(row);
    const f_cell_t* master_cell = get_cell_c(row, master_cell_col);
    if(master_cell == NULL)
        return 0;

    if(get_cell_type(master_cell) != GROUP_MASTER_CELL)
        return 1;

    size_t total_cols = vector_size(row->cells);
    size_t slave_col = master_cell_col + 1;
    while(slave_col < total_cols) {
        const f_cell_t* cell = get_cell_c(row, slave_col);
        if(cell && get_cell_type(cell) == GROUP_SLAVE_CELL) {
            ++slave_col;
        }
        else {
            break;
        }
    }
    return slave_col - master_cell_col;
}

FT_INTERNAL
int get_row_cell_types(const f_row_t* row,
                       enum f_cell_type* types,
                       size_t types_sz)
{
    assert(row);
    assert(types);
    size_t i = 0;
    for(i = 0; i < types_sz; ++i) {
        const f_cell_t* cell = get_cell_c(row, i);
        if(cell) {
            types[i] = get_cell_type(cell);
        }
        else {
            types[i] = COMMON_CELL;
        }
    }
    return FT_SUCCESS;
}

FT_INTERNAL
f_status row_set_cell_span(f_row_t* row, size_t cell_column, size_t hor_span)
{
    assert(row);

    if(hor_span < 2)
        return FT_EINVAL;

    f_cell_t* main_cell = get_cell_and_create_if_not_exists(row, cell_column);
    if(main_cell == NULL) {
        return FT_GEN_ERROR;
    }
    set_cell_type(main_cell, GROUP_MASTER_CELL);
    --hor_span;
    ++cell_column;

    while(hor_span) {
        f_cell_t* slave_cell =
            get_cell_and_create_if_not_exists(row, cell_column);
        if(slave_cell == NULL) {
            return FT_GEN_ERROR;
        }
        set_cell_type(slave_cell, GROUP_SLAVE_CELL);
        --hor_span;
        ++cell_column;
    }

    return FT_SUCCESS;
}

static int print_row_separator_impl(f_conv_context_t* cntx,
                                    const size_t* col_width_arr,
                                    size_t cols,
                                    const f_row_t* upper_row,
                                    const f_row_t* lower_row,
                                    enum f_hor_separator_pos separatorPos,
                                    const f_separator_t* sep)
{
    assert(cntx);

    int status = FT_GEN_ERROR;

    const f_context_t* context = cntx->cntx;

    /* Get cell types
     *
     * Regions above top row and below bottom row areconsidered full of virtual
     * GROUP_SLAVE_CELL cells
     */
    enum f_cell_type* top_row_types =
        (enum f_cell_type*)F_MALLOC(sizeof(enum f_cell_type) * cols * 2);
    if(top_row_types == NULL) {
        return FT_MEMORY_ERROR;
    }
    enum f_cell_type* bottom_row_types = top_row_types + cols;
    if(upper_row) {
        get_row_cell_types(upper_row, top_row_types, cols);
    }
    else {
        size_t i = 0;
        for(i = 0; i < cols; ++i)
            top_row_types[i] = GROUP_SLAVE_CELL;
    }
    if(lower_row) {
        get_row_cell_types(lower_row, bottom_row_types, cols);
    }
    else {
        size_t i = 0;
        for(i = 0; i < cols; ++i)
            bottom_row_types[i] = GROUP_SLAVE_CELL;
    }

    f_table_properties_t* properties = context->table_properties;
    fort_entire_table_properties_t* entire_tprops =
        &properties->entire_table_properties;

    size_t written = 0;
    int tmp = 0;

    enum ft_row_type lower_row_type = FT_ROW_COMMON;
    if(lower_row != NULL) {
        lower_row_type = (enum ft_row_type)get_cell_property_hierarchically(
            properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    }
    enum ft_row_type upper_row_type = FT_ROW_COMMON;
    if(upper_row != NULL) {
        upper_row_type = (enum ft_row_type)get_cell_property_hierarchically(
            properties, context->row - 1, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    }

    /* Row separator anatomy
     *
     *  |      C11    |   C12         C13      |      C14           C15 | L  I
     * I  I   IV  I   I   IT  I  I  I  IB    I    I     II    I    I     R | C21
     * |   C22     |   C23             C24           C25         |
     */
    const char** L = NULL;
    const char** I = NULL;
    const char** IV = NULL;
    const char** R = NULL;
    const char** IT = NULL;
    const char** IB = NULL;
    const char** II = NULL;

    struct fort_border_style* border_style = &properties->border_style;

    typedef const char*(*border_chars_point_t)[BORDER_ITEM_POS_SIZE];
    const char*(*border_chars)[BORDER_ITEM_POS_SIZE] = NULL;
    border_chars = (border_chars_point_t)&border_style->border_chars;
    if(upper_row_type == FT_ROW_HEADER || lower_row_type == FT_ROW_HEADER) {
        border_chars = (border_chars_point_t)&border_style->header_border_chars;
    }

    if(sep && sep->enabled) {
        L = &(border_style->separator_chars[LH_sip]);
        I = &(border_style->separator_chars[IH_sip]);
        IV = &(border_style->separator_chars[II_sip]);
        R = &(border_style->separator_chars[RH_sip]);

        IT = &(border_style->separator_chars[TI_sip]);
        IB = &(border_style->separator_chars[BI_sip]);
        II = &(border_style->separator_chars[IH_sip]);

        if(lower_row == NULL) {
            L = &(*border_chars)[BL_bip];
            R = &(*border_chars)[BR_bip];
        }
        else if(upper_row == NULL) {
            L = &(*border_chars)[TL_bip];
            R = &(*border_chars)[TR_bip];
        }
    }
    else {
        switch(separatorPos) {
        case TOP_SEPARATOR:
            L = &(*border_chars)[TL_bip];
            I = &(*border_chars)[TT_bip];
            IV = &(*border_chars)[TV_bip];
            R = &(*border_chars)[TR_bip];

            IT = &(*border_chars)[TV_bip];
            IB = &(*border_chars)[TV_bip];
            II = &(*border_chars)[TT_bip];
            break;
        case INSIDE_SEPARATOR:
            L = &(*border_chars)[LH_bip];
            I = &(*border_chars)[IH_bip];
            IV = &(*border_chars)[II_bip];
            R = &(*border_chars)[RH_bip];

            IT = &(*border_chars)[TI_bip];
            IB = &(*border_chars)[BI_bip];
            II = &(*border_chars)[IH_bip];
            break;
        case BOTTOM_SEPARATOR:
            L = &(*border_chars)[BL_bip];
            I = &(*border_chars)[BB_bip];
            IV = &(*border_chars)[BV_bip];
            R = &(*border_chars)[BR_bip];

            IT = &(*border_chars)[BV_bip];
            IB = &(*border_chars)[BV_bip];
            II = &(*border_chars)[BB_bip];
            break;
        default:
            break;
        }
    }

    size_t i = 0;

    /* If all chars are not printable, skip line separator */
    /* NOTE: argument of `isprint` should be explicitly converted to
     * unsigned char according to
     * https://en.cppreference.com/w/c/string/byte/isprint
     */
    if((strlen(*L) == 0 || (strlen(*L) == 1 && !isprint((unsigned char)**L))) &&
       (strlen(*I) == 0 || (strlen(*I) == 1 && !isprint((unsigned char)**I))) &&
       (strlen(*IV) == 0 ||
        (strlen(*IV) == 1 && !isprint((unsigned char)**IV))) &&
       (strlen(*R) == 0 || (strlen(*R) == 1 && !isprint((unsigned char)**R)))) {
        status = 0;
        goto clear;
    }

    /* Print left margin */
    CHCK_RSLT_ADD_TO_WRITTEN(
        print_n_strings(cntx, entire_tprops->left_margin, FT_SPACE));

    for(i = 0; i < cols; ++i) {
        if(i == 0) {
            CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *L));
        }
        else {
            if((top_row_types[i] == COMMON_CELL ||
                top_row_types[i] == GROUP_MASTER_CELL) &&
               (bottom_row_types[i] == COMMON_CELL ||
                bottom_row_types[i] == GROUP_MASTER_CELL)) {
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IV));
            }
            else if(top_row_types[i] == GROUP_SLAVE_CELL &&
                    bottom_row_types[i] == GROUP_SLAVE_CELL) {
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *II));
            }
            else if(top_row_types[i] == GROUP_SLAVE_CELL) {
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IT));
            }
            else {
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IB));
            }
        }
        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, col_width_arr[i], *I));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *R));

    /* Print right margin */
    CHCK_RSLT_ADD_TO_WRITTEN(
        print_n_strings(cntx, entire_tprops->right_margin, FT_SPACE));

    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, FT_NEWLINE));

    status = (int)written;

clear:
    F_FREE(top_row_types);
    return status;
}

FT_INTERNAL
int print_row_separator(f_conv_context_t* cntx,
                        const size_t* col_width_arr,
                        size_t cols,
                        const f_row_t* upper_row,
                        const f_row_t* lower_row,
                        enum f_hor_separator_pos separatorPos,
                        const f_separator_t* sep)
{
    return print_row_separator_impl(
        cntx, col_width_arr, cols, upper_row, lower_row, separatorPos, sep);
}

FT_INTERNAL
f_row_t* create_row_from_string(const char* str)
{
    typedef char char_type;
    char_type* (*strdup_)(const char_type* str) = F_STRDUP;
    const char_type zero_char = '\0';
    f_status (*fill_cell_from_string_)(f_cell_t * cell, const char* str) =
        fill_cell_from_string;
    const char_type* const zero_string = "";
#define STRCHR strchr

    char_type* pos = NULL;
    char_type* base_pos = NULL;
    size_t number_of_separators = 0;

    f_row_t* row = create_row();
    if(row == NULL)
        return NULL;

    if(str == NULL)
        return row;

    char_type* str_copy = strdup_(str);
    if(str_copy == NULL)
        goto clear;

    pos = str_copy;
    base_pos = str_copy;
    number_of_separators = 0;
    while(*pos) {
        pos = STRCHR(pos, g_col_separator);
        if(pos != NULL) {
            *(pos) = zero_char;
            ++pos;
            number_of_separators++;
        }

        f_cell_t* cell = create_cell();
        if(cell == NULL)
            goto clear;

        int status = fill_cell_from_string_(cell, base_pos);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        if(pos == NULL)
            break;
        base_pos = pos;
    }

    /* special case if in format string last cell is empty */
    while(vector_size(row->cells) < (number_of_separators + 1)) {
        f_cell_t* cell = create_cell();
        if(cell == NULL)
            goto clear;

        int status = fill_cell_from_string_(cell, zero_string);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }
    }

    F_FREE(str_copy);
    return row;

clear:
    destroy_row(row);
    F_FREE(str_copy);
    return NULL;

#undef STRCHR
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
f_row_t* create_row_from_wstring(const wchar_t* str)
{
    typedef wchar_t char_type;
    char_type* (*strdup_)(const char_type* str) = F_WCSDUP;
    const char_type zero_char = L'\0';
    f_status (*fill_cell_from_string_)(f_cell_t * cell, const wchar_t* str) =
        fill_cell_from_wstring;
    const char_type* const zero_string = L"";
#define STRCHR wcschr

    char_type* pos = NULL;
    char_type* base_pos = NULL;
    size_t number_of_separators = 0;

    f_row_t* row = create_row();
    if(row == NULL)
        return NULL;

    if(str == NULL)
        return row;

    char_type* str_copy = strdup_(str);
    if(str_copy == NULL)
        goto clear;

    pos = str_copy;
    base_pos = str_copy;
    number_of_separators = 0;
    while(*pos) {
        pos = STRCHR(pos, g_col_separator);
        if(pos != NULL) {
            *(pos) = zero_char;
            ++pos;
            number_of_separators++;
        }

        f_cell_t* cell = create_cell();
        if(cell == NULL)
            goto clear;

        int status = fill_cell_from_string_(cell, base_pos);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        if(pos == NULL)
            break;
        base_pos = pos;
    }

    /* special case if in format string last cell is empty */
    while(vector_size(row->cells) < (number_of_separators + 1)) {
        f_cell_t* cell = create_cell();
        if(cell == NULL)
            goto clear;

        int status = fill_cell_from_string_(cell, zero_string);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if(FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }
    }

    F_FREE(str_copy);
    return row;

clear:
    destroy_row(row);
    F_FREE(str_copy);
    return NULL;
#undef STRCHR
}
#endif

FT_INTERNAL
f_row_t* create_row_from_buffer(const f_string_buffer_t* buffer)
{
    switch(buffer->type) {
    case CHAR_BUF:
        return create_row_from_string(buffer->str.cstr);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return create_row_from_wstring(buffer->str.wstr);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return create_row_from_string((const char*)buffer->str.u8str);
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
        return NULL;
    }
}

static int vsnprintf_buffer(f_string_buffer_t* buffer,
                            const struct f_string_view* fmt,
                            va_list* va)
{
    /* Disable compiler diagnostic (format string is not a string literal) */
#if defined(FT_CLANG_COMPILER)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
#if defined(FT_GCC_COMPILER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
    size_t width_capacity = string_buffer_width_capacity(buffer);
    switch(buffer->type) {
    case CHAR_BUF:
        return vsnprintf(buffer->str.cstr, width_capacity, fmt->u.cstr, *va);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return vswprintf(buffer->str.wstr, width_capacity, fmt->u.wstr, *va);
#endif
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return vsnprintf(buffer->str.cstr, width_capacity, fmt->u.cstr, *va);
#endif
    default:
        assert(0);
        return 0;
    }
#if defined(FT_CLANG_COMPILER)
#pragma clang diagnostic pop
#endif
#if defined(FT_GCC_COMPILER)
#pragma GCC diagnostic pop
#endif
}

FT_INTERNAL
f_row_t* create_row_from_fmt_string(const struct f_string_view* fmt,
                                    va_list* va_args)
{
    f_string_buffer_t* buffer =
        create_string_buffer(DEFAULT_STR_BUF_SIZE, fmt->type);
    if(buffer == NULL)
        return NULL;

    size_t cols_origin = number_of_columns_in_format_string(fmt);
    size_t cols = 0;

    while(1) {
        va_list va;
        va_copy(va, *va_args);
        int virtual_sz = vsnprintf_buffer(buffer, fmt, &va);
        va_end(va);
        /* If error encountered */
        if(virtual_sz < 0)
            goto clear;

        /* Successful write */
        if((size_t)virtual_sz < string_buffer_width_capacity(buffer))
            break;

        /* Otherwise buffer was too small, so incr. buffer size ant try again.
         */
        if(!FT_IS_SUCCESS(realloc_string_buffer_without_copy(buffer)))
            goto clear;
    }

    cols = number_of_columns_in_format_buffer(buffer);
    if(cols == cols_origin) {
        f_row_t* row = create_row_from_buffer(buffer);
        if(row == NULL) {
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    if(cols_origin == 1) {
        f_row_t* row = create_row();
        if(row == NULL) {
            goto clear;
        }

        f_cell_t* cell = get_cell_and_create_if_not_exists(row, 0);
        if(cell == NULL) {
            destroy_row(row);
            goto clear;
        }

        f_status result = fill_cell_from_buffer(cell, buffer);
        if(FT_IS_ERROR(result)) {
            destroy_row(row);
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    /*
     * todo: add processing of cols != cols_origin in a general way
     * (when cols_origin != 1).
     */

clear:
    destroy_string_buffer(buffer);
    return NULL;
}

FT_INTERNAL
int snprintf_row(const f_row_t* row,
                 f_conv_context_t* cntx,
                 size_t* col_width_arr,
                 size_t col_width_arr_sz,
                 size_t row_height)
{
    const f_context_t* context = cntx->cntx;
    assert(context);

    if(row == NULL)
        return -1;

    size_t cols_in_row = columns_in_row(row);
    if(cols_in_row > col_width_arr_sz)
        return -1;

    /*  Row separator anatomy
     *
     *  L    data    IV    data   IV   data    R
     */
    f_table_properties_t* properties = context->table_properties;

    typedef const char*(*border_chars_point_t)[BORDER_ITEM_POS_SIZE];
    enum ft_row_type row_type =
        (enum ft_row_type)get_cell_property_hierarchically(
            properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    const char*(*bord_chars)[BORDER_ITEM_POS_SIZE] =
        (row_type == FT_ROW_HEADER)
            ? (border_chars_point_t)(
                  &properties->border_style.header_border_chars)
            : (border_chars_point_t)(&properties->border_style.border_chars);
    const char** L = &(*bord_chars)[LL_bip];
    const char** IV = &(*bord_chars)[IV_bip];
    const char** R = &(*bord_chars)[RR_bip];

    size_t written = 0;
    int tmp = 0;
    size_t i = 0;
    fort_entire_table_properties_t* entire_tprops =
        &context->table_properties->entire_table_properties;
    for(i = 0; i < row_height; ++i) {
        /* Print left margin */
        CHCK_RSLT_ADD_TO_WRITTEN(
            print_n_strings(cntx, entire_tprops->left_margin, FT_SPACE));

        /* Print left table boundary */
        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *L));
        size_t j = 0;
        while(j < col_width_arr_sz) {
            if(j < cols_in_row) {
                ((f_context_t*)context)->column = j;
                f_cell_t* cell = VECTOR_AT(row->cells, j, f_cell_t*);
                size_t cell_vis_width = 0;

                size_t group_slave_sz = group_cell_number(row, j);
                cell_vis_width = col_width_arr[j];
                size_t slave_j = 0;
                size_t master_j = j;
                for(slave_j = master_j + 1;
                    slave_j < (master_j + group_slave_sz);
                    ++slave_j) {
                    cell_vis_width +=
                        col_width_arr[slave_j] + FORT_COL_SEPARATOR_LENGTH;
                    ++j;
                }

                CHCK_RSLT_ADD_TO_WRITTEN(
                    cell_printf(cell, i, cntx, cell_vis_width));
            }
            else {
                /* Print empty cell */
                CHCK_RSLT_ADD_TO_WRITTEN(
                    print_n_strings(cntx, col_width_arr[j], FT_SPACE));
            }

            /* Print boundary between cells */
            if(j < col_width_arr_sz - 1)
                CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *IV));

            ++j;
        }

        /* Print right table boundary */
        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, *R));

        /* Print right margin */
        CHCK_RSLT_ADD_TO_WRITTEN(
            print_n_strings(cntx, entire_tprops->right_margin, FT_SPACE));

        /* Print new line character */
        CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, FT_NEWLINE));
    }
    return (int)written;

clear:
    return -1;
}

/********************************************************
   End of file "row.c"
 ********************************************************/

/********************************************************
   Begin of file "string_buffer.c"
 ********************************************************/

/* #include "string_buffer.h" */ /* Commented by amalgamation script */
/* #include "properties.h" */    /* Commented by amalgamation script */
/* #include "wcwidth.h" */       /* Commented by amalgamation script */
#include <assert.h>
#include <stddef.h>
#ifdef FT_HAVE_WCHAR
#include <wchar.h>
#endif
#if defined(FT_HAVE_UTF8)
/* #include "utf8.h" */ /* Commented by amalgamation script */
#endif

static ptrdiff_t str_iter_width(const char* beg, const char* end)
{
    assert(end >= beg);
    return (end - beg);
}

#ifdef FT_HAVE_WCHAR
static ptrdiff_t wcs_iter_width(const wchar_t* beg, const wchar_t* end)
{
    assert(end >= beg);
    return mk_wcswidth(beg, (size_t)(end - beg));
}
#endif /* FT_HAVE_WCHAR */

static size_t buf_str_len(const f_string_buffer_t* buf)
{
    assert(buf);

    switch(buf->type) {
    case CHAR_BUF:
        return strlen(buf->str.cstr);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return wcslen(buf->str.wstr);
#endif
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return utf8len(buf->str.u8str);
#endif
    }

    assert(0);
    return 0;
}

FT_INTERNAL
size_t strchr_count(const char* str, char ch)
{
    if(str == NULL)
        return 0;

    size_t count = 0;
    str = strchr(str, ch);
    while(str) {
        count++;
        str++;
        str = strchr(str, ch);
    }
    return count;
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
size_t wstrchr_count(const wchar_t* str, wchar_t ch)
{
    if(str == NULL)
        return 0;

    size_t count = 0;
    str = wcschr(str, ch);
    while(str) {
        count++;
        str++;
        str = wcschr(str, ch);
    }
    return count;
}
#endif

#if defined(FT_HAVE_UTF8)
/* todo: do something with code below!!! */
FT_INTERNAL
void* ut8next(const void* str)
{
    utf8_int32_t out_codepoint;
    return utf8codepoint(str, &out_codepoint);
}

FT_INTERNAL
size_t utf8chr_count(const void* str, utf8_int32_t ch)
{
    if(str == NULL)
        return 0;

    size_t count = 0;
    str = utf8chr(str, ch);
    while(str) {
        count++;
        str = ut8next(str);
        str = utf8chr(str, ch);
    }
    return count;
}
#endif /* FT_HAVE_UTF8 */

FT_INTERNAL
const char* str_n_substring_beg(const char* str, char ch_separator, size_t n)
{
    if(str == NULL)
        return NULL;

    if(n == 0)
        return str;

    str = strchr(str, ch_separator);
    --n;
    while(n > 0) {
        if(str == NULL)
            return NULL;
        --n;
        str++;
        str = strchr(str, ch_separator);
    }
    return str ? (str + 1) : NULL;
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
const wchar_t*
wstr_n_substring_beg(const wchar_t* str, wchar_t ch_separator, size_t n)
{
    if(str == NULL)
        return NULL;

    if(n == 0)
        return str;

    str = wcschr(str, ch_separator);
    --n;
    while(n > 0) {
        if(str == NULL)
            return NULL;
        --n;
        str++;
        str = wcschr(str, ch_separator);
    }
    return str ? (str + 1) : NULL;
}
#endif /* FT_HAVE_WCHAR */

#if defined(FT_HAVE_UTF8)
FT_INTERNAL
const void*
utf8_n_substring_beg(const void* str, utf8_int32_t ch_separator, size_t n)
{
    if(str == NULL)
        return NULL;

    if(n == 0)
        return str;

    str = utf8chr(str, ch_separator);
    --n;
    while(n > 0) {
        if(str == NULL)
            return NULL;
        --n;
        str = ut8next(str);
        str = utf8chr(str, ch_separator);
    }
    return str ? (ut8next(str)) : NULL;
}
#endif

FT_INTERNAL
void str_n_substring(const char* str,
                     char ch_separator,
                     size_t n,
                     const char** begin,
                     const char** end)
{
    const char* beg = str_n_substring_beg(str, ch_separator, n);
    if(beg == NULL) {
        *begin = NULL;
        *end = NULL;
        return;
    }

    const char* en = strchr(beg, ch_separator);
    if(en == NULL) {
        en = str + strlen(str);
    }

    *begin = beg;
    *end = en;
    return;
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
void wstr_n_substring(const wchar_t* str,
                      wchar_t ch_separator,
                      size_t n,
                      const wchar_t** begin,
                      const wchar_t** end)
{
    const wchar_t* beg = wstr_n_substring_beg(str, ch_separator, n);
    if(beg == NULL) {
        *begin = NULL;
        *end = NULL;
        return;
    }

    const wchar_t* en = wcschr(beg, ch_separator);
    if(en == NULL) {
        en = str + wcslen(str);
    }

    *begin = beg;
    *end = en;
    return;
}
#endif /* FT_HAVE_WCHAR */

#if defined(FT_HAVE_UTF8)
FT_INTERNAL
void utf8_n_substring(const void* str,
                      utf8_int32_t ch_separator,
                      size_t n,
                      const void** begin,
                      const void** end)
{
    const char* beg = (const char*)utf8_n_substring_beg(str, ch_separator, n);
    if(beg == NULL) {
        *begin = NULL;
        *end = NULL;
        return;
    }

    const char* en = (const char*)utf8chr(beg, ch_separator);
    if(en == NULL) {
        en = (const char*)str + strlen((const char*)str);
    }

    *begin = beg;
    *end = en;
    return;
}
#endif /* FT_HAVE_UTF8 */

FT_INTERNAL
f_string_buffer_t* create_string_buffer(size_t n_chars, enum f_string_type type)
{
    size_t char_sz = 0;
    switch(type) {
    case CHAR_BUF:
        char_sz = 1;
        break;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        char_sz = sizeof(wchar_t);
        break;
#endif
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        char_sz = 4;
        break;
#endif
    }

    size_t sz = n_chars * char_sz;
    f_string_buffer_t* result =
        (f_string_buffer_t*)F_MALLOC(sizeof(f_string_buffer_t));
    if(result == NULL)
        return NULL;
    result->str.data = F_MALLOC(sz);
    if(result->str.data == NULL) {
        F_FREE(result);
        return NULL;
    }
    result->data_sz = sz;
    result->type = type;

    if(sz) {
        switch(type) {
        case CHAR_BUF:
            result->str.cstr[0] = '\0';
            break;
#ifdef FT_HAVE_WCHAR
        case W_CHAR_BUF:
            result->str.wstr[0] = L'\0';
            break;
#endif
#ifdef FT_HAVE_UTF8
        case UTF8_BUF:
            result->str.cstr[0] = '\0';
            break;
#endif
        }
    }

    return result;
}

FT_INTERNAL
void destroy_string_buffer(f_string_buffer_t* buffer)
{
    if(buffer == NULL)
        return;
    F_FREE(buffer->str.data);
    buffer->str.data = NULL;
    F_FREE(buffer);
}

FT_INTERNAL
f_string_buffer_t* copy_string_buffer(const f_string_buffer_t* buffer)
{
    assert(buffer);
    f_string_buffer_t* result =
        create_string_buffer(buffer->data_sz, buffer->type);
    if(result == NULL)
        return NULL;
    switch(buffer->type) {
    case CHAR_BUF:
        if(FT_IS_ERROR(fill_buffer_from_string(result, buffer->str.cstr))) {
            destroy_string_buffer(result);
            return NULL;
        }
        break;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        if(FT_IS_ERROR(fill_buffer_from_wstring(result, buffer->str.wstr))) {
            destroy_string_buffer(result);
            return NULL;
        }
        break;
#endif /* FT_HAVE_WCHAR */
    default:
        destroy_string_buffer(result);
        return NULL;
    }
    return result;
}

FT_INTERNAL
f_status realloc_string_buffer_without_copy(f_string_buffer_t* buffer)
{
    assert(buffer);
    char* new_str = (char*)F_MALLOC(buffer->data_sz * 2);
    if(new_str == NULL) {
        return FT_MEMORY_ERROR;
    }
    F_FREE(buffer->str.data);
    buffer->str.data = new_str;
    buffer->data_sz *= 2;
    return FT_SUCCESS;
}

FT_INTERNAL
f_status fill_buffer_from_string(f_string_buffer_t* buffer, const char* str)
{
    assert(buffer);
    assert(str);

    char* copy = F_STRDUP(str);
    if(copy == NULL)
        return FT_MEMORY_ERROR;

    F_FREE(buffer->str.data);
    buffer->str.cstr = copy;
    buffer->type = CHAR_BUF;

    return FT_SUCCESS;
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
f_status fill_buffer_from_wstring(f_string_buffer_t* buffer, const wchar_t* str)
{
    assert(buffer);
    assert(str);

    wchar_t* copy = F_WCSDUP(str);
    if(copy == NULL)
        return FT_MEMORY_ERROR;

    F_FREE(buffer->str.data);
    buffer->str.wstr = copy;
    buffer->type = W_CHAR_BUF;

    return FT_SUCCESS;
}
#endif /* FT_HAVE_WCHAR */

#ifdef FT_HAVE_UTF8
FT_INTERNAL
f_status fill_buffer_from_u8string(f_string_buffer_t* buffer, const void* str)
{
    assert(buffer);
    assert(str);

    void* copy = F_UTF8DUP(str);
    if(copy == NULL)
        return FT_MEMORY_ERROR;

    F_FREE(buffer->str.u8str);
    buffer->str.u8str = copy;
    buffer->type = UTF8_BUF;

    return FT_SUCCESS;
}
#endif /* FT_HAVE_UTF8 */

FT_INTERNAL
size_t buffer_text_visible_height(const f_string_buffer_t* buffer)
{
    if(buffer == NULL || buffer->str.data == NULL || buf_str_len(buffer) == 0) {
        return 0;
    }
    if(buffer->type == CHAR_BUF)
        return 1 + strchr_count(buffer->str.cstr, '\n');
#ifdef FT_HAVE_WCHAR
    else if(buffer->type == W_CHAR_BUF)
        return 1 + wstrchr_count(buffer->str.wstr, L'\n');
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    else if(buffer->type == UTF8_BUF)
        return 1 + utf8chr_count(buffer->str.u8str, '\n');
#endif /* FT_HAVE_WCHAR */

    assert(0);
    return 0;
}

FT_INTERNAL
size_t string_buffer_cod_width_capacity(const f_string_buffer_t* buffer)
{
    return string_buffer_width_capacity(buffer);
}

FT_INTERNAL
size_t string_buffer_raw_capacity(const f_string_buffer_t* buffer)
{
    return buffer->data_sz;
}

#ifdef FT_HAVE_UTF8
/* User provided function to compute utf8 string visible width */
static int (*_custom_u8strwid)(const void* beg,
                               const void* end,
                               size_t* width) = NULL;

FT_INTERNAL
void buffer_set_u8strwid_func(int (*u8strwid)(const void* beg,
                                              const void* end,
                                              size_t* width))
{
    _custom_u8strwid = u8strwid;
}

static size_t utf8_width(const void* beg, const void* end)
{
    if(_custom_u8strwid) {
        size_t width = 0;
        if(!_custom_u8strwid(beg, end, &width))
            return width;
    }

    size_t sz = (size_t)((const char*)end - (const char*)beg);
    char* tmp = (char*)F_MALLOC(sizeof(char) * (sz + 1));
    // @todo: add check to tmp
    assert(tmp);

    memcpy(tmp, beg, sz);
    tmp[sz] = '\0';
    size_t result = utf8width(tmp);
    F_FREE(tmp);
    return result;
}
#endif /* FT_HAVE_WCHAR */

FT_INTERNAL
size_t buffer_text_visible_width(const f_string_buffer_t* buffer)
{
    size_t max_length = 0;
    if(buffer->type == CHAR_BUF) {
        size_t n = 0;
        while(1) {
            const char* beg = NULL;
            const char* end = NULL;
            str_n_substring(buffer->str.cstr, '\n', n, &beg, &end);
            if(beg == NULL || end == NULL)
                return max_length;

            max_length = MAX(max_length, (size_t)(end - beg));
            ++n;
        }
#ifdef FT_HAVE_WCHAR
    }
    else if(buffer->type == W_CHAR_BUF) {
        size_t n = 0;
        while(1) {
            const wchar_t* beg = NULL;
            const wchar_t* end = NULL;
            wstr_n_substring(buffer->str.wstr, L'\n', n, &beg, &end);
            if(beg == NULL || end == NULL)
                return max_length;

            int line_width = mk_wcswidth(beg, (size_t)(end - beg));
            if(line_width < 0) /* For safety */
                line_width = 0;
            max_length = MAX(max_length, (size_t)line_width);

            ++n;
        }
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    }
    else if(buffer->type == UTF8_BUF) {
        size_t n = 0;
        while(1) {
            const void* beg = NULL;
            const void* end = NULL;
            utf8_n_substring(buffer->str.u8str, '\n', n, &beg, &end);
            if(beg == NULL || end == NULL)
                return max_length;

            max_length = MAX(max_length, (size_t)utf8_width(beg, end));
            ++n;
        }
#endif /* FT_HAVE_WCHAR */
    }

    return max_length; /* shouldn't be here */
}

static void buffer_substring(const f_string_buffer_t* buffer,
                             size_t buffer_row,
                             const void** begin,
                             const void** end,
                             ptrdiff_t* str_it_width)
{
    switch(buffer->type) {
    case CHAR_BUF:
        str_n_substring(buffer->str.cstr,
                        '\n',
                        buffer_row,
                        (const char**)begin,
                        (const char**)end);
        if((*(const char**)begin) && (*(const char**)end))
            *str_it_width =
                str_iter_width(*(const char**)begin, *(const char**)end);
        break;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        wstr_n_substring(buffer->str.wstr,
                         L'\n',
                         buffer_row,
                         (const wchar_t**)begin,
                         (const wchar_t**)end);
        if((*(const wchar_t**)begin) && (*(const wchar_t**)end))
            *str_it_width =
                wcs_iter_width(*(const wchar_t**)begin, *(const wchar_t**)end);
        break;
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        utf8_n_substring(buffer->str.u8str, '\n', buffer_row, begin, end);
        if((*(const char**)begin) && (*(const char**)end))
            *str_it_width = utf8_width(*begin, *end);
        break;
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
    }
}

static int
buffer_print_range(f_conv_context_t* cntx, const void* beg, const void* end)
{
    size_t len;
    switch(cntx->b_type) {
    case CHAR_BUF:
        len = (size_t)((const char*)end - (const char*)beg);
        return ft_nprint(cntx, (const char*)beg, len);
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        len = (size_t)((const wchar_t*)end - (const wchar_t*)beg);
        return ft_nwprint(cntx, (const wchar_t*)beg, len);
#endif /* FT_HAVE_WCHAR */
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return ft_nu8print(cntx, beg, end);
#endif /* FT_HAVE_UTF8 */
    default:
        assert(0);
        return -1;
    }
}

FT_INTERNAL
int buffer_printf(f_string_buffer_t* buffer,
                  size_t buffer_row,
                  f_conv_context_t* cntx,
                  size_t vis_width,
                  const char* content_style_tag,
                  const char* reset_content_style_tag)
{
    const f_context_t* context = cntx->cntx;
    f_table_properties_t* props = context->table_properties;
    size_t row = context->row;
    size_t column = context->column;

    if(buffer == NULL || buffer->str.data == NULL ||
       buffer_row >= buffer_text_visible_height(buffer)) {
        return -1;
    }

    size_t content_width = buffer_text_visible_width(buffer);
    if(vis_width < content_width)
        return -1;

    size_t left = 0;
    size_t right = 0;
    switch(get_cell_property_hierarchically(
        props, row, column, FT_CPROP_TEXT_ALIGN)) {
    case FT_ALIGNED_LEFT:
        left = 0;
        right = (vis_width)-content_width;
        break;
    case FT_ALIGNED_CENTER:
        left = ((vis_width)-content_width) / 2;
        right = ((vis_width)-content_width) - left;
        break;
    case FT_ALIGNED_RIGHT:
        left = (vis_width)-content_width;
        right = 0;
        break;
    default:
        assert(0);
        break;
    }

    size_t written = 0;
    int tmp = 0;
    ptrdiff_t str_it_width = 0;
    const void* beg = NULL;
    const void* end = NULL;
    buffer_substring(buffer, buffer_row, &beg, &end, &str_it_width);
    if(beg == NULL || end == NULL)
        return -1;
    if(str_it_width < 0 || content_width < (size_t)str_it_width)
        return -1;

    size_t padding = content_width - (size_t)str_it_width;

    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, left, FT_SPACE));
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, content_style_tag));
    CHCK_RSLT_ADD_TO_WRITTEN(buffer_print_range(cntx, beg, end));
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, 1, reset_content_style_tag));
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, padding, FT_SPACE));
    CHCK_RSLT_ADD_TO_WRITTEN(print_n_strings(cntx, right, FT_SPACE));
    return (int)written;

clear:
    return -1;
}

FT_INTERNAL
size_t string_buffer_width_capacity(const f_string_buffer_t* buffer)
{
    assert(buffer);
    switch(buffer->type) {
    case CHAR_BUF:
        return buffer->data_sz;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return buffer->data_sz / sizeof(wchar_t);
#endif
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return buffer->data_sz / 4;
#endif
    default:
        assert(0);
        return 0;
    }
}

FT_INTERNAL
void* buffer_get_data(f_string_buffer_t* buffer)
{
    assert(buffer);
    return buffer->str.data;
}

FT_INTERNAL
int buffer_check_align(f_string_buffer_t* buffer)
{
    assert(buffer);
    assert(buffer->str.data);

    switch(buffer->type) {
    case CHAR_BUF:
        return 1;
#ifdef FT_HAVE_WCHAR
    case W_CHAR_BUF:
        return (((unsigned long)buffer->str.data) & (sizeof(wchar_t) - 1)) == 0;
#endif
#ifdef FT_HAVE_UTF8
    case UTF8_BUF:
        return 1;
#endif
    default:
        assert(0);
        return 0;
    }
}

/********************************************************
   End of file "string_buffer.c"
 ********************************************************/

/********************************************************
   Begin of file "table.c"
 ********************************************************/

/* #include "table.h" */         /* Commented by amalgamation script */
/* #include "string_buffer.h" */ /* Commented by amalgamation script */
/* #include "cell.h" */          /* Commented by amalgamation script */
/* #include "vector.h" */        /* Commented by amalgamation script */
/* #include "row.h" */           /* Commented by amalgamation script */

FT_INTERNAL
f_separator_t* create_separator(int enabled)
{
    f_separator_t* res = (f_separator_t*)F_CALLOC(1, sizeof(f_separator_t));
    if(res == NULL)
        return NULL;
    res->enabled = enabled;
    return res;
}

FT_INTERNAL
void destroy_separator(f_separator_t* sep)
{
    F_FREE(sep);
}

FT_INTERNAL
f_separator_t* copy_separator(f_separator_t* sep)
{
    assert(sep);
    return create_separator(sep->enabled);
}

static f_row_t*
get_row_impl(ft_table_t* table, size_t row, enum f_get_policy policy)
{
    if(table == NULL || table->rows == NULL) {
        return NULL;
    }

    switch(policy) {
    case DONT_CREATE_ON_NULL:
        if(row < vector_size(table->rows)) {
            return VECTOR_AT(table->rows, row, f_row_t*);
        }
        return NULL;
    case CREATE_ON_NULL:
        while(row >= vector_size(table->rows)) {
            f_row_t* new_row = create_row();
            if(new_row == NULL)
                return NULL;
            if(FT_IS_ERROR(vector_push(table->rows, &new_row))) {
                destroy_row(new_row);
                return NULL;
            }
        }
        return VECTOR_AT(table->rows, row, f_row_t*);
    }

    assert(0 && "Shouldn't be here!");
    return NULL;
}

FT_INTERNAL
f_row_t* get_row(ft_table_t* table, size_t row)
{
    return get_row_impl(table, row, DONT_CREATE_ON_NULL);
}

FT_INTERNAL
const f_row_t* get_row_c(const ft_table_t* table, size_t row)
{
    return get_row((ft_table_t*)table, row);
}

FT_INTERNAL
f_row_t* get_row_and_create_if_not_exists(ft_table_t* table, size_t row)
{
    return get_row_impl(table, row, CREATE_ON_NULL);
}

FT_INTERNAL
f_string_buffer_t*
get_cur_str_buffer_and_create_if_not_exists(ft_table_t* table)
{
    assert(table);

    f_row_t* row = get_row_and_create_if_not_exists(table, table->cur_row);
    if(row == NULL)
        return NULL;

    f_cell_t* cell = NULL;
    fort_entire_table_properties_t* table_props =
        &table->properties->entire_table_properties;
    switch(table_props->add_strategy) {
    case FT_STRATEGY_INSERT:
        cell = create_cell_in_position(row, table->cur_col);
        break;
    case FT_STRATEGY_REPLACE:
        cell = get_cell_and_create_if_not_exists(row, table->cur_col);
        break;
    default:
        assert(0 && "Unexpected situation inside libfort");
        break;
    }

    if(cell == NULL)
        return NULL;

    return cell_get_string_buffer(cell);
}

/*
 * Returns number of cells (rows * cols)
 */
FT_INTERNAL
f_status get_table_sizes(const ft_table_t* table, size_t* rows, size_t* cols)
{
    *rows = 0;
    *cols = 0;
    if(table && table->rows) {
        *rows = vector_size(table->rows);
        size_t row_index = 0;
        for(row_index = 0; row_index < vector_size(table->rows); ++row_index) {
            f_row_t* row = VECTOR_AT(table->rows, row_index, f_row_t*);
            size_t cols_in_row = columns_in_row(row);
            if(cols_in_row > *cols)
                *cols = cols_in_row;
        }
    }
    return FT_SUCCESS;
}

FT_INTERNAL
f_status table_rows_and_cols_geometry(const ft_table_t* table,
                                      size_t** col_width_arr_p,
                                      size_t* col_width_arr_sz,
                                      size_t** row_height_arr_p,
                                      size_t* row_height_arr_sz,
                                      enum f_geometry_type geom)
{
    if(table == NULL) {
        return FT_GEN_ERROR;
    }

    size_t max_invis_codepoints = 0;
    size_t cols = 0;
    size_t rows = 0;
    int status = get_table_sizes(table, &rows, &cols);
    if(FT_IS_ERROR(status))
        return status;

    size_t* col_width_arr = (size_t*)F_CALLOC(cols, sizeof(size_t));
    size_t* row_height_arr = (size_t*)F_CALLOC(rows, sizeof(size_t));
    if(col_width_arr == NULL || row_height_arr == NULL) {
        F_FREE(col_width_arr);
        F_FREE(row_height_arr);
        return FT_GEN_ERROR;
    }

    int combined_cells_found = 0;
    f_context_t context;
    context.table_properties =
        (table->properties ? table->properties : &g_table_properties);
    size_t col = 0;
    for(col = 0; col < cols; ++col) {
        col_width_arr[col] = 0;
        size_t row = 0;
        for(row = 0; row < rows; ++row) {
            const f_row_t* row_p = get_row_c(table, row);
            const f_cell_t* cell = get_cell_c(row_p, col);
            context.column = col;
            context.row = row;
            if(cell) {
                switch(get_cell_type(cell)) {
                case COMMON_CELL:
                    col_width_arr[col] =
                        MAX(col_width_arr[col], cell_vis_width(cell, &context));
                    break;
                case GROUP_MASTER_CELL:
                    combined_cells_found = 1;
                    break;
                case GROUP_SLAVE_CELL:; /* Do nothing */
                    break;
                }
                row_height_arr[row] =
                    MAX(row_height_arr[row], hint_height_cell(cell, &context));
            }
            else {
                size_t cell_empty_string_height =
                    get_cell_property_hierarchically(context.table_properties,
                                                     context.row,
                                                     context.column,
                                                     FT_CPROP_EMPTY_STR_HEIGHT);
                if(cell_empty_string_height) {
                    size_t cell_top_padding = get_cell_property_hierarchically(
                        context.table_properties,
                        context.row,
                        context.column,
                        FT_CPROP_TOP_PADDING);
                    size_t cell_bottom_padding =
                        get_cell_property_hierarchically(
                            context.table_properties,
                            context.row,
                            context.column,
                            FT_CPROP_BOTTOM_PADDING);
                    row_height_arr[row] =
                        MAX(row_height_arr[row],
                            cell_empty_string_height + cell_top_padding +
                                cell_bottom_padding);
                }
            }
        }

        if(geom == INTERN_REPR_GEOMETRY) {
            max_invis_codepoints = 0;
            for(row = 0; row < rows; ++row) {
                const f_row_t* row_p = get_row_c(table, row);
                const f_cell_t* cell = get_cell_c(row_p, col);
                if(!cell)
                    continue;
                context.column = col;
                context.row = row;
                size_t inv_codepoints = cell_invis_codes_width(cell, &context);
                max_invis_codepoints =
                    MAX(max_invis_codepoints, inv_codepoints);
            }
            col_width_arr[col] += max_invis_codepoints;
        }
    }

    if(combined_cells_found) {
        for(col = 0; col < cols; ++col) {
            size_t row = 0;
            for(row = 0; row < rows; ++row) {
                const f_row_t* row_p = get_row_c(table, row);
                const f_cell_t* cell = get_cell_c(row_p, col);
                context.column = col;
                context.row = row;
                if(cell) {
                    if(get_cell_type(cell) == GROUP_MASTER_CELL) {
                        size_t hint_width = cell_vis_width(cell, &context);
                        if(geom == INTERN_REPR_GEOMETRY) {
                            hint_width +=
                                cell_invis_codes_width(cell, &context);
                        }
                        size_t slave_col = col + group_cell_number(row_p, col);
                        size_t cur_adj_col = col;
                        size_t group_width = col_width_arr[col];
                        size_t i;
                        for(i = col + 1; i < slave_col; ++i)
                            group_width +=
                                col_width_arr[i] + FORT_COL_SEPARATOR_LENGTH;
                        /* adjust col. widths */
                        while(1) {
                            if(group_width >= hint_width)
                                break;
                            col_width_arr[cur_adj_col] += 1;
                            group_width++;
                            cur_adj_col++;
                            if(cur_adj_col == slave_col)
                                cur_adj_col = col;
                        }
                    }
                }
            }
        }
    }

    /* todo: Maybe it is better to move min width checking to a particular cell
     * width checking. At the moment min width includes paddings. Maybe it is
     * better that min width weren't include paddings but be min width of the
     * cell content without padding
     */
    /*
    if (table->properties) {
        for (size_t i = 0; i < cols; ++i) {
            col_width_arr[i] = MAX((int)col_width_arr[i],
    fort_props_column_width(table->properties, i));
        }
    }
    */

    *col_width_arr_p = col_width_arr;
    *col_width_arr_sz = cols;
    *row_height_arr_p = row_height_arr;
    *row_height_arr_sz = rows;
    return FT_SUCCESS;
}

/*
 * Returns geometry in characters
 */
FT_INTERNAL
f_status table_geometry(const ft_table_t* table, size_t* height, size_t* width)
{
    if(table == NULL)
        return FT_GEN_ERROR;

    *height = 0;
    *width = 0;
    size_t cols = 0;
    size_t rows = 0;
    size_t* col_width_arr = NULL;
    size_t* row_height_arr = NULL;

    int status = table_rows_and_cols_geometry(table,
                                              &col_width_arr,
                                              &cols,
                                              &row_height_arr,
                                              &rows,
                                              INTERN_REPR_GEOMETRY);
    if(FT_IS_ERROR(status))
        return status;

    *width = 1 + (cols == 0 ? 1 : cols) +
             1; /* for boundaries (that take 1 symbol) + newline   */
    size_t i = 0;
    for(i = 0; i < cols; ++i) {
        *width += col_width_arr[i];
    }

    /* todo: add check for non printable horizontal row separators */
    *height =
        1 + (rows == 0 ? 1 : rows); /* for boundaries (that take 1 symbol)  */
    for(i = 0; i < rows; ++i) {
        *height += row_height_arr[i];
    }
    F_FREE(col_width_arr);
    F_FREE(row_height_arr);

    f_table_properties_t* properties = table->properties;
    if(properties) {
        *height += properties->entire_table_properties.top_margin;
        *height += properties->entire_table_properties.bottom_margin;
        *width += properties->entire_table_properties.left_margin;
        *width += properties->entire_table_properties.right_margin;
    }

    /* Take into account that border elements can be more than one byte long */
    f_table_properties_t* table_properties =
        properties ? properties : &g_table_properties;
    size_t max_border_elem_len = max_border_elem_strlen(table_properties);
    *width *= max_border_elem_len;

    return FT_SUCCESS;
}

FT_INTERNAL
f_status table_internal_codepoints_geometry(const ft_table_t* table,
                                            size_t* height,
                                            size_t* width)
{
    return table_geometry(table, height, width);
}

/********************************************************
   End of file "table.c"
 ********************************************************/

/********************************************************
   Begin of file "vector.c"
 ********************************************************/

/* #include "vector.h" */ /* Commented by amalgamation script */
#include <assert.h>
#include <string.h>

struct f_vector
{
    size_t m_size;
    void* m_data;
    size_t m_capacity;
    size_t m_item_size;
};

static int vector_reallocate_(f_vector_t* vector, size_t new_capacity)
{
    assert(vector);
    assert(new_capacity > vector->m_capacity);

    size_t new_size = new_capacity * vector->m_item_size;
    vector->m_data = F_REALLOC(vector->m_data, new_size);
    if(vector->m_data == NULL)
        return -1;
    return 0;
}

FT_INTERNAL
f_vector_t* create_vector(size_t item_size, size_t capacity)
{
    f_vector_t* vector = (f_vector_t*)F_MALLOC(sizeof(f_vector_t));
    if(vector == NULL) {
        return NULL;
    }

    size_t init_size = MAX(item_size * capacity, 1);
    vector->m_data = F_MALLOC(init_size);
    if(vector->m_data == NULL) {
        F_FREE(vector);
        return NULL;
    }

    vector->m_size = 0;
    vector->m_capacity = capacity;
    vector->m_item_size = item_size;

    return vector;
}

FT_INTERNAL
void destroy_vector(f_vector_t* vector)
{
    assert(vector);
    F_FREE(vector->m_data);
    F_FREE(vector);
}

FT_INTERNAL
size_t vector_size(const f_vector_t* vector)
{
    assert(vector);
    return vector->m_size;
}

FT_INTERNAL
size_t vector_capacity(const f_vector_t* vector)
{
    assert(vector);
    return vector->m_capacity;
}

FT_INTERNAL
int vector_push(f_vector_t* vector, const void* item)
{
    assert(vector);
    assert(item);

    if(vector->m_size == vector->m_capacity) {
        if(vector_reallocate_(vector, vector->m_capacity * 2) == -1)
            return FT_GEN_ERROR;
        vector->m_capacity = vector->m_capacity * 2;
    }

    size_t offset = vector->m_size * vector->m_item_size;
    memcpy((char*)vector->m_data + offset, item, vector->m_item_size);

    ++(vector->m_size);

    return FT_SUCCESS;
}

FT_INTERNAL
int vector_insert(f_vector_t* vector, const void* item, size_t pos)
{
    assert(vector);
    assert(item);
    size_t needed_capacity = MAX(pos + 1, vector->m_size + 1);
    if(vector->m_capacity < needed_capacity) {
        if(vector_reallocate_(vector, needed_capacity) == -1)
            return FT_GEN_ERROR;
        vector->m_capacity = needed_capacity;
    }
    size_t offset = pos * vector->m_item_size;
    if(pos >= vector->m_size) {
        /* Data in the middle are not initialized */
        memcpy((char*)vector->m_data + offset, item, vector->m_item_size);
        vector->m_size = pos + 1;
        return FT_SUCCESS;
    }
    else {
        /* Shift following data by one position */
        memmove((char*)vector->m_data + offset + vector->m_item_size,
                (char*)vector->m_data + offset,
                vector->m_item_size * (vector->m_size - pos));
        memcpy((char*)vector->m_data + offset, item, vector->m_item_size);
        ++(vector->m_size);
        return FT_SUCCESS;
    }
}

FT_INTERNAL
f_vector_t* vector_split(f_vector_t* vector, size_t pos)
{
    size_t trailing_sz = vector->m_size > pos ? vector->m_size - pos : 0;
    f_vector_t* new_vector = create_vector(vector->m_item_size, trailing_sz);
    if(!new_vector)
        return new_vector;
    if(new_vector->m_capacity < trailing_sz) {
        destroy_vector(new_vector);
        return NULL;
    }

    if(trailing_sz == 0)
        return new_vector;

    size_t offset = vector->m_item_size * pos;
    memcpy(new_vector->m_data,
           (char*)vector->m_data + offset,
           trailing_sz * vector->m_item_size);
    new_vector->m_size = trailing_sz;
    vector->m_size = pos;
    return new_vector;
}

FT_INTERNAL
const void* vector_at_c(const f_vector_t* vector, size_t index)
{
    if(index >= vector->m_size)
        return NULL;

    return (char*)vector->m_data + index * vector->m_item_size;
}

FT_INTERNAL
void* vector_at(f_vector_t* vector, size_t index)
{
    if(index >= vector->m_size)
        return NULL;

    return (char*)vector->m_data + index * vector->m_item_size;
}

FT_INTERNAL
f_status vector_swap(f_vector_t* cur_vec, f_vector_t* mv_vec, size_t pos)
{
    assert(cur_vec);
    assert(mv_vec);
    assert(cur_vec != mv_vec);
    assert(cur_vec->m_item_size == mv_vec->m_item_size);

    size_t cur_sz = vector_size(cur_vec);
    size_t mv_sz = vector_size(mv_vec);
    if(mv_sz == 0) {
        return FT_SUCCESS;
    }

    size_t min_targ_size = pos + mv_sz;
    if(vector_capacity(cur_vec) < min_targ_size) {
        if(vector_reallocate_(cur_vec, min_targ_size) == -1)
            return FT_GEN_ERROR;
        cur_vec->m_capacity = min_targ_size;
    }

    size_t offset = pos * cur_vec->m_item_size;
    void* tmp = NULL;
    size_t new_mv_sz = 0;
    if(cur_sz > pos) {
        new_mv_sz = MIN(cur_sz - pos, mv_sz);
        tmp = F_MALLOC(cur_vec->m_item_size * new_mv_sz);
        if(tmp == NULL) {
            return FT_MEMORY_ERROR;
        }
    }

    if(tmp) {
        memcpy(tmp,
               (char*)cur_vec->m_data + offset,
               cur_vec->m_item_size * new_mv_sz);
    }

    memcpy((char*)cur_vec->m_data + offset,
           mv_vec->m_data,
           cur_vec->m_item_size * mv_sz);

    if(tmp) {
        memcpy(mv_vec->m_data, tmp, cur_vec->m_item_size * new_mv_sz);
    }

    cur_vec->m_size = MAX(cur_vec->m_size, min_targ_size);
    mv_vec->m_size = new_mv_sz;
    F_FREE(tmp);
    return FT_SUCCESS;
}

FT_INTERNAL
void vector_clear(f_vector_t* vector)
{
    vector->m_size = 0;
}

FT_INTERNAL
int vector_erase(f_vector_t* vector, size_t index)
{
    assert(vector);

    if(vector->m_size == 0 || index >= vector->m_size)
        return FT_GEN_ERROR;

    memmove((char*)vector->m_data + vector->m_item_size * index,
            (char*)vector->m_data + vector->m_item_size * (index + 1),
            (vector->m_size - 1 - index) * vector->m_item_size);
    vector->m_size--;
    return FT_SUCCESS;
}

#ifdef FT_TEST_BUILD

f_vector_t* copy_vector(f_vector_t* v)
{
    if(v == NULL)
        return NULL;

    f_vector_t* new_vector = create_vector(v->m_item_size, v->m_capacity);
    if(new_vector == NULL)
        return NULL;

    memcpy(new_vector->m_data, v->m_data, v->m_item_size * v->m_size);
    new_vector->m_size = v->m_size;
    new_vector->m_item_size = v->m_item_size;
    return new_vector;
}

size_t vector_index_of(const f_vector_t* vector, const void* item)
{
    assert(vector);
    assert(item);

    size_t i = 0;
    for(i = 0; i < vector->m_size; ++i) {
        void* data_pos = (char*)vector->m_data + i * vector->m_item_size;
        if(memcmp(data_pos, item, vector->m_item_size) == 0) {
            return i;
        }
    }
    return INVALID_VEC_INDEX;
}

#endif

/********************************************************
   End of file "vector.c"
 ********************************************************/

/********************************************************
   Begin of file "wcwidth.c"
 ********************************************************/

/*
 * This is an implementation of wcwidth() and wcswidth() (defined in
 * IEEE Std 1002.1-2001) for Unicode.
 *
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcwidth.html
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcswidth.html
 *
 * In fixed-width output devices, Latin characters all occupy a single
 * "cell" position of equal width, whereas ideographic CJK characters
 * occupy two such cells. Interoperability between terminal-line
 * applications and (teletype-style) character terminals using the
 * UTF-8 encoding requires agreement on which character should advance
 * the cursor by how many cell positions. No established formal
 * standards exist at present on which Unicode character shall occupy
 * how many cell positions on character terminals. These routines are
 * a first attempt of defining such behavior based on simple rules
 * applied to data provided by the Unicode Consortium.
 *
 * For some graphical characters, the Unicode standard explicitly
 * defines a character-cell width via the definition of the East Asian
 * FullWidth (F), Wide (W), Half-width (H), and Narrow (Na) classes.
 * In all these cases, there is no ambiguity about which width a
 * terminal shall use. For characters in the East Asian Ambiguous (A)
 * class, the width choice depends purely on a preference of backward
 * compatibility with either historic CJK or Western practice.
 * Choosing single-width for these characters is easy to justify as
 * the appropriate long-term solution, as the CJK practice of
 * displaying these characters as double-width comes from historic
 * implementation simplicity (8-bit encoded characters were displayed
 * single-width and 16-bit ones double-width, even for Greek,
 * Cyrillic, etc.) and not any typographic considerations.
 *
 * Much less clear is the choice of width for the Not East Asian
 * (Neutral) class. Existing practice does not dictate a width for any
 * of these characters. It would nevertheless make sense
 * typographically to allocate two character cells to characters such
 * as for instance EM SPACE or VOLUME INTEGRAL, which cannot be
 * represented adequately with a single-width glyph. The following
 * routines at present merely assign a single-cell width to all
 * neutral characters, in the interest of simplicity. This is not
 * entirely satisfactory and should be reconsidered before
 * establishing a formal standard in this area. At the moment, the
 * decision which Not East Asian (Neutral) characters should be
 * represented by double-width glyphs cannot yet be answered by
 * applying a simple rule from the Unicode database content. Setting
 * up a proper standard for the behavior of UTF-8 character terminals
 * will require a careful analysis not only of each Unicode character,
 * but also of each presentation form, something the author of these
 * routines has avoided to do so far.
 *
 * http://www.unicode.org/unicode/reports/tr11/
 *
 * Markus Kuhn -- 2007-05-26 (Unicode 5.0)
 *
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted. The author
 * disclaims all warranties with regard to this software.
 *
 * Latest version: http://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
 */

/* #include "wcwidth.h" */ /* Commented by amalgamation script */

#ifdef FT_HAVE_WCHAR

struct interval
{
    int32_t first;
    int32_t last;
};

/* auxiliary function for binary search in interval table */
static int bisearch(int32_t ucs, const struct interval* table, int max)
{
    int min = 0;

    if(ucs < table[0].first || ucs > table[max].last)
        return 0;
    while(max >= min) {
        int mid = (min + max) / 2;
        if(ucs > table[mid].last)
            min = mid + 1;
        else if(ucs < table[mid].first)
            max = mid - 1;
        else
            return 1;
    }

    return 0;
}

/* The following two functions define the column width of an ISO 10646
 * character as follows:
 *
 *    - The null character (U+0000) has a column width of 0.
 *
 *    - Other C0/C1 control characters and DEL will lead to a return
 *      value of -1.
 *
 *    - Non-spacing and enclosing combining characters (general
 *      category code Mn or Me in the Unicode database) have a
 *      column width of 0.
 *
 *    - SOFT HYPHEN (U+00AD) has a column width of 1.
 *
 *    - Other format characters (general category code Cf in the Unicode
 *      database) and ZERO WIDTH SPACE (U+200B) have a column width of 0.
 *
 *    - Hangul Jamo medial vowels and final consonants (U+1160-U+11FF)
 *      have a column width of 0.
 *
 *    - Spacing characters in the East Asian Wide (W) or East Asian
 *      Full-width (F) category as defined in Unicode Technical
 *      Report #11 have a column width of 2.
 *
 *    - All remaining characters (including all printable
 *      ISO 8859-1 and WGL4 characters, Unicode control characters,
 *      etc.) have a column width of 1.
 *
 * This implementation assumes that wchar_t characters are encoded
 * in ISO 10646.
 */

static int mk_wcwidth(wchar_t wcs)
{
    /* sorted list of non-overlapping intervals of non-spacing characters */
    /* generated by "uniset +cat=Me +cat=Mn +cat=Cf -00AD +1160-11FF +200B c" */
    static const struct interval combining[] = {
        { 0x0300, 0x036F },   { 0x0483, 0x0486 },   { 0x0488, 0x0489 },
        { 0x0591, 0x05BD },   { 0x05BF, 0x05BF },   { 0x05C1, 0x05C2 },
        { 0x05C4, 0x05C5 },   { 0x05C7, 0x05C7 },   { 0x0600, 0x0603 },
        { 0x0610, 0x0615 },   { 0x064B, 0x065E },   { 0x0670, 0x0670 },
        { 0x06D6, 0x06E4 },   { 0x06E7, 0x06E8 },   { 0x06EA, 0x06ED },
        { 0x070F, 0x070F },   { 0x0711, 0x0711 },   { 0x0730, 0x074A },
        { 0x07A6, 0x07B0 },   { 0x07EB, 0x07F3 },   { 0x0901, 0x0902 },
        { 0x093C, 0x093C },   { 0x0941, 0x0948 },   { 0x094D, 0x094D },
        { 0x0951, 0x0954 },   { 0x0962, 0x0963 },   { 0x0981, 0x0981 },
        { 0x09BC, 0x09BC },   { 0x09C1, 0x09C4 },   { 0x09CD, 0x09CD },
        { 0x09E2, 0x09E3 },   { 0x0A01, 0x0A02 },   { 0x0A3C, 0x0A3C },
        { 0x0A41, 0x0A42 },   { 0x0A47, 0x0A48 },   { 0x0A4B, 0x0A4D },
        { 0x0A70, 0x0A71 },   { 0x0A81, 0x0A82 },   { 0x0ABC, 0x0ABC },
        { 0x0AC1, 0x0AC5 },   { 0x0AC7, 0x0AC8 },   { 0x0ACD, 0x0ACD },
        { 0x0AE2, 0x0AE3 },   { 0x0B01, 0x0B01 },   { 0x0B3C, 0x0B3C },
        { 0x0B3F, 0x0B3F },   { 0x0B41, 0x0B43 },   { 0x0B4D, 0x0B4D },
        { 0x0B56, 0x0B56 },   { 0x0B82, 0x0B82 },   { 0x0BC0, 0x0BC0 },
        { 0x0BCD, 0x0BCD },   { 0x0C3E, 0x0C40 },   { 0x0C46, 0x0C48 },
        { 0x0C4A, 0x0C4D },   { 0x0C55, 0x0C56 },   { 0x0CBC, 0x0CBC },
        { 0x0CBF, 0x0CBF },   { 0x0CC6, 0x0CC6 },   { 0x0CCC, 0x0CCD },
        { 0x0CE2, 0x0CE3 },   { 0x0D41, 0x0D43 },   { 0x0D4D, 0x0D4D },
        { 0x0DCA, 0x0DCA },   { 0x0DD2, 0x0DD4 },   { 0x0DD6, 0x0DD6 },
        { 0x0E31, 0x0E31 },   { 0x0E34, 0x0E3A },   { 0x0E47, 0x0E4E },
        { 0x0EB1, 0x0EB1 },   { 0x0EB4, 0x0EB9 },   { 0x0EBB, 0x0EBC },
        { 0x0EC8, 0x0ECD },   { 0x0F18, 0x0F19 },   { 0x0F35, 0x0F35 },
        { 0x0F37, 0x0F37 },   { 0x0F39, 0x0F39 },   { 0x0F71, 0x0F7E },
        { 0x0F80, 0x0F84 },   { 0x0F86, 0x0F87 },   { 0x0F90, 0x0F97 },
        { 0x0F99, 0x0FBC },   { 0x0FC6, 0x0FC6 },   { 0x102D, 0x1030 },
        { 0x1032, 0x1032 },   { 0x1036, 0x1037 },   { 0x1039, 0x1039 },
        { 0x1058, 0x1059 },   { 0x1160, 0x11FF },   { 0x135F, 0x135F },
        { 0x1712, 0x1714 },   { 0x1732, 0x1734 },   { 0x1752, 0x1753 },
        { 0x1772, 0x1773 },   { 0x17B4, 0x17B5 },   { 0x17B7, 0x17BD },
        { 0x17C6, 0x17C6 },   { 0x17C9, 0x17D3 },   { 0x17DD, 0x17DD },
        { 0x180B, 0x180D },   { 0x18A9, 0x18A9 },   { 0x1920, 0x1922 },
        { 0x1927, 0x1928 },   { 0x1932, 0x1932 },   { 0x1939, 0x193B },
        { 0x1A17, 0x1A18 },   { 0x1B00, 0x1B03 },   { 0x1B34, 0x1B34 },
        { 0x1B36, 0x1B3A },   { 0x1B3C, 0x1B3C },   { 0x1B42, 0x1B42 },
        { 0x1B6B, 0x1B73 },   { 0x1DC0, 0x1DCA },   { 0x1DFE, 0x1DFF },
        { 0x200B, 0x200F },   { 0x202A, 0x202E },   { 0x2060, 0x2063 },
        { 0x206A, 0x206F },   { 0x20D0, 0x20EF },   { 0x302A, 0x302F },
        { 0x3099, 0x309A },   { 0xA806, 0xA806 },   { 0xA80B, 0xA80B },
        { 0xA825, 0xA826 },   { 0xFB1E, 0xFB1E },   { 0xFE00, 0xFE0F },
        { 0xFE20, 0xFE23 },   { 0xFEFF, 0xFEFF },   { 0xFFF9, 0xFFFB },
        { 0x10A01, 0x10A03 }, { 0x10A05, 0x10A06 }, { 0x10A0C, 0x10A0F },
        { 0x10A38, 0x10A3A }, { 0x10A3F, 0x10A3F }, { 0x1D167, 0x1D169 },
        { 0x1D173, 0x1D182 }, { 0x1D185, 0x1D18B }, { 0x1D1AA, 0x1D1AD },
        { 0x1D242, 0x1D244 }, { 0xE0001, 0xE0001 }, { 0xE0020, 0xE007F },
        { 0xE0100, 0xE01EF }
    };

    /* We convert wchar_t to int32_t to avoid compiler warnings
     * about implicit integer conversions
     * https://github.com/seleznevae/libfort/issues/20
     *
     * note: didn't test if we can do it
     */
    int32_t ucs = (int32_t)wcs;

    /* test for 8-bit control characters */
    if(ucs == 0)
        return 0;
    if(ucs < 32 || (ucs >= 0x7f && ucs < 0xa0))
        return -1;

    /* binary search in table of non-spacing characters */
    if(bisearch(
           ucs, combining, sizeof(combining) / sizeof(struct interval) - 1))
        return 0;

    /* if we arrive here, ucs is not a combining or C0/C1 control character */

    return 1 +
           (ucs >= 0x1100 &&
            (ucs <= 0x115f || /* Hangul Jamo init. consonants */
             ucs == 0x2329 || ucs == 0x232a ||
             (ucs >= 0x2e80 && ucs <= 0xa4cf &&
              ucs != 0x303f) ||                  /* CJK ... Yi */
             (ucs >= 0xac00 && ucs <= 0xd7a3) || /* Hangul Syllables */
             (ucs >= 0xf900 &&
              ucs <= 0xfaff) || /* CJK Compatibility Ideographs */
             (ucs >= 0xfe10 && ucs <= 0xfe19) || /* Vertical forms */
             (ucs >= 0xfe30 && ucs <= 0xfe6f) || /* CJK Compatibility Forms */
             (ucs >= 0xff00 && ucs <= 0xff60) || /* Fullwidth Forms */
             (ucs >= 0xffe0 && ucs <= 0xffe6) ||
             (ucs >= 0x20000 && ucs <= 0x2fffd) ||
             (ucs >= 0x30000 && ucs <= 0x3fffd)));
}

FT_INTERNAL
int mk_wcswidth(const wchar_t* pwcs, size_t n)
{
    int width = 0;

    for(; *pwcs && n-- > 0; pwcs++) {
        int w;
        if((w = mk_wcwidth(*pwcs)) < 0)
            return -1;
        else
            width += w;
    }

    return width;
}
#endif /* FT_HAVE_WCHAR */

/********************************************************
   End of file "wcwidth.c"
 ********************************************************/
