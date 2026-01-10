#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"

using namespace glm;

void processKeyboardInput ();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubeMap(std::vector<std::string> faces);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool firstMouse = true;
float lastX = 400, lastY = 400;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cameraDistance = 20.0f;

Window window("Marian - The time traveler", 800, 800);
Camera camera;

vec3 playerPos = vec3(0.0f, 10.0f, 0.0f);
float playerRoataion = 0.0f;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, 200.0f);

int main()
{
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader mountainShader("Shaders/mountain_vertex_shader.glsl", "Shaders/mountain_fragment_shader.glsl");
	Shader terrainShader("Shaders/terrain_vertex_shader.glsl", "Shaders/terrain_fragment_shader.glsl");
	//Shader skyboxShader("Shaders/skybox_vertex_shader.glsl", "Shaders/skybox_fragment_shader.glsl");

	//Textures
	GLuint tex = loadBMP("Resources/Textures/wood.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/orange.bmp");
	GLuint tex4 = loadBMP("Resources/Textures/mat0_c.bmp");
	GLuint grassColor = loadBMP("Resources/Textures/Grass001_Diffuse.bmp");
	GLuint grassNormal = loadBMP("Resources/Textures/Grass001_Normal.bmp");
	GLuint munteColor = loadBMP("Resources/Textures/Rock020_4K-JPG_Color.bmp");
	GLuint munteNormal = loadBMP("Resources/Textures/munte_normal.bmp");
	GLuint castelTex = loadBMP("Resources/Textures/castel.bmp");
	GLuint stoneTex = loadBMP("Resources/Textures/Stone.bmp");
	GLuint gateTex = loadBMP("Resources/Textures/gate.bmp");
	/*GLuint right = loadBMP("Resources/Textures/right.bmp");
	GLuint left = loadBMP("Resources/Textures/left.bmp");
	GLuint top = loadBMP("Resources/Textures/top.bmp");
	GLuint bottom = loadBMP("Resources/Textures/bottom.bmp");
	GLuint back = loadBMP("Resources/Textures/back.bmp");
	GLuint front = loadBMP("Resources/Textures/front.bmp");*/
	//GLuint tex5 = loadBMP("Resources/Textures/grass.bmp");

	glEnable(GL_DEPTH_TEST);

	glfwSetScrollCallback(window.getWindow(), scroll_callback);
	glfwSetCursorPosCallback(window.getWindow(), mouse_callback);

	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(1.0f, 1.0f);

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(1.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 1.0f);

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3,   
		1, 2, 3 };

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures2;
	textures2.push_back(Texture());
	textures2[0].id = tex2;
	textures2[0].type = "texture_diffuse";

	std::vector<Texture> textures3;
	textures3.push_back(Texture());
	textures3[0].id = tex3;
	textures3[0].type = "texture_diffuse";

	std::vector<Texture> textures4;
	textures4.push_back(Texture());
	textures4[0].id = tex4;
	textures4[0].type = "texture_diffuse";

	/*std::vector<Texture> textures5;
	textures4.push_back(Texture());
	textures4[0].id = tex5;
	textures4[0].type = "texture_diffuse";*/


	Mesh mesh(vert, ind, textures3);

	std::vector<Texture> emptyTextures;

	// Create Obj files - easier :)
	// we can add here our textures :)
	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh box = loader.loadObj("Resources/Models/cube.obj", textures);
	Mesh plane = loader.loadObj("Resources/Models/plane.obj", emptyTextures);
	Mesh mountain1 = loader.loadObj("Resources/Models/mountain1.obj", emptyTextures);
	Mesh mountain2 = loader.loadObj("Resources/Models/mountain2.obj", emptyTextures);
	Mesh castle = loader.loadObj("Resources/Models/Castle.obj", emptyTextures);
	Mesh wall = loader.loadObj("Resources/Models/Wall.obj", emptyTextures);
	//Mesh skybox = loader.loadObj("Resources/Models/box.obj");
	//Mesh terrain = loader.loadObj("Resources/Models/Terrain2k.obj", textures4);
	//Mesh player = loader.loadObj("Resources/Models/knight.obj", textures2);
	//Mesh terrain = loader.loadObj("Resources/Models/grass.obj", textures3);

	std::vector<std::string> faces = {
	"Resources/Textures/right.bmp",
	"Resources/Textures/left.bmp",
	"Resources/Textures/top.bmp",
	"Resources/Textures/bottom.bmp",
	"Resources/Textures/back.bmp",
	"Resources/Textures/front.bmp"
	};

	unsigned int cubemapID = loadCubeMap(faces);

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput();
		//mouse_callback(window.getWindow(), lastX, lastY);


		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressing mouse button" << std::endl;
		}
		 //// Code for the light ////

		float horizontalDist = cameraDistance * cos(glm::radians(camera.getRotationOx()));
		float verticalDist = cameraDistance * sin(glm::radians(camera.getRotationOx()));

		float offsetX = horizontalDist * sin(glm::radians(camera.getRotationOy()));
		float offsetZ = horizontalDist * cos(glm::radians(camera.getRotationOy()));

		//vec3 cameraOffset = vec3(0.0f, 15.0f, cameraDistance);
		//vec3 currentCameraPos = playerPos + cameraOffset;
		vec3 currentCameraPos = vec3(playerPos.x - offsetX, playerPos.y + verticalDist, playerPos.z - offsetZ);

		glm::mat4 ViewMatrix = glm::lookAt(currentCameraPos, playerPos, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);

		sunShader.use();

		
		//glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");

		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);

		//// End code for the light ////

		shader.use();

		///// Test Obj files for box ////

		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, playerPos);
		//ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		//glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);
		box.draw(shader);
		//player.draw(shader);

		///// Test plane Obj file //////
		//Drawing the plane
		terrainShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassColor);
		glUniform1i(glGetUniformLocation(terrainShader.getId(), "texture_diffuse"), 0);

		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassNormal);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);*/

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 10.0f, 0.0f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 1.0f, 10.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glUniform3f(glGetUniformLocation(terrainShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(terrainShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(terrainShader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);

		plane.draw(terrainShader);

		//mountainShader.use();
		shader.use();

		glUniform3f(glGetUniformLocation(mountainShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(mountainShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(mountainShader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);

		/*glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);*/


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, munteColor);
		glUniform1i(glGetUniformLocation(mountainShader.getId(), "texture_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, munteNormal);
		glUniform1i(glGetUniformLocation(mountainShader.getId(), "texture_normal"), 1);
		//Primul Munte
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(75.0f, 4.0f, -350.0f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mountain1.draw(mountainShader);
		//Al doilea Munte
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 4.0f, -350.0f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mountain2.draw(mountainShader);

		//Castel
		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, castelTex);
		glUniform1i(glGetUniformLocation(mountainShader.getId(), "texture_diffuse"), 0);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 9.0f, -100.0f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		castle.draw(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stoneTex);
		glUniform1i(glGetUniformLocation(mountainShader.getId(), "texture_diffuse"), 0);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 5.0f, -10.0f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		wall.draw(shader);
		//Terenul
		//ModelMatrix = glm::mat4(1.0);
		//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 5.0f, -50.0f));
		////ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		////ModelMatrix = scale(ModelMatrix, glm::vec3(40.0f, 1.0f, 40.0f));
		//MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		//glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		//terrain.draw(shader);


		/*ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 5.0f, 2.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		terrain.draw(shader);*/

		//glDepthFunc(GL_LEQUAL); // Skybox passes depth test at maximum distance
		//skyboxShader.use();
		//mat4 skyboxView = mat4(mat3(ViewMatrix));

		//// Remove translation from the view matrix so the sky follows the player

		//// Set uniforms for the skybox shader
		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "view"), 1, GL_FALSE, &skyboxView[0][0]);

		//// Bind the cubemap and draw the cube
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
		//glUniform1i(glGetUniformLocation(skyboxShader.getId(), "skybox"), 0);
		//skybox.draw(skyboxShader); // Using your 'box' mesh for geometry

		//glDepthFunc(GL_LESS); // Reset depth function to default

		window.update();
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	
	//float x = xpos;
	//float y = ypos;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - float(ypos);
	lastX = xpos;
	lastY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		camera.rotateOy(xoffset);
		camera.rotateOx(yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraDistance -= (float)yoffset * 1.5f;
}

void processKeyboardInput()
{
	float moveSpeed = 30 * deltaTime;

	//translation
	if (window.isPressed(GLFW_KEY_W))
		playerPos.z -= moveSpeed;
	if (window.isPressed(GLFW_KEY_S))
		playerPos.z += moveSpeed;
	if (window.isPressed(GLFW_KEY_A))
		playerPos.x -= moveSpeed;
	if (window.isPressed(GLFW_KEY_D))
		playerPos.x += moveSpeed;
	if (window.isPressed(GLFW_KEY_R))
		playerPos.y += moveSpeed;
	if (window.isPressed(GLFW_KEY_F))
		playerPos.y -= moveSpeed;

	//rotation
	if (window.isPressed(GLFW_KEY_LEFT))
		camera.rotateOy(moveSpeed);
	if (window.isPressed(GLFW_KEY_RIGHT))
		camera.rotateOy(-moveSpeed);
	if (window.isPressed(GLFW_KEY_UP))
		camera.rotateOx(moveSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.rotateOx(-moveSpeed);
}

unsigned int loadCubeMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (unsigned int i = 0; i < faces.size(); i++){
		unsigned int width, height;
		unsigned char* data = loadRawBMP(faces[i].c_str(), width, height);
		if (data){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data
			);
			delete[] data;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}
