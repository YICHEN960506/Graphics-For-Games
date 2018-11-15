#include"renderer.h"
Renderer::Renderer(Window &parent) :OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();
	triangle->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	if (!triangle->GetTexture()) {
		return;
	}
	currentShader = new Shader(SHADERDIR "TexturedVertex.glsl",SHADERDIR "TexturedFragment.glsl");
	
    if (!currentShader->LinkProgram()) {
		return;	
	}
	
	init = true;
	
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,1,-1);
	
    filtering = true;
	repeating = false;
	
}
 Renderer ::~Renderer(void) {
	 delete triangle;
	
}
 void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(currentShader->GetProgram());
	
	UpdateShaderMatrices();
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"diffuseTex"),0);
	
	triangle->Draw();
	
	glUseProgram(0);
	SwapBuffers();
	
}
 void Renderer::UpdateTextureMatrix(float value) {
	 Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	 Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	 Matrix4 rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));
	 textureMatrix = pushPos*rotation*popPos;
	
}
void Renderer::ToggleRepeating() {
	 repeating = !repeating;
	 glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,repeating?GL_REPEAT : GL_CLAMP);//x axis
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,repeating?GL_REPEAT : GL_CLAMP);//y axis
	 glBindTexture(GL_TEXTURE_2D, 0);
	
}
 void Renderer::ToggleFiltering() {
	 filtering = !filtering;
	 glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filtering ? GL_LINEAR : GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,filtering ? GL_LINEAR : GL_NEAREST);
	 glBindTexture(GL_TEXTURE_2D, 0);
	
}
