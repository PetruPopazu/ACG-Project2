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

vec3 playerPos = vec3(-20.0f, 15.0f, 250.0f);
float playerRoataion = 0.0f;

struct Apple {
	vec3 position;
	bool isEaten;
};

std::vector<Apple> mapApples = {
    { vec3(0.0f, 19.0f, 250.0f), false },
	{ vec3(10.0f, 19.0f, 255.0f), false },
	{ vec3(-10.0f, 19.0f, 245.0f), false},
	{ vec3(-10.0f, 19.0f, 255.0f), false},
	{ vec3(-10.0f, 19.0f, 265.0f), false},
	{ vec3(-10.0f, 19.0f, 265.0f), false}
};

vec3 goblin1Pos = vec3(235.0f, 15.0f, -360.0f);
vec3 goblin2Pos = vec3(265.0f, 15.0f, -380.0f);
vec3 goblin3Pos = vec3(300.0f, 15.0f, -344.0f);

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
	GLuint culoareCopac1 = loadBMP("Resources/Textures/culoare_copac1.bmp");
	GLuint culoareCopac2 = loadBMP("Resources/Textures/culoare_copac2.bmp");
	GLuint culoareCopac3 = loadBMP("Resources/Textures/culoare_copac3.bmp");
	GLuint vrajiculoare = loadBMP("Resources/Textures/vraji_color.bmp");
	GLuint vrajinormal = loadBMP("Resources/Textures/vraji_normal.bmp");
	GLuint appleC = loadBMP("Resources/Textures/apple_color.bmp");
	GLuint appleN = loadBMP("Resources/Textures/apple_normal.bmp");
	GLuint king_bob = loadBMP("Resources/Textures/king_bob.bmp");
	GLuint gold = loadBMP("Resources/Textures/gold.bmp");
	GLuint cow_texture = loadBMP("Resources/Textures/cow_tex.bmp");
	GLuint fan_color = loadBMP("Resources/Textures/fan_color.bmp");
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
	Mesh tree3 = loader.loadObj("Resources/Models/tree3.obj", emptyTextures);
	Mesh vraji = loader.loadObj("Resources/Models/vraji.obj", emptyTextures);
	Mesh apple = loader.loadObj("Resources/Models/apple.obj", emptyTextures);
	Mesh cap = loader.loadObj("Resources/Models/marianCap.obj", emptyTextures);
	Mesh king_bobModel = loader.loadObj("Resources/Models/king_bob.obj", emptyTextures);
	Mesh cow = loader.loadObj("Resources/Models/cow.obj", emptyTextures);
	Mesh fan = loader.loadObj("Resources/Models/fan.obj", emptyTextures);
	Mesh stoneRoad = loader.loadObj("Resources/Models/stoneRoad.obj", emptyTextures);

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
		{
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

			//capul
			mat4 capModel = torsoModel;
			capModel = translate(capModel, vec3(0.0f, 1.5f, 0.0f));
			mat4 capMVP = ProjectionMatrix * ViewMatrix * capModel;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &capMVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &capModel[0][0]);
			cap.draw(shader);
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
			{
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
			}
			//Health bar
			{
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
			}
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

		}
		//gobin1
		{
			//firstly the torso
			mat4 torsoModel = mat4(1.0f);
			torsoModel = translate(torsoModel, goblin1Pos);
			torsoModel = rotate(torsoModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
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
			/*if (isSwinging) {
				float swingAngle = sin(swingTimer) * 90.0f;
				rHandModel = rotate(rHandModel, swingAngle, vec3(1.0f, 0.0f, 0.0f));
			}*/
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
			/*if (window.isPressed(GLFW_KEY_W) || window.isPressed(GLFW_KEY_S) || window.isPressed(GLFW_KEY_A) || window.isPressed(GLFW_KEY_D)) {
				float swingAngle = sin(currentFrame * 10.0f) * 30.0f;
				lLegModel = rotate(lLegModel, swingAngle, vec3(1.0f, 0.0f, 0.0f));
			}*/
			lLegModel = rotate(lLegModel, 180.0f, vec3(1.0f, 0.0f, 0.0f));
			lLegModel = rotate(lLegModel, 4.0f, vec3(0.0f, 0.0f, 1.0f));
			lLegModel = scale(lLegModel, vec3(0.1f, 0.6f, 0.09f));
			mat4 lLegMVP = ProjectionMatrix * ViewMatrix * lLegModel;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &lLegMVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &lLegModel[0][0]);
			box.draw(shader);


			//right leg
			{
				mat4 rLegModel = torsoModel;
				rLegModel = translate(rLegModel, vec3(0.4f, -1.1f, 0.2f));
				/*if (window.isPressed(GLFW_KEY_W) || window.isPressed(GLFW_KEY_S) || window.isPressed(GLFW_KEY_A) || window.isPressed(GLFW_KEY_D)) {
					float swingAngle = sin(currentFrame * 10.0f + 3.14f) * 30.0f;
					rLegModel = rotate(rLegModel, swingAngle, vec3(1.0f, 0.0f, 0.0f));
				}*/
				rLegModel = rotate(rLegModel, 180.0f, vec3(1.0f, 0.0f, 0.0f));
				rLegModel = rotate(rLegModel, -4.0f, vec3(0.0f, 0.0f, 1.0f));
				rLegModel = scale(rLegModel, vec3(0.1f, 0.6f, 0.09f));
				mat4 rLegMVP = ProjectionMatrix * ViewMatrix * rLegModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &rLegMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &rLegModel[0][0]);
				box.draw(shader);
			}
			//Health bar
			{
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
			}
			// 2. Foreground (Green health)
			float healthWidth = (playerHealth / maxHealth) * 0.3f;
			mat4 healthBarModel = torsoModel;

			// The offset (-1.0 + healthWidth/2.0) makes the bar expand from the left
			healthBarModel = translate(healthBarModel, vec3(-1.0f + (healthWidth / 2.0f), 2.5f, 0.01f));
			healthBarModel = scale(healthBarModel, vec3(healthWidth, 0.05f, 0.1f));

			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix* ViewMatrix* healthBarModel)[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
			box.draw(shader);
		}
		//goblin2
		{
			//firstly the torso
			mat4 torsoModel = mat4(1.0f);
			torsoModel = translate(torsoModel, goblin2Pos);
			torsoModel = rotate(torsoModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
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
			lLegModel = rotate(lLegModel, 180.0f, vec3(1.0f, 0.0f, 0.0f));
			lLegModel = rotate(lLegModel, 4.0f, vec3(0.0f, 0.0f, 1.0f));
			lLegModel = scale(lLegModel, vec3(0.1f, 0.6f, 0.09f));
			mat4 lLegMVP = ProjectionMatrix * ViewMatrix * lLegModel;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &lLegMVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &lLegModel[0][0]);
			box.draw(shader);


			//right leg
			{
				mat4 rLegModel = torsoModel;
				rLegModel = translate(rLegModel, vec3(0.4f, -1.1f, 0.2f));
				rLegModel = rotate(rLegModel, 180.0f, vec3(1.0f, 0.0f, 0.0f));
				rLegModel = rotate(rLegModel, -4.0f, vec3(0.0f, 0.0f, 1.0f));
				rLegModel = scale(rLegModel, vec3(0.1f, 0.6f, 0.09f));
				mat4 rLegMVP = ProjectionMatrix * ViewMatrix * rLegModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &rLegMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &rLegModel[0][0]);
				box.draw(shader);
			}
			//Health bar
			{
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

			float healthPercent = (playerHealth / maxHealth);
			float healthWidth = healthPercent * 0.3f;
			mat4 healthBarModel = torsoModel;

			float leftEdge = -0.15f;
			healthBarModel = translate(healthBarModel, vec3(leftEdge + (healthWidth / 2.0f), 2.5f, 0.01f));
			healthBarModel = scale(healthBarModel, vec3(healthWidth, 0.05f, 0.1f));

			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBarModel)[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
			box.draw(shader);
		}
		//goblin3
		{
			//firstly the torso
			mat4 torsoModel = mat4(1.0f);
			torsoModel = translate(torsoModel, goblin3Pos);
			torsoModel = rotate(torsoModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
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
			{
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
			}
			//Health bar
			{
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
			}
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
		}

		//apples
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, appleC);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, appleN);
		glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
		for (const auto& appleItem : mapApples) {
			if (!appleItem.isEaten) {
				float appleBob = sin(glfwGetTime() * 2.0f) * 0.5f;

				mat4 appleModel = mat4(1.0f);
				appleModel = translate(appleModel, appleItem.position + vec3(0.0f, appleBob, 0.0f));
				appleModel = scale(appleModel, vec3(0.1f, 0.1f, 0.1f));

				mat4 appleMVP = ProjectionMatrix * ViewMatrix * appleModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &appleMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &appleModel[0][0]);
				apple.draw(shader);
			}
		}

		//Drawing the plane
		terrainShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassColor);
		glUniform1i(glGetUniformLocation(terrainShader.getId(), "texture_diffuse"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grassNormal);
		glUniform1i(glGetUniformLocation(terrainShader.getId(), "texture_normal"), 1);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 10.0f, 0.0f));
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
		//king bob
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, king_bob);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-30.0f, 17.25f, -235.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			king_bobModel.draw(shader);
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
		//statues
		{
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
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, 50.0f));
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
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-215.0f, 10.0f, -205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 130.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				church_StatueModel.draw(shader);
			}
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

			float bobbingOffset = sin(currentFrame * 2.0f) * 0.5f;

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(162.0f, 12.0f + bobbingOffset, 0.5f));
			ModelMatrix = glm::rotate(ModelMatrix, radians(currentFrame * 150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, -70.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.9f, 0.9f, 0.9f));
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
			//villagers
			{
				//villager1
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(35.0f, 12.5f, -20.0f));
					ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager2
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(55.0f, 12.5f, -45.0f));
					ModelMatrix = rotate(ModelMatrix, 160.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager3
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 12.5f, -25.0f));
					ModelMatrix = rotate(ModelMatrix, 120.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager_caruta
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 17.0f, -35.0f));
					ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager_blacksmith
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 12.5f, -2.0f));
					ModelMatrix = rotate(ModelMatrix, -70.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager_copil1
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 12.5f, -235.0f));
					//ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager7_copil2
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-15.0f, 12.5f, -235.0f));
					//ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager_turn1
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 90.0f, 70.0f));
					//ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
				//villager_turn2
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, king_bob);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(25.0f, 90.0f, 70.0f));
					//ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}
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
			ModelMatrix = scale(ModelMatrix, glm::vec3(8.0f, 8.0f, 8.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			signModel.draw(shader);
		}
		//witchhouse
		{
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
			//ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				witchroomModel.draw(shader);
			}
			//witch
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, vrajiculoare);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, vrajinormal);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-210.0f, 10.0f, 185.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				vraji.draw(shader);
			}
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
		//Fences
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
		//fan
		{	
			//fan1
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fan_color);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-224.0f, 10.0f, -142.5f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);


			//fan2
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fan_color);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-207.0f, 10.0f, -169.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			//fan3
			shader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fan_color);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-183.0f, 10.0f, -144.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, -10.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);
		}
		//multe vaci
		{
			//vaca1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-215.0f, 13.0f, -169.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, -70.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(9.0f, 9.0f, 9.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				cow.draw(shader);
			}
			//vaca2
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-183.0f, 13.0f, -130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca3
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-169.0f, 13.0f, -130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 120.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(9.0f, 9.0f, 9.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca4
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-189.0f, 13.0f, -160.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 100.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca5
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-199.0f, 13.0f, -148.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -120.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(9.0f, 9.0f, 9.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca6
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-203.0f, 13.0f, -136.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -120.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca7
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-195.0f, 13.0f, -124.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 120.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(9.0f, 9.0f, 9.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca8
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-224.0f, 13.0f, -152.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca9
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-224.0f, 13.0f, -133.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(9.0f, 9.0f, 9.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
			//vaca10
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cow_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-178.0f, 13.0f, -158.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, -85.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
		}
		//toti copacii
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(261.0f, 10.0f, -293.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(174.0f, 10.0f, -272.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(229.0f, 10.0f, -329.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(270.0f, 10.0f, -210.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(161.0f, 10.0f, -289.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(151.0f, 10.0f, -330.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(190.0f, 10.0f, -311.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(173.0f, 10.0f, -342.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(210.0f, 10.0f, -281.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(140.0f, 10.0f, -311.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -230.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(140.0f, 10.0f, -272.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(132.0f, 10.0f, -290.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -272.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(215.0f, 10.0f, -237.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(255.0f, 10.0f, -245.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -272.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(200.0f, 10.0f, -260.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(145.0f, 10.0f, -350.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(270.0f, 10.0f, -260.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(275.0f, 10.0f, -235.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(190.0f, 10.0f, -285.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);


			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(215.0f, 10.0f, -305.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(205.0f, 10.0f, -320.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(165.0f, 10.0f, -370.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(202.0f, 10.0f, -335.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(115.0f, 10.0f, -305.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(120.0f, 10.0f, -325.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(185.0f, 10.0f, -355.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(203.0f, 10.0f, -375.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(257.0f, 10.0f, -307.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(293.0f, 10.0f, -240.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(285.0f, 10.0f, -315.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(310.0f, 10.0f, -275.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(135.0f, 10.0f, -360.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(147.0f, 10.0f, -380.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);



			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(117.0f, 10.0f, -384.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);



			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(300.0f, 10.0f, -306.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);


			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(220.0f, 10.0f, -386.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(212.0f, 10.0f, -352.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(300.0f, 10.0f, -380.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(312.0f, 10.0f, -361.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(317.0f, 10.0f, -393.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(308.0f, 10.0f, -408.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); \

				ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(330.0f, 10.0f, -420.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			//parcela1
			{

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(15.0f, 10.0f, 131.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
				
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(13.0f, 10.0f, 150.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(55.0f, 10.0f, 120.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(79.0f, 10.0f, 138.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(39.0f, 10.0f, 109.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(42.0f, 10.0f, 121.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(63.0f, 10.0f, 109.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(25.0f, 10.0f, 250.0f));
				ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(45.0f, 10.0f, 275.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(56.0f, 10.0f, 299.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(30.0f, 10.0f, 200.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(77.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(33.0f, 10.0f, 162.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(54.0f, 10.0f, 188.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(98.0f, 10.0f, 188.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(17.0f, 10.0f, 255.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(75.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(8.0f, 10.0f, 121.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(68.0f, 10.0f, 125.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(34.0f, 10.0f, 100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(99.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(53.0f, 10.0f, 200.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(108.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(63.0f, 10.0f, 130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(53.0f, 10.0f, 200.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(33.0f, 10.0f, 259.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, 10.0f, 283.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(79.0f, 10.0f, 250.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, 10.0f, 239.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(95.0f, 10.0f, 214.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(110.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(79.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(67.0f, 10.0f, 199.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(84.0f, 10.0f, 180.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(55.0f, 10.0f, 167.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(103.0f, 10.0f, 195.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(88.0f, 10.0f, 177.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(50.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 10.0f, 190.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, 10.0f, 160.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, 10.0f, 245.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(75.0f, 10.0f, 233.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(96.0f, 10.0f, 229.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(30.0f, 10.0f, 245.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(20.0f, 10.0f, 232.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(23.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(33.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(34.0f, 10.0f, 263.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(40.0f, 10.0f, 222.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(28.0f, 10.0f, 230.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
			}

			//parcela2
			{	int a = 100;
			//alt copac cu mere
			shader.use();

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 121.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 89.0f, 10.0f, 109.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 116.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 52.0f, 10.0f, 142.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 108.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 35.0f, 10.0f, 250.0f));
			ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 275.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 200.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 10.0f, 10.0f, 220.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 193.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 162.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 54.0f, 10.0f, 188.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 98.0f, 10.0f, 188.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 110.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 8.0f, 10.0f, 121.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 125.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 99.0f, 10.0f, 193.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 130.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);

			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 259.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 80.0f, 10.0f, 210.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 239.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 95.0f, 10.0f, 214.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 205.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 199.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 84.0f, 10.0f, 180.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 167.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 88.0f, 10.0f, 177.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 170.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 50.0f, 10.0f, 170.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 85.0f, 10.0f, 190.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 160.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 245.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 233.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 96.0f, 10.0f, 229.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 245.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac2);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 210.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);



			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 40.0f, 10.0f, 222.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);


			//alt copac cu mere
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac1);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			tree3.draw(shader);
			}
			//parcela3
			{
				int a = 200;
				//alt copac cu mere
				shader.use();
				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 105.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 89.0f, 10.0f, 119.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 128.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 32.0f, 10.0f, 121.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 135.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 25.0f, 10.0f, 250.0f));
				ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 275.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 200.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 10.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 162.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 54.0f, 10.0f, 188.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 98.0f, 10.0f, 188.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 8.0f, 10.0f, 121.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 125.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 99.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 259.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 80.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 239.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 95.0f, 10.0f, 214.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 199.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 84.0f, 10.0f, 180.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 167.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 88.0f, 10.0f, 177.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 50.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 85.0f, 10.0f, 190.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 160.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 245.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 233.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 96.0f, 10.0f, 229.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 245.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 40.0f, 10.0f, 222.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
			}
			//parcela 4
			{

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-69.0f, 10.0f, 129.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-56.0f, 10.0f, 299.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-77.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-98.0f, 10.0f, 188.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-68.0f, 10.0f, 125.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-99.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-108.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-63.0f, 10.0f, 130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-80.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, 10.0f, 283.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-79.0f, 10.0f, 250.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, 10.0f, 239.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-95.0f, 10.0f, 214.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-110.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-79.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-67.0f, 10.0f, 199.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-84.0f, 10.0f, 180.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-103.0f, 10.0f, 195.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-88.0f, 10.0f, 177.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-50.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-85.0f, 10.0f, 190.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 10.0f, 160.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 10.0f, 245.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 10.0f, 233.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-96.0f, 10.0f, 229.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
			}
			//parcela 5
			{
				int a = -215;
				
				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 89.0f, 10.0f, 139.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 108.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 129.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 25.0f, 10.0f, 250.0f));
				ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 275.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 98.0f, 10.0f, 188.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 125.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 99.0f, 10.0f, 193.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 259.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 80.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 239.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 95.0f, 10.0f, 214.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 205.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 199.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 84.0f, 10.0f, 180.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 88.0f, 10.0f, 177.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 170.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 85.0f, 10.0f, 190.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 233.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 96.0f, 10.0f, 229.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 210.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.2f, 3.2f, 3.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 40.0f, 10.0f, 222.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
			}
			//parcela 6
			{
				int a = -320;
				shader.use();
				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 52.0f, 10.0f, 112.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 134.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 42.0f, 10.0f, 131.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 29.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 289.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 10.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 120.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 8.0f, 10.0f, 121.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 145.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 73.0f, 10.0f, 130.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 249.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				////alt copac cu mere
				//shader.use();

				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				//glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				//ModelMatrix = glm::mat4(1.0);
				//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
				////ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				////ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				//ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				//MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				//glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				//tree3.draw(shader);


				//alt copac cu mere
				//shader.use();

				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				//glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				//ModelMatrix = glm::mat4(1.0);
				//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
				////ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				////ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				//ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				//MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				//glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				//tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 140.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 263.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.2f, 3.2f, 3.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//ModelMatrix = rotate(ModelMatrix, 50.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				tree3.draw(shader);
			}
		}
		//Road1
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, roadC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, roadN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 250.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
			ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			stoneRoad.draw(shader);
		}
		//Roads
		{
			//Road2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 190.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 160.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road6
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road7
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 70.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road8
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 40.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road9
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, 10.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road10
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, -20.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road11
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, -50.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road12
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, -80.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road13
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, -110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road14
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, -140.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//Road15
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-20.0f, 9.0f, -170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadStanga1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-45.0f, 9.0f, -27.5f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadStanga2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 9.0f, -27.5f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadStanga3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-105.0f, 9.0f, -27.5f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreapta1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f, 9.0f, 25.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreapta2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(35.0f, 9.0f, 25.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreapta3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(65.0f, 9.0f, 25.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreapta4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(95.0f, 9.0f, 25.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreapta5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(125.0f, 9.0f, 25.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptasStanga1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(130.0f, 9.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptasStanga2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(130.0f, 9.0f, -30.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptasStanga3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(130.0f, 9.0f, -60.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptaStangaStanga1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(125.0f, 9.0f, -85.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptaStangaStanga2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(95.0f, 9.0f, -85.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptaStangaStanga3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(65.0f, 9.0f, -85.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptaStangaStanga4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(35.0f, 9.0f, -85.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadDreaptaStangaStanga5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(05.0f, 9.0f, -85.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				//ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadBiserica1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-40.0f, 9.0f, -140.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 150.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadBiserica2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-66.0f, 9.0f, -155.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 150.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadBiserica3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-92.0f, 9.0f, -170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 150.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadBiserica4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-118.0f, 9.0f, -185.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 150.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
			//RoadBiserica5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, roadC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, roadN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-144.0f, 9.0f, -200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
				ModelMatrix = rotate(ModelMatrix, 150.0f, vec3(0.0f, 1.0f, 0.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				stoneRoad.draw(shader);
			}
		}


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

	if (window.isPressed(GLFW_KEY_E)) {
		for (int i = 0; i < mapApples.size(); i++) {
			if (!mapApples[i].isEaten) {
				float dist = glm::distance(playerPos, mapApples[i].position);
				if (dist < 5.0f) {
					mapApples[i].isEaten = true;
					playerHealth += 20.0f;
					if (playerHealth > maxHealth) playerHealth = maxHealth;
				}
			}
		}
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
