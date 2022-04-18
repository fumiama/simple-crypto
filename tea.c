// https://github.com/Mrs4s/MiraiGo/blob/master/binary/tea.go

#include <stdlib.h>
#include <string.h>
#if !__APPLE__
	#include <endian.h>
#else
	#include <machine/endian.h>
#endif
#include "simplecrypto.h"

const static uint32_t qqsumtable[0x10] = {
	0x9e3779b9,
	0x3c6ef372,
	0xdaa66d2b,
	0x78dde6e4,
	0x1715609d,
	0xb54cda56,
	0x5384540f,
	0xf1bbcdc8,
	0x8ff34781,
	0x2e2ac13a,
	0xcc623af3,
	0x6a99b4ac,
	0x08d12e65,
	0xa708a81e,
	0x454021d7,
	0xe3779b90,
};

int64_t tea_encrypt_qq(const TEA t[4], const TEADAT* src, TEADAT* out) {
	int64_t lens = src->len;
	int64_t fill = 10 - (lens+1)%8;
	int64_t dstlen = fill+lens+7;
	uint8_t* dstdat = (uint8_t*)malloc(dstlen);
	((uint32_t*)dstdat)[0] = rand();
	((uint32_t*)dstdat)[1] = rand();
	((uint32_t*)dstdat)[2] = rand();
	dstdat[0] = (fill-3)|0xF8; // 存储pad长度
	memcpy(dstdat+fill, src->data, lens);
	uint64_t iv1 = 0, iv2 = 0, holder;
	for(int64_t i = 0; i < dstlen/8; i++) {
		#ifdef WORDS_BIGENDIAN
			uint64_t block = ((uint64_t*)dstdat)[i];
		#else
			uint64_t block = __builtin_bswap64(((uint64_t*)dstdat)[i]);
		#endif
		holder = block ^ iv1;
		iv1 = holder;
		uint32_t v1 = holder;
		iv1 >>= 32;
		uint32_t v0 = iv1;
		for (int i = 0; i < 0x10; i++) {
			v0 += (v1 + qqsumtable[i]) ^ ((v1 << 4) + t[0]) ^ ((v1 >> 5) + t[1]);
			v1 += (v0 + qqsumtable[i]) ^ ((v0 << 4) + t[2]) ^ ((v0 >> 5) + t[3]);
		}
		iv1 = ((uint64_t)v0<<32) | (uint64_t)v1;
		iv1 = iv1 ^ iv2;
		iv2 = holder;
		#ifdef WORDS_BIGENDIAN
			((uint64_t*)dstdat)[i] = iv1;
		#else
			((uint64_t*)dstdat)[i] = __builtin_bswap64(iv1);
		#endif
	}
	out->len = dstlen;
	out->data = dstdat;
	out->ptr = dstdat;
	return dstlen;
}

int64_t tea_encrypt(const TEA t[4], const uint32_t sumtable[0x10], const TEADAT* src, TEADAT* out) {
	int64_t lens = src->len;
	int64_t fill = 10 - (lens+1)%8;
	int64_t dstlen = fill+lens+7;
	uint8_t* dstdat = (uint8_t*)malloc(dstlen);
	((uint32_t*)dstdat)[0] = rand();
	((uint32_t*)dstdat)[1] = rand();
	((uint32_t*)dstdat)[2] = rand();
	dstdat[0] = (fill-3)|0xF8; // 存储pad长度
	memcpy(dstdat+fill, src->data, lens);
	uint64_t iv1 = 0, iv2 = 0, holder;
	for(int64_t i = 0; i < dstlen/8; i++) {
		#ifdef WORDS_BIGENDIAN
			uint64_t block = ((uint64_t*)dstdat)[i];
		#else
			uint64_t block = __builtin_bswap64(((uint64_t*)dstdat)[i]);
		#endif
		holder = block ^ iv1;
		iv1 = holder;
		uint32_t v1 = holder;
		iv1 >>= 32;
		uint32_t v0 = iv1;
		for (int i = 0; i < 0x10; i++) {
			v0 += (v1 + sumtable[i]) ^ ((v1 << 4) + t[0]) ^ ((v1 >> 5) + t[1]);
			v1 += (v0 + sumtable[i]) ^ ((v0 << 4) + t[2]) ^ ((v0 >> 5) + t[3]);
		}
		iv1 = ((uint64_t)v0<<32) | (uint64_t)v1;
		iv1 = iv1 ^ iv2;
		iv2 = holder;
		#ifdef WORDS_BIGENDIAN
			((uint64_t*)dstdat)[i] = iv1;
		#else
			((uint64_t*)dstdat)[i] = __builtin_bswap64(iv1);
		#endif
	}
	out->len = dstlen;
	out->data = dstdat;
	out->ptr = dstdat;
	return dstlen;
}

int64_t tea_encrypt_native_endian(const TEA t[4], const uint32_t sumtable[0x10], const TEADAT* src, TEADAT* out) {
	int64_t lens = src->len;
	int64_t fill = 10 - (lens+1)%8;
	int64_t dstlen = fill+lens+7;
	uint8_t* dstdat = (uint8_t*)malloc(dstlen);
	((uint32_t*)dstdat)[0] = rand();
	((uint32_t*)dstdat)[1] = rand();
	((uint32_t*)dstdat)[2] = rand();
	dstdat[0] = (fill-3)|0xF8; // 存储pad长度
	memcpy(dstdat+fill, src->data, lens);
	uint64_t iv1 = 0, iv2 = 0, holder;
	for(int64_t i = 0; i < dstlen/8; i++) {
		uint64_t block = ((uint64_t*)dstdat)[i];
		holder = block ^ iv1;
		iv1 = holder;
		uint32_t v1 = holder;
		iv1 >>= 32;
		uint32_t v0 = iv1;
		for (int i = 0; i < 0x10; i++) {
			v0 += (v1 + sumtable[i]) ^ ((v1 << 4) + t[0]) ^ ((v1 >> 5) + t[1]);
			v1 += (v0 + sumtable[i]) ^ ((v0 << 4) + t[2]) ^ ((v0 >> 5) + t[3]);
		}
		iv1 = ((uint64_t)v0<<32) | (uint64_t)v1;
		iv1 = iv1 ^ iv2;
		iv2 = holder;
		((uint64_t*)dstdat)[i] = iv1;
	}
	out->len = dstlen;
	out->data = dstdat;
	out->ptr = dstdat;
	return dstlen;
}

int64_t tea_decrypt_qq(const TEA t[4], const TEADAT* src, TEADAT* out) {
	if (src->len < 16 || (src->len)%8 != 0) return 0;
	uint8_t* dstdat = (uint8_t*)malloc(src->len);
	uint64_t iv1, iv2 = 0, holder = 0;
	for(int64_t i = 0; i < src->len/8; i++) {
		#ifdef WORDS_BIGENDIAN
			iv1 = ((uint64_t*)(src->data))[i];
		#else
			iv1 = __builtin_bswap64(((uint64_t*)(src->data))[i]);
		#endif
		iv2 ^= iv1;
		uint32_t v1 = iv2;
		iv2 >>= 32;
		uint32_t v0 = iv2;
		for (int i = 0x0f; i >= 0; i--) {
			v1 -= (v0 + qqsumtable[i]) ^ ((v0 << 4) + t[2]) ^ ((v0 >> 5) + t[3]);
			v0 -= (v1 + qqsumtable[i]) ^ ((v1 << 4) + t[0]) ^ ((v1 >> 5) + t[1]);
		}
		iv2 = ((uint64_t)v0<<32) | (uint64_t)v1;
		#ifdef WORDS_BIGENDIAN
			((uint64_t*)dstdat)[i] = iv2^holder;
		#else
			((uint64_t*)dstdat)[i] = __builtin_bswap64(iv2^holder);
		#endif
		holder = iv1;
	}
	int start = (dstdat[0]&7)+3;
	out->len = src->len-7-start;
	out->data = dstdat+start;
	out->ptr = dstdat;
	return out->len;
}

int64_t tea_decrypt(const TEA t[4], const uint32_t sumtable[0x10], const TEADAT* src, TEADAT* out) {
	if (src->len < 16 || (src->len)%8 != 0) return 0;
	uint8_t* dstdat = (uint8_t*)malloc(src->len);
	uint64_t iv1, iv2 = 0, holder = 0;
	for(int64_t i = 0; i < src->len/8; i++) {
		#ifdef WORDS_BIGENDIAN
			iv1 = ((uint64_t*)(src->data))[i];
		#else
			iv1 = __builtin_bswap64(((uint64_t*)(src->data))[i]);
		#endif
		iv2 ^= iv1;
		uint32_t v1 = iv2;
		iv2 >>= 32;
		uint32_t v0 = iv2;
		for (int i = 0x0f; i >= 0; i--) {
			v1 -= (v0 + sumtable[i]) ^ ((v0 << 4) + t[2]) ^ ((v0 >> 5) + t[3]);
			v0 -= (v1 + sumtable[i]) ^ ((v1 << 4) + t[0]) ^ ((v1 >> 5) + t[1]);
		}
		iv2 = ((uint64_t)v0<<32) | (uint64_t)v1;
		#ifdef WORDS_BIGENDIAN
			((uint64_t*)dstdat)[i] = iv2^holder;
		#else
			((uint64_t*)dstdat)[i] = __builtin_bswap64(iv2^holder);
		#endif
		holder = iv1;
	}
	int start = (dstdat[0]&7)+3;
	out->len = src->len-7-start;
	out->data = dstdat+start;
	out->ptr = dstdat;
	return out->len;
}

int64_t tea_decrypt_native_endian(const TEA t[4], const uint32_t sumtable[0x10], const TEADAT* src, TEADAT* out) {
	if (src->len < 16 || (src->len)%8 != 0) return 0;
	uint8_t* dstdat = (uint8_t*)malloc(src->len);
	uint64_t iv1, iv2 = 0, holder = 0;
	for(int64_t i = 0; i < src->len/8; i++) {
		iv1 = ((uint64_t*)(src->data))[i];
		iv2 ^= iv1;
		uint32_t v1 = iv2;
		iv2 >>= 32;
		uint32_t v0 = iv2;
		for (int i = 0x0f; i >= 0; i--) {
			v1 -= (v0 + sumtable[i]) ^ ((v0 << 4) + t[2]) ^ ((v0 >> 5) + t[3]);
			v0 -= (v1 + sumtable[i]) ^ ((v1 << 4) + t[0]) ^ ((v1 >> 5) + t[1]);
		}
		iv2 = ((uint64_t)v0<<32) | (uint64_t)v1;
		((uint64_t*)dstdat)[i] = iv2^holder;
		holder = iv1;
	}
	int start = (dstdat[0]&7)+3;
	out->len = src->len-7-start;
	out->data = dstdat+start;
	out->ptr = dstdat;
	return out->len;
}

#ifdef TEST_SIMPLE_CRYPTO
int main(int argc, char **argv) {
	TEADAT* td = (TEADAT*)malloc(sizeof(TEADAT));
	TEA* t = (TEA*)"32107654BA98FEDC";
	TEADAT out;
 
	if (argc != 3) {
		printf("usage: %s -[e|d] 'string'\n", argv[0]);
		return 1;
	}
	switch(argv[1][1]) {
		case 'e':
			td->data = (uint8_t*)(argv[2]);
			td->len = strlen(argv[2]);
			tea_encrypt_qq(t, td, &out);
			// display result
			for (int i = 0; i < out.len; i++) printf("%02x", ((uint8_t*)(out.data))[i]);
			putchar('\n');
			free(out.ptr);
		break;
		case 'd':
			td->len = strlen(argv[2])/2;
			// printf("decode input len: %lld\n", td->len);
			td->data = malloc(td->len);
			int i = td->len;
			while (i--) {
				int x;
				sscanf(argv[2]+i*2, "%02x", &x);
				td->data[i] = x;
				argv[2][i*2] = 0;
			}
			if (tea_decrypt_qq(t, td, &out)) {
				out.data[out.len] = 0;
				// printf("decode output len: %lld\n", tdd->len);
				for (int i = 0; i < out.len; i++) putchar(out.data[i]);
				putchar('\n');
				free(out.ptr);
			} else puts("decode error!");
			free(td->data);
		break;
		default: break;
	}
	free(td);
	return 0;
}
#endif