#include "CubeMap.hpp"

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

CubeMap::CubeMap(string path)
{
    initCubeMap();
    vector<std::string> faces{
        path + "right.jpg",
        path + "left.jpg",
        path + "top.jpg",
        path + "bottom.jpg",
        path + "front.jpg",
        path + "back.jpg"
    };

    texture = loadCubemap(faces);
}

CubeMap::~CubeMap()
{
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
}

void CubeMap::Draw()
{
    Shader& shader = ResourceManager::GetShader("skybox");
    glDepthFunc(GL_EQUAL);
    shader.use();
    shader.setInt("skybox", 0);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void CubeMap::initCubeMap()
{
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
    vertices.push_back(genVertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

    glBindVertexArray(0);
}