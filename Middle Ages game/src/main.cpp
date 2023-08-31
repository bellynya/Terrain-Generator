#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb_image.h"

#include <iostream>

#include "Shader.h"
#include "Terrain.h"
#include "Camera.h"
#include "FaultTerrainFormation.h"
#include "TextureGenerator.h"
#include "Texture.h"
#include "MidpointDispTerrain.h"
#include "Skybox.h"
#include "House.h"

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(207.44f, 56.0417f, 124.854f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

// Тайминги
float deltaTime = 0.0f;	// время между текущим кадром и последним кадром
float lastFrame = 0.0f;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::SHIFTED_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::SHIFTED_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::SHIFTED_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void checkGLError()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << err;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void CreateFirstVillage(std::vector<House>& houses)
{
    houses[0].Translate(133.0f, 38.0f, 76.0f);
    houses[0].Rotate(0.0f, 1.0f, 0.0f, 45.0f);
    houses[0].Rotate(0.0f, 0.0f, 1.0f, -10.0f);

    houses[1].Translate(127.5f, 40.0f, 78.6f);
    houses[1].Rotate(0.0f, 1.0f, 0.0f, 90.0f);

    houses[2].Translate(133.177f, 38.0f, 84.2806f);
    houses[2].Rotate(0.0f, 1.0f, 0.0f, 180.0f);
    houses[2].Rotate(1.0f, 0.0f, 1.0f, 5.0f);

    houses[3].Translate(141.937f, 36.6752f, 80.7014f);
    houses[3].Rotate(0.0f, 1.0f, 0.0f, -90.0f);

    houses[4].Translate(141.374f, 36.6752f, 83.8139f);
    houses[4].Rotate(0.0f, 1.0f, 0.0f, -135.0f);

    houses[5].Translate(137.92f, 37.07f, 72.8338f);
    houses[5].Rotate(0.0f, 1.0f, 0.0f, 0.0f);
}

void CreateSecondVillage(std::vector<House>& houses)
{
    houses[0].Translate(54.8268f, 60.745f, 173.638f);
    houses[0].Rotate(0.0f, 1.0f, 0.0f, 120.0f);

    houses[1].Translate(75.0401f, 59.7163f, 167.281f);
    houses[1].Rotate(0.0f, 1.0f, 0.0f, 140.0f);

    houses[2].Translate(54.788f, 60.5f, 157.718f);
    houses[2].Rotate(0.0f, 1.0f, 0.0f, 53.0f);
    houses[2].Rotate(1.0f, 0.0f, 0.0f, 10.0f);

    houses[3].Translate(68.9636f, 57.3928f, 160.853f);
    houses[3].Rotate(0.0f, 1.0f, 0.0f, -56.0f);

    houses[4].Translate(65.9549f, 59.4867f, 174.43f);
    houses[4].Rotate(0.0f, 1.0f, 0.0f, 190.0f);

    houses[5].Translate(69.79f, 58.789f, 171.062f);
    houses[5].Rotate(0.0f, 1.0f, 0.0f, 90.0f);
}

void CreateThirdVillage(std::vector<House>& houses)
{
    houses[0].Translate(194.233f, 78.4313f, 287.318f);
    houses[0].Rotate(0.0f, 1.0f, 0.0f, 120.0f);

    houses[1].Translate(197.974f, 78.6f, 285.574f);
    houses[1].Rotate(0.0f, 1.0f, 0.0f, -90.0f);
    houses[1].Rotate(1.0f, 0.0f, 0.0f, 10.0f);
}

void RenderGroupOne(std::vector<House>& houses)
{
    houses[0].Render(camera);
    houses[1].Render(camera);
    houses[2].Render(camera);
    houses[3].Render(camera);
    houses[4].Render(camera);
    houses[5].Render(camera);
}

void RenderGroupTwo(std::vector<House>& houses)
{
    houses[0].Render(camera);
    houses[1].Render(camera);
    houses[2].Render(camera);
    houses[3].Render(camera);
    houses[4].Render(camera);
    houses[5].Render(camera);

}

void RenderGroupThree(std::vector<House>& houses)
{
    houses[0].Render(camera);
    houses[1].Render(camera);
}

int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif


    // glfw создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Midd Age game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Terrain terrain("src/shader/vertex.shader", "src/shader/fragment.shader", 200, 200);
    //terrain.Init();
    int Size = 512;
    MidpointDispTerrain terrain("src/shader/terrain/vertex.shader", "src/shader/terrain/fragment.shader", Size, Size);

    float Roughness = 1.0f;
    float MinHeight = 0.0f;
    float MaxHeight = 90.0f;

    terrain.CreateMidpointDisplacement(Size, Roughness, MinHeight, MaxHeight);
    terrain.Init();

    TextureGenerator TexGen;

    TexGen.LoadTile("textures/rock02_2.jpg");

    TexGen.LoadTile("textures/rock01.jpg");

    TexGen.LoadTile("textures/tilable-IMG_0044-verydark.png");

    TexGen.LoadTile("textures/water.png");
    int TextureSize = 1024;

    Texture* pTexture = TexGen.GenerateTexture(TextureSize, &terrain, MinHeight, MaxHeight);
    terrain.SetTexture(pTexture);

    Skybox skybox("src/shader/skybox/vertex.shader", "src/shader/skybox/fragment.shader");

    std::vector<House> houses1(6, House("src/shader/houses/vertex.vs", "src/shader/houses/frag.fs", 1000.0f, 1000.0f, 250.0f));

    std::vector<House> houses2(6, House("src/shader/houses/vertex.vs", "src/shader/houses/frag.fs", 1000.0f, 1000.0f, 250.0f));

    std::vector<House> houses3(2, House("src/shader/houses/vertex.vs", "src/shader/houses/frag.fs", 1000.0f, 1000.0f, 250.0f));



    //House house("src/shader/houses/vertex.vs", "src/shader/houses/frag.fs", 1000.0f, 1000.0f, 250.0f);

    std::vector<std::string> faces
    {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };

    skybox.Init(faces);

    for (auto& i : houses1)
    {
        i.Init("textures/dom2_1.png");
    }

    for (auto& i : houses2)
    {
        i.Init("textures/dom2_2.png");
    }

    for (auto& i : houses3)
    {
        i.Init("textures/dom2_3.png");
    }


    CreateFirstVillage(houses1);
    CreateSecondVillage(houses2);
    CreateThirdVillage(houses3);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Обработка ввода
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain.Render(camera);

        skybox.Render(camera);

        RenderGroupOne(houses1); 

        RenderGroupTwo(houses2);

        RenderGroupThree(houses3);

        std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glfwTerminate();
    return 0;
}