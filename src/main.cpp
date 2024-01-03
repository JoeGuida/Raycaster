#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <unordered_map>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "line.hpp"
#include "intersection.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "shader.hpp"

void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float get_closest_line_hit(const Ray& ray, const std::vector<Line>& lines);
std::vector<Line> get_lines(const std::vector<Rectangle>& rectangles);
std::vector<Rectangle> get_rectangles(const char* map, std::unordered_map<int, glm::vec3>& colors, int count);
GLFWwindow* initialize();
void process_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void set_wireframe();

// Screen
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr float VIEWPORT_WIDTH = 2.0f;
constexpr float VIEWPORT_HEIGHT = 2.0f;
constexpr double ASPECT_RATIO = double(SCREEN_WIDTH) / double(SCREEN_HEIGHT);

// Map
constexpr int MAP_WIDTH = 16;
constexpr int MAP_HEIGHT = 16;
constexpr float MAP_SCREEN_PERCENTAGE = 0.4f;
constexpr float MAP_SIZE = VIEWPORT_WIDTH / static_cast<float>(MAP_WIDTH) * MAP_SCREEN_PERCENTAGE * MAP_WIDTH;
constexpr int EMPTY_SPACE = -16;

// Camera
constexpr float CAMERA_VELOCITY = 0.01f;
constexpr float CAMERA_ROTATION_VELOCITY = 0.1f;
constexpr int FOV_RAY_COUNT = SCREEN_WIDTH;
Camera camera(
    glm::radians(45.0f),
    Transform(glm::vec3(-1.65f, 0.9f, 0.0f)),
    glm::vec3(0.0f, -1.0f, 0.0f),
    FOV_RAY_COUNT
);

// Input
static bool first_mouse = true;
static bool wireframe = false;
static double last_x = 0;
static double last_y = 0;

// Materials
static const Material RED_MATERIAL(glm::vec3(1.0f, 0.0f, 0.0f));
static const Material GREEN_MATERIAL(glm::vec3(0.0f, 1.0f, 0.0f));
static const Material BLUE_MATERIAL(glm::vec3(0.0f, 0.0f, 1.0f));
static const Material WHITE_MATERIAL(glm::vec3(1.0f));

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
        {5, glm::vec3(0.0823f, 0.0549f, 0.0627f)},
        {4, glm::vec3(0.1529f, 0.1529f, 0.2235f)},
        {3, glm::vec3(0.2235f, 0.2196f, 0.2862f)},
        {2, glm::vec3(0.2313f, 0.2549f, 0.3215f)},
        {1, glm::vec3(0.3098f, 0.3529f, 0.3921f)},
        {0, glm::vec3(0.4666f, 0.5333f, 0.5490f)}
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
    static const float size = VIEWPORT_WIDTH / static_cast<float>(MAP_WIDTH) * MAP_SCREEN_PERCENTAGE * MAP_WIDTH;
    Rectangle map_background(size, Material(glm::vec3(0.2f)), 
        Transform(glm::vec3(-ASPECT_RATIO + size / 2.0f, 1.0f - size / 2.0f, 0.0f)));
    std::vector<Rectangle> rectangles = get_rectangles(MAP, COLORS, MAP_WIDTH * MAP_HEIGHT);
    std::vector<Line> lines = get_lines(rectangles);

    // floor/ceiling
    Rectangle floor(ASPECT_RATIO * 2.0, 1.0f, Material(COLORS[2]), Transform(glm::vec3(0.0f, -0.5f, 0.0f)));
    Rectangle ceiling(ASPECT_RATIO * 2.0, 1.0f, Material(COLORS[0]), Transform(glm::vec3(0.0f, 0.5f, 0.0f)));

    camera.update_rays();
    Point camera_point(5.0f, GREEN_MATERIAL, camera.transform.position);
    Ray* camera_rays = camera.get_rays();

    // --------------------------------------------------------------------------------------------
    // Render Loop
    // --------------------------------------------------------------------------------------------
    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0);
    shader->set("projection", projection);

    while (!glfwWindowShouldClose(window)) {
        // clear window
        glm::vec3 background_color = COLORS['0'];
        glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw the minimap camera point and view direction
        camera_point.transform.position = camera.transform.position;
        Line camera_direction(camera.transform.position, camera.transform.position + camera.direction * 0.1f);
        Renderer::draw(camera_point, shader);
        Renderer::draw(camera_direction, Color::RED, shader);

        // draw the minimap camera rays
        std::array<float, FOV_RAY_COUNT> heights;
        for (int i = 0; i < FOV_RAY_COUNT; i++) {
            float closest_hit = get_closest_line_hit(camera_rays[i], lines);
            float view_angle = glm::angle(camera.direction, camera_rays[i].direction);
            heights[i] = (size - closest_hit) * cos(view_angle);
            Renderer::draw(camera_rays[i], WHITE_MATERIAL, shader, closest_hit);
        }

        // draw the minimap rectangles and background
        for (int i = 0; i < rectangles.size(); i++) {
            Renderer::draw(rectangles[i], shader);
        }
        Renderer::draw(map_background, shader);

        // draw the raycasted lines
        for (int i = 0; i < heights.size(); i++) {
            glm::vec3 pos(-ASPECT_RATIO + i * (ASPECT_RATIO * 2 / FOV_RAY_COUNT), 0.0f, 0.0f);
            Line line(pos + glm::vec3(0.0f, -heights[i], 0.0f), pos + glm::vec3(0.0f, heights[i], 0.0f));
            Renderer::draw(line, Color(COLORS[1]), shader);
        }

        // draw floor/ceiling
        Renderer::draw(floor, shader);
        Renderer::draw(ceiling, shader);

        // swap buffers and poll for input events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    if (xpos > last_x) {
        camera.rotate(-CAMERA_ROTATION_VELOCITY);
    }
    if (xpos < last_x) {
        camera.rotate(CAMERA_ROTATION_VELOCITY);
    }

    last_x = xpos;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float get_closest_line_hit(const Ray& ray, const std::vector<Line>& lines) {
    float closest_hit = MAP_SIZE;
    for (const Line& line : lines) {
        float distance = Intersection::raycast(ray, line);
        if (distance != -1.0f && distance < closest_hit) {
            closest_hit = distance;
        }
    }

    return closest_hit;
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

        lines.push_back(Line(
            rectangles[i].transform.position + vertices[0], 
            rectangles[i].transform.position + vertices[1])); // top
        lines.push_back(Line(
            rectangles[i].transform.position + vertices[1], 
            rectangles[i].transform.position + vertices[3])); // right
        lines.push_back(Line(
            rectangles[i].transform.position + vertices[2], 
            rectangles[i].transform.position + vertices[3])); // bottom
        lines.push_back(Line(
            rectangles[i].transform.position + vertices[0], 
            rectangles[i].transform.position + vertices[2])); // left
    }
    return lines;
}

std::vector<Rectangle> get_rectangles(const char* map, std::unordered_map<int, glm::vec3>& colors, int count) {
    std::vector<Rectangle> rectangles;
    rectangles.reserve(count);

    static const float size = VIEWPORT_WIDTH / static_cast<float>(MAP_WIDTH) * MAP_SCREEN_PERCENTAGE;
    static const glm::vec2 start(-ASPECT_RATIO + size / 2.0f, 1.0f - size / 2.0f);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i * MAP_WIDTH + j] - '0' == EMPTY_SPACE) {
                continue;
            }

            glm::vec3 color = colors[map[i * MAP_WIDTH + j] - '0'];
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
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glm::vec3 offset(0.0f);
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        offset += glm::vec3(0.0f, -1.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        offset += glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        offset += glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        offset += glm::vec3(-1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
    }

    camera.translate(offset);
}

void set_wireframe() {
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}