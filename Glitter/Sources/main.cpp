// Local Headers
#include "glitter.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
//#include "../../Build/Model.hpp"
//#include "../../Build/Cube.hpp"
#include "Scene.hpp"


// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Scene* scene);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//jump-crouch
bool pressedBeforeJump = false;
bool pressedBeforeCrouch = false;
bool finishedJump = true;
bool dead = false;
int numberOfJumpFrames = 0;
int numJumps = 0;


//camera
Camera* camera =new Camera(glm::vec3(-1.2f, 2.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 2.0f, 0.0f);


Cube* mouseCube = nullptr;
glm::mat4 mouseModel;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Global opengl state
    glEnable(GL_DEPTH_TEST);
    
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    ResourceManager::LoadShader("../Glitter/Shaders/shader.vert", "../Glitter/Shaders/shader.frag",nullptr, "shader");
    ResourceManager::LoadShader("../Glitter/Shaders/ModelShader.vert", "../Glitter/Shaders/ModelShader.frag", nullptr, "model");
    ResourceManager::LoadShader("../Glitter/Shaders/skyboxShader.vert", "../Glitter/Shaders/skyboxShader.frag", nullptr, "skybox");
    ResourceManager::LoadShader("../Glitter/Shaders/ScreenShader.vert", "../Glitter/Shaders/ScreenShader.frag", nullptr, "screen");
    ResourceManager::LoadShader("../Glitter/Shaders/Basic.vert", "../Glitter/Shaders/Basic.frag", nullptr ,"basic");
    ResourceManager::LoadShader("../Glitter/Shaders/LightShader.vert", "../Glitter/Shaders/LightShader.frag", nullptr, "lightShader");
    ResourceManager::LoadShader("../Glitter/Shaders/sDepthShader.vert", "../Glitter/Shaders/sDepthShader.frag", nullptr, "simpleDepth");
    //ResourceManager::LoadShader("../Glitter/Shaders/ShaddowMap.vert", "../Glitter/Shaders/ShaddowMap.frag", nullptr, "depth");

    
    //Shader shader("Resources/Shaders/shader.vert", "Resources/Shaders/shader.frag");
    //Shader LightShader("Resources/Shaders/LightShader.vert", "Resources/Shaders/LightShader.frag");
    //Shader modelShader("Resources/Shaders/ModelShader.vert", "Resources/Shaders/ModelShader.frag");
    //Shader outlineShader("Resources/Shaders/OutlineShader.vert", "Resources/Shaders/OutlineShader.frag");
    //Shader skyboxShader("Resources/Shaders/skyboxShader.vert", "Resources/Shaders/skyboxShader.frag");
    //screen
    //Shader screenShader("Resources/Shaders/ScreenShader.vert", "Resources/Shaders/ScreenShader.frag");

    ResourceManager::GetShader("shader").use();
    ResourceManager::GetShader("shader").setInt("material.diffuse", 0);
    ResourceManager::GetShader("shader").setInt("material.specular", 1);
    //ResourceManager::GetShader("shader").setInt("shadowMap", 2);
    ResourceManager::GetShader("shader").setInt("material.shininess", 64);
    ResourceManager::GetShader("shader").setInt("gamma", true);


    ResourceManager::GetShader("skybox").use();
    ResourceManager::GetShader("skybox").setInt("skybox", 0);

    ResourceManager::GetShader("screen").use();
    ResourceManager::GetShader("screen").setInt("screenTexture", 0);
    ResourceManager::GetShader("screen").setFloat("targetWidth", SCR_WIDTH/4);
    ResourceManager::GetShader("screen").setFloat("targetHeight", SCR_HEIGHT/4);

    ResourceManager::GetShader("basic").use();
    ResourceManager::GetShader("basic").setInt("texture1", 0);
    
    //Cube light(glm::vec3(-0.6f, 0.6f, -0.1f), glm::vec3(0.2f, 0.2f, 0.2f), "../Glitter/Textures/", "light", ".jpg");
    
    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mouseCube = new Cube(glm::vec3(-0.01f, -0.01f, 0.0f), glm::vec3(0.03f, 0.04f, 0.0f), "../Glitter/Resources/Textures/", "cursor", ".png");
    mouseModel = glm::mat4(1.0f);

    Scene* scene = new Scene();
    scene->camera = camera;
    scene->level->camera = camera;
    //AABB box = scene->level->DestCubes[0].boundingBox;
    //Cube* cube = new Cube(glm::vec3(box.min.x, box.max.y, box.min.z), glm::vec3(box.max.x - box.min.x, -(box.min.y - box.max.y), box.max.z - box.min.z), "Resources/Textures/", "grass.jpg");


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::perspective(glm::radians(camera->Zoom), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // Cubes
        ResourceManager::GetShader("shader").use();
        ResourceManager::GetShader("shader").setMat4("model", model);
        ResourceManager::GetShader("shader").setMat4("view", view);
        ResourceManager::GetShader("shader").setMat4("projection", projection);
        ResourceManager::GetShader("shader").setVec3("viewPos", camera->Position);

        // model
        ResourceManager::GetShader("model").use();
        ResourceManager::GetShader("model").setMat4("view", view);
        ResourceManager::GetShader("model").setMat4("projection", projection);

        // skybox
        ResourceManager::GetShader("skybox").use();
        view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        ResourceManager::GetShader("skybox").setMat4("view", view);
        ResourceManager::GetShader("skybox").setMat4("projection", projection);

        ResourceManager::GetShader("simpleDepth").use();
        ResourceManager::GetShader("simpleDepth").setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        scene->level->DrawShadowMap();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // input
        // -----
        processInput(window, scene);
        // render
        // ------
        //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->Draw();
        
        ResourceManager::GetShader("basic").use();
        ResourceManager::GetShader("basic").setMat4("model", mouseModel);
        if (scene->startingScreen->MouseEnabled)
        {
            mouseCube->Draw();
        }
        ResourceManager::GetShader("basic").setMat4("model", model);

        ResourceManager::GetShader("lightShader").use();
        ResourceManager::GetShader("lightShader").setMat4("view", view);
        ResourceManager::GetShader("lightShader").setMat4("projection", projection);
        ResourceManager::GetShader("lightShader").setMat4("model", model);


        //glEnable(GL_FRAMEBUFFER_SRGB);

        //ResourceManager::GetShader("shader").use();
        //light.Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        ResourceManager::GetShader("screen").use();
        if (scene->startingScreen->MouseEnabled)
        {
            ResourceManager::GetShader("screen").setBool("blur", true);
        }
        else {
            ResourceManager::GetShader("screen").setBool("blur", false);
        }
        ResourceManager::GetShader("screen").setVec2("tlMenuCoords[3]", glm::vec2((*mouseCube).boundingBox.min.x, (*mouseCube).boundingBox.max.y));
        ResourceManager::GetShader("screen").setVec2("brMenuCoords[3]", glm::vec2((*mouseCube).boundingBox.max.x, (*mouseCube).boundingBox.min.y));
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Scene* scene)
{
    if (scene->startingScreen->MouseEnabled)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {   
            if (intersect(scene->startingScreen->StartButton.boundingBox, mouseCube->boundingBox))
            {
                if (dead)
                {
                    scene->level->character->walk(Right, deltaTime);
                    scene->level->character->resetModel();
                    scene->level->character->resetBoxes();
                }
                scene->startingScreen->MouseEnabled = false;
            }
            else if (intersect(scene->startingScreen->LevelsButton.boundingBox, mouseCube->boundingBox))
            {
                cout << "Levels" << endl;
            }
            else if (intersect(scene->startingScreen->ExitButton.boundingBox, mouseCube->boundingBox))
            {
                glfwSetWindowShouldClose(window, true);
            }
        }
    }
    else 
    {
        scene->processMovement(window, deltaTime);
        if (scene->level->character->box().min.y < -4)
        {
            scene->startingScreen->MouseEnabled = true;
            dead = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    /*if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (mouseCube->boundingBox.min.x + xoffset * 0.007f> -1 && mouseCube->boundingBox.min.y + yoffset * 0.007f > -1 && mouseCube->boundingBox.max.x + xoffset * 0.007f < 1 && mouseCube->boundingBox.max.y + yoffset * 0.007f < 1)
    {
        mouseCube->updateBox(glm::vec3(xoffset*0.007f, yoffset*0.007f, 0.0f));
        mouseModel = glm::translate(mouseModel, glm::vec3(xoffset * 0.007f, yoffset * 0.007f, 0.0f));
    }
    camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
