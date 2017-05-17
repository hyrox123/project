

/* MD5.H - header file for MD5C.C
*/
#ifndef _MS_MD5_H_
#define _MS_MD5_H_
/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software.
*/
/* POINTER defines a generic pointer type */
typedef unsigned char *MS_POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int MS_UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int MS_UINT4;
/* MD5 context. */

typedef struct {
    MS_UINT4 state[4];                                   /* state (ABCD) */
    MS_UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];                         /* input buffer */
} MS_MD5_CTX;

#ifdef __cplusplus
extern "C"
{
#endif
    void MD5Init (MS_MD5_CTX *);
    void MD5Update(MS_MD5_CTX *, unsigned char *, unsigned int);
    void MD5Final (unsigned char [16], MS_MD5_CTX *);
    void mingsoft_md5_passwd(unsigned char *oldpasswd,unsigned char *md5_passwd,unsigned int len);
    void MD5_memcpy(MS_POINTER output,MS_POINTER input,unsigned int len);
    void MD5Transform (MS_UINT4 state[4],unsigned char block[64]);
    void Encode (unsigned char *output,MS_UINT4 *input,unsigned int len);
    void MD5_memset (MS_POINTER output,int value,unsigned int len);
    void Decode (MS_UINT4 *output,unsigned char *input,unsigned int len);
#ifdef __cplusplus
}
#endif
#endif


