
#include "prefix.h"

#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <Ws2tcpip.h>
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif


#ifndef max
#define max(a,b) (((a) (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


// convert mask 8 -> 0xFF 00 00 00
static uint32_t maskbit2host(uint8_t maskbit)
{
    const uint8_t max_mask = sizeof(uint32_t) * 8;
    uint8_t mask;
    uint32_t v;

    mask = min(max_mask, maskbit);
    v = (0x01 << (max_mask - mask))-1;
    v = ~v;
    return v;
}

// floor the p->mask
static void prefix_floor_mask(struct prefix* p)
{
    uint32_t h;
    uint32_t n;
    uint32_t maskh;
    const uint8_t max_mask = sizeof(uint32_t) * 8;

    maskh = maskbit2host(p->maskbit);
    h = ntohl(p->sin);
    h = h & maskh;
    n = htonl(h);
    if (n != p->sin) {
        p->sin = n;
        p->maskbit = min(max_mask, p->maskbit);
        inet_ntop(AF_INET, &p->sin, p->sin_str, sizeof(p->sin_str));
    }
}

/* Convert 127.0.0.1/32 to network order prefix struct.
    If only give 127.0.0.1, then the mask default is 32.
  依据掩码做格式化，比如  10.1.0.0/9，被格式化为  10.0.0.0/9.

  Return 0 on success.
*/
int prefix_format(struct prefix* p, const char* str)
{
    const char* sep;

    for (sep = str; (*sep != 0) && (*sep != '/'); sep += 1) {

    }

    if (*sep == 0) {
        return -1;
    }
    snprintf(p->sin_str, sizeof(p->sin_str), "%.*s", (int)(sep-str), str);
    sep++;
    if (*sep == 0) {
        return -1;
    }
    char s[10] = { 0 };
    snprintf(s, sizeof(s), "%s", sep);
    uint64_t number;
    number = (uint64_t)strtol(s, 0, 10);
    if (number >= UINT8_MAX) {
        return -1;
    }
    p->maskbit = (uint8_t)number;
    inet_pton(AF_INET, p->sin_str, &p->sin);
    prefix_floor_mask(p);
    snprintf(p->string, sizeof(p->string), "%s/%u", p->sin_str, p->maskbit);
    p->host = ntohl(p->sin);
    return 0;
}

int prefix_fprintf(struct prefix* p, FILE* f)
{
    return fprintf(f, "%s", p->string);
}

bool prefix_cmp(struct prefix * p1, struct prefix * p2)
{
    if(p1->maskbit != p2->maskbit){
        return false;
    }
    uint32_t h1 = p1->host;
    uint32_t h2 = p2->host;
    uint32_t mask;
    mask = 0xFFFFFFFFu;
    mask = mask >> p1->maskbit;
    mask = ~mask;
    h1 = h1 & mask;
    h2 = h2 & mask;
    return h1 == h2;
}