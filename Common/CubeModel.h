#pragma once

#include "ObjectModel.h"

#ifdef WINDOWS_BUILD
#include "glad.h"

#else
#include <GLES2/gl2.h>

#endif


class CubeModel : public ObjectModel
{
public:
	CubeModel();
	CubeModel(FileLoader*);
	
	~CubeModel();
		
	bool Update(Camera* camera) ; // we supply an update
	bool Draw(); // and a draw 
	
	
	

};
