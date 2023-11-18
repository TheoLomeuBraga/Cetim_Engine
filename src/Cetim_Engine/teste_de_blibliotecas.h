






#include <iostream>
using namespace std;


#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
using namespace glm;





#include <lua/lua.hpp>
void testar_script(){

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	int i = luaL_dofile(L,"recursos/Scripts/AAAAA.lua");
	if (i != LUA_OK) {
		cout <<  "LUA NOT OK" << endl;
		string erro = lua_tostring(L, -1);
		cout <<  erro << endl;
	}
	lua_getglobal(L, "START");
	lua_call(L, 0, 0);

}



#include <SFML/Audio.hpp>
void testar_som(){

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("recursos/Audio/teste de audio.wav")){
	}
	cout << "som" << endl;
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	while(true){}

}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_GLU

void testat_janela(){

		if( !glfwInit() )
	{
	    fprintf( stderr, "Failed to initialize GLFW\n" );
	}
		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
		GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
		window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
		if( window == NULL ){
		    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		    glfwTerminate();
		}
		glfwMakeContextCurrent(window); // Initialize GLEW
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		do{
		    glClear( GL_COLOR_BUFFER_BIT );
		    glfwSwapBuffers(window);
		    glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

}

GLuint VertexArrayID;
GLuint vertexbuffer;

void testat_janela_ogl(){
	// start GL context and O/S window using the GLFW helper library
		  if (!glfwInit()) {
		    fprintf(stderr, "ERROR: could not start GLFW3\n");
		  }

		  // uncomment these lines if on Apple OS X
		  /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

		  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
		  	  if (!window) {
		  	    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		  	    glfwTerminate();
		  	  }
		  	  glfwMakeContextCurrent(window);

		  	  // start GLEW extension handler
		  	  glewExperimental = GL_TRUE;
		  	  glewInit();

		  	  // get version info
		  	  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		  	  const GLubyte* version = glGetString(GL_VERSION); // version as a string
		  	  printf("Renderer: %s\n", renderer);
		  	  printf("OpenGL version supported %s\n", version);

		  	  // tell GL to only draw onto a pixel if the shape is closer to the viewer
		  	  glEnable(GL_DEPTH_TEST); // enable depth-testing
		  	  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	float points[] = {
	   0.0f,  0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f
	};
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	const char* vertex_shader =
	"#version 400\n"
	"in vec3 vp;"
	"void main() {"
	"  gl_Position = vec4(vp, 1.0);"
	"}";
	const char* fragment_shader =
	"#version 400\n"
	"out vec4 frag_colour;"
	"void main() {"
	"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
	"}";
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);





	  /* OTHER STUFF GOES HERE NEXT */
	  while(!glfwWindowShouldClose(window)) {
	    // wipe the drawing surface clear
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glUseProgram(shader_programme);
	    glBindVertexArray(vao);
	    // draw points 0-3 from the currently bound VAO with current in-use shader
	    glDrawArrays(GL_TRIANGLES, 0, 3);
	    // update other events like input handling
	    glfwPollEvents();
	    // put the stuff we've been drawing onto the display
	    glfwSwapBuffers(window);
	  }
	  // close GL context and any other GLFW resources
	  glfwTerminate();


	}








#include"box2d/box2d.h"
void testar_fisica(){

	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	for (int32 i = 0; i < 60; ++i)
	{
	    world.Step(timeStep, velocityIterations, positionIterations);
	    b2Vec2 position = body->GetPosition();
	    float angle = body->GetAngle();
	    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}
}
#include <thread>
int main(int argc, char** argv)
{

	testar_script();

	testar_fisica();

	thread a(testar_som);
	//thread b(testat_janela);
	thread c(testat_janela_ogl);


	//b.join();
	c.join();





	cout << "fim\n";
return 0;

}


