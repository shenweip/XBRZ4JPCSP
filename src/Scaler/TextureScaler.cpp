#include "TextureScaler.h"
#include "../xbrz/xbrz.h"
#include "../Common/util/util.h"

void TextureScaler::ScaleXBRZ(int factor, u32* source, u32* dest, int width, int height){
	   xbrz::ScalerCfg cfg;
	   xbrz::scale(factor, source, dest, width, height, cfg, 0, height);
}

void TextureScaler::convert4444(u16* data, u32* out, int width, int l, int u) {
	for(int y = l; y < u; ++y) {
		for(int x = 0; x < width; ++x) {
			u32 val = data[y*width + x];
			u32 r = ((val>>12) & 0xF) * 17;
			u32 g = ((val>> 8) & 0xF) * 17;
			u32 b = ((val>> 4) & 0xF) * 17;
			u32 a = ((val>> 0) & 0xF) * 17;
			out[y*width + x] = (a << 24) | (b << 16) | (g << 8) | r;
		}
	}
}

void TextureScaler::convert4444REV(u16* data, u32* out, int width, int l, int u) {
	for(int y = l; y < u; ++y) {
		for(int x = 0; x < width; ++x) {
			u32 val = data[y*width + x];
			u32 r = ((val>> 0) & 0xF) * 17;
			u32 g = ((val>> 4) & 0xF) * 17;
			u32 b = ((val>> 8) & 0xF) * 17;
			u32 a = ((val>>12) & 0xF) * 17;
			out[y*width + x] = (a << 24) | (b << 16) | (g << 8) | r;
		}
	}
}

void TextureScaler::convert565(u16* data, u32* out, int width, int l, int u) {
	for(int y = l; y < u; ++y) {
		for(int x = 0; x < width; ++x) {
			u32 val = data[y*width + x];
			u32 r = Convert5To8((val>>11) & 0x1F);
			u32 g = Convert6To8((val>> 5) & 0x3F);
			u32 b = Convert5To8((val    ) & 0x1F);
			out[y*width + x] = (0xFF << 24) | (b << 16) | (g << 8) | r;
		}
	}
}

void TextureScaler::convert565REV(u16* data, u32* out, int width, int l, int u) {
	for(int y = l; y < u; ++y) {
		for(int x = 0; x < width; ++x) {
			u32 val = data[y*width + x];
			u32 r = Convert5To8((val>> 0) & 0x1F);
			u32 g = Convert6To8((val>> 5) & 0x3F);
			u32 b = Convert5To8((val>>11) & 0x1F);
			out[y*width + x] = (0xFF << 24) | (b << 16) | (g << 8) | r;
		}
	}
}

void TextureScaler::convert5551(u16* data, u32* out, int width, int l, int u) {
	for(int y = l; y < u; ++y) {
		for(int x = 0; x < width; ++x) {
			u32 val = data[y*width + x];
			u32 r = Convert5To8((val>>11) & 0x1F);
			u32 g = Convert5To8((val>> 6) & 0x1F);
			u32 b = Convert5To8((val>> 1) & 0x1F);
			u32 a = (val & 0x1) * 255;
			out[y*width + x] = (a << 24) | (b << 16) | (g << 8) | r;
		}
	}
}

void TextureScaler::convert1555REV(u16* data, u32* out, int width, int l, int u) {
	for(int y = l; y < u; ++y) {
		for(int x = 0; x < width; ++x) {
			u32 val = data[y*width + x];
			u32 r = Convert5To8((val>> 0) & 0x1F);
			u32 g = Convert5To8((val>> 5) & 0x1F);
			u32 b = Convert5To8((val>>10) & 0x1F);
			u32 a = ((val>> 15) & 0x1) * 255;
			out[y*width + x] = (a << 24) | (b << 16) | (g << 8) | r;
		}
	}
}

void TextureScaler::ConvertTo8888(GLenum format, u32* source, u32* &dest, int width, int height) {
	switch(format) {
	case GL_UNSIGNED_BYTE:
		 dest = source;  // already fine
		 break;

	case GL_UNSIGNED_SHORT_4_4_4_4:
		 convert4444((u16*)source, dest, width, 0, height);
		 break;

	case GL_UNSIGNED_SHORT_5_6_5:
		 convert565((u16*)source, dest, width, 0, height);
		 break;

	case GL_UNSIGNED_SHORT_5_5_5_1:
		 convert5551((u16*)source, dest, width, 0, height);
		 break;
      
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
		 convert4444REV((u16*)source, dest, width, 0, height);
		 break;

	case GL_UNSIGNED_SHORT_5_6_5_REV:
		 convert565REV((u16*)source, dest, width, 0, height);
		 break;

	case GL_UNSIGNED_SHORT_1_5_5_5_REV:
		 convert1555REV((u16*)source, dest, width, 0, height);
		 break;
	default:
		dest = source;
	}
}

bool TextureScaler::IsEmptyOrFlat(u32* data, int pixels, GLenum fmt) {
	int pixelsPerWord = (fmt == GL_UNSIGNED_BYTE) ? 1 : 2;
	u32 ref = data[0];
	for(int i=0; i<pixels/pixelsPerWord; ++i) {
		if(data[i]!=ref) 
		   return false;
	}
	return true;
}

void TextureScaler::Scale(u32* &data, GLenum &dstFmt, int &width, int bufwidth, int &height, int factor) {
	// prevent processing empty or flat textures (this happens a lot in some games)
	// doesn't hurt the standard case, will be very quick for textures with actual texture
	if(IsEmptyOrFlat(data, width*height, dstFmt)) {
		return;
	}

	tmpTexBufRearrange.resize(std::max(bufwidth, width) * height);
		if (width != bufwidth) {
			int pixelSize;
			switch (dstFmt) {
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_SHORT_5_6_5:
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				pixelSize = 2;
				break;
			default:
				pixelSize = 4;
				break;
		}

		int inRowBytes = bufwidth * pixelSize;
		int outRowBytes = width * pixelSize;

		const u8 *read = (const u8 *)data;
		u8 *write = 0;
		if (width > bufwidth) {
			write = (u8 *)tmpTexBufRearrange.data();
			data = tmpTexBufRearrange.data();
		} else {
			write = (u8 *)data;
		}
		for (int y = 0; y < height; y++) {
			memmove(write, read, outRowBytes);
			read += inRowBytes;
			write += outRowBytes;
		}
	}

	bufInput.resize(width*height); // used to store the input image image if it needs to be reformatted
	bufOutput.resize(width*height*factor*factor); // used to store the upscaled image
	u32 *inputBuf = bufInput.data();
	u32 *outputBuf = bufOutput.data();

	// convert texture to correct format for scaling
	ConvertTo8888(dstFmt, data, inputBuf, width, height);
	ScaleXBRZ(factor, inputBuf, outputBuf, width, height);
	// update values accordingly
	data = outputBuf;
	dstFmt = GL_UNSIGNED_BYTE;
	width *= factor;
	height *= factor;
}