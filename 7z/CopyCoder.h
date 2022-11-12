// Compress/CopyCoder.h

#ifndef __COMPRESS_COPY_CODER_H
#define __COMPRESS_COPY_CODER_H

#include "MyCom.h"

#include "ICoder.h"

namespace NCompress {

class CCopyCoder:
  public ICompressCoder,
  public ICompressGetInStreamProcessedSize,
  public CMyUnknownImp
{
  Byte *_buffer;
  GUID IID_ICompressGetInStreamProcessedSize = { 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x24, 0x00, 0x00 };
public:
  UInt64 TotalSize;
  CCopyCoder(): TotalSize(0) , _buffer(0) {};
  ~CCopyCoder();

  MY_UNKNOWN_IMP1(ICompressGetInStreamProcessedSize)

  STDMETHOD(Code)(ISequentialInStream *inStream, ISequentialOutStream *outStream,
      const UInt64 *inSize, const UInt64 *outSize, ICompressProgressInfo *progress);
  STDMETHOD(GetInStreamProcessedSize)(UInt64 *value);
};

}

#endif
