#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdbool.h>

/**
 * \defgroup ErrorMessages Return messages for perror
 * @{
 */

 /**
  * @brief Memory error code
  * Indicates a generic memory error
  */
#define MEM_CODE "memoryError"

  /**
   * @brief File error code
   * Indicates a generic file error
   */
#define FILE_CODE "fileError"

   /**@}*/

   /**
    * \defgroup ErrorCodes Return codes indicating status
    * @{
    */

    /**
     * @brief File open return code
     * Indicates that there was an issue openning a file
     */
#define FILE_OPEN 5

     /**
      * @brief Miscellaneous return code
      * Indicates that there was an error somewhere in a function return
      */
#define BAD_ARGUMENT 4

      /**
       * @brief No parameters return code
       * Indicates that no or insufficient parameters were given for a function.
       */
#define NO_PARAM 3

       /**
        * @brief Thread error return code
        * Indicates that a thread returned an error
        */
#define THREAD_ERROR 2

        /**
         * @brief Memory allocation return code
         * Indicates that memory allocation was not successful
         */
#define ALLOCATION_ERROR 1

         /**
          * @brief Success return code
          * Indicates function has returned with no errors
          */
#define SUCCESS 0
          /**@}*/

typedef int err_t;

          //  Memory or file check
#define VALID(check, code, err) \
    if ((check) == NULL) { \
        fprintf(stderr, "%s: %s\n", code, strerror(errno)); \
        perror(code); \
        f_ret.ret_val.error = err; \
        goto exit; \
    }

#define HANDLE_ERR(check, code) \
    if ((check) != 0) { \
        fprintf(stderr, "%s: %s\n", code, strerror(errno)); \
        perror(code); \
        f_ret.ret_val.error = THREAD_ERROR; \
        goto exit; \
    }

//  Memory alloc, checking, and allignment
#define MEM(ptr, size, type) \
    ptr = (type *)malloc(size*sizeof(type)); \
    VALID(ptr, MEM_CODE, ALLOCATION_ERROR); \
    memset(ptr, 0, size*sizeof(type));

#define MEM_(ptr, size, type) \
    type *tmp = (ptr); \
    tmp = (type *)realloc(tmp, (size)*sizeof(type)); \
    VALID(tmp, MEM_CODE, ALLOCATION_ERROR); \
    (ptr) = tmp;

#define SFREE(ptr) \
    if (ptr) \
        free(ptr); \
    ptr = NULL;

//  Return condition check
#define CHECK(F) \
    f_ret = F; \
    if (f_ret.ret_type == ERROR) { \
        if (f_ret.ret_val.error > 0) \
            goto exit; \
    }

#define HANDLE(c, s, v, err) \
    if (c s v) { \
        f_ret.ret_type = ERROR; \
        f_ret.ret_val.error = err; \
        goto exit; \
    }

#endif // _UTILS_H_