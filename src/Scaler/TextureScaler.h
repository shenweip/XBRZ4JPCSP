#pragma once

#include <GL/glew.h>
#include "../Common/Type/CommonType.h"
#include "../Common/util/MemoryUtil.h"

class TextureScaler {
public:
	TextureScaler(){}

	void Scale(u32* &data, GLenum &dstFmt, int &width, int bufwidth, int &height, int factor);

private:
	void ScaleXBRZ(int factor, u32* source, u32* dest, int width, int height);
	void ConvertTo8888(GLenum format, u32* source, u32* &dest, int width, int height);

	void convert4444(u16* data, u32* out, int width, int l, int u);
	void convert4444REV(u16* data, u32* out, int width, int l, int u);
	void convert565(u16* data, u32* out, int width, int l, int u);
	void convert565REV(u16* data, u32* out, int width, int l, int u);
	void convert5551(u16* data, u32* out, int width, int l, int u);
	void convert1555REV(u16* data, u32* out, int width, int l, int u);

	bool IsEmptyOrFlat(u32* data, int pixels, GLenum fmt);

	SimpleBuf<u32> bufInput,bufOutput,tmpTexBufRearrange;
};