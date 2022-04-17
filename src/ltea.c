#include "lua.h"
#include "lauxlib.h"

#include "simplecrypto.h"

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#elif
#define DLLEXPORT
#endif /* _WIN32 */


int64_t encrypt_qq_len(int64_t src_len)
{
    int64_t fill = 10 - (src_len + 1) % 8;
    return fill + src_len + 7;
}


static int
lencrypt_qq(lua_State *L)
{
    size_t key_len;
    const char *key = luaL_checklstring(L, 1, &key_len);
    if (key_len != 16)
    {
        luaL_error(L, "key must be 16 bytes len!\n");
        return 0;
    }
    size_t src_len;
    const char *src = luaL_checklstring(L, 2, &src_len);
    int64_t out_len = encrypt_qq_len((int64_t) src_len);
    uint8_t *out = (uint8_t *) lua_newuserdata(L, (size_t) out_len);
    int64_t buffer_updated = tea_encrypt_qq((uint32_t *) key, (uint8_t *) src, (int64_t) src_len, out, out_len);
    if (buffer_updated < 0)
    {
        luaL_error(L, "encrypt wrong\n");
        return 0;
    }
    lua_pushlstring(L, (const char *) out, (size_t) buffer_updated);
    return 1;
}

static int
lencrypt(lua_State *L)
{
    size_t key_len;
    const char *key = luaL_checklstring(L, 1, &key_len);
    if (key_len != 16)
    {
        luaL_error(L, "key must be 16 bytes len!\n");
        return 0;
    }
    size_t sumtable_len;
    const char *sumtable = luaL_checklstring(L, 2, &sumtable_len);
    if (sumtable_len != 64)
    {
        luaL_error(L, "sum table must be 64 bytes len!\n");
        return 0;
    }

    size_t src_len;
    const char *src = luaL_checklstring(L, 3, &src_len);
    int64_t out_len = encrypt_qq_len((int64_t) src_len);
    uint8_t *out = (uint8_t *) lua_newuserdata(L, (size_t) out_len);

    int64_t buffer_updated = tea_encrypt((uint32_t *) key, (uint32_t *) sumtable, (const uint8_t *) src,
                                         (int64_t) src_len, out, out_len);
    if (buffer_updated < 0)
    {
        luaL_error(L, "encrypt wrong\n");
        return 0;
    }
    lua_pushlstring(L, (const char *) out, (size_t) buffer_updated);
    return 1;
}

static int
lencrypt_native_endian(lua_State *L)
{
    size_t key_len;
    const char *key = luaL_checklstring(L, 1, &key_len);
    if (key_len != 16)
    {
        luaL_error(L, "key must be 16 bytes len!\n");
        return 0;
    }
    size_t sumtable_len;
    const char *sumtable = luaL_checklstring(L, 2, &sumtable_len);
    if (sumtable_len != 64)
    {
        luaL_error(L, "sum table must be 64 bytes len!\n");
        return 0;
    }

    size_t src_len;
    const char *src = luaL_checklstring(L, 3, &src_len);
    int64_t out_len = encrypt_qq_len((int64_t) src_len);
    uint8_t *out = (uint8_t *) lua_newuserdata(L, (size_t) out_len);

    int64_t buffer_updated = tea_encrypt_native_endian((uint32_t *) key, (uint32_t *) sumtable, (const uint8_t *) src,
                                                       (int64_t) src_len, out, out_len);
    if (buffer_updated < 0)
    {
        luaL_error(L, "encrypt wrong\n");
        return 0;
    }
    lua_pushlstring(L, (const char *) out, (size_t) buffer_updated);
    return 1;
}

static int
ldecrypt_qq(lua_State *L)
{
    size_t key_len;
    const char *key = luaL_checklstring(L, 1, &key_len);
    if (key_len != 16)
    {
        luaL_error(L, "key must be 16 bytes len!\n");
        return 0;
    }
    size_t src_len;
    const char *src = luaL_checklstring(L, 2, &src_len);
    uint8_t *out = (uint8_t *) lua_newuserdata(L, (size_t) src_len);

    int64_t buffer_updated = tea_decrypt_qq((uint32_t *) key, (const uint8_t *) src, (int64_t) src_len, out,
                                            (int64_t) src_len);
    if (buffer_updated < 0)
    {
        luaL_error(L, "decrypt wrong\n");
        return 0;
    }
    lua_pushlstring(L, (const char *) out, (size_t) buffer_updated);
    return 1;
}

static int
ldecrypt(lua_State *L)
{
    size_t key_len;
    const char *key = luaL_checklstring(L, 1, &key_len);
    if (key_len != 16)
    {
        luaL_error(L, "key must be 16 bytes len!\n");
        return 0;
    }
    size_t sumtable_len;
    const char *sumtable = luaL_checklstring(L, 2, &sumtable_len);
    if (sumtable_len != 64)
    {
        luaL_error(L, "sum table must be 64 bytes len!\n");
        return 0;
    }
    size_t src_len;
    const char *src = luaL_checklstring(L, 3, &src_len);
    uint8_t *out = (uint8_t *) lua_newuserdata(L, (size_t) src_len);
    int64_t buffer_updated = tea_decrypt((uint32_t *) key, (uint32_t *) sumtable, (const uint8_t *) src,
                                         (int64_t) src_len, out, (int64_t) src_len);
    if (buffer_updated < 0)
    {
        luaL_error(L, "decrypt wrong\n");
        return 0;
    }
    lua_pushlstring(L, (const char *) out, (size_t) buffer_updated);
    return 1;
}

static int
ldecrypt_native_endian(lua_State *L)
{
    size_t key_len;
    const char *key = luaL_checklstring(L, 1, &key_len);
    if (key_len != 16)
    {
        luaL_error(L, "key must be 16 bytes len!\n");
        return 0;
    }
    size_t sumtable_len;
    const char *sumtable = luaL_checklstring(L, 2, &sumtable_len);
    if (sumtable_len != 64)
    {
        luaL_error(L, "sum table must be 64 bytes len!\n");
        return 0;
    }
    size_t src_len;
    const char *src = luaL_checklstring(L, 3, &src_len);
    uint8_t *out = (uint8_t *) lua_newuserdata(L, (size_t) src_len);
    int64_t buffer_updated = tea_decrypt_native_endian((uint32_t *) key, (uint32_t *) sumtable, (const uint8_t *) src,
                                         (int64_t) src_len, out, (int64_t) src_len);
    if (buffer_updated < 0)
    {
        luaL_error(L, "decrypt wrong\n");
        return 0;
    }
    lua_pushlstring(L, (const char *) out, (size_t) buffer_updated);
    return 1;
}

static luaL_Reg lua_funcs[] = {
        {"encrypt_qq",            &lencrypt_qq},
        {"encrypt",               &lencrypt},
        {"encrypt_native_endian", &lencrypt_native_endian},
        {"decrypt_qq",            &ldecrypt_qq},
        {"decrypt",               &ldecrypt},
        {"decrypt_native_endian", &ldecrypt_native_endian},
        {NULL, NULL}
};

DLLEXPORT int luaopen_tea(lua_State *L)
{
    luaL_newlib(L, lua_funcs);
    return 1;
}