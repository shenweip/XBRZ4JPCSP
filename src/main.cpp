#include "main.h"
#include "XBRZNative/XBRZNative.h"
#include "Scaler/TextureScaler.h"
#include <GL/glew.h>

TextureScaler scaler;

// Implented jni functions
JNIEXPORT void JNICALL Java_jpcsp_graphics_RE_XBRZNative_ScaleandSetTexImage__I_3BIIIIIII
  (JNIEnv *env, jclass cls, jint factor, jbyteArray data, jint level, jint internalFormat, jint width, jint height, jint bufwidth, jint fmt, jint typ){
	  jboolean b = JNI_FALSE;
	  jbyte* buf = env->GetByteArrayElements(data,&b);
	  u32* Buf = (u32*) buf;
	  GLenum Typ = (GLenum)typ;
	  int Width = (int)width;
	  int Height = (int)height;
	  scaler.Scale(Buf, Typ, Width, bufwidth, Height, factor);
	  if (typ == GL_UNSIGNED_SHORT_5_6_5 || typ == GL_UNSIGNED_SHORT_5_6_5_REV) {
		  internalFormat = GL_RGBA;
		  fmt = GL_RGBA;
	  }
	  glTexImage2D(GL_TEXTURE_2D, level, internalFormat, Width, Height, 0, fmt, Typ, Buf);
	  env->ReleaseByteArrayElements(data, buf, 0);	 
}
JNIEXPORT void JNICALL Java_jpcsp_graphics_RE_XBRZNative_ScaleandSetTexImage__I_3IIIIIIII
  (JNIEnv *env, jclass cls, jint factor, jintArray data, jint level, jint internalFormat, jint width, jint height, jint bufwidth, jint fmt, jint typ){
      jboolean b = JNI_FALSE;
	  jint* buf = env->GetIntArrayElements(data,&b);
	  u32* Buf = (u32*) buf;
	  GLenum Typ = (GLenum)typ;
	  int Width = (int)width;
	  int Height = (int)height;
	  scaler.Scale(Buf, Typ, Width, bufwidth, Height, factor);
	  if (typ == GL_UNSIGNED_SHORT_5_6_5 || typ == GL_UNSIGNED_SHORT_5_6_5_REV) {
		  internalFormat = GL_RGBA;
		  fmt = GL_RGBA;
	  }
	  glTexImage2D(GL_TEXTURE_2D, level, internalFormat, Width, Height, 0, fmt, Typ, Buf);
	  env->ReleaseIntArrayElements(data, buf, 0);
}

JNIEXPORT void JNICALL Java_jpcsp_graphics_RE_XBRZNative_ScaleandSetTexImage__I_3SIIIIIII
  (JNIEnv *env, jclass cls, jint factor, jshortArray data, jint level, jint internalFormat, jint width, jint height, jint bufwidth, jint fmt, jint typ){
	  jboolean b = JNI_FALSE;
	  jshort* buf = env->GetShortArrayElements(data,&b);
	  u32* Buf = (u32*) buf;
	  GLenum Typ = (GLenum)typ;
	  int Width = (int)width;
	  int Height = (int)height;
	  scaler.Scale(Buf, Typ, Width, bufwidth, Height, factor);
	  if (typ == GL_UNSIGNED_SHORT_5_6_5 || typ == GL_UNSIGNED_SHORT_5_6_5_REV) {
		  internalFormat = GL_RGBA;
		  fmt = GL_RGBA;
	  }
	  glTexImage2D(GL_TEXTURE_2D, level, internalFormat, Width, Height, 0, fmt, Typ, Buf);
	  env->ReleaseShortArrayElements(data, buf, 0);
}
JNIEXPORT void JNICALL Java_jpcsp_graphics_RE_XBRZNative_ScaleandSetTexImage__I_3FIIIIIII
  (JNIEnv *env, jclass cls, jint factor, jfloatArray data, jint level, jint internalFormat, jint width, jint height, jint bufwidth, jint fmt, jint typ){
	  jboolean b = JNI_FALSE;
	  jfloat* buf = env->GetFloatArrayElements(data,&b);
	  u32* Buf = (u32*) buf;
	  GLenum Typ = (GLenum)typ;
	  int Width = (int)width;
	  int Height = (int)height;
	  scaler.Scale(Buf, Typ, Width, bufwidth, Height, factor);
	  if (typ == GL_UNSIGNED_SHORT_5_6_5 || typ == GL_UNSIGNED_SHORT_5_6_5_REV) {
		  internalFormat = GL_RGBA;
		  fmt = GL_RGBA;
	  }
	  glTexImage2D(GL_TEXTURE_2D, level, internalFormat, Width, Height, 0, fmt, Typ, Buf);
	  env->ReleaseFloatArrayElements(data, buf, 0);
}
