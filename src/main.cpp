#include <ostream>

#include "../headers/color.hpp"
#include "../headers/logger.hpp"
#include "../headers/scene.hpp"

int main() {
    const int CANVAS_WIDTH = 512;
    const int CANVAS_HEIGHT = 512;
    const Color BACKGROUND_COLOR(255, 255, 255);
    char* fp = std::getenv("RAYCASTER_IMAGE_FILEPATH");
    const std::string FILEPATH = (fp == nullptr) ? std::string("") : std::string(fp);

    Logger::initialize();
    
    if (FILEPATH.empty()) {
        Logger::error("RAYCASTER_IMAGE_FILEPATH not set!");
        return EXIT_FAILURE;
    }

    Scene scene();
    Logger::info("Starting Render");

    std::ofstream image(FILEPATH);
    if (image.is_open()) {
        image << "P3\n" << CANVAS_WIDTH << ' ' << CANVAS_HEIGHT << "\n255\n";
        for (int y = -CANVAS_HEIGHT / 2; y < CANVAS_HEIGHT / 2; y++) {
            for (int x = -CANVAS_WIDTH / 2; x < CANVAS_WIDTH / 2; x++) {
                Color color = BACKGROUND_COLOR;
                image << color.r << ' ' << color.g << ' ' << color.b << std::endl;
            }
        }
    }
    image.close();
    Logger::info("Render Finished Successfully");

    return EXIT_SUCCESS;
}