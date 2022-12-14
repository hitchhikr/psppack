// DeflateEncoder.h

#ifndef __DEFLATE_ENCODER_H
#define __DEFLATE_ENCODER_H

extern "C"
{
  #include "LzFind.h"
}

#include "MyWindows.h"
#include "MyCom.h"

#include "ICoder.h"

#include "BitlEncoder.h"
#include "DeflateConst.h"

namespace NCompress {
namespace NDeflate {
namespace NEncoder {

struct CCodeValue
{
  UInt16 Len;
  UInt16 Pos;
  void SetAsLiteral() { Len = (1 << 15); }
  bool IsLiteral() const { return (Len >= (1 << 15)); }
};

struct COptimal
{
  UInt32 Price;
  UInt16 PosPrev;
  UInt16 BackPrev;
};

const UInt32 kNumOptsBase = 1 << 12;
const UInt32 kNumOpts = kNumOptsBase + kMatchMaxLen;

class CCoder;

struct CTables: public CLevels
{
  bool UseSubBlocks;
  bool StoreMode;
  bool StaticMode;
  UInt32 BlockSizeRes;
  UInt32 m_Pos;
  void InitStructures();
};

typedef struct _CSeqInStream
{
  ISeqInStream SeqInStream;
  CMyComPtr<ISequentialInStream> RealStream;
} CSeqInStream;

class CCoder
{
  CMatchFinder _lzInWindow;
  CBitlEncoder m_OutStream;

  CSeqInStream _seqInStream;

public:
  CCodeValue *m_Values;

  UInt16 *m_MatchDistances;
  UInt32 m_NumFastBytes;
  bool _fastMode;
  bool _btMode;

  UInt16 *m_OnePosMatchesMemory;
  UInt16 *m_DistanceMemory;

  UInt32 m_Pos;

  int m_NumPasses;
  int m_NumDivPasses;
  bool m_CheckStatic;
  bool m_IsMultiPass;
  UInt32 m_ValueBlockSize;

  UInt32 m_NumLenCombinations;
  UInt32 m_MatchMaxLen;
  const Byte *m_LenStart;
  const Byte *m_LenDirectBits;

  bool m_Created;
  bool m_Deflate64Mode;

  Byte m_LevelLevels[kLevelTableSize];
  int m_NumLitLenLevels;
  int m_NumDistLevels;
  UInt32 m_NumLevelCodes;
  UInt32 m_ValueIndex;

  bool m_SecondPass;
  UInt32 m_AdditionalOffset;

  UInt32 m_OptimumEndIndex;
  UInt32 m_OptimumCurrentIndex;
  
  Byte  m_LiteralPrices[256];
  Byte  m_LenPrices[kNumLenSymbolsMax];
  Byte  m_PosPrices[kDistTableSize64];

  CLevels m_NewLevels;
  UInt32 mainFreqs[kFixedMainTableSize];
  UInt32 distFreqs[kDistTableSize64];
  UInt32 mainCodes[kFixedMainTableSize];
  UInt32 distCodes[kDistTableSize64];
  UInt32 levelCodes[kLevelTableSize];
  Byte levelLens[kLevelTableSize];

  UInt32 BlockSizeRes;

  CTables *m_Tables;
  COptimal m_Optimum[kNumOpts];

  UInt32 m_MatchFinderCycles;
  // IMatchFinderSetNumPasses *m_SetMfPasses;

  void GetMatches();
  void MovePos(UInt32 num);
  UInt32 Backward(UInt32 &backRes, UInt32 cur);
  UInt32 GetOptimal(UInt32 &backRes);
  UInt32 GetOptimalFast(UInt32 &backRes);

  void LevelTableDummy(const Byte *levels, int numLevels, UInt32 *freqs);

  void WriteBits(UInt32 value, int numBits);
  void LevelTableCode(const Byte *levels, int numLevels, Byte *lens, const UInt32 *codes);
  void ModLevels(const Byte *levels, int numLevels, Byte *lens, const UInt32 *codes);

  void MakeTables(unsigned maxHuffLen);
  UInt32 GetLzBlockPrice() const;
  void TryBlock();
  UInt32 TryDynBlock(int tableIndex, UInt32 numPasses, int Change);

  UInt32 TryFixedBlock(int tableIndex);

  void SetPrices(const CLevels &levels);
  void WriteBlock();

  HRESULT Create();
  void Free();

  void WriteStoreBlock(UInt32 blockSize, UInt32 additionalOffset, bool finalBlock);
  void WriteTables(bool writeMode, bool finalBlock);
  
  void WriteBlockData(bool writeMode, bool finalBlock);

  void ReleaseStreams()
  {
    _seqInStream.RealStream.Release();
//    m_OutStream.ReleaseStream();
  }
  class CCoderReleaser
  {
    CCoder *m_Coder;
  public:
    CCoderReleaser(CCoder *coder): m_Coder(coder) {}
    ~CCoderReleaser() { m_Coder->ReleaseStreams(); }
  };
  friend class CCoderReleaser;

  UInt32 GetBlockPrice(int tableIndex, int numDivPasses);
  void CodeBlock(int tableIndex, bool finalBlock);

public:
  CCoder(bool deflate64Mode = false);
  ~CCoder();

  HRESULT CodeReal(ISequentialInStream *inStream, ISequentialOutStream *outStream,
      const UInt64 *inSize, const UInt64 *outSize, ICompressProgressInfo *progress);

  HRESULT BaseCode(ISequentialInStream *inStream, ISequentialOutStream *outStream,
      const UInt64 *inSize, const UInt64 *outSize, ICompressProgressInfo *progress);

//  HRESULT BaseSetEncoderProperties2(const PROPID *propIDs, const PROPVARIANT *props, UInt32 numProps);
};


class CCOMCoder :
  public ICompressCoder,
  public ICompressSetCoderProperties,
  public CMyUnknownImp,
  public CCoder
{
public:
  MY_UNKNOWN_IMP1(ICompressSetCoderProperties)
  CCOMCoder(): CCoder(false) {};
  STDMETHOD(Code)(ISequentialInStream *inStream, ISequentialOutStream *outStream,
      const UInt64 *inSize, const UInt64 *outSize, ICompressProgressInfo *progress);
  STDMETHOD(SetCoderProperties)(const PROPID *propIDs, const PROPVARIANT *props, UInt32 numProps);
};

class CCOMCoder64 :
  public ICompressCoder,
  public ICompressSetCoderProperties,
  public CMyUnknownImp,
  public CCoder
{
public:
  MY_UNKNOWN_IMP1(ICompressSetCoderProperties)
  CCOMCoder64(): CCoder(true) {};
  STDMETHOD(Code)(ISequentialInStream *inStream, ISequentialOutStream *outStream,
      const UInt64 *inSize, const UInt64 *outSize, ICompressProgressInfo *progress);
  STDMETHOD(SetCoderProperties)(const PROPID *propIDs, const PROPVARIANT *props, UInt32 numProps);
};

}}}

#endif
