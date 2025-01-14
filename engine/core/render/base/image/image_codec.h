#pragma once

#include "engine/core/util/Buffer.h"
#include "pixel_format.h"
#include "image.h"

namespace Echo
{
	class ImageCodec
	{
	public:
		ImageCodec(ImageFormat imgFmt);
		virtual ~ImageCodec();

		virtual ImageFormat			getImageFormat() const;
		virtual bool				codeToFile(ImageFormat imgFmt, const Buffer &buff, const Image::ImageInfo &imgInfo, const String &filename) const;
		virtual bool				encode(const Buffer &inBuff, Buffer &outBuff, const Image::ImageInfo &imgInfo) const;
		virtual bool				decode(const Buffer &inBuff, Buffer &outBuff, Image::ImageInfo &imgInfo);
		virtual DataStream*			decode(DataStream* inStream, Image::ImageInfo& imgInfo);

		static ImageFormat			GetImageFormat(const String &filename);

	protected:
		bool						doEncode(ImageFormat imgFmt, const Buffer &inBuff, Buffer &outBuff, const Image::ImageInfo &imgInfo) const;
		bool						doDecode(ImageFormat imgFmt, const Buffer &inBuff, Buffer &outBuff, Image::ImageInfo &imgInfo);
		DataStream*					doDecode(ImageFormat imgFmt, DataStream* inStream, Image::ImageInfo& imgInfo);

	protected:
		ImageFormat			m_imgFmt;
	};
}
