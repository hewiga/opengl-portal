#include "application.h"

Application::Application() :
    WINDOWS_WIDTH(1600),
    WINDOWS_HEIGHT(900),
    CAMERA_SPEED(0.05f),
    m_firstMouseMove(true),
    m_camera(),
    MAX_PORTAL_DEPTH(1)
{
    m_lastX = WINDOWS_WIDTH / 2;
    m_lastY = WINDOWS_HEIGHT / 2; 
}

void Application::handleMouse(GLFWwindow* window)
{
    double xPos, yPos;
    float yaw = m_camera.getYaw();
    float pitch = m_camera.getPitch();

    glfwGetCursorPos(window, &xPos, &yPos);

    if (m_firstMouseMove) {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouseMove = false;
    }

    float xoffset = xPos - m_lastX;
    float yoffset = m_lastY - yPos;
    m_lastX = xPos;
    m_lastY = yPos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    //make sure that when pitch is out of bounds screen doesnt get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    m_camera.updateCameraDirection(glm::normalize(front), yaw, pitch);
}

void Application::handleInputs(GLFWwindow* window) {

    glm::vec3 velocity(0.0f);
    glm::vec3 cameraUp = m_camera.getCameraUp();
    glm::vec3 cameraFront = m_camera.getCameraFront();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        velocity += CAMERA_SPEED * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        velocity -= CAMERA_SPEED * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        velocity += glm::normalize(glm::cross(cameraFront, cameraUp)) * CAMERA_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        velocity -= glm::normalize(glm::cross(cameraFront, cameraUp)) * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    m_camera.updateCameraPosition(velocity);
}

void Application::drawPortal(glm::mat4 view, Shader& shader, uint8_t recursionLevel) {
    shader.setMat4("view", view);

    for (int i = 0; i < m_portals.size(); i++) {

        if ((recursionLevel > 0) && ((i == 0) || (i == 2))) continue;

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        glDisable(GL_DEPTH_TEST);

        glEnable(GL_STENCIL_TEST);

        glStencilFunc(GL_NOTEQUAL, recursionLevel, 0xFF);

        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

        glStencilMask(0xFF);

        m_portals[i].draw(shader);

        glm::mat4 destView = m_portals[i].portalView(view);
     
        if (recursionLevel == MAX_PORTAL_DEPTH)
        {
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);

            glClear(GL_DEPTH_BUFFER_BIT);
            
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);

            glStencilMask(0x00);

            glStencilFunc(GL_EQUAL, recursionLevel + 1, 0xFF);

            shader.setMat4("view", destView);
            for (int j = 0; j < m_meshes.size(); j++) {
                m_meshes[j].draw(shader);
            }
        }
        else
        {
            drawPortal(destView, shader, recursionLevel + 1);
        }
        shader.setMat4("view", view);

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);

        glStencilFunc(GL_NOTEQUAL, recursionLevel + 1, 0xFF);
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

        m_portals[i].draw(shader);
    }

    glDisable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glDepthFunc(GL_ALWAYS);

    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < m_portals.size(); i++) {
        if ((recursionLevel > 0) && ((i == 0) || (i == 2))) continue;

        m_portals[i].draw(shader);
    }

    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    glStencilFunc(GL_LEQUAL, recursionLevel, 0xFF);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i].draw(shader);
    }
}

int Application::run() {

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "OpenGL", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glewInit() != GLEW_OK) {
            std::cout << "Glew init Error!" << std::endl;
        }

        Shader shader("vertex.shader", "fragment.shader");
        shader.activateShader();

        objl::Loader loader;
        loader.LoadFile("res/models/scene.obj");

        for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
            std::cout << loader.LoadedMeshes[i].MeshName << std::endl;
            if (loader.LoadedMeshes[i].MeshName.substr(0, loader.LoadedMeshes[i].MeshName.find(".")) != "Portal") {
                m_meshes.push_back(Mesh(loader.LoadedMeshes[i], m_textures));
            }
            else {
                m_portals.push_back(Portal(loader.LoadedMeshes[i]));
            }
        }
        m_portals[0].connectWithPortal(&m_portals[6]);
        m_portals[1].connectWithPortal(&m_portals[7]);
        m_portals[2].connectWithPortal(&m_portals[4]);
        m_portals[3].connectWithPortal(&m_portals[5]);

        m_portals[4].connectWithPortal(&m_portals[2]);
        m_portals[5].connectWithPortal(&m_portals[3]);
        m_portals[6].connectWithPortal(&m_portals[0]);
        m_portals[7].connectWithPortal(&m_portals[1]);


        glm::mat4 view = glm::mat4(1.0f);

        glm::mat4 model = glm::mat4(1.0f);
    
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT, 0.01f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("projection", projection);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        while (!glfwWindowShouldClose(window))
        {

            glfwPollEvents();
            handleMouse(window);
            handleInputs(window);
            for (int i = 0; i < m_portals.size(); i++) {
                if (m_camera.collisionDetection(m_portals[i])) {
                    m_camera.warpCamera(m_portals[i]);
                }
            }

            view = m_camera.getLookAt();
        
            glClearColor(0.4, 0.4, 0.9, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            shader.activateShader();

            drawPortal(view, shader, 0);

            glDisable(GL_STENCIL_TEST);

            for (int i = 0; i < m_meshes.size(); i++) {
                m_meshes[i].draw(shader);
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }

        shader.deleteShader();

        glfwTerminate();
        return 0;
}