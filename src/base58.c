/*
	base58 lua module
	compile with:
		gcc -Wall -shared -fPIC -o base58.so base58.c -ltarantool
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tarantool/lua.h>
#include <tarantool/lauxlib.h>

char *nb58 = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
/****************************************************************************************************************************************************
 * Following code was generated with:                                                                                                                                  *
 * perl -E 'my $bb = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ"; for (0..255) { $i = index $bb, chr($_); print("$i, "); } say ""' *
 ***************************************************************************************************************************************************/
char b58n[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
	-1, 34, 35, 36, 37, 38, 39, 40, 41, -1, 42, 43, 44, 45, 46, -1,
	47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
	-1,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, -1, 20, 21, 22,
	23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};


static int
encode_base58(struct lua_State *L) {
	size_t len;
	unsigned char *input = (unsigned char *) lua_tolstring(L, 1, &len);
	if(len < 1) { lua_pushlstring(L, "", 0); return 1; }
	size_t rlen = (len / 2 + 1) * 3;
	unsigned char *ret = malloc(rlen + len);
	if(!ret) { luaL_error(L, "Can't allocate memory for buffer"); }
	unsigned char *src = ret + rlen;
	memcpy(src, input, len);
	unsigned char *rptr = ret + rlen;
	unsigned char *ptr, *e = src + len - 1;
	unsigned char rest;
	unsigned int c;
	while(src <= e ) {
		if(*src) {
			rest = 0;
			ptr = src;
			while(ptr <= e) {
				c = rest * 256;
				rest = (c + *ptr) % 58; // (rest * 256 + *ptr) % 58
				*ptr = (c + *ptr) / 58; // (rest * 256 + *ptr) % 58
				ptr++;
			}
			--rptr; *rptr = nb58[rest];
		} else {
			src++;
		}
	}
	lua_pushlstring(L, (char *) rptr, ret + rlen - rptr);
	free(ret);
	return 1;
}

static int
decode_base58(struct lua_State *L) {
	size_t len;
	unsigned char *src = (unsigned char *) lua_tolstring(L, 1, &len);
	size_t rlen = (len / 4 + 1) * 3;
	unsigned char *ret = malloc(rlen);
	if(!ret) { luaL_error(L, "Can't allocate memory for buffer"); }
	unsigned char *rptr = ret + rlen;
	size_t i;
	unsigned int c;
	unsigned char *ptr;
	char rest;
	for(i = 0; i < len; i++) {
		rest = b58n[src[i]];
		if(rest < 0) {
			free(ret);
			luaL_error(L, "Bad simbol in base58 string");
		}
		for(ptr = ret + rlen - 1; ptr >= rptr; ptr--) {
			c = rest + *ptr * 58;
			*ptr = c % 256;
			rest = c / 256;
		}
		if(rest > 0) {
			rptr--;
			if(rptr < ret) { luaL_error(L, "Error in "); }
			*rptr = rest;
		}
	}
	
	lua_pushlstring(L, (char *) rptr, ret + rlen - rptr);
	free(ret);
	return 1;
}

LUA_API int
luaopen_base58(lua_State *L) {
	lua_pushcfunction(L, encode_base58);
	lua_setglobal(L, "encode_base58");
	lua_pushcfunction(L, decode_base58);
	lua_setglobal(L, "decode_base58");
	return 0;
}

