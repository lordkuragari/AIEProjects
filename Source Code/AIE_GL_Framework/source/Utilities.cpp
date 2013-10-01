//////////////////////////////////////////////////////////////////////////
// Author:	Conan Bourke
// Date:	December 4 2012
// Brief:	Utility functions
//////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GL/wglew.h>
#include <Gl/glfw.h>
#include <FreeImage.h>
#include <stdio.h>
#include <random>

#include "Utilities.h"
#include <vector>

// disable the warning for microsoft "safe" functions
#pragma warning( disable : 4996 )

static double s_dPrevTime = 0;

//////////////////////////////////////////////////////////////////////////
void AIE::ResetTimer()
{
	s_dPrevTime = glfwGetTime();
}

//////////////////////////////////////////////////////////////////////////
float AIE::GetDeltaTime()
{
	double dTime = glfwGetTime();
	float fDT = (float)(dTime - s_dPrevTime);
	s_dPrevTime = dTime;
	return fDT;
}

//////////////////////////////////////////////////////////////////////////
// builds a simple 2 triangle quad with a position and UV
void AIE::Build3DPlane(float a_fSize, unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO)
{
	float fHalfSize = a_fSize * 0.5f;

	// create mesh
	Vertex aoVertices[4];
	aoVertices[0].position = AIE::vec4(-fHalfSize,0,-fHalfSize,1);
	aoVertices[0].uv = AIE::vec2(0,0);
	aoVertices[1].position = AIE::vec4(fHalfSize,0,-fHalfSize,1);
	aoVertices[1].uv = AIE::vec2(1,0);
	aoVertices[2].position = AIE::vec4(fHalfSize,0,fHalfSize,1);
	aoVertices[2].uv = AIE::vec2(1,1);
	aoVertices[3].position = AIE::vec4(-fHalfSize,0,fHalfSize,1);
	aoVertices[3].uv = AIE::vec2(0,1);

	unsigned int auiIndex[6] = {
		0,1,3,
		1,2,3
	};

	// create and bind buffers to a vertex array object
	glGenBuffers(1, &a_ruiVBO);
	glGenBuffers(1, &a_ruiIBO);
	glGenVertexArrays(1, &a_ruiVAO);

	glBindVertexArray(a_ruiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, a_ruiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ruiIBO);

	glBufferData(GL_ARRAY_BUFFER,			4 * sizeof(Vertex),			aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,	6 * sizeof(unsigned int),	auiIndex,	GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	// unbind vertex array
	glBindVertexArray(0);
}


void AIE::BuildArbitraryPlane( float a_fSize, int a_iVertsWidth, int a_iVertsLength, unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO )
{
	if( a_iVertsWidth < 2 )
		a_iVertsWidth = 2;
	if( a_iVertsLength < 2 )
		a_iVertsLength = 2;

	float halfSize = a_fSize/2;


	std::vector<Vertex> aoVertices;
	int iNumVerts = a_iVertsWidth * a_iVertsLength;

	int iNumTris = ((a_iVertsWidth-1) * (a_iVertsLength-1)) * 2;
	int iNumIndices = iNumTris * 3;

	std::vector<unsigned int> auiIndex;


	for( int z = 0; z < a_iVertsLength; ++z )
	{
		for( int x = 0; x < a_iVertsWidth; ++x )
		{
			aoVertices.push_back( Vertex() );

			float xPos = x == 0 ? 0 : a_fSize * (static_cast<float>(x)/static_cast<float>(a_iVertsWidth-1));
			float zPos = z == 0 ? 0 : a_fSize * (static_cast<float>(z)/static_cast<float>(a_iVertsLength-1));
			aoVertices[z*a_iVertsLength + x].position = AIE::vec4( xPos-halfSize, 0.0f, zPos-halfSize, 1.0f );
		
			float u = x == 0 ? 0 : xPos/a_fSize;
			float v = z == 0 ? 0 : zPos/a_fSize;
			aoVertices[z*a_iVertsLength + x].uv = AIE::vec2( u, v );
		}
	}

	for( int z = 0; z < a_iVertsLength-1; ++z )
	{
		for( int x = 0; x < a_iVertsWidth-1; ++x )
		{
			auiIndex.push_back( (z*a_iVertsWidth)+x );
			auiIndex.push_back( ((z*a_iVertsWidth)+x) + 1 );
			auiIndex.push_back( ((z+1)*a_iVertsWidth)+x );
		
			auiIndex.push_back( ((z*a_iVertsWidth)+x) + 1 );
			auiIndex.push_back( ((z+1)*a_iVertsWidth)+x + 1 );
			auiIndex.push_back( ((z+1)*a_iVertsWidth)+x );
		}
	}

	glGenBuffers(1, &a_ruiVBO);
	glGenBuffers(1, &a_ruiIBO);
	glGenVertexArrays(1, &a_ruiVAO);

	glBindVertexArray(a_ruiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, a_ruiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ruiIBO);

	glBufferData(GL_ARRAY_BUFFER, (a_iVertsWidth*a_iVertsLength)*sizeof(Vertex), &aoVertices[0], GL_STATIC_DRAW );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNumIndices * sizeof(unsigned int), &auiIndex[0], GL_STATIC_DRAW );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16 );

	glBindVertexArray(0);
}

void AIE::BuildCube( float a_fSize, AIE::vec4 a_vPosition, unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO )
{
	float fHalfSize = a_fSize * 0.5f;

	Vertex aoVertices[8];
	aoVertices[0].position	= a_vPosition + AIE::vec4( -fHalfSize, fHalfSize, fHalfSize, 0.f );
	aoVertices[0].uv		= AIE::vec2( 0.f, 0.f );
	aoVertices[1].position	=  a_vPosition + AIE::vec4( fHalfSize, fHalfSize, fHalfSize, 0.f );
	aoVertices[1].uv		= AIE::vec2( 1.f, 0.f );
	aoVertices[2].position	=  a_vPosition + AIE::vec4( fHalfSize, fHalfSize, -fHalfSize, 0.f );
	aoVertices[2].uv		= AIE::vec2( 1.f, 1.f );
	aoVertices[3].position	=  a_vPosition + AIE::vec4( -fHalfSize, fHalfSize, -fHalfSize, 0.f );
	aoVertices[3].uv		= AIE::vec2( 0.f, 1.f );
	aoVertices[4].position	=  a_vPosition + AIE::vec4( -fHalfSize, -fHalfSize, fHalfSize, 0.f );
	aoVertices[4].uv		= AIE::vec2( 0.f, 0.f );
	aoVertices[5].position	=  a_vPosition + AIE::vec4( fHalfSize, -fHalfSize, fHalfSize, 0.f );
	aoVertices[5].uv		= AIE::vec2( 1.f, 0.f );
	aoVertices[6].position	=  a_vPosition + AIE::vec4( fHalfSize, -fHalfSize, -fHalfSize, 0.f );
	aoVertices[6].uv		= AIE::vec2( 1.f, 1.f );
	aoVertices[7].position	=  a_vPosition + AIE::vec4( -fHalfSize, -fHalfSize, -fHalfSize, 0.f );
	aoVertices[7].uv		= AIE::vec2( 0.f, 1.f );

	unsigned int auiIndex[36] = {
		3,1,0,
		3,2,1,
		7,2,3,
		7,6,2,
		4,6,7,
		4,5,6,
		4,0,1,
		5,4,1,
		6,1,2,
		6,5,1,
		4,3,0,
		4,7,3
	};

	glGenBuffers(		1, &a_ruiVBO );
	glGenBuffers(		1, &a_ruiIBO );
	glGenVertexArrays(	1, &a_ruiVAO );
	
	glBindVertexArray( a_ruiVAO );
	glBindBuffer( GL_ARRAY_BUFFER,			a_ruiVBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,	a_ruiIBO );

	glBufferData( GL_ARRAY_BUFFER,			8 * sizeof(Vertex),			aoVertices, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,	36 * sizeof(unsigned int),	auiIndex,	GL_STATIC_DRAW );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glBindVertexArray(0);
}

void AIE::BuildBox( float a_fWidth, float a_fHeight, float a_fDepth, AIE::vec4 a_vPosition, unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO)
{
	Vertex aoVertices[8];
	aoVertices[0].position	= a_vPosition + AIE::vec4( -a_fWidth/2, a_fHeight/2, a_fDepth/2, 0.f );
	aoVertices[0].uv		= AIE::vec2( 0.f, 0.f );
	aoVertices[1].position	=  a_vPosition + AIE::vec4( a_fWidth/2, a_fHeight/2, a_fDepth/2, 0.f );
	aoVertices[1].uv		= AIE::vec2( 1.f, 0.f );
	aoVertices[2].position	=  a_vPosition + AIE::vec4( a_fWidth/2, a_fHeight/2, -a_fDepth/2, 0.f );
	aoVertices[2].uv		= AIE::vec2( 1.f, 1.f );
	aoVertices[3].position	=  a_vPosition + AIE::vec4( -a_fWidth/2, a_fHeight/2, -a_fDepth/2, 0.f );
	aoVertices[3].uv		= AIE::vec2( 0.f, 1.f );
	aoVertices[4].position	=  a_vPosition + AIE::vec4( -a_fWidth/2, -a_fHeight/2, a_fDepth/2, 0.f );
	aoVertices[4].uv		= AIE::vec2( 0.f, 0.f );
	aoVertices[5].position	=  a_vPosition + AIE::vec4( a_fWidth/2, -a_fHeight/2, a_fDepth/2, 0.f );
	aoVertices[5].uv		= AIE::vec2( 1.f, 0.f );
	aoVertices[6].position	=  a_vPosition + AIE::vec4( a_fWidth/2, -a_fHeight/2, -a_fDepth/2, 0.f );
	aoVertices[6].uv		= AIE::vec2( 1.f, 1.f );
	aoVertices[7].position	=  a_vPosition + AIE::vec4( -a_fWidth/2, -a_fHeight/2, -a_fDepth/2, 0.f );
	aoVertices[7].uv		= AIE::vec2( 0.f, 1.f );

	unsigned int auiIndex[36] = {
		3,1,0,
		3,2,1,
		7,2,3,
		7,6,2,
		4,6,7,
		4,5,6,
		4,0,1,
		5,4,1,
		6,1,2,
		6,5,1,
		4,3,0,
		4,7,3
	};

	glGenBuffers(		1, &a_ruiVBO );
	glGenBuffers(		1, &a_ruiIBO );
	glGenVertexArrays(	1, &a_ruiVAO );
	
	glBindVertexArray( a_ruiVAO );
	glBindBuffer( GL_ARRAY_BUFFER,			a_ruiVBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,	a_ruiIBO );

	glBufferData( GL_ARRAY_BUFFER,			8 * sizeof(Vertex),			aoVertices, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,	36 * sizeof(unsigned int),	auiIndex,	GL_STATIC_DRAW );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glBindVertexArray(0);
}


void AIE::BuildSkybox( float a_fSize, unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO)
{
	float fHalfSize = a_fSize * 0.5f;

	Vertex aoVertices[8];
	aoVertices[0].position	= AIE::vec4( -fHalfSize, fHalfSize, fHalfSize, 1.f );
	aoVertices[0].uv		= AIE::vec2( 0.f, 0.f );
	aoVertices[1].position	= AIE::vec4( fHalfSize, fHalfSize, fHalfSize, 1.f );
	aoVertices[1].uv		= AIE::vec2( 1.f, 0.f );
	aoVertices[2].position	= AIE::vec4( fHalfSize, fHalfSize, -fHalfSize, 1.f );
	aoVertices[2].uv		= AIE::vec2( 1.f, 1.f );
	aoVertices[3].position	= AIE::vec4( -fHalfSize, fHalfSize, -fHalfSize, 1.f );
	aoVertices[3].uv		= AIE::vec2( 0.f, 1.f );
	aoVertices[4].position	= AIE::vec4( -fHalfSize, -fHalfSize, fHalfSize, 1.f );
	aoVertices[4].uv		= AIE::vec2( 0.f, 0.f );
	aoVertices[5].position	= AIE::vec4( fHalfSize, -fHalfSize, fHalfSize, 1.f );
	aoVertices[5].uv		= AIE::vec2( 1.f, 0.f );
	aoVertices[6].position	= AIE::vec4( fHalfSize, -fHalfSize, -fHalfSize, 1.f );
	aoVertices[6].uv		= AIE::vec2( 1.f, 1.f );
	aoVertices[7].position	= AIE::vec4( -fHalfSize, -fHalfSize, -fHalfSize, 1.f );
	aoVertices[7].uv		= AIE::vec2( 0.f, 1.f );

	unsigned int auiIndex[36] = {
		0,1,3,
		1,2,3,
		3,2,7,
		2,6,7,
		7,6,4,
		6,5,4,
		1,0,4,
		1,4,5,
		2,1,6,
		1,5,6,
		0,3,4,
		3,7,4
	};

	glGenBuffers(		1, &a_ruiVBO );
	glGenBuffers(		1, &a_ruiIBO );
	glGenVertexArrays(	1, &a_ruiVAO );
	
	glBindVertexArray( a_ruiVAO );
	glBindBuffer( GL_ARRAY_BUFFER, a_ruiVBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, a_ruiIBO );

	glBufferData( GL_ARRAY_BUFFER,			8 * sizeof(Vertex),			aoVertices, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,	36 * sizeof(unsigned int),	auiIndex,	GL_STATIC_DRAW );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
unsigned int AIE::LoadTexture(const char* a_szTexture, unsigned int a_uiFormat /* = GL_RGBA */, unsigned int* a_uiWidth /* = nullptr */, unsigned int* a_uiHeight /* = nullptr */)
{
	FIBITMAP* pBitmap = nullptr;

	// check the file signature and deduce its format and load it
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(a_szTexture, 0);
	if (fif != FIF_UNKNOWN && 
		FreeImage_FIFSupportsReading(fif)) 
	{
		pBitmap = FreeImage_Load(fif, a_szTexture);
	}

	if (pBitmap == nullptr) 
	{
		printf("Error: Failed to load image '%s'!\n", a_szTexture);
		return 0;
	}

	// optionally get the image width and height
	if (a_uiWidth != nullptr)
		*a_uiWidth = FreeImage_GetWidth(pBitmap);
	if (a_uiHeight != nullptr)
		*a_uiHeight = FreeImage_GetHeight(pBitmap);

	// force the image to RGBA
	unsigned int bpp = FreeImage_GetBPP(pBitmap);
	FREE_IMAGE_COLOR_TYPE fi_colourType = FreeImage_GetColorType(pBitmap);
	if (fi_colourType != FIC_RGBALPHA ) 
	{
		FIBITMAP* ndib = FreeImage_ConvertTo32Bits(pBitmap);
		FreeImage_Unload(pBitmap);
		pBitmap = ndib;
		bpp = FreeImage_GetBPP(pBitmap);
		fi_colourType = FreeImage_GetColorType(pBitmap);
	}

	// get the pixel data
	BYTE* pData = FreeImage_GetBits(pBitmap);

	// try to determine data type of file (bytes/floats)
	FREE_IMAGE_TYPE fit = FreeImage_GetImageType(pBitmap);
	GLenum eType = (fit == FIT_RGBF || fit == FIT_FLOAT) ? GL_FLOAT : GL_UNSIGNED_BYTE;

	// create GL texture
	GLuint textureID;
	glGenTextures( 1, &textureID );
	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(pBitmap), FreeImage_GetHeight(pBitmap), 0, a_uiFormat, eType, pData);

	// specify default filtering and wrapping
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// unbind texture
	glBindTexture( GL_TEXTURE_2D, 0 );

	// delete data
	FreeImage_Unload(pBitmap);

	return textureID;
}

//////////////////////////////////////////////////////////////////////////
// loads only a vertex and pixel shader from text files
unsigned int AIE::LoadShader(unsigned int a_uiInputAttributeCount, const char** a_aszInputAttributes,
							unsigned int a_uiOutputAttributeCount, const char** a_aszOutputAttributes,
							const char* a_szVertexShader, const char* a_szPixelShader,
							const char* a_szGeometryShader /* = nullptr */,
							const char* a_szTessellationControlShader /* = nullptr */, const char* a_szTessellationEvaluationShader /* = nullptr */)
{
	GLint iSuccess;
	GLchar acLog[256];

	// load files into char buffers
	char* vsSource = FileToBuffer(a_szVertexShader);
	char* fsSource = FileToBuffer(a_szPixelShader);
	char* gsSource = a_szGeometryShader == nullptr ? nullptr : FileToBuffer(a_szGeometryShader);
	char* tcsSource = a_szTessellationControlShader == nullptr ? nullptr : FileToBuffer(a_szTessellationControlShader);
	char* tesSource = a_szTessellationEvaluationShader == nullptr ? nullptr : FileToBuffer(a_szTessellationEvaluationShader);

	// must have vertex and pixel
	if (vsSource == nullptr || fsSource == nullptr)
	{
		return 0;
	}

	// create 2 shader handles
	GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint gsHandle = 0;
	GLuint tcsHandle = 0;
	GLuint tesHandle = 0;

	// compile vertex shader and log errors
	glShaderSource(vsHandle, 1, (const char**)&vsSource, 0);
	glCompileShader(vsHandle);

	glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &iSuccess);
	glGetShaderInfoLog(vsHandle, sizeof(acLog), 0, acLog);

	if (iSuccess == GL_FALSE)
	{
		printf("Error: Failed to compile vertex shader!\n");
		printf(acLog);
		printf("\n");
		return 0;
	}

	// compile pixel shader and log errors
	glShaderSource(fsHandle, 1, (const char**)&fsSource, 0);
	glCompileShader(fsHandle);

	glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &iSuccess);
	glGetShaderInfoLog(fsHandle, sizeof(acLog), 0, acLog);

	if (iSuccess == GL_FALSE)
	{
		printf("Error: Failed to compile pixel shader!\n");
		printf(acLog);
		printf("\n");
		return 0;
	}

	if (gsSource != nullptr)
	{
		gsHandle = glCreateShader(GL_GEOMETRY_SHADER);

		// compile geometry shader and log errors
		glShaderSource(gsHandle, 1, (const char**)&gsSource, 0);
		glCompileShader(gsHandle);

		glGetShaderiv(gsHandle, GL_COMPILE_STATUS, &iSuccess);
		glGetShaderInfoLog(gsHandle, sizeof(acLog), 0, acLog);

		if (iSuccess == GL_FALSE)
		{
			printf("Error: Failed to compile geometry shader!\n");
			printf(acLog);
			printf("\n");
			return 0;
		}
	}

	if (tesSource != nullptr && tcsSource != nullptr)
	{
		tesHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		tcsHandle = glCreateShader(GL_TESS_CONTROL_SHADER);

		// compile tessellation control shader and log errors
		glShaderSource(tcsHandle, 1, (const char**)&tcsSource, 0);
		glCompileShader(tcsHandle);

		glGetShaderiv(tcsHandle, GL_COMPILE_STATUS, &iSuccess);
		glGetShaderInfoLog(tcsHandle, sizeof(acLog), 0, acLog);

		if (iSuccess == GL_FALSE)
		{
			printf("Error: Failed to compile tessellation control shader!\n");
			printf(acLog);
			printf("\n");
			return 0;
		}

		// compile tessellation evaluation shader and log errors
		glShaderSource(tesHandle, 1, (const char**)&tesSource, 0);
		glCompileShader(tesHandle);

		glGetShaderiv(tesHandle, GL_COMPILE_STATUS, &iSuccess);
		glGetShaderInfoLog(tesHandle, sizeof(acLog), 0, acLog);

		if (iSuccess == GL_FALSE)
		{
			printf("Error: Failed to compile tessellation evaluation shader!\n");
			printf(acLog);
			printf("\n");
			return 0;
		}
	}

	// create a shader program and attach the shaders to it
	GLuint uiProgramHandle = glCreateProgram();
	glAttachShader(uiProgramHandle, vsHandle);
	glAttachShader(uiProgramHandle, fsHandle);
	if (gsHandle != 0)
		glAttachShader(uiProgramHandle, gsHandle);
	if (tcsHandle != 0)
		glAttachShader(uiProgramHandle, tcsHandle);
	if (tesHandle != 0)
		glAttachShader(uiProgramHandle, tesHandle);

	// specify vertex input attributes
	for ( unsigned int i = 0 ; i < a_uiInputAttributeCount ; ++i )
		glBindAttribLocation(uiProgramHandle, i, a_aszInputAttributes[i]);

	// specify pixel shader outputs
	for ( unsigned int i = 0 ; i < a_uiOutputAttributeCount ; ++i )
		glBindFragDataLocation(uiProgramHandle, i, a_aszOutputAttributes[i]);

	// link the program together and log errors
	glLinkProgram(uiProgramHandle);

	glGetProgramiv(uiProgramHandle, GL_LINK_STATUS, &iSuccess);
	glGetProgramInfoLog(uiProgramHandle, sizeof(acLog), 0, acLog);

	if (iSuccess == GL_FALSE)
	{
		printf("Error: Failed to link shader program!\n");
		printf(acLog);
		printf("\n");
		return 0;
	}

	glUseProgram(uiProgramHandle);

	free(vsSource);
	free(fsSource);
	if (gsSource != nullptr)
		free(gsSource);
	if (tcsSource != nullptr)
		free(tcsSource);
	if (tesSource != nullptr)
		free(tesSource);

	return uiProgramHandle;
}

//////////////////////////////////////////////////////////////////////////
char* AIE::FileToBuffer(const char* a_sPath)
{
	// open file for text reading
	FILE* pFile = fopen(a_sPath,"rt");
	if (pFile == nullptr)
	{
		printf("Error: Unable to open file '%s' for reading!\n",a_sPath);
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	char* acBuffer = new char[uiLength + 1];
	memset(acBuffer,0,uiLength + 1);
	fread(acBuffer, sizeof(char), uiLength, pFile);

	fclose(pFile);
	return acBuffer;
}

//////////////////////////////////////////////////////////////////////////
void AIE::FreeMovement(float a_fDeltaTime, AIE::mat4& a_rmFrame, float a_fSpeed, const AIE::vec4& a_rvUp /* = vec4 */)
{
	// update mouse wheel
	static int siPrevMouseWheel = 0;
	int iMouseWheel = glfwGetMouseWheel();
	int iMouseWheelDelta = iMouseWheel - siPrevMouseWheel;
	siPrevMouseWheel = iMouseWheel;

	// Get the camera's forward, right, up, and location vectors
	AIE::vec4 vForward = a_rmFrame.row2;
	AIE::vec4 vRight = a_rmFrame.row0;
	AIE::vec4 vUp = a_rmFrame.row1;
	AIE::vec4 vTranslation = a_rmFrame.row3;

	// Translate camera
	float fSpeed = glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS ? a_fSpeed * 2 : a_fSpeed;	

	if (glfwGetKey('W') == GLFW_PRESS)
	{
		vTranslation += vForward * (a_fDeltaTime * fSpeed);
	}
	if (glfwGetKey('S') == GLFW_PRESS)
	{
		vTranslation -= vForward * (a_fDeltaTime * fSpeed);
	}
	if (glfwGetKey('D') == GLFW_PRESS)
	{
		vTranslation += vRight * (a_fDeltaTime * fSpeed);
	}
	if (glfwGetKey('A') == GLFW_PRESS)
	{
		vTranslation -= vRight * (a_fDeltaTime * fSpeed);
	}
	if (glfwGetKey('Q') == GLFW_PRESS)
	{
		vTranslation += vUp * (a_fDeltaTime * fSpeed);
	}
	if (glfwGetKey('E') == GLFW_PRESS)
	{
		vTranslation -= vUp * (a_fDeltaTime * fSpeed);
	}
	if (iMouseWheelDelta != 0)
	{
		vTranslation += vForward * (a_fDeltaTime * fSpeed * iMouseWheelDelta);
	}

	a_rmFrame.row3 = vTranslation;

	// check for camera rotation
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static int siPrevMouseX = 0;
		static int siPrevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetMousePos(&siPrevMouseX,&siPrevMouseY);
		}

		int iMouseX = 0, iMouseY = 0;
		glfwGetMousePos(&iMouseX,&iMouseY);

		int iDeltaX = iMouseX - siPrevMouseX;
		int iDeltaY = iMouseY - siPrevMouseY;

		siPrevMouseX = iMouseX;
		siPrevMouseY = iMouseY;

		AIE::mat4 mMat;
		
		// pitch
		if (iDeltaY != 0)
		{
			mMat.RotateAxis(-iDeltaY / 150.f, vRight);
			vForward = mMat * vForward;
			vUp = mMat * vUp;
			vRight = mMat * vRight;
		}

		// yaw
		if (iDeltaX != 0)
		{
			mMat.RotateAxis(-iDeltaX / 150.0f, a_rvUp);
			vForward = mMat * vForward;
			vUp = mMat * vUp;
			vRight = mMat * vRight;
		}

		a_rmFrame.row0 = vRight;
		a_rmFrame.row1 = vUp;
		a_rmFrame.row2 = vForward;
	}
	else
	{
		sbMouseButtonDown = false;
	}
}

float AIE::fRand( float a_fMin, float a_fMax )
{
	float ret = a_fMax - a_fMin;
	if( ret == 0 )
		return a_fMin;

	float randVal = (static_cast<float>( rand() ) / RAND_MAX);
	return a_fMin + (randVal * ret);
}

AIE::vec2 AIE::v2Rand( AIE::vec2 a_vMin, AIE::vec2 a_vMax )
{
	AIE::vec2 ret;

	ret.x = fRand( a_vMin.x, a_vMax.x );
	ret.y = fRand( a_vMin.y, a_vMax.y );

	return ret;
}

AIE::vec4 AIE::v4Rand( AIE::vec4 a_vMin, AIE::vec4 a_vMax )
{
	AIE::vec4 ret;

	ret.x = fRand( a_vMin.x, a_vMax.x );
	ret.y = fRand( a_vMin.y, a_vMax.y );
	ret.z = fRand( a_vMin.z, a_vMax.z );
	ret.w = fRand( a_vMin.w, a_vMax.w );

	return ret;
}