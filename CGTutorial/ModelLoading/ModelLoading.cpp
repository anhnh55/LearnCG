#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HAUtilities/HAUtilities.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main();
void processInputGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInputUpdate(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float lastFrame = 0.0f; // Time of last frame
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
    glfwWindowHint(GLFW_SAMPLES, 4);

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
    // build and compile our shader zprogram
    // ------------------------------------
    HAUtilities::Shader backpackShader("Backpack.vs", "Backpack.fs");
    HAUtilities::Shader lightSourceShader("LightSource.vs", "LightSource.fs");
    HAUtilities::Model backpackModel("..\\Resources\\backpack\\backpack.obj");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

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
        backpackShader.Use();
        glm::vec3 camPos = freeMovementCamera.GetPosition();
        backpackShader.SetUniform("viewPos", camPos.x, camPos.y, camPos.z);
        backpackShader.SetMatrixUniform("viewMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetViewMatrix()));
        backpackShader.SetMatrixUniform("projectionMat", 1, GL_FALSE, glm::value_ptr(freeMovementCamera.GetProjectionMatrix()));

        // render the loaded model
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        backpackShader.SetMatrixUniform("modelMat", 1, GL_FALSE, glm::value_ptr(modelMat));

        glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
        backpackShader.SetMatrixUniform("normMat", 1, GL_FALSE, glm::value_ptr(normalMat), 3);
        glm::vec3 lightPos(0.0f, 0.0f, 2.0f);
        backpackShader.SetUniform("light.position", lightPos.x, lightPos.y, lightPos.z);
        backpackShader.SetUniform("light.ambient", 0.2f, 0.2f, 0.2f);
        backpackShader.SetUniform("light.diffuse", 0.7f, 0.7f, 0.7f);
        backpackShader.SetUniform("light.specular", 1.0f, 1.0f, 1.0f);
        backpackShader.SetUniform("material.shininess", 32.0f);

        backpackModel.Draw(backpackShader);

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