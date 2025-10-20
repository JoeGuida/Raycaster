#include "main.hpp"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
    }

    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path shader_path = current_path / "shaders";
    std::filesystem::path map_path = current_path / "maps";
    if(shader_path.empty() || map_path.empty()) {
        spdlog::error("environment variables could not be read!");
    }

    spdlog::info("initializing window");
    Input input { .initialized = false };
    auto window = initialize_window(instance, show_window_flags, width, height, L"Window Class", L"Raycaster", input);
    if(!window.has_value()) {
        spdlog::error("{}", window.error());
    }

    Scene scene {
        .background_color = glm::vec3(0.2157f, 0.1843f, 0.2275f)
    };

    Camera camera {
        .position = glm::vec4(),
        .direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
        .fov = 45.0f,
        .aspect = width / static_cast<float>(height)
    };

    spdlog::info("initializing renderer");
    Renderer renderer;
    initialize(renderer);
    renderer.scene_data = scene;

    spdlog::info("loading map");
    Map map;
    load_map_from_file(map, (map_path / "map.txt").string());
    write_map_to_buffers(map, renderer, camera.aspect, color_palette);

    spdlog::info("setting up renderer to draw");
    glm::vec2 rect_size(camera.aspect / map.width, 2.0f / map.height);
    camera.position = glm::vec4(-camera.aspect + rect_size.x * 3.0f, 0.8f, 0.0f, 0.0f);
    float x = rect_size.x / 2.0f;
    float y = rect_size.y / 2.0f;
    renderer.vertices = { 
        glm::vec2(-x,  y), // rect
        glm::vec2( x,  y),
        glm::vec2(-x, -y),
        glm::vec2( x, -y),
        glm::vec2(0.0f, 0.0f), // line
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f) // point
    };

    glm::mat4 projection = glm::ortho(-camera.aspect, camera.aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    spdlog::info("compiling shaders");
    auto shaders = compile_shaders({"rect", "line", "point", "view"}, shader_path.string());
    if(!shaders.has_value()) {
        spdlog::info("{}", shaders.error());
        return -1;
    }

    renderer.shaders = { 
        shaders.value()["rect"], 
        shaders.value()["line"], 
        shaders.value()["point"], 
        shaders.value()["view"] 
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    spdlog::info("setting shader uniforms");

    for(auto shader : renderer.shaders) { 
        glUseProgram(shader);
        set_shader_uniform(shader, "projection", projection);
        set_shader_uniform(shader, "view", view);
    }

    glUseProgram(shaders.value()["point"]);
    set_shader_uniform(shaders.value()["point"], "size", 3.0f);

    renderer.positions[POINT_OFFSET + renderer.point_count] = to_vec4(camera.position);
    renderer.colors[POINT_OFFSET + renderer.point_count] = to_vec4(camera.direction);
    renderer.point_count++;

    write_camera_fov_to_buffers(renderer, camera);
    raycast_fov(renderer, camera.direction);

    spdlog::info("starting message loop");

    renderer.view_count = width;
    setup(renderer);
    run_message_loop(window.value(), renderer, renderer.scene_data.background_color);

    return 0;
}

