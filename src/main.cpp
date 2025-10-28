#include "main.hpp"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
    }

    std::filesystem::path scene_path = std::filesystem::current_path() / "scenes";
    if(scene_path.empty()) {
        spdlog::error("could not resolve project paths");
    }

    spdlog::info("initializing window");
    Input input { .initialized = false };
    auto window = initialize_window(instance, show_window_flags, 
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 
                                    L"Window Class", L"Raycaster", input);

    if(!window.has_value()) {
        spdlog::error("{}", window.error());
    }

    spdlog::info("loading scene");
    Scene scene = load_scene_from_file((scene_path / "scene.yml").string());

    // camera
    glm::vec2 size(scene.camera.aspect / scene.map.width / 2.0f,
                   2.0f / scene.map.height / 2.0f);
    scene.camera.position = glm::vec2(-scene.camera.aspect + size.x * 3.0f, 0.8f);
    spdlog::info("({}, {})", scene.camera.position.x, scene.camera.position.y);

    spdlog::info("initializing renderer");
    Renderer renderer;
    initialize(renderer);
    
    for(const auto& i : scene.points) {
        renderer.positions[POINT_OFFSET + renderer.point_count] = to_vec4(i.position);
        renderer.colors[POINT_OFFSET + renderer.point_count] = to_vec4(i.color);
        renderer.point_count++;
    }

    spdlog::info("loading map");
    write_map_to_buffers(scene.map, renderer, scene.camera.aspect, scene.palette);

    spdlog::info("setting up renderer to draw");


    for(int i = 0; i < LINE_INDEX; ++i) {
        renderer.vertices[i] *= size;
    }

    glm::mat4 projection = glm::ortho(-scene.camera.aspect, scene.camera.aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));


    enable(renderer, { GL_DEPTH_TEST, GL_PROGRAM_POINT_SIZE });

    renderer.shaders = std::move(scene.shaders);
    spdlog::info("setting shader uniforms");
    for(const auto& [name, id] : renderer.shaders) {
        glUseProgram(id);
        set_shader_uniform(id, "projection", projection);
        set_shader_uniform(id, "view", view);
        set_shader_uniform(id, "size", 3.0f);
    }
    
    write_camera_fov_to_buffers(renderer, scene.camera);
    raycast_fov(renderer, scene.camera.direction);

    spdlog::info("starting message loop");

    renderer.view_count = scene.width;
    setup(renderer);
    run_message_loop(window.value(), renderer, scene.background_color);

    return 0;
}

