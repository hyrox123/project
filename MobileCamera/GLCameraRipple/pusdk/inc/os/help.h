#ifndef __HELP_H__
#define __HELP_H__

#ifdef __cplusplus
extern "C"
{
#endif

// Helper functions

int print_outmsg(const char *, ...);


#define LOG print_outmsg


#ifdef __cplusplus
}
#endif

#endif
