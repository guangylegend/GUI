#ifndef FREE_NEHE_H
#define FREE_NEHE_H

//FreeType ͷ�ļ�
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL ͷ�ļ�
#include <windows.h>		
#include <GL/gl.h>
#include <GL/glu.h>

//STL ͷ�ļ�
#include <vector>
#include <string>

//STL�쳣��
#include <stdexcept>

#pragma warning(disable: 4786) 

// �����еĲ����������ֿռ�freetype�У��������Ա��������������ĳ�ͻ
namespace freetype 
{
	//ʹ��vector��string���ֿռ�
	using std::vector;
	using std::string;

	//����ṹ����������Ϣ 
	struct font_data 
	{
		float h;			// ����ĸ߶�
		GLuint * textures;	// ʹ�õ����� 
		GLuint list_base;	// ��ʾ�б��ֵ

		//��ʼ���ṹ
		void init(const char * fname, unsigned int h);

		//������е���Դ
		void clean();
	};

	//���ַ��������Ļ
	void print(const font_data &ft_font, float x, float y, const char *fmt, ...) ;
}
#endif