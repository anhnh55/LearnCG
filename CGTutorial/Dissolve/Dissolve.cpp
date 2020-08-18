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
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float lastFrame = 0.0f; // Time of last frame
//camera
HAUtilities::FreeMovementCamera freeMovementCamera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0, 40.0f, 800.0f, 600.0f, 0.1f, 100.0f);

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

    float squareVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.0f,  0.0f, 0.0f, //bottom left
         0.5f, -0.5f, -0.0f,  1.0f, 0.0f, //bottom right
         0.5f,  0.5f, -0.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.0f,  0.0f, 0.0f, //bottom left
    };

    float decalVertices[] = {
        // positions          // texture Coords
        -0.2f, -0.2f, -0.0f,  0.0f, 0.0f, //bottom left
         0.2f, -0.2f, -0.0f,  1.0f, 0.0f, //bottom right
         0.2f,  0.2f, -0.0f,  1.0f, 1.0f,
         0.2f,  0.2f, -0.0f,  1.0f, 1.0f,
        -0.2f,  0.2f, -0.0f,  0.0f, 1.0f,
        -0.2f, -0.2f, -0.0f,  0.0f, 0.0f, //bottom left
    };    
    
    float stencilVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.0f,  0.0f, 0.0f, //bottom left
            0.5f, -0.5f, -0.0f,  1.0f, 0.0f, //bottom right
            0.5f,  0.5f, -0.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.0f,  0.0f, 0.0f, //bottom left
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

    // build and compile our shader zprogram
    // ------------------------------------
    std::vector<std::string> cubeTextures{ DIFFUSE_TEXTURE_STRING, "..\\Resources\\marble.jpg" };
    std::vector<std::string> decalTextures{ DIFFUSE_TEXTURE_STRING, "..\\Resources\\awesomeface.png"};
    std::vector<std::string> planeTextures{ DIFFUSE_TEXTURE_STRING, "..\\Resources\\metal.png" };
    std::vector<std::string> stencilTextures;
    HAUtilities::Model squareModel(squareVertices, sizeof(squareVertices) / sizeof(float), false, true, cubeTextures);
    HAUtilities::Model cubeModel(cubeVertices, sizeof(cubeVertices) / sizeof(float), false, true, cubeTextures);
    HAUtilities::Model stencilModel(stencilVertices, sizeof(stencilVertices) / sizeof(float), false, true, stencilTextures);
    HAUtilities::Model decalModel(decalVertices, sizeof(decalVertices) / sizeof(float), false, true, decalTextures);
    HAUtilities::Model planeModel(planeVertices, sizeof(planeVertices) / sizeof(float), false, true, planeTextures);
    HAUtilities::Shader cubeShader("Cube.vs", "Cube.fs");
    HAUtilities::Shader floorShader("Cube.vs", "Floor.fs");
    //HAUtilities::Shader lightSourceShader("LightSource.vs", "LightSource.fs");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    float step = 0.01f;
    float interval = 0.0f;
    bool shouldDissolve = false;
    float scale = 0.0f;
    float scaleInterval = 0.01;
    while (!glfwWindowShouldClose(window))
    {
        //clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        interval += deltaTime;
        if (interval >= 0.05f) {
            shouldDissolve = true;
            interval -= 0.05f;
            scale += scaleInterval;
            if (scale >= 1.0f) {
                scale = 1.0f;
                scaleInterval *= -1.0f;
            }
            else if (scale <= 0.0f) {
                scale = 0.0f;
                scaleInterval *= -1.0f;
            }
        }

        processInputUpdate(window, deltaTime); 
        cubeShader.Use();
        glm::vec3 camPos = freeMovementCamera.GetPosition();
        cubeShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        cubeShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        cubeShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));

        //render floor
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(scale, scale, 1.0f));
        cubeShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));

        ////render stencil buffer
        glStencilFunc(GL_NEVER, 1, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
        stencilModel.Draw(cubeShader);
        // render container
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        cubeShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
        squareModel.Draw(cubeShader);

        //test reflection
        //cubeModel.Draw(cubeShader);


        //floorShader.Use();
        //floorShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        //floorShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        //floorShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));
        //modelMat = glm::mat4(1.0f);
        //modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        //modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));
        //floorShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));

        //draw to stencil buffer
        //glDisable(GL_DEPTH_TEST);
        //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        //glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glStencilMask(0xFF);
        //glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        //planeModel.Draw(floorShader);
        //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        //glEnable(GL_DEPTH_TEST);

        //draw reflected cube
        //cubeShader.Use();
        //modelMat = glm::mat4(1.0f);
        //modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.2f, 0.2f));
        //modelMat = glm::scale(modelMat, glm::vec3(1.0f, -1.0f, 1.0f));
        //cubeShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
        //glStencilFunc(GL_EQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        //cubeModel.Draw(cubeShader);

        //draw real floor
        //glDisable(GL_STENCIL_TEST);
        //floorShader.Use();
        //floorShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        //floorShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        //floorShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));
        //modelMat = glm::mat4(1.0f);
        //modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        //modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));
        //floorShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ////glColor4f(0.7, 0.0, 0.0, 0.3);
        //floorShader.SetUniform("isDrawingBackFace", 0);
        //planeModel.Draw(floorShader);
        //glDisable(GL_BLEND);
        //glCullFace(GL_BACK);
        //glDisable(GL_DEPTH_TEST);
        //planeModel.Draw(floorShader);
        //glDisable(GL_CULL_FACE);
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

void PrintStencilBuffer(GLubyte* data) {
    std::ofstream myfile("stencil.txt");
    const int size = 480000;
    int rowBreak = 0;
    if (myfile.is_open())
    {
        //for (int count = 0; count < size; count++) {
        //    myfile << data[count] << " ";
        //}
        //if (rowBreak < 800) {
        //    ++rowBreak;
        //}
        //else {
        //    myfile << std::endl;
        //    rowBreak = 0;
        //}
        myfile.write((char*)data, 480000);
        myfile.close();
    }
    else std::cout << "Unable to open file";
}