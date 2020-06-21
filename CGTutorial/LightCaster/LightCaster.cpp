#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HAUtilities/HAUtilities.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define NR_POINT_LIGHTS 4
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main();
void processInputGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInputUpdate(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float mixFactor = 0.5f;
float lastFrame = 0.0f; // Time of last frame

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//camera
HAUtilities::FreeMovementCamera freeMovementCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0, 40.0f, 800.0f, 600.0f, 0.1f, 100.0f);
//HAUtilities::FPSCamera freeMovementCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0, 40.0f, 800.0f, 600.0f, 0.1f, 100.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    float normalVecs[] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
    };

    float colors[] = {
         1.0f, 0.0f, 0.0f, //top right
         0.0f, 1.0f, 0.0f,  // bottom right
         0.0f, 0.0f, 1.0f,  // bottom left
         1.0f, 1.0f, 1.0f   // top 
    };

    float texCoords[] = {
         0.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         1.0f, 1.0f,
         0.0f, 1.0f,
         0.0f, 0.0f,
         0.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         1.0f, 1.0f,
         0.0f, 1.0f,
         0.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f,
         0.0f, 1.0f,
         0.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f,
         0.0f, 1.0f,
         0.0f, 0.0f,
         1.0f, 0.0f,
         0.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, 0.0f,
         1.0f, 0.0f,
         0.0f, 0.0f,
         0.0f, 1.0f,
         0.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, 0.0f,
         1.0f, 0.0f,
         0.0f, 0.0f,
         0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // build and compile our shader zprogram
    // ------------------------------------
    HAUtilities::Shader cubeShader("Cube.vs", "Cube.fs");
    HAUtilities::Shader lightSourceShader("LightSource.vs", "LightSource.fs");
    HAUtilities::TriangleDataBindingWrapper cubeDataBinding(vertices, sizeof(vertices), colors, sizeof(colors), texCoords, sizeof(texCoords), indices, sizeof(indices), normalVecs, sizeof(normalVecs), 0, 1, 2, 3);
    HAUtilities::TriangleDataBindingWrapper lightSourceDataBinding(vertices, sizeof(vertices), colors, sizeof(colors), texCoords, sizeof(texCoords), indices, sizeof(indices), normalVecs, sizeof(normalVecs), 0, 1, 2, 3);
    //HAUtilities::Texture2DWrapper crateTexture("..\\Resources\\container.jpg", GL_RGB, GL_RGB, GL_TEXTURE0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true);
    //HAUtilities::Texture2DWrapper smilingTexture("..\\Resources\\awesomeface.png", GL_RGBA, GL_RGBA, GL_TEXTURE1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true);
    HAUtilities::Texture2DWrapper cubeDiffuseTexture("..\\Resources\\container2.png", GL_RGBA, GL_RGBA, GL_TEXTURE0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true);
    HAUtilities::Texture2DWrapper cubeSpecTexture("..\\Resources\\container2_specular.png", GL_RGBA, GL_RGBA, GL_TEXTURE1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true);
    HAUtilities::Texture2DWrapper cubeEmisTexture("..\\Resources\\matrix.jpg", GL_RGB, GL_RGB, GL_TEXTURE2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, true);

    // render loop
    // -----------
    glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
    glEnable(GL_DEPTH_TEST);
    glm::vec3 direction;
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInputUpdate(window, deltaTime);
        glm::vec3 lightPos(5.0f, 0.0f, -2.0f);
        //const float radius = 3.0f;
        //lightPos.x = sin(glfwGetTime()) * radius;
        //lightPos.z = cos(glfwGetTime()) * radius;
        // render container
        cubeDiffuseTexture.Use();
        cubeSpecTexture.Use();
        //cubeEmisTexture.Use();
        cubeShader.Use();
        glm::vec3 camPos = freeMovementCamera.GetPosition();
        glm::vec3 camDir = freeMovementCamera.GetForward();
        cubeShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        cubeShader.SetUniform("material.ambient", 1.0f, 0.5f, 0.31f);
        cubeShader.SetUniform("material.diffuse", (GLint)cubeDiffuseTexture.GetTextureUnit() - GL_TEXTURE0);
        GLint texID = (GLint)cubeSpecTexture.GetTextureUnit() - (GLint)GL_TEXTURE0;
        cubeShader.SetUniform("material.specular", (GLint)cubeSpecTexture.GetTextureUnit() - GL_TEXTURE0);
        //cubeShader.SetUniform("material.emission", (GLint)cubeEmisTexture.GetTextureUnit() - GL_TEXTURE0);
        cubeShader.SetUniform("material.shininess", 32.0f);
        cubeShader.SetUniform("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        cubeShader.SetUniform("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        cubeShader.SetUniform("dirLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetUniform("dirLight.direction", -0.0f, -0.0f, -1.0f);
        // point light 1
        cubeShader.SetUniform("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        cubeShader.SetUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetUniform("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetUniform("pointLights[0].constant", 1.0f);
        cubeShader.SetUniform("pointLights[0].linear", 0.09f);
        cubeShader.SetUniform("pointLights[0].quadratic", 0.032f);
        // point light 2
        cubeShader.SetUniform("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        cubeShader.SetUniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetUniform("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetUniform("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetUniform("pointLights[1].constant", 1.0f);
        cubeShader.SetUniform("pointLights[1].linear", 0.09f);
        cubeShader.SetUniform("pointLights[1].quadratic", 0.032f);
        // point light 3
        cubeShader.SetUniform("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        cubeShader.SetUniform("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetUniform("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetUniform("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetUniform("pointLights[2].constant", 1.0f);
        cubeShader.SetUniform("pointLights[2].linear", 0.09f);
        cubeShader.SetUniform("pointLights[2].quadratic", 0.032f);
        // point light 4
        cubeShader.SetUniform("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        cubeShader.SetUniform("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetUniform("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetUniform("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetUniform("pointLights[3].constant", 1.0f);
        cubeShader.SetUniform("pointLights[3].linear", 0.09f);
        cubeShader.SetUniform("pointLights[3].quadratic", 0.032f);

        cubeShader.SetUniform("spotLight.direction", camDir.x, camDir.y, camDir.z);
        cubeShader.SetUniform("spotLight.position", camPos.x, camPos.y, camPos.z);
        cubeShader.SetUniform("spotLight.constant", 1.0f);
        cubeShader.SetUniform("spotLight.cutOffCosine", glm::cos(glm::radians(12.0f)));
        cubeShader.SetUniform("spotLight.outerCutOffCosine", glm::cos(glm::radians(15.0f)));
        cubeShader.SetUniform("spotLight.linear", 0.045f);
        cubeShader.SetUniform("spotLight.quadratic", 0.0075f);
        cubeShader.SetUniform("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        cubeShader.SetUniform("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
        //set view mat
        cubeShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        cubeShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));

        cubeDataBinding.Use();
        
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, cubePositions[i]);
            //modelMat = glm::rotate(modelMat, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            cubeShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));
            glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
            cubeShader.SetMatrixUniform("normMat", 1, GL_FALSE, glm::value_ptr(normalMat), 3);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            //render lightsource
            lightSourceShader.Use();
            //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
            glm::mat4 lightSourceModelMat = glm::mat4(1.0f);
            lightSourceModelMat = glm::mat4(1.0f);
            lightSourceModelMat = glm::translate(lightSourceModelMat, pointLightPositions[i]);
            lightSourceModelMat = glm::scale(lightSourceModelMat, glm::vec3(0.2f));
            //set view mat
            lightSourceShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
            lightSourceShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));
            lightSourceShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(lightSourceModelMat));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cubeDataBinding.Delete();

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