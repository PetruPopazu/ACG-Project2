#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"

using namespace glm;

void processKeyboardInput();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubeMap(std::vector<std::string> faces);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool firstMouse = true;
float lastX = 400, lastY = 400;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cameraDistance = 20.0f;

float playerHealth = 20.0f;
float maxHealth = 100.0f;

float swingTimer = 0.0f;
bool isSwinging = false;
float swingSpeed = 5.0f;

Window window("Marian - The time traveler", 1024, 960);
Camera camera;

vec3 playerPos = vec3(0.0f, 15.0f, 250.0f);
float playerRoataion = 0.0f;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(180.0f, 200.0f, 350.0f);

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
	GLuint handNormal = loadBMP("Resources/Textures/hand_normal.bmp");
	GLuint handDiffuse = loadBMP("Resources/Textures/hand_diffuse.bmp");
	GLuint treeTex = loadBMP("Resources/Textures/Tree_Branch.bmp");
	GLuint blacksmith = loadBMP("Resources/Textures/blacksmith.bmp");
	GLuint tent = loadBMP("Resources/Textures/tent.bmp");
	GLuint house = loadBMP("Resources/Textures/house.bmp");
	GLuint well = loadBMP("Resources/Textures/well.bmp");
	GLuint barrels = loadBMP("Resources/Textures/barrels.bmp");
	GLuint MedievalC = loadBMP("Resources/Textures/MedievalColor.bmp");
	GLuint MedievalN = loadBMP("Resources/Textures/MedievalNormal.bmp");
	GLuint marketC = loadBMP("Resources/Textures/marketC.bmp");
	GLuint marketN = loadBMP("Resources/Textures/marketN.bmp");
	GLuint witchroomC = loadBMP("Resources/Textures/witchroomC.bmp");
	GLuint witchroomN = loadBMP("Resources/Textures/witchroomN.bmp");
	GLuint tombstoneC = loadBMP("Resources/Textures/tombstoneC.bmp");
	GLuint tombstoneN = loadBMP("Resources/Textures/tombstoneN.bmp");
	GLuint fenceC = loadBMP("Resources/Textures/fenceC.bmp");
	GLuint fenceN = loadBMP("Resources/Textures/fenceN.bmp");
	GLuint churchC = loadBMP("Resources/Textures/churchC.bmp");
	GLuint swordN = loadBMP("Resources/Textures/churchN.bmp");
	GLuint swordC = loadBMP("Resources/Textures/swordC.bmp");
	GLuint churchN = loadBMP("Resources/Textures/swordN.bmp");
	GLuint signC = loadBMP("Resources/Textures/signC.bmp");
	GLuint signN = loadBMP("Resources/Textures/signN.bmp");
	GLuint tavernC = loadBMP("Resources/Textures/tavernC.bmp");
	GLuint tavernN = loadBMP("Resources/Textures/tavernN.bmp");
	GLuint armorC = loadBMP("Resources/Textures/armorC.bmp");
	GLuint armorN = loadBMP("Resources/Textures/armorN.bmp");
	GLuint fountainC = loadBMP("Resources/Textures/fountainC.bmp");
	GLuint fountainN = loadBMP("Resources/Textures/fountainN.bmp");
	GLuint statue1 = loadBMP("Resources/Textures/statue1.bmp");
	GLuint statue2C = loadBMP("Resources/Textures/statue2C.bmp");
	GLuint statue2N = loadBMP("Resources/Textures/statue2N.bmp");
	GLuint church_StatueC = loadBMP("Resources/Textures/church_StatueC.bmp");
	GLuint church_StatueN = loadBMP("Resources/Textures/church_StatueN.bmp");
	GLuint roadC = loadBMP("Resources/Textures/stoneC.bmp");
	GLuint roadN = loadBMP("Resources/Textures/stoneN.bmp");
	GLuint red = loadBMP("Resources/Textures/red.bmp");
	GLuint green = loadBMP("Resources/Textures/green.bmp");

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
	Mesh box = loader.loadObj("Resources/Models/cube.obj", emptyTextures);
	Mesh plane = loader.loadObj("Resources/Models/plane.obj", emptyTextures);
	Mesh mountain1 = loader.loadObj("Resources/Models/mountain1.obj", emptyTextures);
	Mesh mountain2 = loader.loadObj("Resources/Models/mountain2.obj", emptyTextures);
	Mesh castle = loader.loadObj("Resources/Models/Castle.obj", emptyTextures);
	Mesh wall = loader.loadObj("Resources/Models/Wall.obj", emptyTextures);
	Mesh torso = loader.loadObj("Resources/Models/torso.obj", textures2);
	Mesh tree = loader.loadObj("Resources/Models/tree.obj", emptyTextures);
	Mesh blacksmithModel = loader.loadObj("Resources/Models/blacksmith.obj", emptyTextures);
	Mesh tentModel = loader.loadObj("Resources/Models/tent.obj", emptyTextures);
	Mesh houseModel = loader.loadObj("Resources/Models/house.obj", emptyTextures);
	Mesh wellModel = loader.loadObj("Resources/Models/well.obj", emptyTextures);
	Mesh barrelsModel = loader.loadObj("Resources/Models/barrels.obj", emptyTextures);
	Mesh lamppostModel = loader.loadObj("Resources/Models/lamppost.obj", emptyTextures);
	Mesh cart2Model = loader.loadObj("Resources/Models/cart2.obj", emptyTextures);
	Mesh cart1Model = loader.loadObj("Resources/Models/cart1.obj", emptyTextures);
	Mesh marketModel = loader.loadObj("Resources/Models/market.obj", emptyTextures);
	Mesh campfireModel = loader.loadObj("Resources/Models/campfire.obj", emptyTextures);
	Mesh woodpileModel = loader.loadObj("Resources/Models/woodpile.obj", emptyTextures);
	Mesh marketstallModel = loader.loadObj("Resources/Models/marketstall.obj", emptyTextures);
	Mesh witchroomModel = loader.loadObj("Resources/Models/witchroom.obj", emptyTextures);
	Mesh tombstoneModel = loader.loadObj("Resources/Models/tombstone.obj", emptyTextures);
	Mesh fenceModel = loader.loadObj("Resources/Models/fence.obj", emptyTextures);
	Mesh churchModel = loader.loadObj("Resources/Models/church.obj", emptyTextures);
	Mesh signModel = loader.loadObj("Resources/Models/sign.obj", emptyTextures);
	Mesh swordModel = loader.loadObj("Resources/Models/sword.obj", emptyTextures);
	Mesh tavernModel = loader.loadObj("Resources/Models/tavern.obj", emptyTextures);
	Mesh armorModel = loader.loadObj("Resources/Models/armor.obj", emptyTextures);
	Mesh fountainModel = loader.loadObj("Resources/Models/fountain.obj", emptyTextures);
	Mesh statue1Model = loader.loadObj("Resources/Models/statue1.obj", emptyTextures);
	Mesh statue2Model = loader.loadObj("Resources/Models/statue2.obj", emptyTextures);
	Mesh church_StatueModel = loader.loadObj("Resources/Models/church_Statue.obj", emptyTextures);
	


	//Mesh hand = loader.loadObj("Resources/Models/hand2.obj", emptyTextures);
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

		float floorLevel = 10.0f;
		float characterHeight = 6.0f;

		if (playerPos.y > floorLevel + characterHeight) {
			playerPos.y -= 20.0f * deltaTime;
		}
		if (playerPos.y < floorLevel + characterHeight) {
			playerPos.y = floorLevel + characterHeight;
		}
		//mouse_callback(window.getWindow(), lastX, lastY);


		//test mouse input
		/*if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressing mouse button" << std::endl;
		}*/

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			isSwinging = true;
		}

		if (isSwinging) {
			swingTimer += deltaTime * swingSpeed;
			if (swingTimer > 3.14f) {
				swingTimer = 0.0f;
				isSwinging = false;
			}
		}


		//// Code for the light ////

		float horizontalDist = cameraDistance * cos(glm::radians(camera.getRotationOx()));
		float verticalDist = cameraDistance * sin(glm::radians(camera.getRotationOx()));

		float offsetX = horizontalDist * sin(glm::radians(camera.getRotationOy()));
		float offsetZ = horizontalDist * cos(glm::radians(camera.getRotationOy()));

		float targetCameraY = playerPos.y + verticalDist;
		float mapFloor = 10.0f;
		if (targetCameraY < mapFloor + 1.0f) {
			targetCameraY = mapFloor + 1.0f;
		}

		//vec3 cameraOffset = vec3(0.0f, 15.0f, cameraDistance);
		//vec3 currentCameraPos = playerPos + cameraOffset;
		vec3 currentCameraPos = vec3(playerPos.x - offsetX, targetCameraY, playerPos.z - offsetZ);

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

		//ModelMatrix = glm::mat4(1.0);
		//ModelMatrix = glm::translate(ModelMatrix, playerPos);
		////ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		//MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		//glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		//glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		//glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		////glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);
		//box.draw(shader);
		//player.draw(shader);

		//ModelMatrix = glm::mat4(1.0);
		//ModelMatrix = glm::translate(ModelMatrix, playerPos);
		////ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		//MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		//glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		//glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		//glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		////glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);
		//torso.draw(shader);

		//draw body parts
		//firstly the torso
		mat4 torsoModel = mat4(1.0f);
		torsoModel = translate(torsoModel, playerPos);
		torsoModel = rotate(torsoModel, radians(playerRoataion + 180.0f), vec3(0.0f, 1.0f, 0.0f));
		torsoModel = scale(torsoModel, vec3(2.0f, 2.0f, 2.0f));
		mat4 torsoMVP = ProjectionMatrix * ViewMatrix * torsoModel;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &torsoMVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &torsoModel[0][0]);
		torso.draw(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, handDiffuse);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, handNormal);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

		//then the right hand
		mat4 rHandModel = torsoModel;
		if (isSwinging) {
			float swingAngle = sin(swingTimer) * 90.0f;
			rHandModel = rotate(rHandModel, swingAngle, vec3(1.0f, 0.0f, 0.0f));
		}
		rHandModel = translate(rHandModel, vec3(0.7f, 0.3f, 0.4f));
		rHandModel = rotate(rHandModel, -80.0f, vec3(1.0f, 0.0f, 0.0f));
		rHandModel = rotate(rHandModel, -20.0f, vec3(0.0f, 1.0f, 0.0f));
		rHandModel = rotate(rHandModel, 10.0f, vec3(0.0f, 0.0f, 1.0f));
		rHandModel = scale(rHandModel, vec3(0.07f, 0.11f, -0.45f));
		mat4 rHandMVP = ProjectionMatrix * ViewMatrix * rHandModel;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &rHandMVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &rHandModel[0][0]);
		box.draw(shader);

		//then the left hand
		mat4 lHandModel = torsoModel;
		lHandModel = translate(lHandModel, vec3(-0.7f, 0.3f, 0.4f));
		lHandModel = rotate(lHandModel, -80.0f, vec3(1.0f, 0.0f, 0.0f));
		lHandModel = rotate(lHandModel, 20.0f, vec3(0.0f, 1.0f, 0.0f));
		lHandModel = rotate(lHandModel, -10.0f, vec3(0.0f, 0.0f, 1.0f));
		lHandModel = scale(lHandModel, vec3(0.07f, 0.11f, -0.45f));
		mat4 lHandMVP = ProjectionMatrix * ViewMatrix * lHandModel;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &lHandMVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &lHandModel[0][0]);
		box.draw(shader);

		//now let's draw the legs
		//left leg firstly
		mat4 lLegModel = torsoModel;
		lLegModel = translate(lLegModel, vec3(-0.4f, -1.1f, 0.2f));
		if (window.isPressed(GLFW_KEY_W) || window.isPressed(GLFW_KEY_S) || window.isPressed(GLFW_KEY_A) || window.isPressed(GLFW_KEY_D)) {
			float swingAngle = sin(currentFrame * 10.0f) * 30.0f;
			lLegModel = rotate(lLegModel, swingAngle, vec3(1.0f, 0.0f, 0.0f));
		}
		lLegModel = rotate(lLegModel, 180.0f, vec3(1.0f, 0.0f, 0.0f));
		lLegModel = rotate(lLegModel, 4.0f, vec3(0.0f, 0.0f, 1.0f));
		lLegModel = scale(lLegModel, vec3(0.1f, 0.6f, 0.09f));
		mat4 lLegMVP = ProjectionMatrix * ViewMatrix * lLegModel;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &lLegMVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &lLegModel[0][0]);
		box.draw(shader);


		//right leg
		mat4 rLegModel = torsoModel;
		rLegModel = translate(rLegModel, vec3(0.4f, -1.1f, 0.2f));
		if (window.isPressed(GLFW_KEY_W) || window.isPressed(GLFW_KEY_S) || window.isPressed(GLFW_KEY_A) || window.isPressed(GLFW_KEY_D)) {
			float swingAngle = sin(currentFrame * 10.0f + 3.14f) * 30.0f;
			rLegModel = rotate(rLegModel, swingAngle, vec3(1.0f, 0.0f, 0.0f));
		}
		rLegModel = rotate(rLegModel, 180.0f, vec3(1.0f, 0.0f, 0.0f));
		rLegModel = rotate(rLegModel, -4.0f, vec3(0.0f, 0.0f, 1.0f));
		rLegModel = scale(rLegModel, vec3(0.1f, 0.6f, 0.09f));
		mat4 rLegMVP = ProjectionMatrix * ViewMatrix * rLegModel;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &rLegMVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &rLegModel[0][0]);
		box.draw(shader);

		//Health bar

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, red);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
		mat4 healthBgModel = torsoModel;

		// Only translate relative to the torso's center (0,0,0)
		healthBgModel = translate(healthBgModel, vec3(0.0f, 2.5f, 0.0f));
		healthBgModel = scale(healthBgModel, vec3(0.3f, 0.05f, 0.1f));

		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBgModel)[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBgModel[0][0]); // Update model matrix for lighting
		glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.2f, 0.2f, 0.2f);
		box.draw(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, green);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

		// 2. Foreground (Green health)
		float healthWidth = (playerHealth / maxHealth) * 0.3f;
		mat4 healthBarModel = torsoModel;

		// The offset (-1.0 + healthWidth/2.0) makes the bar expand from the left
		healthBarModel = translate(healthBarModel, vec3(-1.0f + (healthWidth / 2.0f), 2.5f, 0.01f));
		healthBarModel = scale(healthBarModel, vec3(healthWidth, 0.05f, 0.1f));

		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBarModel)[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
		box.draw(shader);


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

		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);

		/*glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);*/


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, munteColor);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, munteNormal);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
		//Munti
		{
			//Primul Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(200.0f, -4.0f, -820.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				mountain1.draw(shader);
			}
			//Al doilea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-220.0f, -1.0f, -680.0f));
				ModelMatrix = glm::rotate(ModelMatrix, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				mountain2.draw(shader);
			}
			//Al treilea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-560.0f, -1.0f, -500.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, -60.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				mountain1.draw(shader);
			}
			//Al patrulea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-600.0f, -20.0f, -100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				mountain2.draw(shader);
			}
			//Al cincilea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(700.0f, -4.0f, -600.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, -95.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				mountain2.draw(shader);
			}
			//Al saselea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(550.0f, -25.0f, -100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				mountain1.draw(shader);
			}
		}
		//Castel
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, stoneTex);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-45.0f, 9.0f, -300.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.04f, 0.04f, 0.04f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			castle.draw(shader);
		}
		//Wall
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, stoneTex);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -28.0f, -180.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.2f));
			ModelMatrix = glm::rotate(ModelMatrix, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			wall.draw(shader);
		}
		//statue1
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, statue1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-70.0f, 10.0f, 50.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, -180.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			statue1Model.draw(shader);
		}
		//statue2
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, statue2C);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, statue2N);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(70.0f, 10.0f, 60.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			statue2Model.draw(shader);
		}
		//statue3
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, church_StatueC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, church_StatueN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-237.0f, 10.0f, -192.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			church_StatueModel.draw(shader);
		}
		//Copaci
		{
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, grassColor);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 28.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 2.0f, 3.0f));
			ModelMatrix = glm::rotate(ModelMatrix, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			tree.draw(shader);*/
		}
		//Tent
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tent);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-165.0f, 10.0f, -30.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.07f, 0.07f, 0.07f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tentModel.draw(shader);
		}
		//Blacksmith
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, blacksmith);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 24.0f, -5.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, -70.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(15.0f, 15.0f, 15.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			blacksmithModel.draw(shader);
		}
		//armor
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, armorC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, armorN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(162.0f, 12.0f, 0.5f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, -70.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			armorModel.draw(shader);
		}
		//fountain
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fountainC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, fountainN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-113.0f, 13.5f, -27.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fountainModel.draw(shader);
		}
		//Houses
		{
			//House1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-180.0f, 10.0f, 0.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				houseModel.draw(shader);
			}
			//House2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(150.0f, 10.0f, -130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -135.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				houseModel.draw(shader);
			}
			//House3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-175.0f, 10.0f, -80.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				houseModel.draw(shader);
			}
			//House4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 10.0f, -80.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -180.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				houseModel.draw(shader);
			}
		}
		//Well
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, well);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 10.0f, -50.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			wellModel.draw(shader);
		}
		//Barrels
		{
			//Barrel1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, barrels);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(155.0f, 10.0f, -25.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				barrelsModel.draw(shader);
			}
			//Barrel2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, barrels);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(155.0f, 10.0f, -28.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				barrelsModel.draw(shader);
			}
			//Barrel3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, barrels);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(155.0f, 13.5f, -26.5f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				barrelsModel.draw(shader);
			}
		}
		//Lampposts
		{
			//Lamppost1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-155.0f, 10.0f, 25.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 135.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
			//Lamppost2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-165.0f, 10.0f, -50.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
			//Lamppost3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-155.0f, 10.0f, -100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
			//Lamppost4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(120.0f, 10.0f, -140.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
			//Lamppost5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 10.0f, -50.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
			//Lamppost6
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(155.0f, 10.0f, 35.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -135.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
		}
		//Cart1
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, MedievalC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, MedievalN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, -55.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, 15.0f, vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			cart1Model.draw(shader);
		}
		//Cart2
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, MedievalC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, MedievalN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 10.0f, -35.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			cart2Model.draw(shader);
		}
		//MarketPlace
		{
			//market1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, marketC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, marketN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(50.0f, 12.0f, -55.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 15.0f, vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				marketModel.draw(shader);
			}
			//market2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, marketC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, marketN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(95.0f, 12.0f, -30.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				marketModel.draw(shader);
			}
			//market3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, marketC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, marketN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(65.0f, 12.0f, -5.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 190.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				marketModel.draw(shader);
			}
			//campfire
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, 10.0f, -30.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 15.0f, vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				campfireModel.draw(shader);
			}
			//woodpile
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(90.0f, 10.0f, 0.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 190.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				woodpileModel.draw(shader);
			}
			//marketstall
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MedievalC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MedievalN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(35.0f, 10.0f, -20.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				marketstallModel.draw(shader);
			}
		}
		//Church
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, churchC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, churchN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-200.0f, 10.0f, -240.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, -50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			churchModel.draw(shader);
		}
		//taverns
		{
			//tavern1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tavernC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tavernN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-120.0f, 10.0f, 45.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.5f, 3.5f, 3.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tavernModel.draw(shader);
			}
			//tavern2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tavernC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tavernN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(120.0f, 10.0f, 47.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tavernModel.draw(shader);
			}
		}
		//sign
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, signC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, signN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 10.0f, 103.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, 75.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			signModel.draw(shader);
		}
		//witchroom
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, witchroomC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, witchroomN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-225.0f, 10.0f, 200.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			witchroomModel.draw(shader);
		}
		//sword
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, swordC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, swordN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			float bobbingOffset = sin(currentFrame * 2.0f) * 0.5f;
			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-205.0f, 10.0f + bobbingOffset, 200.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			float rotationAngle = currentFrame * 150.0f;
			ModelMatrix = rotate(ModelMatrix, radians(rotationAngle), vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			swordModel.draw(shader);
		}
		//tombstones
		{
			//tombstone1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 10.0f, 230.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
			//tombstone2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 10.0f, 180.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
			//tombstone3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 10.0f, 160.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
			//tombstone4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-250.0f, 10.0f, 230.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
			//tombstone5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-250.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
			//tombstone6
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-250.0f, 10.0f, 190.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
			//tombstone7
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tombstoneC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tombstoneN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-250.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tombstoneModel.draw(shader);
			}
		}
		//fences
		{
			//fence1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, fenceC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, fenceN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-245.0f, 10.0f, 240.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fenceModel.draw(shader);
			}
			//fence2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, fenceC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, fenceN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-273.0f, 10.0f, 212.5f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fenceModel.draw(shader);
			}
			//fence3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, fenceC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, fenceN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-273.0f, 10.0f, 180.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fenceModel.draw(shader);
			}
			//fence4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, fenceC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, fenceN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-245.0f, 10.0f, 152.5f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fenceModel.draw(shader);
			}
		}

		//Road
		shader.use();

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, roadC);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, roadN);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 9.0f, 152.5f));
		ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
		ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		stoneRoad.draw(shader);*/


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

		//glDepthFunc(GL_LEQUAL);
		//skyboxShader.use();
		//mat4 skyboxView = mat4(mat3(ViewMatrix));

		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "view"), 1, GL_FALSE, &skyboxView[0][0]);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
		//glUniform1i(glGetUniformLocation(skyboxShader.getId(), "skybox"), 0);
		//skybox.draw(skyboxShader);

		//glDepthFunc(GL_LESS);

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
		camera.rotateOy(-xoffset);
		camera.rotateOx(-yoffset);
		playerRoataion = camera.getRotationOy();
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraDistance -= (float)yoffset * 1.5f;
	cameraDistance = glm::clamp(cameraDistance, 2.0f, 30.0f);
}


void processKeyboardInput()
{
	float moveSpeed = 100.0f * deltaTime;
	if (window.isPressed(GLFW_KEY_LEFT_SHIFT))
		moveSpeed *= 2.0f;
	bool isMoving = false;
	vec3 moveDir = vec3(0.0f);
	float angleRad = camera.getRotationOy();
	vec3 forward = vec3(sin(radians(angleRad)), 0.0f, cos(radians(angleRad)));
	vec3 right = vec3(cos(radians(angleRad)), 0.0f, -sin(radians(angleRad)));

	if (window.isPressed(GLFW_KEY_W)) {
		moveDir += forward;
		isMoving = true;
	}
	if (window.isPressed(GLFW_KEY_S)) {
		moveDir -= forward;
		isMoving = true;
	}
	if (window.isPressed(GLFW_KEY_A)) {
		moveDir += right;
		isMoving = true;
	}
	if (window.isPressed(GLFW_KEY_D)) {
		moveDir -= right;
		isMoving = true;
	}

	if (isMoving) {
		//moveDir = normalize(moveDir);
		playerPos += moveDir * moveSpeed;
		playerRoataion = radians(atan2(moveDir.x, moveDir.z));
	}

	if (window.isPressed(GLFW_KEY_R))
		playerPos.y += moveSpeed;
	if (window.isPressed(GLFW_KEY_F))
		playerPos.y -= moveSpeed;
	if (window.isPressed(GLFW_KEY_LEFT_SHIFT))
		moveSpeed *= 2.0f;

	//translation
	/*if (window.isPressed(GLFW_KEY_W))
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
	if (window.isPressed(GLFW_KEY_LEFT_SHIFT))
		moveSpeed *= 2.0f;*/

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
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned int width, height;
		unsigned char* data = loadRawBMP(faces[i].c_str(), width, height);
		if (data) {
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
