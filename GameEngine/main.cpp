#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"

//#include "C:\ECG\acg\Dependencies\imgui\imgui.h"// -- Alexutz
//#include "C:\ECG\acg\Dependencies\imgui\backends\imgui_impl_glfw.h"// -- Alexutz
//#include "C:\ECG\acg\Dependencies\imgui\backends\imgui_impl_opengl3.h"// -- Alexutz
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\imgui.h"// -- Petru Calc
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\backends\imgui_impl_glfw.h"// -- Petru Calc
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\backends\imgui_impl_openl3.h"// -- Petru Calc
//#include "C:\ECG\acg\Dependencies\imgui\imgui.h" //-- Alexutz
//#include "C:\ECG\acg\Dependencies\imgui\backends\imgui_impl_glfw.h" //-- Alexutz
//#include "C:\ECG\acg\Dependencies\imgui\backends\imgui_impl_opengl3.h" //-- Alexutz
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\imgui.h" //-- Petru Calc
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\backends\imgui_impl_glfw.h" //-- Petru Calc
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\backends\imgui_impl_opengl3.h" //-- Petru Calc
//#include "C:\Users\Popazu\Desktop\ECG\ProjGit\ACG-Project2\Dependencies\imgui\imgui.h" //-- Alex
//#include "C:\Users\Popazu\Desktop\ECG\ProjGit\ACG-Project2\Dependencies\imgui\backends\imgui_impl_glfw.h" //-- Alex
//#include "C:\Users\Popazu\Desktop\ECG\ProjGit\ACG-Project2\Dependencies\imgui\backends\imgui_impl_opengl3.h" //--Alex

#include "D:\ACG\ACG-Project2\Dependencies\imgui\imgui.h"
#include "D:\ACG\ACG-Project2\Dependencies\imgui\backends\imgui_impl_glfw.h"
#include "D:\ACG\ACG-Project2\Dependencies\imgui\backends\imgui_impl_opengl3.h"

//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\imgui.h"
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\backends\imgui_impl_glfw.h"
//#include "H:\alexutzvaci\PetruPopazu\ACG-Project2\Dependencies\imgui\backends\imgui_impl_opengl3.h"

using namespace glm;

struct Collide {
	vec3 min, max;
};

const float square_size = 20.0f;
const float grid_size = 200.0f;
const float offset = 1000.0f;//we use the offset in order to have positive values for the array
std::vector<Collide> grid[200][200];

void registerCollide(Mesh& mesh, vec3 worldPos, vec3 scale);
bool isColliding(vec3 playerPos);

void processKeyboardInput();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool firstMouse = true;
float lastX = 400, lastY = 400;

int applecnt = 0;

vec3 worldPos = vec3(0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cameraDistance = 20.0f;

float playerHealth = 20.0f;
float maxHealth = 100.0f;

float swingTimer = 0.0f;
bool isSwinging = false;
float swingSpeed = 5.0f;

float goblin1Health = 100.0f;
float goblin2Health = 100.0f;
float goblin3Health = 100.0f;

float goblinSpeed = 15.0f;
float attackRange = 12.0f;
float detectionRange = 100.0f;
float g1SwingTimer = 0.0f;
bool g1IsSwinging = false;
float g2SwingTimer = 0.0f;
bool g2IsSwinging = false;
float g3SwingTimer = 0.0f;
bool g3IsSwinging = false;

bool swordHitRegistered = false;
bool isDead = false;

bool showStory = true;
bool showHelloApple = false;
bool questKing1 = false;
bool questArmor = false;
bool equippedArmor = false;
bool questArmorStarted = false;
bool questKing2 = false;
bool questWitch = false;
bool questWitchStarted = false;
bool talkToWitch = false;
bool swordTaken = false;
bool questKing3 = false;
bool questFight = false;
bool inFight = false;


Window window("Marian - The time traveler", 1920, 1080);
Camera camera;

//vec3 playerPos = vec3(-20.0f, 15.0f, 250.0f);
vec3 playerPos = vec3(-145.0f, 15.0f, -500.0f);
vec3 kingBobPos = vec3(-30.0f, 17.25f, -235.0f);
vec3 armorPos = vec3(162.0f, 12.0f, 0.5f);
vec3 witchPos = vec3(-210.0f, 10.0f, 185.0f);
float playerRoataion = 0.0f;


struct Apple {
	vec3 position;
	bool isEaten;
};

std::vector<Apple> mapApples = {
	{ vec3(20.0f, 15.0f, 180.0f), false },
	{ vec3(80.0f, 15.0f, 260.0f), false },
	{ vec3(100.0f, 15.0f, 145.0f), false},
	{ vec3(-87.0f, 15.0f, 150.0f), false},
	{ vec3(-100.0f, 15.0f, 260.0f), false},
	{ vec3(-150.0f, 15.0f, 140.0f), false}
};

vec3 goblin1Pos = vec3(225.0f, 15.0f, -360.0f);
vec3 goblin2Pos = vec3(265.0f, 15.0f, -410.0f);
vec3 goblin3Pos = vec3(320.0f, 15.0f, -390.0f);

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(180.0f, 200.0f, 350.0f);

int main()
{
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader mountainShader("Shaders/mountain_vertex_shader.glsl", "Shaders/mountain_fragment_shader.glsl");
	Shader terrainShader("Shaders/terrain_vertex_shader.glsl", "Shaders/terrain_fragment_shader.glsl");

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
	GLuint swordN = loadBMP("Resources/Textures/swordN.bmp");
	GLuint swordC = loadBMP("Resources/Textures/swordC.bmp");
	GLuint churchN = loadBMP("Resources/Textures/churchN.bmp");
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
	GLuint cube1 = loadBMP("Resources/Textures/cube1.bmp");
	GLuint portalN = loadBMP("Resources/Textures/portalN.bmp");
	GLuint portalC = loadBMP("Resources/Textures/portalC.bmp");
	GLuint gaina_texture = loadBMP("Resources/Textures/gaina_text.bmp");
	GLuint gard_texture = loadBMP("Resources/Textures/gard_text.bmp");
	GLuint helmetN = loadBMP("Resources/Textures/Helmet_NORM.bmp");
	GLuint helmetC = loadBMP("Resources/Textures/Helmet_DIFF.bmp");
	GLuint boxC = loadBMP("Resources/Textures/wooden_box_color.bmp");
	GLuint boxN = loadBMP("Resources/Textures/wooden_box_normal.bmp");
	GLuint hambarC = loadBMP("Resources/Textures/hambarC.bmp");
	GLuint hambarN = loadBMP("Resources/Textures/hambarN.bmp");
	GLuint horse_texture = loadBMP("Resources/Textures/hourse.bmp");
	GLuint cusca_gaini_text = loadBMP("Resources/Textures/cusca_gaini_texture.bmp");
	GLuint crownC = loadBMP("Resources/Textures/crownColor.bmp");
	GLuint crownN = loadBMP("Resources/Textures/crownNormal.bmp");
	GLuint spearC = loadBMP("Resources/Textures/spear.bmp");
	GLuint spearN = loadBMP("Resources/Textures/spearN.bmp");
	GLuint princessC = loadBMP("Resources/Textures/front.bmp");


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
	Mesh torso = loader.loadObj("Resources/Models/torso.obj", emptyTextures);
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
	Mesh tree3 = loader.loadObj("Resources/Models/tree3.obj", emptyTextures);
	Mesh vraji = loader.loadObj("Resources/Models/vraji.obj", emptyTextures);
	Mesh apple = loader.loadObj("Resources/Models/apple.obj", emptyTextures);
	Mesh cap = loader.loadObj("Resources/Models/marianCap.obj", emptyTextures);
	Mesh king_bobModel = loader.loadObj("Resources/Models/king_bob.obj", emptyTextures);
	Mesh cow = loader.loadObj("Resources/Models/cow.obj", emptyTextures);
	Mesh fan = loader.loadObj("Resources/Models/fan.obj", emptyTextures);
	Mesh stoneRoad = loader.loadObj("Resources/Models/stoneRoad.obj", emptyTextures);
	Mesh cube1Model = loader.loadObj("Resources/Models/cube1.obj", emptyTextures);
	Mesh portalModel = loader.loadObj("Resources/Models/portal.obj", emptyTextures);
	Mesh helmet = loader.loadObj("Resources/Models/helmet.obj", emptyTextures);
	Mesh gaina = loader.loadObj("Resources/Models/gaina.obj", emptyTextures);
	Mesh gard = loader.loadObj("Resources/Models/gard.obj", emptyTextures);
	Mesh nest = loader.loadObj("Resources/Models/nest.obj", emptyTextures);
	Mesh wooden_box = loader.loadObj("Resources/Models/wooden_box.obj", emptyTextures);
	Mesh hambar = loader.loadObj("Resources/Models/hambar.obj", emptyTextures);
	Mesh horse = loader.loadObj("Resources/Models/horse.obj", emptyTextures);
	Mesh cusca_gaini = loader.loadObj("Resources/Models/cusca_gaini.obj", emptyTextures);
	Mesh crownModel = loader.loadObj("Resources/Models/crown.obj", emptyTextures);
	Mesh spear = loader.loadObj("Resources/Models/spear.obj", emptyTextures);
	Mesh princess = loader.loadObj("Resources/Models/princess.obj", emptyTextures);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (!isDead)
			processKeyboardInput();

		float floorLevel = 10.0f;
		float characterHeight = 6.0f;

		if (playerPos.y > floorLevel + characterHeight) {
			playerPos.y -= 20.0f * deltaTime;
		}
		if (playerPos.y < floorLevel + characterHeight) {
			playerPos.y = floorLevel + characterHeight;
		}

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT) && swordTaken) {
			isSwinging = true;
		}

		if (isSwinging) {
			swingTimer += deltaTime * swingSpeed;
			if (swingTimer > 1.5f && !swordHitRegistered) {
				vec3* gobPos[3] = { &goblin1Pos, &goblin2Pos, &goblin3Pos };
				float* gobHealth[3] = { &goblin1Health, &goblin2Health, &goblin3Health };
				for (int i = 0; i < 3; i++) {
					if (*gobHealth[i] > 0.0f) {
						float d = distance(playerPos, *gobPos[i]);

						if (d < 15.0f) {
							*gobHealth[i] -= 25.0f;
							std::cout << "Goblin " << i + 1 << "was hit! Health: " << *gobHealth[i] << std::endl;
							swordHitRegistered = true;
						}
					}
				}
			}
			if (swingTimer > 3.14f) {
				swingTimer = 0.0f;
				isSwinging = false;
				swordHitRegistered = false;
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

		vec3 currentCameraPos = vec3(playerPos.x - offsetX, targetCameraY, playerPos.z - offsetZ);

		glm::mat4 ViewMatrix = glm::lookAt(currentCameraPos, playerPos, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);

		sunShader.use();

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

		//draw body parts
		{
			//firstly the torso
			if (!equippedArmor) {

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, handDiffuse);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, handNormal);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, armorC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, armorN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
			}

			mat4 torsoModel = mat4(1.0f);
			torsoModel = translate(torsoModel, playerPos);
			if (isDead) {
				torsoModel = rotate(torsoModel, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
				torsoModel = translate(torsoModel, vec3(0.0f, -1.5f, 0.0f));
			}
			else
				torsoModel = rotate(torsoModel, radians(playerRoataion + 180.0f), vec3(0.0f, 1.0f, 0.0f));
			torsoModel = scale(torsoModel, vec3(2.0f, 2.0f, 2.0f));
			mat4 torsoMVP = ProjectionMatrix * ViewMatrix * torsoModel;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &torsoMVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &torsoModel[0][0]);
			torso.draw(shader);

			//capul
			if (!equippedArmor) {
				mat4 capModel = torsoModel;
				capModel = translate(capModel, vec3(0.0f, 0.9f, -0.1f));
				capModel = rotate(capModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				capModel = scale(capModel, vec3(3.4f, 3.4f, 3.4f));
				mat4 capMVP = ProjectionMatrix * ViewMatrix * capModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &capMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &capModel[0][0]);
				cap.draw(shader);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, helmetC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, helmetN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				mat4 helmetModel = torsoModel;
				helmetModel = translate(helmetModel, vec3(0.0f, 1.0f, 0.0f));
				helmetModel = rotate(helmetModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				helmetModel = scale(helmetModel, vec3(0.8f, 0.8f, 0.8f));
				mat4 helmetMVP = ProjectionMatrix * ViewMatrix * helmetModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &helmetMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &helmetModel[0][0]);
				helmet.draw(shader);
			}

			if (!equippedArmor) {

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, handDiffuse);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, handNormal);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, armorC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, armorN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
			}

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

			//sword
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, swordC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, swordN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				if (!swordTaken) {
					float bobbingOffset = sin(currentFrame * 2.0f) * 0.5f;
					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-205.0f, 10.0f + bobbingOffset, 200.0f));
					float rotationAngle = currentFrame * 150.0f;
					ModelMatrix = rotate(ModelMatrix, radians(rotationAngle), vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					swordModel.draw(shader);
				}
				else {
					mat4 marianSwordModel = rHandModel;
					marianSwordModel = translate(marianSwordModel, vec3(1.0f, 0.0f, 2.5f));
					marianSwordModel = rotate(marianSwordModel, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
					marianSwordModel = scale(marianSwordModel, vec3(15.0f, 15.0f, 15.0f));
					mat4 swordMVP = ProjectionMatrix * ViewMatrix * marianSwordModel;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &swordMVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &marianSwordModel[0][0]);
					swordModel.draw(shader);
				}
			}
			if (!equippedArmor) {

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, handDiffuse);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, handNormal);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, armorC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, armorN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
			}

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
			if (!showStory)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, red);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				mat4 healthBgModel = torsoModel;

				healthBgModel = translate(healthBgModel, vec3(0.0f, 2.5f, 0.0f));
				healthBgModel = scale(healthBgModel, vec3(0.3f, 0.05f, 0.1f));

				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBgModel)[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBgModel[0][0]);
				glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.2f, 0.2f, 0.2f);
				box.draw(shader);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, green);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			}
			float healthWidth = (playerHealth / maxHealth) * 0.3f;
			mat4 healthBarModel = torsoModel;

			healthBarModel = translate(healthBarModel, vec3(-0.8f + healthWidth * 2.65f, 2.5f, -0.05f));
			healthBarModel = scale(healthBarModel, vec3(healthWidth + 0.01f, 0.055f, 0.12f));

			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBarModel)[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
			box.draw(shader);
		}
		if (playerHealth <= 0.0f && !isDead) {
			playerHealth = 0.0f; // Clamp to 0
			isDead = true;
		}

		//gobin1
		{}
		{
			if (goblin1Health) {
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
				if (g1IsSwinging) {
					float swingAngle = sin(g1SwingTimer) * 80.0f;
					rHandModel = rotate(rHandModel, radians(swingAngle), vec3(1.0f, 0.0f, 0.0f));
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

				mat4 goblinSwordModel = rHandModel;
				goblinSwordModel = translate(goblinSwordModel, vec3(0.1f, 0.0f, 2.1f));
				goblinSwordModel = rotate(goblinSwordModel, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
				goblinSwordModel = scale(goblinSwordModel, vec3(15.0f, 15.0f, 15.0f));
				mat4 swordMVP = ProjectionMatrix * ViewMatrix * goblinSwordModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &swordMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &goblinSwordModel[0][0]);
				swordModel.draw(shader);

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

					healthBgModel = translate(healthBgModel, vec3(0.0f, 2.5f, 0.0f));
					healthBgModel = scale(healthBgModel, vec3(0.3f, 0.05f, 0.1f));

					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBgModel)[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBgModel[0][0]);
					glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.2f, 0.2f, 0.2f);
					box.draw(shader);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, green);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				}
				float healthWidth = (goblin1Health / maxHealth) * 0.3f;
				mat4 healthBarModel = torsoModel;

				healthBarModel = translate(healthBarModel, vec3(-0.8f + healthWidth * 2.65f, 2.5f, -0.05f));
				healthBarModel = scale(healthBarModel, vec3(healthWidth + 0.01f, 0.055f, 0.12f));

				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBarModel)[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
				glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
				box.draw(shader);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, helmetC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, helmetN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				mat4 helmetModel = torsoModel;
				helmetModel = translate(helmetModel, vec3(0.0f, 1.0f, 0.0f));
				helmetModel = rotate(helmetModel, 180.0f, vec3(0.0f, 0.9f, -0.1f));
				helmetModel = scale(helmetModel, vec3(0.8f, 0.8f, 0.8f));
				mat4 helmetMVP = ProjectionMatrix * ViewMatrix * helmetModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &helmetMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &helmetModel[0][0]);
				helmet.draw(shader);
			}

		}
		//goblin2
		{
			if (goblin2Health > 0.0f) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, handDiffuse);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, handNormal);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				//firstly the torso
				mat4 torsoModel = mat4(1.0f);
				torsoModel = translate(torsoModel, goblin2Pos);
				torsoModel = rotate(torsoModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				torsoModel = scale(torsoModel, vec3(2.0f, 2.0f, 2.0f));
				mat4 torsoMVP = ProjectionMatrix * ViewMatrix * torsoModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &torsoMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &torsoModel[0][0]);
				torso.draw(shader);



				//then the right hand
				mat4 rHandModel = torsoModel;
				if (g2IsSwinging) {
					float swingAngle = sin(g2SwingTimer) * 80.0f;
					rHandModel = rotate(rHandModel, radians(swingAngle), vec3(1.0f, 0.0f, 0.0f));
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

				mat4 goblinSwordModel = rHandModel;
				goblinSwordModel = translate(goblinSwordModel, vec3(0.1f, 0.0f, 2.1f));
				goblinSwordModel = rotate(goblinSwordModel, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
				goblinSwordModel = scale(goblinSwordModel, vec3(15.0f, 15.0f, 15.0f));
				mat4 swordMVP = ProjectionMatrix * ViewMatrix * goblinSwordModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &swordMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &goblinSwordModel[0][0]);
				swordModel.draw(shader);

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

					healthBgModel = translate(healthBgModel, vec3(0.0f, 2.5f, 0.0f));
					healthBgModel = scale(healthBgModel, vec3(0.3f, 0.05f, 0.1f));

					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBgModel)[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBgModel[0][0]);
					glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.2f, 0.2f, 0.2f);
					box.draw(shader);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, green);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				}
				float healthWidth = (goblin2Health / maxHealth) * 0.3f;
				mat4 healthBarModel = torsoModel;

				healthBarModel = translate(healthBarModel, vec3(-0.8f + healthWidth * 2.65f, 2.5f, -0.05f));
				healthBarModel = scale(healthBarModel, vec3(healthWidth + 0.01f, 0.055f, 0.12f));

				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBarModel)[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
				glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
				box.draw(shader);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, helmetC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, helmetN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				mat4 helmetModel = torsoModel;
				helmetModel = translate(helmetModel, vec3(0.0f, 0.9f, -0.1f));
				helmetModel = rotate(helmetModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				helmetModel = scale(helmetModel, vec3(0.8f, 0.8f, 0.8f));
				mat4 helmetMVP = ProjectionMatrix * ViewMatrix * helmetModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &helmetMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &helmetModel[0][0]);
				helmet.draw(shader);
			}
		}
		//goblin3
		{
			if (goblin3Health > 0.0f) {
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
				if (g3IsSwinging) {
					float swingAngle = sin(g3SwingTimer) * 80.0f;
					rHandModel = rotate(rHandModel, radians(swingAngle), vec3(1.0f, 0.0f, 0.0f));
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

				mat4 goblinSwordModel = rHandModel;
				goblinSwordModel = translate(goblinSwordModel, vec3(0.1f, 0.0f, 2.1f));
				goblinSwordModel = rotate(goblinSwordModel, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
				goblinSwordModel = scale(goblinSwordModel, vec3(15.0f, 15.0f, 15.0f));
				mat4 swordMVP = ProjectionMatrix * ViewMatrix * goblinSwordModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &swordMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &goblinSwordModel[0][0]);
				swordModel.draw(shader);



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

					healthBgModel = translate(healthBgModel, vec3(0.0f, 2.5f, 0.0f));
					healthBgModel = scale(healthBgModel, vec3(0.3f, 0.05f, 0.1f));

					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBgModel)[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBgModel[0][0]);
					glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.2f, 0.2f, 0.2f);
					box.draw(shader);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, green);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				}
				float healthWidth = (goblin3Health / maxHealth) * 0.3f;
				mat4 healthBarModel = torsoModel;

				healthBarModel = translate(healthBarModel, vec3(-0.8f + healthWidth * 2.65f, 2.5f, -0.05f));
				healthBarModel = scale(healthBarModel, vec3(healthWidth + 0.01f, 0.055f, 0.12f));

				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &(ProjectionMatrix * ViewMatrix * healthBarModel)[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &healthBarModel[0][0]);
				glUniform3f(glGetUniformLocation(shader.getId(), "objectColor"), 0.0f, 1.0f, 0.0f);
				box.draw(shader);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, helmetC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, helmetN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				mat4 helmetModel = torsoModel;
				helmetModel = translate(helmetModel, vec3(0.0f, 0.9f, -0.1f));
				helmetModel = rotate(helmetModel, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				helmetModel = scale(helmetModel, vec3(0.8f, 0.8f, 0.8f));
				mat4 helmetMVP = ProjectionMatrix * ViewMatrix * helmetModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &helmetMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &helmetModel[0][0]);
				helmet.draw(shader);

				/*glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, spearC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, spearN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				mat4 spearModel = rHandModel;
				spearModel = translate(spearModel, vec3(0.0f, 0.0f, 1.5f));
				spearModel = rotate(spearModel, -70.0f, vec3(1.0f, 0.0f, 0.0f));
				spearModel = scale(spearModel, vec3(15.0f, 40.0f, 15.0f));
				mat4 spearMVP = ProjectionMatrix * ViewMatrix * spearModel;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &spearMVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &spearModel[0][0]);
				spear.draw(shader);*/
			}
		}
		if (inFight && !isDead) {
			vec3* gPositions[3] = { &goblin1Pos, &goblin2Pos, &goblin3Pos };
			float* gHealths[3] = { &goblin1Health, &goblin2Health, &goblin3Health };
			float* gTimers[3] = { &g1SwingTimer, &g2SwingTimer, &g3SwingTimer };
			bool* gSwinging[3] = { &g1IsSwinging, &g2IsSwinging, &g3IsSwinging };
			for (int i = 0; i < 3; i++) {
				if (*gHealths[i] <= 0) continue;

				float dist = distance(playerPos, *gPositions[i]);

				if (dist < detectionRange && dist > attackRange) {
					vec3 dir = normalize(playerPos - *gPositions[i]);
					gPositions[i]->x += dir.x * goblinSpeed * deltaTime;
					gPositions[i]->z += dir.z * goblinSpeed * deltaTime;
				}

				if (dist <= attackRange && !(*gSwinging[i])) {
					*gSwinging[i] = true;
					*gTimers[i] = 0.0f;
				}

				if (*gSwinging[i]) {
					*gTimers[i] += deltaTime * 6.0f;
					if (*gTimers[i] > 3.14f) {
						*gSwinging[i] = false;
						if (dist <= attackRange + 2.0f) playerHealth -= 5.0f;
					}
				}
			}
		}



		//princess
		{}
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, princessC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			mat4 princessModel = mat4(1.0f);
			princessModel = translate(princessModel, vec3(310.0f, 10.0f, -430.0f));
			//princessModel = rotate(princessModel, -70.0f, vec3(1.0f, 0.0f, 0.0f));
			princessModel = scale(princessModel, vec3(2.0f, 2.0f, 2.0f));
			mat4 princessMVP = ProjectionMatrix * ViewMatrix * princessModel;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &princessMVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &princessModel[0][0]);
			princess.draw(shader);
		}
		//apples
		{}
		{
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
					appleModel = scale(appleModel, vec3(0.05f, 0.05f, 0.05f));

					mat4 appleMVP = ProjectionMatrix * ViewMatrix * appleModel;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &appleMVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &appleModel[0][0]);
					apple.draw(shader);
				}
			}
		}
		//Drawing the plane
		{
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

			shader.use();

			glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), currentCameraPos.x, currentCameraPos.y, currentCameraPos.z);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, munteColor);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, munteNormal);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);
		}
		//Munti
		{
			//Primul Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(200.0f, -4.0f, -820.0f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(-560.0f, -1.0f, -500.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain1.draw(shader);
			}
			//Al patrulea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-600.0f, -20.0f, -100.0f));
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(-600.0f, -20.0f, -100.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain2.draw(shader);
			}
			//Al cincilea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(700.0f, -4.0f, -600.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(700.0f, -4.0f, -600.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain2.draw(shader);
			}
			//Al saselea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(550.0f, -25.0f, -100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(550.0f, -25.0f, -100.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain1.draw(shader);
			}
			//Al saptelea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(750.0f, -25.0f, 300.0f));
				ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(750.0f, -25.0f, 300.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain2.draw(shader);
			}
			//Al optulea Munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(300.0f, -25.0f, 620.0f));
				ModelMatrix = rotate(ModelMatrix, -60.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(300.0f, -25.0f, 620.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain1.draw(shader);
			}
			//Al noulea muntedw
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-300.0f, -25.0f, 620.0f));
				ModelMatrix = rotate(ModelMatrix, 210.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(-300.0f, -25.0f, 620.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain2.draw(shader);
			}
			//Al zecelea munte
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-750.0f, -25.0f, 300.0f));
				ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				//registerCollide(portalModel, vec3(-750.0f, -25.0f, 300.0f), vec3(10.0f, 10.0f, 10.0f));
				mountain1.draw(shader);
			}
			//Coliziuni munti
			registerCollide(cube1Model, vec3(-20.0f, 12.0f, 330.0f), vec3(1000.0f, 10.0f, 5.0f));
			registerCollide(cube1Model, vec3(330.0f, 12.0f, 280.0f), vec3(5.0f, 10.0f, 1000.0f));
			registerCollide(cube1Model, vec3(-270.0f, 12.0f, 280.0f), vec3(5.0f, 10.0f, 1000.0f));
		}
		//Hub 
		{}
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cube1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-145.0f, 46.0f, -500.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(35.0f, 35.0f, 35.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			cube1Model.draw(shader);
		}
		//hub walls
		{
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cube1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-145.0f, 46.0f, -570.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(35.0f, 35.0f, 35.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(portalModel, vec3(-145.0f, 46.0f, -570.0f), vec3(10.0f, 1.0f, 1.3f));
				cube1Model.draw(shader);
			}
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cube1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 46.0f, -500.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(35.0f, 35.0f, 35.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(portalModel, vec3(-75.0f, 46.0f, -500.0f), vec3(1.0f, 1.0f, 3.0f));
				cube1Model.draw(shader);
			}
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cube1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-145.0f, 36.0f, -460.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(35.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(portalModel, vec3(-145.0f, 36.0f, -460.0f), vec3(10.0f, 1.0f, 1.0f));
				cube1Model.draw(shader);
			}
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cube1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-215.0f, 46.0f, -500.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(35.0f, 35.0f, 35.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(portalModel, vec3(-215.0f, 46.0f, -500.0f), vec3(1.0f, 1.0f, 3.0f));
				cube1Model.draw(shader);
			}
		}
		//portal
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, portalC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, portalN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-145.0f, 17.0f, -520.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			portalModel.draw(shader);
			registerCollide(portalModel, vec3(-145.0f, 17.0f, -520.0f), vec3(0.2f, 0.2f, 0.2f));
		}
		//Castel
		{
			mountainShader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, stoneTex);
			glUniform1i(glGetUniformLocation(mountainShader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-45.0f, 9.0f, -300.0f));
			registerCollide(castle, vec3(-45.0f, 9.0f, -300.0f), vec3(0.04f, 0.04f, 0.04f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.04f, 0.04f, 0.04f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			castle.draw(mountainShader);
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
			registerCollide(portalModel, vec3(-30.0f, 17.25f, -235.0f), vec3(0.1f, 0.5f, 0.3f));
			king_bobModel.draw(shader);

			//crown
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, crownC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, crownN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-30.0f, 21.5f, -235.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				crownModel.draw(shader);
			}
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(statue1Model, vec3(-70.0f, 10.0f, 50.0f), vec3(1.5f, 1.5f, 1.5f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(statue2Model, vec3(80.0f, 10.0f, 50.0f), vec3(0.2f, 0.2f, 0.2f));
				statue2Model.draw(shader);
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
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.07f, 0.07f, 0.07f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(portalModel, vec3(-165.0f, 10.0f, -30.0f), vec3(0.2f, 1.0f, 0.3f));
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
			ModelMatrix = rotate(ModelMatrix, -70.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(15.0f, 15.0f, 15.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(portalModel, vec3(170.0f, 24.0f, -5.0f), vec3(0.2f, 1.0f, 1.0f));
			blacksmithModel.draw(shader);
		}
		//armor
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, armorN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 0);

			float bobbingOffset = sin(currentFrame * 2.0f) * 0.5f;

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(162.0f, 12.0f + bobbingOffset, 0.5f));
			ModelMatrix = glm::rotate(ModelMatrix, radians(currentFrame * 150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.9f, 0.9f, 0.9f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			if (!equippedArmor)
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
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(portalModel, vec3(-113.0f, 13.5f, -27.0f), vec3(0.2f, 0.1f, 0.2f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.5f, 4.5f, 4.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(portalModel, vec3(-180.0f, 10.0f, 0.0f), vec3(0.5f, 0.5f, 0.8f));
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
				ModelMatrix = rotate(ModelMatrix, -135.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.5f, 4.5f, 4.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(150.0f, 10.0f, -130.0f), vec3(4.4f, 4.4f, 4.4f));
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
				ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.5f, 4.5f, 4.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(-175.0f, 10.0f, -80.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
			//House4
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(170.0f, 10.0f, -70.0f));
				ModelMatrix = rotate(ModelMatrix, -180.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.5f, 4.5f, 4.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(170.0f, 10.0f, -70.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
			//House5
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(250.0f, 10.0f, 28.0f));
				ModelMatrix = rotate(ModelMatrix, -180.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.5f, 4.5f, 4.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(250.0f, 10.0f, 28.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
			//House6
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(260.0f, 10.0f, -20.0f));
				ModelMatrix = rotate(ModelMatrix, -180.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.5f, 4.5f, 4.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(260.0f, 10.0f, -20.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
			//House7
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -85.0f));
				ModelMatrix = rotate(ModelMatrix, -135.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.4f, 4.4f, 4.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(245.0f, 10.0f, -85.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
			//House8
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-250.0f, 10.0f, 25.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.4f, 4.4f, 4.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(-250.0f, 10.0f, 25.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
			//House9
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, house);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-235.0f, 10.0f, -30.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(4.4f, 4.4f, 4.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(houseModel, vec3(-235.0f, 10.0f, -30.0f), vec3(4.4f, 4.4f, 4.4f));
				houseModel.draw(shader);
			}
		}
		//Well
		{}
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, well);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 10.0f, -50.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wellModel, vec3(-150.0f, 10.0f, -50.0f), vec3(0.05f, 0.05f, 0.05f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(barrelsModel, vec3(155.0f, 10.0f, -25.0f), vec3(0.1f, 0.1f, 0.1f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(barrelsModel, vec3(155.0f, 10.0f, -28.0f), vec3(0.1f, 0.1f, 0.1f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(barrelsModel, vec3(155.0f, 13.5f, -26.5f), vec3(0.1f, 0.1f, 0.1f));
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
				ModelMatrix = rotate(ModelMatrix, -135.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				lamppostModel.draw(shader);
			}
		}
		//Cart1
		{}
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
			ModelMatrix = rotate(ModelMatrix, 15.0f, vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(cart1Model, vec3(80.0f, 10.0f, -55.0f), vec3(5.0f, 5.0f, 5.0f));
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
			ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(cart2Model, vec3(170.0f, 10.0f, -35.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(marketModel, vec3(50.0f, 12.0f, -55.0f), vec3(2.0f, 2.0f, 2.0f));
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
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(marketModel, vec3(95.0f, 12.0f, -30.0f), vec3(3.0f, 2.0f, 2.0f));
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
				ModelMatrix = rotate(ModelMatrix, 190.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(marketModel, vec3(65.0f, 12.0f, -5.0f), vec3(2.0f, 2.0f, 2.0f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(campfireModel, vec3(60.0f, 10.0f, -30.0f), vec3(4.0f, 4.0f, 4.0f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(woodpileModel, vec3(90.0f, 10.0f, 0.0f), vec3(7.0f, 7.0f, 7.0f));
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
				ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(marketstallModel, vec3(35.0f, 10.0f, -20.0f), vec3(5.0f, 5.0f, 5.0f));
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
					registerCollide(king_bobModel, vec3(35.0f, 12.5f, -20.0f), vec3(0.75f, 0.75f, 0.75f));
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
					registerCollide(king_bobModel, vec3(55.0f, 12.5f, -45.0f), vec3(0.75f, 0.75f, 0.75f));
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
					registerCollide(king_bobModel, vec3(85.0f, 12.5f, -25.0f), vec3(0.75f, 0.75f, 0.75f));
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
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(king_bobModel, vec3(-20.0f, 12.5f, -235.0f), vec3(0.75f, 0.75f, 0.75f));
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
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.75f, 0.75f, 0.75f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(king_bobModel, vec3(-15.0f, 12.5f, -235.0f), vec3(0.75f, 0.75f, 0.75f));
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
					ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					king_bobModel.draw(shader);
				}

			}
			//gaini
			{}
			//gard gaini
			{
				{
					//gaina1
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-50.0f, 9.0f, -82.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina2
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-53.0f, 9.0f, -91.0f));
						ModelMatrix = rotate(ModelMatrix, 40.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina3
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-47.0f, 9.0f, -99.0f));
						ModelMatrix = rotate(ModelMatrix, 62.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina4
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-69.0f, 9.0f, -82.0f));
						ModelMatrix = rotate(ModelMatrix, 120.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina5
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-83.0f, 9.0f, -91.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina6
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-80.0f, 9.0f, -81.0f));
						ModelMatrix = rotate(ModelMatrix, -90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina7
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-69.0f, 9.0f, -97.0f));
						ModelMatrix = rotate(ModelMatrix, -110.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina8
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-47.0f, 9.0f, -72.0f));
						ModelMatrix = rotate(ModelMatrix, -40.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina9
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-53.0f, 9.0f, -70.0f));
						ModelMatrix = rotate(ModelMatrix, 110.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina10
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-42.0f, 9.0f, -91.0f));
						ModelMatrix = rotate(ModelMatrix, 80.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina11
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-42.0f, 9.0f, -86.0f));
						ModelMatrix = rotate(ModelMatrix, 100.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina12
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-65.0f, 9.0f, -60.0f));
						ModelMatrix = rotate(ModelMatrix, -40.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina13
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-73.0f, 9.0f, -72.0f));
						ModelMatrix = rotate(ModelMatrix, -100.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina14
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 9.0f, -62.0f));
						ModelMatrix = rotate(ModelMatrix, -100.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina15
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-83.0f, 9.0f, -65.0f));
						ModelMatrix = rotate(ModelMatrix, 100.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina16
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-80.0f, 9.0f, -47.0f));
						ModelMatrix = rotate(ModelMatrix, 40.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina17
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-72.0f, 9.0f, -45.0f));
						ModelMatrix = rotate(ModelMatrix, -100.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina18
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-55.0f, 9.0f, -43.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina19
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-50.0f, 9.0f, -50.0f));
						ModelMatrix = rotate(ModelMatrix, -80.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
					//gaina20
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gaina_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-46.0f, 9.0f, -55.0f));
						ModelMatrix = rotate(ModelMatrix, 125.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						gaina.draw(shader);
					}
				}
				//gard gaini nord
				{}
				{
					{
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-83.0f, 9.0f, -104.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-83.0f, 9.0f, -104.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-73.0f, 9.0f, -104.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-73.0f, 9.0f, -104.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-63.0f, 9.0f, -104.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-63.0f, 9.0f, -104.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-53.0f, 9.0f, -104.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-53.0f, 9.0f, -104.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-43.0f, 9.0f, -104.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-43.0f, 9.0f, -104.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
				}
				//gard gaini vest
				{
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-90.0f, 9.0f, -98.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-90.0f, 9.0f, -98.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-90.0f, 9.0f, -88.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-90.0f, 9.0f, -88.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-90.0f, 9.0f, -78.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-90.0f, 9.0f, -78.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-90.0f, 9.0f, -68.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-90.0f, 9.0f, -68.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-90.0f, 9.0f, -58.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-90.0f, 9.0f, -58.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-90.0f, 9.0f, -48.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-90.0f, 9.0f, -48.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
				}
				//gard gaini est
				{}
				{
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-36.0f, 9.0f, -98.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-36.0f, 9.0f, -98.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-36.0f, 9.0f, -88.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-36.0f, 9.0f, -88.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-36.0f, 9.0f, -78.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-36.0f, 9.0f, -78.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-36.0f, 9.0f, -68.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-36.0f, 9.0f, -68.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-36.0f, 9.0f, -58.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-36.0f, 9.0f, -58.0f), vec3(0.1f, 0.1f, 0.1f));

						gard.draw(shader);
					}
					{
						shader.use();

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gard_texture);
						glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

						ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-36.0f, 9.0f, -48.0f));
						ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
						ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
						MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						registerCollide(gard, vec3(-36.0f, 9.0f, -48.0f), vec3(0.1f, 0.1f, 0.1f));
						registerCollide(cube1Model, vec3(-63.0f, 9.0f, -73.0f), vec3(30.0f, 30.0f, 30.0f));
						gard.draw(shader);
					}
				}
				//gard gaini sud
				{}
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gard_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-83.0f, 9.0f, -41.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(gard, vec3(-83.0f, 9.0f, -41.0f), vec3(0.3f, 0.3f, 0.3f));

					gard.draw(shader);

					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gard_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-73.0f, 9.0f, -41.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(gard, vec3(-73.0f, 9.0f, -41.0f), vec3(0.3f, 0.3f, 0.3f));

					gard.draw(shader);

					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gard_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-63.0f, 9.0f, -41.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(gard, vec3(-63.0f, 9.0f, -41.0f), vec3(0.3f, 0.3f, 0.3f));

					gard.draw(shader);

					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gard_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-53.0f, 9.0f, -41.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(gard, vec3(-53.0f, 9.0f, -41.0f), vec3(0.3f, 0.3f, 0.3f));

					gard.draw(shader);

					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gard_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-43.0f, 9.0f, -41.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(gard, vec3(-43.0f, 9.0f, -41.0f), vec3(0.3f, 0.3f, 0.3f));

					gard.draw(shader);


					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gard_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-43.0f, 9.0f, -41.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
					registerCollide(gard, vec3(-43.0f, 9.0f, -41.0f), vec3(0.3f, 0.3f, 0.3f));

					gard.draw(shader);
				}
			}
			//nest
			{}
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-42.0f, 11.0f, -45.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(nest, vec3(-42.0f, 11.0f, -45.0f), vec3(0.4f, 0.4f, 0.4f));
				nest.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-55.0f, 11.0f, -48.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(nest, vec3(-55.0f, 11.0f, -48.0f), vec3(0.4f, 0.4f, 0.4f));
				nest.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-70.0f, 11.0f, -52.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(nest, vec3(-70.0f, 11.0f, -52.0f), vec3(0.4f, 0.4f, 0.4f));
				nest.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-58.0f, 11.0f, -81.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(nest, vec3(-58.0f, 11.0f, -81.0f), vec3(0.4f, 0.4f, 0.4f));
				nest.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-55.0f, 11.0f, -98.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(nest, vec3(-55.0f, 11.0f, -98.0f), vec3(0.4f, 0.4f, 0.4f));
				nest.draw(shader);
			}
			//cusca gaini 1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cusca_gaini_text);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(20.0f, 6.5f, -30.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.5f, 5.5f, 5.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(cusca_gaini, vec3(20.0f, 6.5f, -30.0f), vec3(7.0f, 7.0f, 7.0f));
				cusca_gaini.draw(shader);
				//gaini din cusca
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gaina_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(19.0f, 9.5f, -29.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					gaina.draw(shader);

					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gaina_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(22.0f, 9.5f, -32.0f));
					ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					gaina.draw(shader);
				}
			}
			//cusca gaini 2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cusca_gaini_text);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f, 6.5f, -30.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.5f, 5.5f, 5.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(cusca_gaini, vec3(5.0f, 6.5f, -30.0f), vec3(7.0f, 7.0f, 7.0f));
				cusca_gaini.draw(shader);

				//gaini din cusca
				{
					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gaina_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(7.0f, 9.5f, -29.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					gaina.draw(shader);

					shader.use();

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gaina_texture);
					glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

					ModelMatrix = glm::mat4(1.0);
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(3.0f, 9.5f, -32.0f));
					ModelMatrix = rotate(ModelMatrix, -45.0f, vec3(0.0f, 1.0f, 0.0f));
					ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
					MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
					glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

					gaina.draw(shader);
				}
			}
		}
		//Church
		{}
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
			ModelMatrix = rotate(ModelMatrix, -50.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(churchModel, vec3(-200.0f, 10.0f, -240.0f), vec3(5.0f, 5.0f, 5.0f));
			churchModel.draw(shader);
		}
		//hambar
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, hambarC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, hambarN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(90.0f, 10.0f, -107.0f));
			ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(hambar, vec3(20.0f, 10.0f, -107.0f), vec3(5.0f, 4.0f, 2.0f));
			hambar.draw(shader);
		}
		//cai hambar
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, horse_texture);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(18.0f, 11.0f, -140.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			horse.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, horse_texture);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(34.0f, 11.0f, -140.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			horse.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, horse_texture);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(50.0f, 11.0f, -140.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			horse.draw(shader);
		}
		//fan hambar
		{
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 10.0f, -142.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 12.0f, -142.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, -142.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 12.0f, -142.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, -147.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 12.0f, -147.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);


			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 10.0f, -147.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, culoareCopac3);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 12.0f, -147.5f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			fan.draw(shader);
		}
		//taverns
		{}
		{
			{}
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.5f, 3.5f, 3.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tavernModel, vec3(-120.0f, 10.0f, 45.0f), vec3(3.5f, 3.5f, 3.5f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tavernModel, vec3(120.0f, 10.0f, 47.0f), vec3(2.5f, 2.5f, 2.5f));
				tavernModel.draw(shader);
			}
			//tavern3
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tavernC);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tavernN);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, 47.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tavernModel, vec3(80.0f, 10.0f, 47.0f), vec3(2.5f, 2.5f, 2.5f));
				tavernModel.draw(shader);
			}
		}
		//cutii 
		{
			//cutii dreapta
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(178.0f, 14.0f, 38.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(178.0f, 14.0f, 38.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(185.0f, 14.0f, 32.0f));
			ModelMatrix = rotate(ModelMatrix, 45.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(185.0f, 14.0f, 32.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(180.0f, 21.0f, 35.0f));
			ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(180.0f, 21.0f, 35.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			//cutii piata

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(18.0f, 14.0f, -58.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(18.0f, 14.0f, -58.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(25.0f, 14.0f, -52.0f));
			ModelMatrix = rotate(ModelMatrix, 45.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(25.0f, 14.0f, -52.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(20.0f, 21.0f, -55.0f));
			ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(20.0f, 21.0f, -55.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 14.0f, -63.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(0.0f, 14.0f, -63.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(8.0f, 14.0f, -57.0f));
			ModelMatrix = rotate(ModelMatrix, 45.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(8.0f, 14.0f, -57.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, boxC);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, boxN);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_normal"), 1);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 21.0f, -60.0f));
			ModelMatrix = rotate(ModelMatrix, 30.0f, vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(wooden_box, vec3(2.0f, 21.0f, -60.0f), vec3(7.0f, 7.0f, 7.0f));
			wooden_box.draw(shader);
		}
		//sign
		{}
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(witchroomModel, vec3(-225.0f, 10.0f, 200.0f), vec3(7.0f, 7.0f, 7.0f));
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
				registerCollide(vraji, vec3(-210.0f, 10.0f, 185.0f), vec3(0.75f, 0.75f, 0.75f));
				vraji.draw(shader);
			}
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-230.0f, 10.0f, 230.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-230.0f, 10.0f, 180.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-230.0f, 10.0f, 160.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-250.0f, 10.0f, 230.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-250.0f, 10.0f, 210.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-250.0f, 10.0f, 190.0f), vec3(5.0f, 5.0f, 5.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tombstoneModel, vec3(-250.0f, 10.0f, 170.0f), vec3(5.0f, 5.0f, 5.0f));
				tombstoneModel.draw(shader);
			}
		}
		//Fences
		{}
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(fenceModel, vec3(-245.0f, 10.0f, 240.0f), vec3(1.0f, 0.5f, 1.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(fenceModel, vec3(-273.0f, 10.0f, 212.5f), vec3(0.2f, 0.5f, 30.0f));
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
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(fenceModel, vec3(-273.0f, 10.0f, 180.0f), vec3(0.2f, 0.5f, 30.0f));
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
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(fenceModel, vec3(-245.0f, 10.0f, 152.5f), vec3(1.0f, 0.5f, 1.0f));
				fenceModel.draw(shader);
			}
		}
		//fan
		{
			//fan1
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-224.0f, 10.0f, -142.5f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fan.draw(shader);
			}
			//fan2
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-207.0f, 10.0f, -169.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fan.draw(shader);
			}
			//fan3
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-183.0f, 10.0f, -144.0f));
				ModelMatrix = rotate(ModelMatrix, -10.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 7.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				fan.draw(shader);
			}
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
				ModelMatrix = rotate(ModelMatrix, -85.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cow.draw(shader);
			}
		}
		//gard vaci
		{}
		{
			//gard vaci nord
			{}
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-223.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-223.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));

				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-213.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-213.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-203.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-203.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-193.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-193.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-183.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-183.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-173.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-173.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-163.0f, 9.0f, -174.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-163.0f, 9.0f, -174.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				registerCollide(gard, vec3(-223.0f, 9.0f, -156.0f), vec3(0.3f, 0.1f, 6.1f));
				registerCollide(gard, vec3(-213.0f, 9.0f, -156.0f), vec3(0.3f, 0.1f, 6.1f));
				registerCollide(gard, vec3(-203.0f, 9.0f, -156.0f), vec3(0.3f, 0.1f, 6.1f));
				registerCollide(cube1Model, vec3(-193.0f, 9.0f, -156.0f), vec3(30.0f, 30.0f, 30.0f));
				registerCollide(gard, vec3(-183.0f, 9.0f, -156.0f), vec3(0.3f, 0.1f, 6.1f));
				registerCollide(gard, vec3(-173.0f, 9.0f, -156.0f), vec3(0.3f, 0.1f, 6.1f));
				registerCollide(gard, vec3(-163.0f, 9.0f, -156.0f), vec3(0.3f, 0.1f, 6.1f));

			}
			//gard vaci est
			{}
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-156.0f, 9.0f, -168.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-156.0f, 9.0f, -158.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-156.0f, 9.0f, -158.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-156.0f, 9.0f, -148.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-156.0f, 9.0f, -148.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-156.0f, 9.0f, -138.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-156.0f, 9.0f, -138.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-156.0f, 9.0f, -128.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-156.0f, 9.0f, -128.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-156.0f, 9.0f, -123.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-156.0f, 9.0f, -123.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);
			}
			//gard vaci vest
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 9.0f, -168.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 9.0f, -158.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-230.0f, 9.0f, -158.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 9.0f, -148.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-230.0f, 9.0f, -148.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 9.0f, -138.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-230.0f, 9.0f, -138.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-230.0f, 9.0f, -124.0f));
				ModelMatrix = rotate(ModelMatrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-230.0f, 9.0f, -124.0f), vec3(0.1f, 0.1f, 0.1f));
				gard.draw(shader);
			}
			// gard vaci sud
			{
				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-223.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-223.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-213.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-213.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-203.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-203.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-193.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-193.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-183.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-183.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-173.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-173.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);

				shader.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gard_texture);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-163.0f, 9.0f, -118.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(gard, vec3(-163.0f, 9.0f, -118.0f), vec3(0.3f, 0.1f, 0.2f));
				gard.draw(shader);
			}
		}
		//Roads
		{}
		{
			//Road1
			{}
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
		//toti copacii
		{}
		{
			//COPACI SPATE
			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(261.0f, 10.0f, -293.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(261.0f, 10.0f, -293.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(174.0f, 10.0f, -272.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(174.0f, 10.0f, -272.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(229.0f, 10.0f, -329.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(229.0f, 10.0f, -329.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(270.0f, 10.0f, -210.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(270.0f, 10.0f, -210.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(161.0f, 10.0f, -289.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(161.0f, 10.0f, -289.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(151.0f, 10.0f, -330.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(151.0f, 10.0f, -330.0f), vec3(2.1f, 2.1f, 2.1f));


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(190.0f, 10.0f, -311.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(190.0f, 10.0f, -311.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			shader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(173.0f, 10.0f, -342.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(173.0f, 10.0f, -342.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(210.0f, 10.0f, -281.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(210.0f, 10.0f, -281.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(140.0f, 10.0f, -311.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(140.0f, 10.0f, -311.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -230.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(245.0f, 10.0f, -230.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(140.0f, 10.0f, -272.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(140.0f, 10.0f, -272.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(132.0f, 10.0f, -290.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(132.0f, 10.0f, -290.0f), vec3(2.1f, 2.1f, 2.1f));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gold);
			glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -272.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(245.0f, 10.0f, -272.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(215.0f, 10.0f, -237.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(215.0f, 10.0f, -237.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(255.0f, 10.0f, -245.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(255.0f, 10.0f, -245.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(245.0f, 10.0f, -272.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(245.0f, 10.0f, -272.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(200.0f, 10.0f, -260.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(200.0f, 10.0f, -260.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(145.0f, 10.0f, -350.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(145.0f, 10.0f, -350.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(270.0f, 10.0f, -260.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(270.0f, 10.0f, -260.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(275.0f, 10.0f, -235.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(275.0f, 10.0f, -235.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(190.0f, 10.0f, -285.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(190.0f, 10.0f, -285.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(215.0f, 10.0f, -305.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(215.0f, 10.0f, -305.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(205.0f, 10.0f, -320.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(205.0f, 10.0f, -320.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(165.0f, 10.0f, -370.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(165.0f, 10.0f, -370.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(202.0f, 10.0f, -335.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(202.0f, 10.0f, -335.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(115.0f, 10.0f, -305.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(115.0f, 10.0f, -305.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(120.0f, 10.0f, -325.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(120.0f, 10.0f, -325.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(185.0f, 10.0f, -355.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(185.0f, 10.0f, -355.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(203.0f, 10.0f, -375.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(203.0f, 10.0f, -375.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);



			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(257.0f, 10.0f, -307.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(203.0f, 10.0f, -375.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(293.0f, 10.0f, -240.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(293.0f, 10.0f, -240.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(285.0f, 10.0f, -315.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(285.0f, 10.0f, -315.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(310.0f, 10.0f, -275.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(310.0f, 10.0f, -275.0f), vec3(2.1f, 2.1f, 2.1f));


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(135.0f, 10.0f, -360.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(135.0f, 10.0f, -360.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(147.0f, 10.0f, -380.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(147.0f, 10.0f, -380.0f), vec3(2.1f, 2.1f, 2.1f));


			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(117.0f, 10.0f, -384.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(117.0f, 10.0f, -384.0f), vec3(2.1f, 2.1f, 2.1f));


			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(300.0f, 10.0f, -306.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(300.0f, 10.0f, -306.0f), vec3(2.1f, 2.1f, 2.1f));


			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(220.0f, 10.0f, -386.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(220.0f, 10.0f, -386.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(212.0f, 10.0f, -352.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(212.0f, 10.0f, -352.0f), vec3(2.1f, 2.1f, 2.1f));

			tree3.draw(shader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(300.0f, 10.0f, -380.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(300.0f, 10.0f, -380.0f), vec3(2.1f, 2.1f, 2.1f));


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(312.0f, 10.0f, -361.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(312.0f, 10.0f, -361.0f), vec3(2.1f, 2.1f, 2.1f));

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(317.0f, 10.0f, -393.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(317.0f, 10.0f, -393.0f), vec3(2.1f, 2.1f, 2.1f));

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(308.0f, 10.0f, -408.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(308.0f, 10.0f, -408.0f), vec3(2.1f, 2.1f, 2.1f));

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(330.0f, 10.0f, -420.0f));
			ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			registerCollide(tree3, vec3(330.0f, 10.0f, -420.0f), vec3(2.1f, 2.1f, 2.1f));

			//parcela1
			{

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(15.0f, 10.0f, 131.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(15.0f, 10.0f, 131.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(13.0f, 10.0f, 150.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(13.0f, 10.0f, 150.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(55.0f, 10.0f, 120.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(55.0f, 10.0f, 120.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(79.0f, 10.0f, 138.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(79.0f, 10.0f, 138.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(39.0f, 10.0f, 109.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(39.0f, 10.0f, 109.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(42.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(42.0f, 10.0f, 121.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(63.0f, 10.0f, 109.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(63.0f, 10.0f, 109.0f), vec3(2.0f, 2.0f, 2.0f));

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
				registerCollide(tree3, vec3(25.0f, 10.0f, 250.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(45.0f, 10.0f, 275.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(45.0f, 10.0f, 275.0f), vec3(2.4f, 2.4f, 2.4f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(56.0f, 10.0f, 299.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(56.0f, 10.0f, 299.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(30.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(30.0f, 10.0f, 200.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(10.0f, 10.0f, 220.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(77.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(77.0f, 10.0f, 193.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(33.0f, 10.0f, 162.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(33.0f, 10.0f, 162.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(54.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(54.0f, 10.0f, 188.0f), vec3(1.7f, 1.7f, 1.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(98.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(98.0f, 10.0f, 188.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(17.0f, 10.0f, 255.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(17.0f, 10.0f, 255.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(75.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(75.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(8.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(8.0f, 10.0f, 121.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(68.0f, 10.0f, 125.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(68.0f, 10.0f, 125.0f), vec3(1.66f, 1.66f, 1.66f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(34.0f, 10.0f, 100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(34.0f, 10.0f, 100.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(99.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(99.0f, 10.0f, 193.0f), vec3(1.6f, 1.6f, 1.6f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(53.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(53.0f, 10.0f, 200.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(108.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(108.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(63.0f, 10.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(63.0f, 10.0f, 130.0f), vec3(1.5f, 1.5f, 1.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(53.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(53.0f, 10.0f, 200.0f), vec3(1.7f, 1.7f, 1.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(33.0f, 10.0f, 259.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(33.0f, 10.0f, 259.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(80.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(80.0f, 10.0f, 210.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, 10.0f, 283.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(100.0f, 10.0f, 283.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, 10.0f, 239.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(100.0f, 10.0f, 239.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(95.0f, 10.0f, 214.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(95.0f, 10.0f, 214.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(110.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(110.0f, 10.0f, 205.0f), vec3(2.3f, 2.3f, 2.3f));

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(79.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(79.0f, 10.0f, 205.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(67.0f, 10.0f, 199.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(67.0f, 10.0f, 199.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(84.0f, 10.0f, 180.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(84.0f, 10.0f, 180.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(55.0f, 10.0f, 167.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(55.0f, 10.0f, 167.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(103.0f, 10.0f, 195.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(103.0f, 10.0f, 195.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(88.0f, 10.0f, 177.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(88.0f, 10.0f, 177.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(100.0f, 10.0f, 170.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(50.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(50.0f, 10.0f, 170.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.0f, 10.0f, 190.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(85.0f, 10.0f, 190.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, 10.0f, 160.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(60.0f, 10.0f, 160.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(60.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(75.0f, 10.0f, 233.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(75.0f, 10.0f, 233.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(96.0f, 10.0f, 229.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(96.0f, 10.0f, 229.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(30.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(30.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(20.0f, 10.0f, 232.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(20.0f, 10.0f, 232.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(23.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(23.0f, 10.0f, 220.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);



				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(33.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(33.0f, 10.0f, 210.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(34.0f, 10.0f, 263.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(34.0f, 10.0f, 263.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(40.0f, 10.0f, 222.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(40.0f, 10.0f, 222.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(28.0f, 10.0f, 230.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(28.0f, 10.0f, 230.0f), vec3(2.6f, 2.6f, 2.6f));

				tree3.draw(shader);
			}
			//parcela2
			{}
			{
				int a = 100;
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 55.0f, 10.0f, 121.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 89.0f, 10.0f, 109.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 89.0f, 10.0f, 109.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 116.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 67.0f, 10.0f, 116.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 52.0f, 10.0f, 142.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 52.0f, 10.0f, 142.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 108.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 53.0f, 10.0f, 108.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 35.0f, 10.0f, 250.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 35.0f, 10.0f, 250.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 275.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 45.0f, 10.0f, 275.0f), vec3(2.4f, 2.4f, 2.4f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 56.0f, 10.0f, 299.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 30.0f, 10.0f, 200.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 10.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 10.0f, 10.0f, 220.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 77.0f, 10.0f, 193.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 162.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 162.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 54.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 54.0f, 10.0f, 188.0f), vec3(1.7f, 1.7f, 1.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 98.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 98.0f, 10.0f, 188.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 17.0f, 10.0f, 255.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 8.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 8.0f, 10.0f, 121.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 125.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 68.0f, 10.0f, 125.0f), vec3(1.66f, 1.66f, 1.66f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 100.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 99.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 99.0f, 10.0f, 193.0f), vec3(1.6f, 1.6f, 1.6f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 53.0f, 10.0f, 200.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 108.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 63.0f, 10.0f, 130.0f), vec3(1.5f, 1.5f, 1.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 53.0f, 10.0f, 200.0f), vec3(1.7f, 1.7f, 1.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 259.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 259.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 80.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 80.0f, 10.0f, 210.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 283.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 250.0f), vec3(1.9f, 1.9f, 1.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 239.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 239.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 95.0f, 10.0f, 214.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 95.0f, 10.0f, 214.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 110.0f, 10.0f, 205.0f), vec3(2.3f, 2.3f, 2.3f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 205.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 199.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 67.0f, 10.0f, 199.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 84.0f, 10.0f, 180.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 84.0f, 10.0f, 180.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 167.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 55.0f, 10.0f, 167.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);


				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 103.0f, 10.0f, 195.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 88.0f, 10.0f, 177.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 88.0f, 10.0f, 177.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 170.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 50.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 50.0f, 10.0f, 170.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 85.0f, 10.0f, 190.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 85.0f, 10.0f, 190.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 160.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 60.0f, 10.0f, 160.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 60.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 233.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 233.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 96.0f, 10.0f, 229.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 96.0f, 10.0f, 229.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 30.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 20.0f, 10.0f, 232.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 23.0f, 10.0f, 220.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 210.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 263.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 40.0f, 10.0f, 222.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 40.0f, 10.0f, 222.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 28.0f, 10.0f, 230.0f), vec3(2.6f, 2.6f, 2.6f));

				tree3.draw(shader);
			}
			//parcela3
			{}
			{
				int a = 200;
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 105.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 55.0f, 10.0f, 105.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 89.0f, 10.0f, 119.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 89.0f, 10.0f, 119.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 128.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 67.0f, 10.0f, 128.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 32.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 32.0f, 10.0f, 121.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 135.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 63.0f, 10.0f, 135.0f), vec3(2.0f, 2.0f, 2.0f));

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
				registerCollide(tree3, vec3(a + 25.0f, 10.0f, 250.0f), vec3(3.0f, 3.0f, 3.0f));
				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 275.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 45.0f, 10.0f, 275.0f), vec3(2.4f, 2.4f, 2.4f));
				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 56.0f, 10.0f, 299.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 30.0f, 10.0f, 200.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 10.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 10.0f, 10.0f, 220.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 77.0f, 10.0f, 193.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 162.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 162.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 54.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 54.0f, 10.0f, 188.0f), vec3(1.7f, 1.7f, 1.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 98.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 98.0f, 10.0f, 188.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 17.0f, 10.0f, 255.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 8.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 8.0f, 10.0f, 121.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 125.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 68.0f, 10.0f, 125.0f), vec3(1.66f, 1.66f, 1.66f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 100.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 99.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 99.0f, 10.0f, 193.0f), vec3(1.6f, 1.6f, 1.6f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 53.0f, 10.0f, 200.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 108.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 63.0f, 10.0f, 130.0f), vec3(1.5f, 1.5f, 1.5f));

				tree3.draw(shader);

				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 53.0f, 10.0f, 200.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.7f, 1.7f, 1.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 53.0f, 10.0f, 200.0f), vec3(1.7f, 1.7f, 1.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 259.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 259.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 80.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 80.0f, 10.0f, 210.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 283.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 250.0f), vec3(1.9f, 1.9f, 1.9f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 239.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 239.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 95.0f, 10.0f, 214.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 95.0f, 10.0f, 214.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);


				//alt copac cu mere
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 110.0f, 10.0f, 205.0f), vec3(2.3f, 2.3f, 2.3f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 205.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 199.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 67.0f, 10.0f, 199.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 84.0f, 10.0f, 180.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 84.0f, 10.0f, 180.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 55.0f, 10.0f, 167.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 55.0f, 10.0f, 167.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 103.0f, 10.0f, 195.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 88.0f, 10.0f, 177.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 88.0f, 10.0f, 177.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 170.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 50.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 50.0f, 10.0f, 170.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 85.0f, 10.0f, 190.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 85.0f, 10.0f, 190.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 160.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 60.0f, 10.0f, 160.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 60.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 233.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 233.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 96.0f, 10.0f, 229.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 96.0f, 10.0f, 229.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 30.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 30.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 20.0f, 10.0f, 232.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 23.0f, 10.0f, 220.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 210.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 263.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 40.0f, 10.0f, 222.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 40.0f, 10.0f, 222.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 28.0f, 10.0f, 230.0f), vec3(2.6f, 2.6f, 2.6f));

				tree3.draw(shader);
			}
			//parcela 4
			{}
			{
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-69.0f, 10.0f, 129.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-69.0f, 10.0f, 129.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-56.0f, 10.0f, 299.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-56.0f, 10.0f, 299.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-77.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-77.0f, 10.0f, 193.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-98.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-98.0f, 10.0f, 188.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-75.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-68.0f, 10.0f, 125.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-68.0f, 10.0f, 125.0f), vec3(1.66f, 1.66f, 1.66f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-99.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-99.0f, 10.0f, 193.0f), vec3(1.6f, 1.6f, 1.6f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-108.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-108.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-63.0f, 10.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-63.0f, 10.0f, 130.0f), vec3(1.5f, 1.5f, 1.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-80.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-80.0f, 10.0f, 210.0f), vec3(1.0f, 1.0f, 1.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, 10.0f, 283.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-100.0f, 10.0f, 283.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-79.0f, 10.0f, 250.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-79.0f, 10.0f, 250.0f), vec3(1.9f, 1.9f, 1.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, 10.0f, 239.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-100.0f, 10.0f, 239.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-95.0f, 10.0f, 214.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-95.0f, 10.0f, 214.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-110.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-110.0f, 10.0f, 205.0f), vec3(2.3f, 2.3f, 2.3f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-79.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-79.0f, 10.0f, 205.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-67.0f, 10.0f, 199.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-67.0f, 10.0f, 199.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-84.0f, 10.0f, 180.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-84.0f, 10.0f, 180.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-103.0f, 10.0f, 195.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-103.0f, 10.0f, 195.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-88.0f, 10.0f, 177.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-88.0f, 10.0f, 177.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-100.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-100.0f, 10.0f, 170.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-50.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-50.0f, 10.0f, 170.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-85.0f, 10.0f, 190.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-85.0f, 10.0f, 190.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 10.0f, 160.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-60.0f, 10.0f, 160.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 10.0f, 245.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-60.0f, 10.0f, 245.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-75.0f, 10.0f, 233.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-75.0f, 10.0f, 233.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-96.0f, 10.0f, 229.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(-96.0f, 10.0f, 229.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);
			}
			//parcela 5
			{}
			{
				int a = -215;

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 89.0f, 10.0f, 139.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 89.0f, 10.0f, 139.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 108.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 67.0f, 10.0f, 108.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 129.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 23.0f, 10.0f, 129.0f), vec3(2.0f, 2.0f, 2.0f));

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
				registerCollide(tree3, vec3(a + 25.0f, 10.0f, 250.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 275.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 45.0f, 10.0f, 275.0f), vec3(2.4f, 2.4f, 2.4f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 56.0f, 10.0f, 299.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 77.0f, 10.0f, 193.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 98.0f, 10.0f, 188.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 98.0f, 10.0f, 188.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 17.0f, 10.0f, 255.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 125.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 68.0f, 10.0f, 125.0f), vec3(1.66f, 1.66f, 1.66f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 100.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 99.0f, 10.0f, 193.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.6f, 1.6f, 1.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 99.0f, 10.0f, 193.0f), vec3(1.6f, 1.6f, 1.6f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 108.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 63.0f, 10.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 63.0f, 10.0f, 130.0f), vec3(1.5f, 1.5f, 1.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 259.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 259.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 80.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 80.0f, 10.0f, 210.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 283.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 250.0f), vec3(1.9f, 1.9f, 1.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 239.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 239.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 95.0f, 10.0f, 214.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 95.0f, 10.0f, 214.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 110.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.3f, 2.3f, 2.3f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 110.0f, 10.0f, 205.0f), vec3(2.3f, 2.3f, 2.3f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 205.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 205.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 67.0f, 10.0f, 199.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 67.0f, 10.0f, 199.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 84.0f, 10.0f, 180.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 84.0f, 10.0f, 180.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 103.0f, 10.0f, 195.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 103.0f, 10.0f, 195.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 88.0f, 10.0f, 177.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 88.0f, 10.0f, 177.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 170.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.8f, 2.8f, 2.8f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 170.0f), vec3(2.8f, 2.8f, 2.8f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 85.0f, 10.0f, 190.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 85.0f, 10.0f, 190.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 233.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 233.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 96.0f, 10.0f, 229.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 96.0f, 10.0f, 229.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 20.0f, 10.0f, 232.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 23.0f, 10.0f, 220.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 210.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 210.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.2f, 3.2f, 3.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 263.0f), vec3(3.2f, 3.2f, 3.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 40.0f, 10.0f, 222.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 40.0f, 10.0f, 222.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.6f, 2.6f, 2.6f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 28.0f, 10.0f, 230.0f), vec3(2.6f, 2.6f, 2.6f));

				tree3.draw(shader);
			}
			//parcela 6
			{}
			{
				int a = -320;
				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 52.0f, 10.0f, 112.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 52.0f, 10.0f, 112.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 77.0f, 10.0f, 134.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 77.0f, 10.0f, 134.0f), vec3(2.5f, 2.5f, 2.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 42.0f, 10.0f, 131.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 42.0f, 10.0f, 131.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 29.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 29.0f, 10.0f, 110.0f), vec3(2.0f, 2.0f, 2.0f));

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 45.0f, 10.0f, 289.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.4f, 2.4f, 2.4f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 45.0f, 10.0f, 289.0f), vec3(2.4f, 2.4f, 2.4f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 56.0f, 10.0f, 299.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 56.0f, 10.0f, 299.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 10.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 10.0f, 10.0f, 220.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 17.0f, 10.0f, 255.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 17.0f, 10.0f, 255.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 120.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 120.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 8.0f, 10.0f, 121.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 8.0f, 10.0f, 121.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 68.0f, 10.0f, 145.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.66f, 1.66f, 1.66f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 68.0f, 10.0f, 145.0f), vec3(1.66f, 1.66f, 1.66f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 100.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.7f, 2.7f, 2.7f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 100.0f), vec3(2.7f, 2.7f, 2.7f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 108.0f, 10.0f, 110.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 108.0f, 10.0f, 110.0f), vec3(3.0f, 3.0f, 3.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 73.0f, 10.0f, 130.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 73.0f, 10.0f, 130.0f), vec3(1.5f, 1.5f, 1.5f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 33.0f, 10.0f, 249.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 33.0f, 10.0f, 249.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 100.0f, 10.0f, 283.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 100.0f, 10.0f, 283.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 79.0f, 10.0f, 250.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(1.9f, 1.9f, 1.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 79.0f, 10.0f, 250.0f), vec3(1.9f, 1.9f, 1.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 60.0f, 10.0f, 140.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 60.0f, 10.0f, 140.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 75.0f, 10.0f, 263.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 75.0f, 10.0f, 263.0f), vec3(2.0f, 2.0f, 2.0f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac2);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 20.0f, 10.0f, 232.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.2f, 2.2f, 2.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 20.0f, 10.0f, 232.0f), vec3(2.2f, 2.2f, 2.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 23.0f, 10.0f, 220.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.9f, 2.9f, 2.9f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 23.0f, 10.0f, 220.0f), vec3(2.9f, 2.9f, 2.9f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac1);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 34.0f, 10.0f, 263.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(3.2f, 3.2f, 3.2f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 34.0f, 10.0f, 263.0f), vec3(3.2f, 3.2f, 3.2f));

				tree3.draw(shader);

				shader.use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, culoareCopac3);
				glUniform1i(glGetUniformLocation(shader.getId(), "texture_diffuse"), 0);

				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(a + 28.0f, 10.0f, 230.0f));
				ModelMatrix = scale(ModelMatrix, glm::vec3(2.1f, 2.1f, 2.1f));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				registerCollide(tree3, vec3(a + 28.0f, 10.0f, 230.0f), vec3(2.1f, 2.1f, 2.1f));

				tree3.draw(shader);
			}
			registerCollide(castle, vec3(-45.0f, 9.0f, -300.0f), vec3(0.04f, 0.04f, 0.04f));
			registerCollide(wall, vec3(0.0f, -28.0f, -180.0f), vec3(0.3f, 0.3f, 0.2f));
		}


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (showStory) {
			ImGui::SetNextWindowPos(ImVec2(window.getWidth() / 2.0f - 200, window.getHeight() / 2.0f - 100));
			ImGui::SetNextWindowSize(ImVec2(400, 200));
			ImGui::Begin("Headquarters of the Time Travelers", &showStory, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::TextWrapped("Hello Marian! So you've finished the training I see. I hope you did well in the Time Travelers academy.");
			ImGui::Separator();
			ImGui::TextWrapped("This is your very first mission. You must travel back in time to the 17th century and save the Princess of Luminara from the gypsy goblins.");
			ImGui::TextWrapped("It will not be an easy mission, be careful the past is unforgiving, and danger lurks at every step.");
			ImGui::TextWrapped("I wish you goodluck! As soon as you arrive in the Kingdom of Luminara the Omnitrix will guide you.");

			ImGui::Spacing();
			if (ImGui::Button("Travel?", ImVec2(120, 0))) {
				showStory = false;
				showHelloApple = true;
				playerPos = vec3(-20.0f, 15.0f, 250.0f);
			}
			ImGui::End();
		}
		else if (showHelloApple) {
			ImGui::SetNextWindowPos(ImVec2(window.getWidth() - 400.0f, 30.0f));
			ImGui::SetNextWindowSize(ImVec2(400.0f, 320.0f));
			ImGui::Begin("Kingdom Arrival", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

			ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), "WELCOME TO THE KINGDOM");
			ImGui::Separator();
			ImGui::TextWrapped("You have arrived in the 17th century at the village gates.\n Use the WASD keys to move and the Right Mouse to look around. In order to interact with other objects use E.");
			ImGui::Separator();
			ImGui::TextWrapped("Your first journey through time did not go as planned. The trip through time has weakened you, and your health is low. Search for apples and eat them to restore your strength before continuing your mission");
			ImGui::Separator();

			if (playerHealth == 100.0f && applecnt >= 4) {
				showHelloApple = false;
				questKing1 = true;
			}

			ImGui::End();

		}
		if (!showStory && !showHelloApple) {
			ImGui::SetNextWindowPos(ImVec2(window.getWidth() - 400.0f, 30.0f));
			ImGui::SetNextWindowSize(ImVec2(400.0f, 320.0f));
			if (questKing1) {
				ImGui::Begin("King Bob", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "ACTIVE MISSION:");
				ImGui::Separator();
				ImGui::TextWrapped("Now that you have restored your health go look for the King.");
				ImGui::BulletText("HINT! Look for the castle.");
				if (distance(playerPos, kingBobPos) < 15.0f) {
					ImGui::Spacing();
					ImGui::Separator();
					if (ImGui::Button("Talk to King Bob", ImVec2(-1.0f, 40.0f))) {
						questKing1 = false;
						questArmor = true;
					}
				}
				ImGui::End();
			}
			if (questArmor) {
				ImGui::Begin("Acquire Armour", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				if (!questArmorStarted) {
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1), "BELOOOOO, Welcome to the Kingdom of Luminara!");
					ImGui::Separator();
					ImGui::TextWrapped("Dark times have fallen upon this kingdom. My daughter, the princess, has been taken hostage by the Gypsy goblins, and I need your help to find her. You will not survive this battle unprepared. Arm yourself with proper armor and a sword. Go to the blacksmith i've prepared you something there.");
					if (ImGui::Button("Start quest", ImVec2(-1.0f, 40.0f)))
						questArmorStarted = true;
				}
				else {
					ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "ACTIVE MISSION:");
					ImGui::Separator();
					ImGui::TextWrapped("Find the blacksmith in the village and acquire proper armour for the battle ahead.");
					ImGui::BulletText("HINT! At the gate is a sign that indicates where the blacksmith is.");
					if (distance(playerPos, armorPos) < 10.0f) {
						ImGui::Spacing();
						ImGui::Separator();
						if (ImGui::Button("Pick up Armour", ImVec2(-1.0f, 40.0f))) {
							questArmor = false;
							questKing2 = true;
							equippedArmor = true;
						}
					}
				}
				ImGui::End();
			}
			if (questKing2) {
				ImGui::Begin("Return to King Bob", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "ACTIVE MISSION:");
				ImGui::Separator();
				ImGui::TextWrapped("Looks like you are protected with this armor. Go back to King Bob for further instructions.");
				ImGui::BulletText("HINT! Look for the castle.");
				if (distance(playerPos, kingBobPos) < 15.0f) {
					ImGui::Spacing();
					ImGui::Separator();
					if (ImGui::Button("Talk to King Bob", ImVec2(-1.0f, 40.0f))) {
						questKing2 = false;
						questWitch = true;
					}
				}
				ImGui::End();
			}
			if (questWitch) {
				ImGui::Begin("Finding the Witch", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				if (!questWitchStarted) {
					ImGui::TextWrapped("This armor suits you very well Knight! Now all you need is a sword! This is a special sword, it used to belong to my dear Father, but i trust it in your hands.");
					if (ImGui::Button("Start quest", ImVec2(-1.0f, 40.0f))) {
						questWitchStarted = true;
					}
				}
				else {
					ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "ACTIVE MISSION:");
					ImGui::Separator();
					ImGui::TextWrapped("Find the Witch's Hut outside the village and tell her to give you the Sword of Luminara. You just tell her i sent you.");
					ImGui::BulletText("HINT! At the gate is a sign that indicates where the witch is.");
					if (distance(playerPos, witchPos) < 10.0f) {
						ImGui::Spacing();
						ImGui::Separator();
						if (ImGui::Button("Talk to witch", ImVec2(-1.0f, 40.0f))) {
							talkToWitch = true;
							questWitch = false;
						}
					}
				}
				ImGui::End();
			}
			if (talkToWitch) {
				ImGui::Begin("The strange witch", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
				ImGui::TextWrapped("Ohh, a Traveler!? Haven't seen one of you in ages. So, the King sent you for his father's blade?");
				ImGui::TextWrapped("\nFine. Take the Sword of Luminara. It's the only thing that can pierce goblin hide. Now leave before anyone sees we've spoken!");
				if (ImGui::Button("Take Sword of Luminara", ImVec2(-1.0f, 40.0f))) {
					talkToWitch = false;
					questKing3 = true;
					swordTaken = true;
				}
				ImGui::End();
			}
			if (questKing3) {
				ImGui::Begin("Return to King Bob", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "ACTIVE MISSION:");
				ImGui::Separator();
				ImGui::TextWrapped("You have the Sword of Luminara! Now return to King Bob at the castle and prepare for the battle ahead.");
				ImGui::BulletText("HINT! Look for the castle.");
				if (distance(playerPos, kingBobPos) < 15.0f) {
					ImGui::Spacing();
					ImGui::Separator();
					if (ImGui::Button("Talk to King Bob", ImVec2(-1.0f, 40.0f))) {
						questKing3 = false;
						questFight = true;
					}
				}
				ImGui::End();
			}
			if (questFight) {
				ImGui::Begin("Saving Princess", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "FINAL MISSION:");
				ImGui::Separator();
				ImGui::TextWrapped("You are now ready to face the gypsy goblins and save the princess! They have taken her to their Golden Forrest, which is located somewhere Far in North-East.\n");
				ImGui::TextWrapped("Be careful they are some fearless creatures.\n I wish you goodluck.\n");
				if (ImGui::Button("Start Mission", ImVec2(-1.0f, 40.0f))) {
					questFight = false;
					inFight = true;
				}
				ImGui::End();
			}
			if (inFight) {
				ImGui::Begin("Saving Princess", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
				ImGui::TextColored(ImVec4(1, 0.9f, 0, 1), "FINAL MISSION:");
				ImGui::Separator();
				ImGui::TextWrapped("Find and defeat the gypsy goblins in the Golden Forrest and save the Princess!");
				ImGui::BulletText("HINT! Head North-East from the village.");
				if (goblin1Health <= 0 && goblin2Health <= 0 && goblin3Health <= 0) {
					ImGui::TextColored(ImVec4(0, 1, 0, 1), "ALL GOBLINS DEFEATED!");
					ImGui::Separator();
					ImGui::TextWrapped("The Princess is safe! You have completed your first mission as a Time Traveler.");
					if (ImGui::Button("Return to the Future (Close Game)", ImVec2(-1, 40))) {
						glfwSetWindowShouldClose(window.getWindow(), true);
					}
				}
				ImGui::End();
			}
			if (isDead) {
				ImGui::Begin("YOU DIED", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "MARIAN HAS FALLEN!");
				ImGui::Separator();
				ImGui::TextWrapped("The timeline has been fractured. You failed to save the Princess.");
				ImGui::Spacing();
				if (ImGui::Button("Respawn at Village Gates", ImVec2(-1.0f, 40.0f))) {
					playerHealth = 100.0f;
					isDead = false;
					playerPos = vec3(-20.0f, 15.0f, 250.0f);
				}
				ImGui::End();
			}



		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
	float moveSpeed = 25.0f * deltaTime;
	vec3 nextPos = playerPos;
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

		vec3 v = moveDir * moveSpeed;

		vec3 nextPosx = playerPos + vec3(v.x, 0.0f, 0.0f);
		if (!isColliding(nextPosx)) {
			playerPos.x = nextPosx.x;
		}
		else {
			std::cout << "COLLISION DETECTED ON X!" << std::endl;
		}

		vec3 nextPosZ = playerPos + vec3(0.0f, 0.0f, v.z);
		if (!isColliding(nextPosZ)) {
			playerPos.z = nextPosZ.z;
		}
		else {
			std::cout << "COLLISION ON Z!" << std::endl;
		}

		playerRoataion = atan2(moveDir.x, moveDir.z);
	}

	if (window.isPressed(GLFW_KEY_E)) {
		for (int i = 0; i < mapApples.size(); i++) {
			if (!mapApples[i].isEaten) {
				float dist = distance(playerPos, mapApples[i].position);
				if (dist < 5.0f) {
					mapApples[i].isEaten = true;
					if (showHelloApple)
						applecnt++;
					playerHealth += 20.0f;
					if (playerHealth > maxHealth) playerHealth = maxHealth;
				}
			}
		}
	}

	/*if (window.isPressed(GLFW_KEY_R))
		playerPos.y += moveSpeed;
	if (window.isPressed(GLFW_KEY_F))
		playerPos.y -= moveSpeed;*/
	if (window.isPressed(GLFW_KEY_LEFT_SHIFT))
		moveSpeed *= 2.0f;

	//rotation
	/*if (window.isPressed(GLFW_KEY_LEFT))
		camera.rotateOy(moveSpeed);
	if (window.isPressed(GLFW_KEY_RIGHT))
		camera.rotateOy(-moveSpeed);
	if (window.isPressed(GLFW_KEY_UP))
		camera.rotateOx(moveSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.rotateOx(-moveSpeed);*/
}

void registerCollide(Mesh& mesh, vec3 pos, vec3 scale) {
	Collide box;
	box.min = (mesh.minB * scale) + pos;
	box.max = (mesh.maxB * scale) + pos;

	int minX = (int)((box.min.x + offset) / square_size);
	int maxX = (int)((box.max.x + offset) / square_size);
	int minZ = (int)((box.min.z + offset) / square_size);
	int maxZ = (int)((box.max.z + offset) / square_size);

	for (int x = minX; x <= maxX; x++) {
		for (int z = minZ; z <= maxZ; z++) {
			if (x >= 0 && x < 199 && z >= 0 && z < 199) {
				grid[x][z].push_back(box);
			}
		}
	}
}

bool isColliding(vec3 pos) {
	int cx = (pos.x + offset) / square_size;
	int cz = (pos.z + offset) / square_size;

	if (cx < 0 || cx >= 200 || cz < 0 || cz >= 200)
		return false;

	for (Collide box : grid[cx][cz]) {
		if (pos.x + 2.0f >= box.min.x && pos.x - 2.0f <= box.max.x && pos.z + 2.0f >= box.min.z && pos.z - 2.0f <= box.max.z) {
			return true;
		}
	}
	return false;
}