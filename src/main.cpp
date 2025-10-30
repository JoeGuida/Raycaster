#include "main.hpp"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
    }

    std::filesystem::path scene_path = std::filesystem::current_path() / "scenes";
    if(scene_path.empty()) {
        spdlog::error("could not resolve project paths");
        return -1;
    }

    spdlog::info("initializing window");
    Input input { .initialized = false };
    auto window = initialize_window(instance, show_window_flags, 
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 
                                    L"Window Class", L"Raycaster", input);

    if(!window.has_value()) {
        spdlog::error("{}", window.error());
        return -1;
    }

    spdlog::info("loading scene");
    Scene scene = load_scene_from_file((scene_path / "scene.yml").string());

    spdlog::info("initializing renderer");
    Renderer renderer;
    initialize(renderer);
    enable(renderer, { GL_DEPTH_TEST, GL_PROGRAM_POINT_SIZE });
    renderer.shaders = std::move(scene.shaders);

    // scale rect size by map width & height
    glm::vec2 size(scene.camera.aspect / scene.map.width / 2.0f,
                   2.0f / scene.map.height / 2.0f);
    for(int i = 0; i < LINE_INDEX; ++i) {
        renderer.vertices[i] *= size;
    }
    
    // put all points into renderer buffere
    for(const auto& i : scene.points) {
        renderer.positions[POINT_OFFSET + renderer.point_count] = to_vec4(i.position);
        renderer.colors[POINT_OFFSET + renderer.point_count] = to_vec4(i.color);
        renderer.point_count++;
    }

    spdlog::info("loading map");
    write_map_to_buffers(scene.map, renderer, scene.camera.aspect, scene.palette);

    // now that the renderer vertices have been adjusted and the map has been written to buffersthe
    // the rect lines can be precomputed
    renderer.rect_lines.reserve(renderer.rect_count * 4);
    precompute_lines(renderer);

    spdlog::info("setting up renderer to draw");
    glm::mat4 projection = glm::ortho(-scene.camera.aspect, scene.camera.aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    spdlog::info("setting shader uniforms");
    for(const auto& [name, id] : renderer.shaders) {
        glUseProgram(id);
        set_shader_uniform(id, "projection", projection);
        set_shader_uniform(id, "view", view);
        set_shader_uniform(id, "size", 3.0f);
    }
    
    write_camera_fov_to_buffers(renderer, scene.camera);
    raycast_fov(renderer, scene.camera);

    spdlog::info("starting message loop");
    renderer.view_count = scene.width;
    setup(renderer);
    run_message_loop(window.value(), renderer, scene.background_color);

    return 0;
}

