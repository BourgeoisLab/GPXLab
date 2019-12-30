#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Changes an environment variable
 * @param name Variable
 * @param value Value
 */
void UTILS_setenv(const char *name, const char *value);

/**
 * @brief Seletes the variable name from the environment
 * @param name Variable
 */
void UTILS_unsetenv(const char *name);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
