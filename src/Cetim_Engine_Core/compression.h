#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <zlib.h>

class compression_manager
{
private:
    z_stream zs;

    unsigned char power = 2;

    void start_comp()
    {
        zs.zalloc = Z_NULL;
        zs.zfree = Z_NULL;
        zs.opaque = Z_NULL;

        int ret;

        switch (power)
        {
        case 0:
            ret = deflateInit(&zs, Z_NO_COMPRESSION);
            break;
        case 1:
            ret = deflateInit(&zs, Z_BEST_SPEED);
            break;
        case 2:
            ret = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
            break;
        case 3:
            ret = deflateInit(&zs, Z_BEST_COMPRESSION);
            break;
        }

        if (ret != Z_OK)
            throw std::runtime_error("Failed to initialize zlib for compression");
    }

    void end_comp()
    {
        deflateEnd(&zs);
    }

public:
    compression_manager(unsigned char power = 2)
    {
        this->power = power;
    }

    std::string getZlibErrorMessage(int err_code)
    {
        switch (err_code)
        {
        case Z_OK:
            return "Success";
        case Z_STREAM_END:
            return "End of stream";
        case Z_NEED_DICT:
            return "Need dictionary";
        case Z_ERRNO:
            return "File error";
        case Z_STREAM_ERROR:
            return "Stream error";
        case Z_DATA_ERROR:
            return "Data error";
        case Z_MEM_ERROR:
            return "Memory error";
        case Z_BUF_ERROR:
            return "Buffer error";
        case Z_VERSION_ERROR:
            return "Version error";
        default:
            return "Unknown error";
        }
    }

    std::string compress(const std::string &data)
    {
        start_comp();
        std::string compressed_data;
        int ret;

        zs.avail_in = data.size();
        zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));

        // Reservar espaço suficiente para armazenar os dados comprimidos
        compressed_data.resize(data.size() * 1.1 + 12);

        zs.avail_out = compressed_data.size();
        zs.next_out = reinterpret_cast<Bytef *>(const_cast<char *>(compressed_data.data()));

        ret = deflate(&zs, Z_FINISH);
        if (ret != Z_STREAM_END)
        {
            deflateEnd(&zs);
            throw std::runtime_error("Failed to compress data with zlib: " + getZlibErrorMessage(ret));
        }

        // Redimensiona a string para o tamanho real dos dados comprimidos
        compressed_data.resize(compressed_data.size() - zs.avail_out);

        end_comp();
        return compressed_data;
    }

    std::string decompress(const std::string &data)
    {
        start_comp();
        std::string decompressed_data;
        int ret;

        zs.avail_in = data.size();
        zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));

        // Assume decompressed size may be up to 10 times larger than compressed size
        decompressed_data.resize(data.size() * 10);

        zs.avail_out = decompressed_data.size();
        zs.next_out = reinterpret_cast<Bytef *>(const_cast<char *>(decompressed_data.data()));

        ret = inflateInit(&zs);
        if (ret != Z_OK)
            throw std::runtime_error("Failed to initialize zlib for decompression: " + std::string(getZlibErrorMessage(ret)));

        do
        {
            ret = inflate(&zs, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END)
            {
                inflateEnd(&zs);
                throw std::runtime_error("Failed to decompress data with zlib: " + std::string(getZlibErrorMessage(ret)));
            }
        } while (zs.avail_out == 0);

        inflateEnd(&zs);

        decompressed_data.resize(decompressed_data.size() - zs.avail_out);

        end_comp();

        return decompressed_data;
    }
};

