#include <yart/core/device.h>
#include <yart/core/scene.h>
#include <yart/geometry/sphere.h>
#include <yart/material/lambertian.h>
#include <yart/material/metal.h>
#include <yart/material/dielectric.h>
#include <yart/texture/constant.h>
#include <yart/texture/checkboard.h>
#include <yart/texture/image.h>
#include <yart/camera/perspective.h>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <common.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

constexpr const unsigned g_width = 1200;
constexpr const unsigned g_height = 800;
constexpr const unsigned g_samples = 100;
constexpr const unsigned g_depth = 50;

class Image
{
public:
    Image(const std::string& filename)
    {
        pixels = stbi_load(filename.c_str(), &width, &height, nullptr, 0);
    }
    ~Image() { stbi_image_free(pixels); }

public:
    unsigned char* pixels;
    int width;
    int height;
};

// ray tracing in one week scene
void gen_scene_rtiow(std::vector<std::unique_ptr<yart::Geometry>>& geometries,
                     std::vector<std::unique_ptr<yart::Material>>& materials,
                     std::vector<std::unique_ptr<yart::Texture>>& textures,
                     const yart::Device& device)
{
    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(0.0f, 1.0f);

    geometries.push_back(std::make_unique<yart::Sphere>(
        device, 1000.0f, Eigen::Vector3f(0.0f, -1000.0f, 0.0f)));
    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.5f, 0.5f, 0.5f));
    materials.push_back(
        std::make_unique<yart::Lambertian>(textures.back().get()));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float rd_mat = rd_number(rd_gen);
            Eigen::Vector3f center(a + 0.9f * rd_number(rd_gen),
                                   0.2f,
                                   b + 0.9f * rd_number(rd_gen));
            if ((center - Eigen::Vector3f(4.0f, 2.0f, 0.0f)).norm() > 0.9f) {
                geometries.push_back(
                    std::make_unique<yart::Sphere>(device, 0.2f, center));
                if (rd_mat < 0.8f) { // diffuse
                    textures.push_back(std::make_unique<yart::ConstantTexture>(
                        rd_number(rd_gen) * rd_number(rd_gen),
                        rd_number(rd_gen) * rd_number(rd_gen),
                        rd_number(rd_gen) * rd_number(rd_gen)));
                    materials.push_back(std::make_unique<yart::Lambertian>(
                        textures.back().get()));
                }
                else if (rd_mat < 0.95f) { // metal
                    materials.push_back(std::make_unique<yart::Metal>(
                        0.5 * (1.0f + rd_number(rd_gen)),
                        0.5 * (1.0f + rd_number(rd_gen)),
                        0.5 * (1.0f + rd_number(rd_gen)),
                        0.5 * rd_number(rd_gen)));
                }
                else { // glass
                    materials.push_back(
                        std::make_unique<yart::Dielectric>(1.5f));
                }
            }
        }
    }

    geometries.push_back(std::make_unique<yart::Sphere>(
        device, 1.0f, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
    materials.push_back(std::make_unique<yart::Dielectric>(1.5f));

    geometries.push_back(std::make_unique<yart::Sphere>(
        device, 1.0f, Eigen::Vector3f(-4.0f, 1.0f, 0.0f)));
    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.4f, 0.2f, 0.1f));
    materials.push_back(
        std::make_unique<yart::Lambertian>(textures.back().get()));

    geometries.push_back(std::make_unique<yart::Sphere>(
        device, 1.0f, Eigen::Vector3f(4.0f, 1.0f, 0.0f)));
    materials.push_back(std::make_unique<yart::Metal>(0.7f, 0.6f, 0.5f, 0.0f));
}

// ray tracing the next week scene
void gen_scene_rttnw(std::vector<std::unique_ptr<yart::Geometry>>& geometries,
                     std::vector<std::unique_ptr<yart::Material>>& materials,
                     std::vector<std::unique_ptr<yart::Texture>>& textures,
                     std::vector<Image>& images,
                     const yart::Device& device)
{
    geometries.push_back(std::make_unique<yart::Sphere>(
        device, 1000.0f, Eigen::Vector3f(0.0f, -1000.0f, 0.0f)));
    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.2f, 0.3f, 0.1f));
    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.9f, 0.9f, 0.9f));
    materials.push_back(
        std::make_unique<yart::Lambertian>(textures.back().get()));

    geometries.push_back(std::make_unique<yart::Sphere>(
        device, 1.0f, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
    std::string file(YART_EXAMPLE_IMG_DIR);
    file.append("earthmap1k.jpg");
    images.emplace_back(file);
    textures.push_back(std::make_unique<yart::ImageTexture>(
        images[0].pixels, images[0].width, images[0].height));
    materials.push_back(
        std::make_unique<yart::Lambertian>(textures.back().get()));
    materials.push_back(std::make_unique<yart::Dielectric>(1.5f));
}

int main(int argc, char* argv[])
{
    auto device = yart::Device();

    std::vector<std::unique_ptr<yart::Geometry>> geometries;
    std::vector<std::unique_ptr<yart::Material>> materials;
    std::vector<std::unique_ptr<yart::Texture>> textures;
    std::vector<Image> images;
    if (argc > 1 && std::string(argv[1]) == "ray_tracing_the_next_week") {
        gen_scene_rttnw(geometries, materials, textures, images, device);
    }
    else {
        gen_scene_rtiow(geometries, materials, textures, device);
    }

    auto scene = yart::Scene(device);
    for (size_t i = 0; i < geometries.size(); ++i) {
        scene.add(*geometries[i], *materials[i]);
    }

    yart::PerspectiveCamera camera(Eigen::Vector3f(13.0f, 2.0f, 3.0f),
                                   Eigen::Vector3f(0.0f, 0.0f, 0.0f),
                                   Eigen::Vector3f(0.0f, 1.0f, 0.0f),
                                   0.01f,
                                   20.0f,
                                   g_width,
                                   g_height);
    camera.zoom(10.0f);

    std::vector<unsigned char> pixels(g_width * g_height * 3);
    auto tstart = std::chrono::high_resolution_clock::now();
    auto num_rays = scene.render(
        camera, pixels.data(), g_width, g_height, g_samples, true, g_depth);
    auto tend = std::chrono::high_resolution_clock::now();
    auto tspent =
        std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart);
    auto mrays = (num_rays / 1000000) / (tspent.count() / 1000);
    std::cout << "Time used(ms): " << tspent.count() << std::endl;
    std::cout << "Mrays/sec: " << mrays << std::endl;

    std::string out_file(YART_EXAMPLE_OUTPUT_DIR);
    out_file.append("out.png");
    stbi_write_png(out_file.c_str(),
                   g_width,
                   g_height,
                   3,
                   pixels.data(),
                   3 * g_width * sizeof(unsigned char));

    return 0;
}
