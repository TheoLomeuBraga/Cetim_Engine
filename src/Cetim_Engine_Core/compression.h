#pragma once
#include <stdexcept>
#include <zlib.h>

class compression_manager
{
private:
    z_stream zs;
public:
    void start()
    {
        zs.zalloc = Z_NULL;
        zs.zfree = Z_NULL;
        zs.opaque = Z_NULL;

        if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK)
            throw std::runtime_error("zlib fail to start");
    }

    void end()
    {
        deflateEnd(&zs);
    }
};
