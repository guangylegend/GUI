#ifndef FREE_NEHE_H
#define FREE_NEHE_H

//FreeType 头文件
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL 头文件
#include <windows.h>		
#include <GL/gl.h>
#include <GL/glu.h>

//STL 头文件
#include <vector>
#include <string>

//STL异常类
#include <stdexcept>

#pragma warning(disable: 4786) 

// 把所有的操作放在名字空间freetype中，这样可以避免与其他函数的冲突
namespace freetype 
{
	//使用vector和string名字空间
	using std::vector;
	using std::string;

	//这个结构保存字体信息 
	struct font_data 
	{
		float h;			// 字体的高度
		GLuint * textures;	// 使用的纹理 
		GLuint list_base;	// 显示列表的值

		//初始化结构
		void init(const char * fname, unsigned int h);

		//清楚所有的资源
		void clean();
	};

	//把字符输出到屏幕
	void print(const font_data &ft_font, float x, float y, const char *fmt, ...) ;
}
#endif