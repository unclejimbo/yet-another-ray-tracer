#include <yart/camera/perspective.h>
#include <yart/core/device.h>
#include <yart/core/scene.h>
#include <yart/geometry/plane.h>
#include <yart/geometry/sphere.h>
#include <yart/material/dielectric.h>
#include <yart/material/diffuse_light.h>
#include <yart/material/lambertian.h>
#include <yart/material/metal.h>
#include <yart/texture/constant.h>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <common.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

constexpr const unsigned g_width = 400;
constexpr const unsigned g_height = 400;
constexpr const unsigned g_samples = 100;
constexpr const unsigned g_depth = 50;

// ray tracing in one weekend
void gen_scene_rtiow(const yart::Device& device,
                     yart::Scene& scene,
                     std::unique_ptr<yart::Camera>& camera,
                     std::vector<std::unique_ptr<yart::Geometry>>& geometries,
                     std::vector<std::unique_ptr<yart::Material>>& materials,
                     std::vector<std::unique_ptr<yart::Texture>>& textures)
{
    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(0.0f, 1.0f);

    camera = std::make_unique<yart::PerspectiveCamera>(
        Eigen::Vector3f(13.0f, 2.0f, 3.0f),
        Eigen::Vector3f(0.0f, 0.0f, 0.0f),
        Eigen::Vector3f(0.0f, 1.0f, 0.0f),
        0.01f,
        20.0f,
        g_width,
        g_height);
    camera->zoom(10.0f);

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

    scene.set_background(Eigen::Vector3f(1.0f, 1.0f, 1.0f));
    for (size_t i = 0; i < geometries.size(); ++i) {
        scene.add(*geometries[i], *materials[i]);
    }
}

void gen_scene_cornell_box(
    const yart::Device& device,
    yart::Scene& scene,
    std::unique_ptr<yart::Camera>& camera,
    std::vector<std::unique_ptr<yart::Geometry>>& geometries,
    std::vector<std::unique_ptr<yart::Material>>& materials,
    std::vector<std::unique_ptr<yart::Texture>>& textures)
{
    camera = std::make_unique<yart::PerspectiveCamera>(
        Eigen::Vector3f(278, 278, 800),
        Eigen::Vector3f(278, 278, 0),
        Eigen::Vector3f(0.0f, 1.0f, 0.0f),
        0.01f,
        40.0f,
        g_width,
        g_height);
    camera->zoom(10.0f);

    geometries.push_back(
        std::make_unique<yart::Plane>(device,
                                      Eigen::Vector3f(0, 0, -555),
                                      Eigen::Vector3f(555, 0, 0),
                                      Eigen::Vector3f(0, 555, 0))); // back
    geometries.push_back(
        std::make_unique<yart::Plane>(device,
                                      Eigen::Vector3f(0, 0, 0),
                                      Eigen::Vector3f(555, 0, 0),
                                      Eigen::Vector3f(0, 0, -555))); // bottom
    geometries.push_back(
        std::make_unique<yart::Plane>(device,
                                      Eigen::Vector3f(0, 555, -555),
                                      Eigen::Vector3f(555, 0, 0),
                                      Eigen::Vector3f(0, 0, 555))); // top
    geometries.push_back(
        std::make_unique<yart::Plane>(device,
                                      Eigen::Vector3f(0, 0, 0),
                                      Eigen::Vector3f(0, 0, -555),
                                      Eigen::Vector3f(0, 555, 0))); // left
    geometries.push_back(
        std::make_unique<yart::Plane>(device,
                                      Eigen::Vector3f(555, 0, -555),
                                      Eigen::Vector3f(0, 0, 555),
                                      Eigen::Vector3f(0, 555, 0))); // right
    geometries.push_back(
        std::make_unique<yart::Plane>(device,
                                      Eigen::Vector3f(213, 554, -227),
                                      Eigen::Vector3f(130, 0, 0),
                                      Eigen::Vector3f(0, 0, 110))); // light

    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.73f, 0.73f, 0.73f));
    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.12f, 0.45f, 0.15f));
    textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.65f, 0.05f, 0.05f));
    textures.push_back(std::make_unique<yart::ConstantTexture>(15, 15, 15));

    materials.push_back(std::make_unique<yart::Lambertian>(textures[0].get()));
    materials.push_back(std::make_unique<yart::Lambertian>(textures[1].get()));
    materials.push_back(std::make_unique<yart::Lambertian>(textures[2].get()));
    materials.push_back(
        std::make_unique<yart::DiffuseLight>(textures[3].get()));

    scene.add(*geometries[0], *materials[0]);
    scene.add(*geometries[1], *materials[0]);
    scene.add(*geometries[2], *materials[0]);
    scene.add(*geometries[3], *materials[1]);
    scene.add(*geometries[4], *materials[2]);
    scene.add(*geometries.back(), *materials.back());
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "examples <n>" << std::endl
                  << "n=0, ray tracing in one weekend scene" << std::endl
                  << "n=1, cornnel box scene" << std::endl;
        return -1;
    }

    yart::Device device;
    yart::Scene scene(device);
    std::unique_ptr<yart::Camera> camera;
    std::vector<std::unique_ptr<yart::Geometry>> geometries;
    std::vector<std::unique_ptr<yart::Material>> materials;
    std::vector<std::unique_ptr<yart::Texture>> textures;
    std::string filename;
    if (strcmp(argv[1], "0") == 0) {
        gen_scene_rtiow(device, scene, camera, geometries, materials, textures);
        filename = "rtiow.png";
    }
    else if (strcmp(argv[1], "1") == 0) {
        gen_scene_cornell_box(
            device, scene, camera, geometries, materials, textures);
        filename = "cornell.png";
    }
    else {
        std::cout << "unrecognised scene number" << std::endl
                  << "examples <n>" << std::endl
                  << "n=0, ray tracing in one weekend scene" << std::endl
                  << "n=1, cornnel box scene" << std::endl;
        return -1;
    }

    std::vector<unsigned char> pixels(g_width * g_height * 3);
    auto tstart = std::chrono::high_resolution_clock::now();
    auto num_rays = scene.render(
        *camera, pixels.data(), g_width, g_height, g_samples, true, g_depth);
    auto tend = std::chrono::high_resolution_clock::now();
    auto tspent =
        std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart);
    auto mrays = (num_rays / 1000000) / (tspent.count() / 1000 + 1e-6f);
    std::cout << "Time used(ms): " << tspent.count() << std::endl;
    std::cout << "Mrays/sec: " << mrays << std::endl;

    std::string out_file(YART_EXAMPLE_OUTPUT_DIR);
    out_file.append(filename);
    stbi_write_png(out_file.c_str(),
                   g_width,
                   g_height,
                   3,
                   pixels.data(),
                   3 * g_width * sizeof(unsigned char));

    return 0;
}
