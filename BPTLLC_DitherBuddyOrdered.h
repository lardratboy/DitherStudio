// ----------------------------------------------------------------------------
/*
The MIT License (MIT)

Copyright (c) 2000- Brad P. Taylor

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// ----------------------------------------------------------------------------

#if !defined(AFX_BPTLLC_DITHERBUDDY_ORDERED_H__D145CC32_FDCC_49a3_9DDB_400DD95197B1__INCLUDED_)
#define AFX_BPTLLC_DITHERBUDDY_ORDERED_H__D145CC32_FDCC_49a3_9DDB_400DD95197B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DitherBuddy.h"

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================

	bool DitherBuddy_inplace_32bpp(
		void * pBufferData
		,const int nBufferWidth 
		,const int nBufferHeight
		,const int nBufferPitch
		,const int xOriginOffset = 0
		,const int yOriginOffset = 0
		,const unsigned buffer_R_ChannelMask = 0x00ff0000
		,const int buffer_R_Shift = 16
		,const int buffer_R_OutputSigBits = 5
		,const unsigned buffer_G_ChannelMask = 0x0000ff00
		,const int buffer_G_Shift = 8
		,const int buffer_G_OutputSigBits = 6
		,const unsigned buffer_B_ChannelMask = 0x000000ff
		,const int buffer_B_Shift = 0
		,const int buffer_B_OutputSigBits = 5
		,const BPT::DITHER_STYLE ditherStyle = BPT::FLOYD_STEINBERG
		,const int nOutputFormat = 565 /* 555, 565, 444 */
		,const int nMaxDrift = 32
		,const float Brighten = 0.0f
		,const bool bSerpentine = true
		,const bool bForceDirection = false
		,const bool bUseNoise = false
		,const int nNoiseModulo = 7
		,const bool bUseReset = false
		,const int nResetModulo = 7
		,const bool bJustQuantize = false
		,const bool bStoreDifference = false
	);

	bool OrderedDither_Alpha_inplace_32bpp(
		void * pBufferData
		,const int nBufferWidth 
		,const int nBufferHeight
		,const int nBufferPitch
		,const int nOutputBits
		,const int nDitherSizePow2
		,const bool bPreserveOpaqueAndTransparent = true
		,const int xOriginOffset = 0
		,const int yOriginOffset = 0
		,const unsigned buffer_A_ChannelMask = 0xff000000
		,const int buffer_A_Shift = 24
		,const float brighten = 0.0f
		,const bool bAddNoise = false
	);

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(AFX_BPTLLC_DITHERBUDDY_ORDERED_H__D145CC32_FDCC_49a3_9DDB_400DD95197B1__INCLUDED_)
