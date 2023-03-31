#include <OpenGL/OpenGL.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

struct CameraController {
    constexpr static glm::vec3 initialPosition = glm::vec3(15.f, 15.f, -40.f);
    glm::vec3 position = initialPosition;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    float horizontalAngle = 0.f;
    float verticalAngle = 0.f;
    float fov = 45.f;
    float speed = 5.f;
    float mouseSpeed = 0.07f;
    double lastTime;
    bool mouseCapture = true;
    GLFWwindow *window;

    CameraController(GLFWwindow *window) : window(window) {}

    void update() {
        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (mouseCapture) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            int xWindow, yWindow;
            glfwGetWindowSize(window, &xWindow, &yWindow);
            glfwSetCursorPos(window, xWindow / 2., yWindow / 2.);

            float cursorXDelta = xWindow / 2. - x;
            float cursorYDelta = yWindow / 2. - y;

            horizontalAngle += mouseSpeed * deltaTime * cursorXDelta;
            verticalAngle += mouseSpeed * deltaTime * cursorYDelta;

            direction = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
            );

            right = glm::vec3(
                sin(horizontalAngle - M_PI / 2.),
                0,
                cos(horizontalAngle - M_PI / 2.)
            );

            up = glm::cross(right, direction);
        }

        float frameSpeed = speed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            frameSpeed *= 3;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += direction * deltaTime * frameSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position -= right * deltaTime * frameSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position -= direction * deltaTime * frameSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position += right * deltaTime * frameSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position += up * deltaTime * frameSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            position -= up * deltaTime * frameSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            horizontalAngle = 0.;
            verticalAngle = 0.;
            position = initialPosition;
        }

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            fmt::print("{} {} {} / {} {}\n", position.x, position.y, position.z, horizontalAngle, verticalAngle);
        }
    }

    void uncaptureMouse() {
        mouseCapture = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void captureMouse() {
        mouseCapture = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    glm::mat4 vp() const {
        int w, h;
        glfwGetWindowSize(window, &w, &h);

        auto projection = glm::perspective(
            glm::radians(fov),
            (float)w / (float)h,
            0.1f,
            100.f
        );

        auto view = glm::lookAt(
            position,
            position + direction,
            up
        );

        return projection * view;
    }
};