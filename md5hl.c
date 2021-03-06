/* md5hl.c
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@login.dkuug.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 *
 * $Id: md5hl.c,v 1.8.2.1 1998/02/18 02:24:05 jkh Exp $
 *
 */

#include <sys/types.h>
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "md5.h"
#include "error.h"

char *
MD5End(MD5_CTX *ctx, char *buf)
{
    int i;
    unsigned char digest[MD5_HASHBYTES];
    static const char hex[]="0123456789abcdef";

    if (!buf)
        buf = mem_alloc(33);
    if (!buf)
	return 0;
    MD5Final(digest,ctx);
    for (i=0;i<MD5_HASHBYTES;i++) {
	buf[i+i] = hex[digest[i] >> 4];
	buf[i+i+1] = hex[digest[i] & 0x0f];
    }
    buf[i+i] = '\0';
    return buf;
}

char *
MD5File (const char *filename, char *buf)
{
    unsigned char buffer[BUFSIZ];
    MD5_CTX ctx;
    int i,j;
	FILE *f;

    MD5Init(&ctx);

#ifndef WIN32
    if (!(f = fopen(filename, "r")))
#else
	printf("%s\n", filename);for(;;);
	if (fopen_s(&f, filename, "r") != 0) /* tady to hnije .. */
#endif
	return 0;
    while ((i = fread(buffer,sizeof buffer,1,f)) > 0) {
	MD5Update(&ctx,buffer,i);
    }
    j = errno;
    fclose(f);
    errno = j;
    if (i < 0) return 0;
    return MD5End(&ctx, buf);
}

char *
MD5Data (const unsigned char *data, unsigned int len, char *buf)
{
    MD5_CTX ctx;

    MD5Init(&ctx);
    MD5Update(&ctx,data,len);
    return MD5End(&ctx, buf);
}
