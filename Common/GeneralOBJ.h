
#pragma once

#include "ObjectModel.h"



class GeneralOBJ : public ObjectModel
{
public:
	GeneralOBJ();
	GeneralOBJ(char* FileName, FileLoader*);

	~GeneralOBJ();
		
	bool Update(Camera* camera);  // we supply an update
	bool Draw();  // and a draw 
	

	
	

};

