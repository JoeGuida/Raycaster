#include "main.hpp"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
    }

    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path shader_path = current_path / "shaders";
    std::filesystem::path scene_path = current_path / "scenes";
    if(shader_path.empty() || scene_path.empty()) {
        spdlog::error("could not resolve project paths");
    }

    spdlog::info("initializing window");
    Input input { .initialized = false };
    auto window = initialize_window(instance, show_window_flags, width, height, L"Window Class", L"Raycaster", input);
    if(!window.has_value()) {
        spdlog::error("{}", window.error());
    }

    Scene scene = load_scene_from_file((scene_path / "scene.yml").string());

    spdlog::info("initializing renderer");
    Renderer renderer;
    initialize(renderer);
    //renderer.scene_data = scene;

    spdlog::info("loading map");
    write_map_to_buffers(scene.map, renderer, scene.camera.aspect, color_palette);

    spdlog::info("setting up renderer to draw");
    glm::vec2 rect_size(scene.camera.aspect / scene.map.width, 2.0f / scene.map.height);
    scene.camera.position = glm::vec4(-scene.camera.aspect + rect_size.x * 3.0f, 0.8f, 0.0f, 0.0f);
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

    glm::mat4 projection = glm::ortho(-scene.camera.aspect, scene.camera.aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    renderer.shaders = std::move(scene.shaders);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    spdlog::info("setting shader uniforms");

    for(const auto& [name, shader] : renderer.shaders) { 
        glUseProgram(shader);
        set_shader_uniform(shader, "projection", projection);
        set_shader_uniform(shader, "view", view);
    }

    glUseProgram(renderer.shaders["point"]);
    set_shader_uniform(renderer.shaders["point"], "size", 3.0f);

    renderer.positions[POINT_OFFSET + renderer.point_count] = to_vec4(scene.camera.position);
    renderer.colors[POINT_OFFSET + renderer.point_count] = to_vec4(scene.camera.direction);
    renderer.point_count++;

    write_camera_fov_to_buffers(renderer, scene.camera);
    raycast_fov(renderer, scene.camera.direction);

    spdlog::info("starting message loop");

    renderer.view_count = width;
    setup(renderer);
    run_message_loop(window.value(), renderer, scene.background_color);

    return 0;
}

