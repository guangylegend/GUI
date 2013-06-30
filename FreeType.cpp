#include "freetype.h"

namespace freetype 
{

// ����������ر�a��ģ���������ӽ�a��2�Ĵη�����
inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

// Ϊ�������ַ�����һ����ʾ�б�
void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base ) 
{
	//��������ַ�������
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//������������
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//������ת��Ϊλͼ
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//����λͼ
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//ת��ΪOpenGl����ʹ�õĴ�С
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	//����λͼ����
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	// ��������ʹ��8λ��ʾ����8λ��ʾalphaֵ
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}


	//����������������������
    glBindTexture( GL_TEXTURE_2D, tex_base[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//�����
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

	//�ͷŷ�����ڴ�
    delete [] expanded_data;

	//������ʾ�б�
	glNewList(list_base+ch,GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D,tex_base[ch]);

	//�������������ƶ�һ��
	glTranslatef(bitmap_glyph->left,0,0);

	//�������������ƶ�һ�㣬��ֻ��'g','y'֮����ַ�����
	//��ʹ�����е��ַ�����һ������
	glPushMatrix();
	glTranslatef(0,bitmap_glyph->top-bitmap.rows,0);

	//����λͼ���ַ�ͼ��Ŀ��
	float	x=(float)bitmap.width / (float)width,
			y=(float)bitmap.rows / (float)height;

	//����һ�������Σ���ʾ�ַ�
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
	glTexCoord2d(0,y); glVertex2f(0,0);
	glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
	glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(face->glyph->advance.x >> 6 ,0,0);


	//������ʾ�б�Ļ���
	glEndList();
}


//��ʼ���������ַ���ʾ�б�
void font_data::init(const char * fname, unsigned int h) 
{
	//��������ID.
	textures = new GLuint[128];

	this->h=h;

	//����FreeType��
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::runtime_error("FT_Init_FreeType failed");

	// ��FreeType���б���������Ϣ�������face
	FT_Face face;

	//ʹ���������Freetype�ַ��ļ���ʼ��face��
	if (FT_New_Face( library, fname, 0, &face )) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	// ��FreeType��ʹ��1/64��Ϊһ�����صĸ߶�����������Ҫ����h���������Ҫ��
	FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);

	//����128����ʾ�б�
	list_base=glGenLists(128);
	glGenTextures( 128, textures );
	for(unsigned char i=0;i<128;i++)
		make_dlist(face,i,list_base,textures);

	//�ͷ�face��
	FT_Done_Face(face);

	//�ͷ�FreeType��
	FT_Done_FreeType(library);
}

void font_data::clean() 
{
	glDeleteLists(list_base,128);
	glDeleteTextures(128,textures);
	delete [] textures;
}

//���浱ǰ�ľ��󣬲������ӿ��뵱ǰ�Ĵ��ڴ�Сƥ��
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	glPopAttrib();
}

//����pushScreenCoordinateMatrix����ľ���
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

// �������
void print(const font_data &ft_font, float x, float y, const char *fmt, ...)  {
	
	// ���浱ǰ����
	pushScreenCoordinateMatrix();					
	
	GLuint font=ft_font.list_base;
	float h=ft_font.h/.63f;						
	
	char		text[256];								// ����256���ַ�
	va_list		ap;										

	if (fmt == NULL)									
		*text=0;											

	else {
	va_start(ap, fmt);									
	    vsprintf(text, fmt, ap);						
	va_end(ap);											
	}


	//��������ַ������س��ָ�
	const char *start_line=text;
	vector<string> lines;
	for(const char *c=text;*c;c++) {
		if(*c=='\n') {
			string line;
			for(const char *n=start_line;n<c;n++) line.append(1,*n);
			lines.push_back(line);
			start_line=c+1;
		}
	}
	if(start_line) {
		string line;
		for(const char *n=start_line;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glListBase(font);

	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//����Ĵ�����ɾ���Ļ��ƹ��� 
	for(int i=0;i<lines.size();i++) {
		

		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y-h*i,0);
		glMultMatrixf(modelview_matrix);

		//������ʾ�б����
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());

		glPopMatrix();

		

	}


	glPopAttrib();		

	pop_projection_matrix();
}

}