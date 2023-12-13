#ifndef __H_QUOTE
#define __H_QUOTE
/**
 * quote input string and returns a
 * new allocated string
 * @param pInBuf [in] input string
 * @return returns allocated string
 */
char *quote_string(const char *pInBuf);
/**
 * dequote stringand returns a dequoted string
 * this required a leading " and trailing "
 * else the input buffer will be returned
 * @param pInBuf [in] input string
 * @return returns allocated string
 */
char *dequote_string(const char *pInBuf);
#endif //__H_QUOTE
