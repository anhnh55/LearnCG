#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HAUtilities/HAUtilities.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInputGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInputUpdate(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void PrintStencilBuffer(GLubyte* data);
unsigned int loadCubemap(std::vector<std::string> faces);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float lastFrame = 0.0f; // Time of last frame
//camera
HAUtilities::FreeMovementCamera freeMovementCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0, 40.0f, SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_DEPTH_BITS, 32);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, processInputGLFWCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    HAUtilities::stbi_set_flip_vertically_on_load(true);

    //float cubeVertices[] = {
    //    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    //    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    //    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    //    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    //    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    //};

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    // build and compile our shader zprogram
    // ------------------------------------
    //std::vector<std::string> cubeTextures ;
    std::vector<std::string> cubeTextures = { DIFFUSE_TEXTURE_STRING, "..\\Resources\\marble.jpg" };
    std::vector<std::string> planeTextures{ DIFFUSE_TEXTURE_STRING, "..\\Resources\\metal.png" };
    std::vector<std::string> cubeMapTextures;
    HAUtilities::Model cubeModel(cubeVertices, sizeof(cubeVertices) / sizeof(float), false, true, cubeTextures);
    //HAUtilities::Model cubeModel(cubeVertices, sizeof(cubeVertices) / sizeof(float), true, false, cubeTextures);
    HAUtilities::Model floorModel(planeVertices, sizeof(planeVertices) / sizeof(float), false, true, planeTextures);
    HAUtilities::Model cubeMapModel(skyboxVertices, sizeof(skyboxVertices) / sizeof(float), false, false, cubeMapTextures);

    HAUtilities::Shader cubeShader("Cube.vs", "Cube.fs");
    HAUtilities::Shader cubeReflectShader("CubeReflect.vs", "CubeReflect.fs");
    HAUtilities::Shader cubeMapShader("CubeMap.vs", "CubeMap.fs");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    //gen cube texture
    std::vector<std::string> faces
    {
        "..\\Resources\\skybox\\right.jpg",
        "..\\Resources\\skybox\\left.jpg",
        "..\\Resources\\skybox\\top.jpg",
        "..\\Resources\\skybox\\bottom.jpg",
        "..\\Resources\\skybox\\front.jpg",
        "..\\Resources\\skybox\\back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    std::vector<HAUtilities::Texture> textures;
    HAUtilities::Texture texture;
    texture.id = cubemapTexture;
    texture.type = CUBE_TEXTURE_STRING;
    textures.push_back(texture);
    cubeMapModel.SetTextures(textures);
    //cubeModel.SetTextures(textures);

    glm::mat4 modelMat = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInputUpdate(window, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        cubeShader.Use();
        glm::vec3 camPos = freeMovementCamera.GetPosition();
        //cubeShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        cubeShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        cubeShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));
        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.251f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        cubeShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
        cubeModel.Draw(cubeShader);

        //cubeReflectShader.Use();
        //glm::vec3 camPos = freeMovementCamera.GetPosition();
        //cubeReflectShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        //cubeReflectShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        //cubeReflectShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));
        //modelMat = glm::mat4(1.0f);
        //modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.251f, 0.0f));
        //modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        //cubeReflectShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
        //glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
        //cubeReflectShader.SetMatrixUniform("normMat", 1, GL_FALSE, glm::value_ptr(normalMat), 3);
        //cubeModel.Draw(cubeReflectShader);

        //render floor
        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));
        cubeShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
        floorModel.Draw(cubeShader);

        //render cubemap
        glDepthFunc(GL_LEQUAL);
        cubeMapShader.Use();
        glm::mat4 cubeMapViewMatrix = glm::mat4(glm::mat3(freeMovementCamera.GetViewMatrix()));
        cubeMapShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(cubeMapViewMatrix));
        cubeMapShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));
        cubeMapModel.Draw(cubeMapShader);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInputGLFWCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void processInputUpdate(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        freeMovementCamera.MoveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        freeMovementCamera.MoveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        freeMovementCamera.MoveStrafeLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        freeMovementCamera.MoveStrafeRight(deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    freeMovementCamera.Rotate(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    freeMovementCamera.Zoom(-yoffset);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    HAUtilities::stbi_set_flip_vertically_on_load(false);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = HAUtilities::stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            HAUtilities::stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            HAUtilities::stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    HAUtilities::stbi_set_flip_vertically_on_load(true);
    return textureID;
}