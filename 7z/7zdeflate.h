#ifndef __7ZDEFLATE_H_
#define __7ZDEFLATE_H_
bool compress_rfc1950_7z(const unsigned char *in_data,
                         unsigned in_size,
                         unsigned char *out_data,
                         unsigned &out_size) throw ();
#endif
