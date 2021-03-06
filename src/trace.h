#ifndef __TRACE_H__
#define __TRACE_H__

void uart_char(unsigned int ra);
void uart_str(const char* str);
void uart_strln(const char* str);

#ifdef TRACE
void taskSwitchOut(void * taskHandle);
void taskSwitchIn(void * taskHandle);

#define traceTASK_SWITCHED_OUT() taskSwitchOut( (void*)pxCurrentTCB );
#define traceTASK_SWITCHED_IN() taskSwitchIn( (void*)pxCurrentTCB );

#endif // TRACE

#endif
