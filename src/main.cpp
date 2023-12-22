#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <unordered_map>
#include <vector>

#include "camera.hpp"
#include "line.hpp"
#include "intersection.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::vector<Line> get_lines(const std::vector<Rectangle>& rectangles);
std::vector<Rectangle> get_rectangles(const char* map, std::unordered_map<int, glm::vec3>& colors, int count);
GLFWwindow* initialize();
void process_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void set_wireframe();

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 1080;
const int VIEWPORT_WIDTH = 2;
const int VIEWPORT_HEIGHT = 2;
const int FOV_RAY_COUNT = 1920;
constexpr double ASPECT_RATIO = double(SCREEN_WIDTH) / double(SCREEN_HEIGHT);
const int MAP_WIDTH = 16;
const int MAP_HEIGHT = 16;
const int EMPTY_SPACE = -16;

bool wireframe = false;

const Material RED_MATERIAL(glm::vec3(1.0f, 0.0f, 0.0f));
const Material GREEN_MATERIAL(glm::vec3(0.0f, 1.0f, 0.0f));
const Material BLUE_MATERIAL(glm::vec3(0.0f, 0.0f, 1.0f));
const Material WHITE_MATERIAL(glm::vec3(1.0f));

int main() {
    // --------------------------------------------------------------------------------------------
    // Setup GLFW, GLAD, Renderer, initialize window
    // --------------------------------------------------------------------------------------------
    GLFWwindow* window = initialize();
    Renderer::initialize();

    // --------------------------------------------------------------------------------------------
    // Shader Setup
    // --------------------------------------------------------------------------------------------
    std::shared_ptr<Shader> shader = ShaderFactory::create("default");

    // --------------------------------------------------------------------------------------------
    // Map Setup
    // --------------------------------------------------------------------------------------------
    std::unordered_map<int, glm::vec3> COLORS {
        {0, glm::vec3(0.0823f, 0.0549f, 0.0627f)},
        {1, glm::vec3(0.1529f, 0.1529f, 0.2235f)},
        {2, glm::vec3(0.2235f, 0.2196f, 0.2862f)},
        {3, glm::vec3(0.2313f, 0.2549f, 0.3215f)},
        {4, glm::vec3(0.3098f, 0.3529f, 0.3921f)},
        {5, glm::vec3(0.4666f, 0.5333f, 0.5490f)}
    };

    const char MAP[] {
        "0000222222220000"\
        "1              0"\
        "1     111111   0"\
        "1     0        0"\
        "0     0  1110000"\
        "0     3        0"\
        "0   10000      0"\
        "0   0   11100  0"\
        "0   0   0      0"\
        "0   0   1  00000"\
        "0       1      0"\
        "2       1      0"\
        "0       0      0"\
        "0 0000000      0"\
        "0              0"\
        "0002222222200000"
    };
    assert(sizeof(MAP) == MAP_WIDTH * MAP_HEIGHT + 1);

    // --------------------------------------------------------------------------------------------
    // Object Setup
    // --------------------------------------------------------------------------------------------
    std::vector<Rectangle> rectangles = get_rectangles(MAP, COLORS, MAP_WIDTH * MAP_HEIGHT);
    std::vector<Line> lines = get_lines(rectangles);

    Camera camera(
        glm::radians(45.0f), 
        Transform(glm::vec3(-0.65f, 0.75f, 0.0f)),
        glm::vec3(0.0f, -1.0f, 0.0f),
        FOV_RAY_COUNT
    );
    camera.update_rays();
    Line camera_direction(
        camera.transform.position, 
        camera.transform.position + camera.get_direction() * 1.0f,
        BLUE_MATERIAL,
        Transform(glm::vec3(0.0f))
    );

    // --------------------------------------------------------------------------------------------
    // Render Loop
    // --------------------------------------------------------------------------------------------
    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0);
    shader->set("projection", projection);
    while (!glfwWindowShouldClose(window)) {
        // clear window
        const char x = '5';
        glm::vec3 background_color = COLORS['0'];
        glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        Point camera_point(5.0f, Material(glm::vec3(1.0f, 0.0f, 0.0f)), camera.transform.position);
        Ray* camera_rays = camera.get_rays();

        for (int i = 0; i < lines.size(); i++) {
            Renderer::draw(lines[i], shader);
        }

        for (int i = 0; i < rectangles.size(); i++) {
            Renderer::draw(rectangles[i], shader);
        }

        // swap buffers and poll for input events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::vector<Line> get_lines(const std::vector<Rectangle>& rectangles) {
    std::vector<Line> lines;
    for (int i = 0; i < rectangles.size(); i++) {
        float width = rectangles[i].width;
        float height = rectangles[i].height;

        std::array<glm::vec3, 4> vertices{
            glm::vec3(-width / 2.0f,  height / 2.0f, 0.0f),
            glm::vec3( width / 2.0f,  height / 2.0f, 0.0f),
            glm::vec3(-width / 2.0f, -height / 2.0f, 0.0f),
            glm::vec3( width / 2.0f, -height / 2.0f, 0.0f)
        };

        lines.push_back(Line(vertices[0], vertices[1], RED_MATERIAL, rectangles[i].transform)); // top
        lines.push_back(Line(vertices[1], vertices[3], BLUE_MATERIAL, rectangles[i].transform)); // right
        lines.push_back(Line(vertices[2], vertices[3], GREEN_MATERIAL, rectangles[i].transform)); // bottom
        lines.push_back(Line(vertices[0], vertices[2], WHITE_MATERIAL, rectangles[i].transform)); // left
    }
    return lines;
}

std::vector<Rectangle> get_rectangles(const char* map, std::unordered_map<int, glm::vec3>& colors, int count) {
    std::vector<Rectangle> rectangles;
    rectangles.reserve(count);

    static const float size = VIEWPORT_WIDTH / static_cast<float>(MAP_WIDTH);
    static const glm::vec2 start(-1.0f + size / 2.0f, 1.0f - size / 2.0f);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i * MAP_WIDTH + j] - '0' == EMPTY_SPACE) {
                continue;
            }

            glm::vec3 color = colors[map[i * MAP_WIDTH + j]];
            glm::vec3 pos(start.x + j * size, start.y - i * size, 0.0f);
            Rectangle rect(size, Material(color), Transform(pos));
            rectangles.push_back(rect);
        }
    }

    return rectangles;
}

GLFWwindow* initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create GLFW window and set callbacks
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, process_input);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    return window;
}

void process_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        wireframe = !wireframe;
        set_wireframe();
    }
}

void set_wireframe() {
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}