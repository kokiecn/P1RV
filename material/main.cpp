#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include  "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float sensivility = 0.01;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader objShader("vertexshader.vs", "fragmentshader.fs");
   
    Shader lightShader("lightvert.vs", "lightfrag.fs");

    float vertices[] = {
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
   
    unsigned int VBO, objVAO;
    glGenVertexArrays(1, &objVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(objVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

   


   
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objShader.use();
        objShader.setVec3("light.position", lightPos);
    
        objShader.setVec3("viewPos", camera.Position);

        // light 
        //faire changer la couleur au fil de temps
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); 
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 
        objShader.setVec3("light.ambient", ambientColor);
        objShader.setVec3("light.diffuse", diffuseColor);
        objShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material 
        objShader.setVec3("material.ambient", 0.5f, 0.5f, 0.0f);
        objShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.0f);
        objShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); 
        objShader.setFloat("material.shininess", 32.0f);


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        objShader.setMat4("projection", projection);
        objShader.setMat4("view", view);
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        objShader.setMat4("model", model);

        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // activate shader
        lightShader.use();
        
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lightVAO); glDeleteVertexArrays(1, &objVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
 
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

/*
vertex shader

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

fragment shader

#version 330 core
out vec4 FragColor;

struct Material {   // structure de materiel (propriété)
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {     // structure similaire à Materiel 
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;     // on va mettre la valeur  "-Material-.-propriété-"
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}


light

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set alle 4 vector values to 1.0
}
*/