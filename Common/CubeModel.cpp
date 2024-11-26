#include "Camera.h"
#include "CubeModel.h"

GLfloat TheCubeVertices[] = {
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	0.0f,
	0.5f,
	-0.5f,
	-0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	-0.5f,
	0.5f,
	-0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	0.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	0.5f,
	-0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	1.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	1.0f,
	-0.5f,
	0.5f,
	0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	-0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	-0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	-0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	-0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	-0.5f,
	-0.5f,
	0.0f,
	1.0f,

	-0.5f,
	0.5f,
	-0.5f,
	0.0f,
	1.0f,
	0.5f,
	0.5f,
	-0.5f,
	1.0f,
	1.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	0.5f,
	0.5f,
	0.5f,
	1.0f,
	0.0f,
	-0.5f,
	0.5f,
	0.5f,
	0.0f,
	0.0f,
	-0.5f,
	0.5f,
	-0.5f,
	0.0f,
	1.0f
};


// a simple cube init, but it send the vertices to the VBO
// unfortunatly it does this for every instance of a cube, which might be wasteful

CubeModel::CubeModel()
{

	mTranslationMatrix = glm::mat4(1.0f);
	mRotationMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::mat4(1.0f);
	Model = glm::mat4(1.0f);  // give the model I as an initialisation
	glm::vec3 Pos = glm::vec3(1);
	this->SetPosition(Pos);

	Scales = glm::vec3(1.0f, 1.0f, 1.0f);
	SetModelMatrix();
	this->Vertices = TheCubeVertices;
	Colour = glm::vec4(Rand(1.0f), Rand(1.0f), Rand(1.0f), 1.0f);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(TheCubeVertices),
				 TheCubeVertices,
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


}


// this is a rather pointless system, hard coding the file here?
CubeModel::CubeModel(MyFiles* FH)
	: CubeModel()
{

	// Load, create texture and generate mipmaps
	int width, height;

	char* image = FH->Load((char*)"../Common/Assets/Textures/container.jpg", &width, &height);

	glGenTextures(1, &texture1);  // create our 1st texture and store the handle in texture
	glBindTexture(GL_TEXTURE_2D, texture1);  // bind it, so it now is what we are looking at
	if(glGetError() != GL_NO_ERROR) printf("Oh bugger, Model graphic init failed\n");
	// set up some paramaters
		// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// now put the image in there...might be tricky
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture when done, so we won't accidentily mess up our texture.

}

CubeModel::~CubeModel()
{

}

bool CubeModel::Update(Camera* camera)
{
	//set up model view

	Rotations = glm::vec3(0.0);
	mScaleMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::scale(mScaleMatrix, Scales);
	MakeRotationMatrix();
	MakeTranslationMatrix();
	MakeModelMatrix();

	// get these from the camera normally

	glm::mat4 Projection = camera->GetProjection();
	glm::mat4 View = camera->GetView();

	glUseProgram(this->programObject);

	glEnable(GL_DEPTH_TEST);
	// make the MVP 
	glm::mat4 MVP = Projection * View * Model;  // Remember order seems backwards
	GLuint MatrixID = glGetUniformLocation(programObject, "MVP");  //LOOK!!!!
	GLuint AmbID = glGetUniformLocation(programObject, "Ambient");

	float Amb[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glUniform4fv(AmbID, 1, &Amb[0]);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	return true;
}
bool CubeModel::Draw()
{
	// Use the program object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	glUseProgram(this->programObject);



	//load the vertex data
	glVertexAttribPointer(this->positionLoc,
						  3,
						  // write 3 values
						  GL_FLOAT,
						  // they are floats
						  GL_FALSE,
						  // not normals
						  5 * sizeof(GLfloat),
						  //now many bytes till the next
						  0 // where does it start
	);  // use 3 values, but add 5 each time to get to the next


	// Load the texture coordinate
	glVertexAttribPointer(this->texCoordLoc,
						  2,
						  GL_FLOAT,
						  GL_FALSE,
						  5 * sizeof(GLfloat),
						  (GLfloat*)(3 * sizeof(GLfloat))
	);

	glEnableVertexAttribArray(this->positionLoc);
	glEnableVertexAttribArray(this->texCoordLoc);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture1);
	glUniform1i(this->samplerLoc, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	return true;
}
