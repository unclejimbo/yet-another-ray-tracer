#define _USE_MATH_DEFINES
#include <yart/camera/perspective.h>
#include <yart/core/device.h>
#include <yart/core/scene.h>
#include <yart/geometry/box.h>
#include <yart/geometry/instance.h>
#include <yart/geometry/plane.h>
#include <yart/geometry/sphere.h>
#include <yart/material/dielectric.h>
#include <yart/material/diffuse_light.h>
#include <yart/material/lambertian.h>
#include <yart/material/metal.h>
#include <yart/renderer/pathtracer.h>
#include <yart/texture/constant.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <common.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

constexpr const unsigned g_width = 400;
constexpr const unsigned g_height = 400;
constexpr const unsigned g_samples = 100;
constexpr const unsigned g_depth = 50;

yart::Device g_device;
std::unique_ptr<yart::Camera> g_camera;
std::vector<std::unique_ptr<yart::Geometry>> g_geometries;
std::vector<std::unique_ptr<yart::Material>> g_materials;
std::vector<std::unique_ptr<yart::Texture>> g_textures;
std::vector<std::unique_ptr<yart::Scene>> g_scenes;

// ray tracing in one weekend
void gen_scene_rtiow()
{
    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(0.0f, 1.0f);

    g_scenes[0]->set_background(Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    g_camera = std::make_unique<yart::PerspectiveCamera>(
        Eigen::Vector3f(13.0f, 2.0f, 3.0f),
        Eigen::Vector3f(0.0f, 0.0f, 0.0f),
        Eigen::Vector3f(0.0f, 1.0f, 0.0f),
        0.01f,
        20.0f,
        g_width,
        g_height);
    g_camera->zoom(10.0f);
    g_scenes[0]->set_camera(*g_camera);

    g_geometries.push_back(std::make_unique<yart::Sphere>(
        g_device, 1000.0f, Eigen::Vector3f(0.0f, -1000.0f, 0.0f)));
    g_textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.5f, 0.5f, 0.5f));
    g_materials.push_back(
        std::make_unique<yart::Lambertian>(g_textures[0].get()));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float rd_mat = rd_number(rd_gen);
            Eigen::Vector3f center(a + 0.9f * rd_number(rd_gen),
                                   0.2f,
                                   b + 0.9f * rd_number(rd_gen));
            if ((center - Eigen::Vector3f(4.0f, 2.0f, 0.0f)).norm() > 0.9f) {
                g_geometries.push_back(
                    std::make_unique<yart::Sphere>(g_device, 0.2f, center));
                if (rd_mat < 0.8f) { // diffuse
                    g_textures.push_back(
                        std::make_unique<yart::ConstantTexture>(
                            rd_number(rd_gen) * rd_number(rd_gen),
                            rd_number(rd_gen) * rd_number(rd_gen),
                            rd_number(rd_gen) * rd_number(rd_gen)));
                    g_materials.push_back(std::make_unique<yart::Lambertian>(
                        g_textures.back().get()));
                }
                else if (rd_mat < 0.95f) { // metal
                    g_materials.push_back(std::make_unique<yart::Metal>(
                        0.5 * (1.0f + rd_number(rd_gen)),
                        0.5 * (1.0f + rd_number(rd_gen)),
                        0.5 * (1.0f + rd_number(rd_gen)),
                        0.5 * rd_number(rd_gen)));
                }
                else { // glass
                    g_materials.push_back(
                        std::make_unique<yart::Dielectric>(1.5f));
                }
            }
        }
    }

    g_geometries.push_back(std::make_unique<yart::Sphere>(
        g_device, 1.0f, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
    g_materials.push_back(std::make_unique<yart::Dielectric>(1.5f));

    g_geometries.push_back(std::make_unique<yart::Sphere>(
        g_device, 1.0f, Eigen::Vector3f(-4.0f, 1.0f, 0.0f)));
    g_textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.4f, 0.2f, 0.1f));
    g_materials.push_back(
        std::make_unique<yart::Lambertian>(g_textures.back().get()));

    g_geometries.push_back(std::make_unique<yart::Sphere>(
        g_device, 1.0f, Eigen::Vector3f(4.0f, 1.0f, 0.0f)));
    g_materials.push_back(
        std::make_unique<yart::Metal>(0.7f, 0.6f, 0.5f, 0.0f));

    for (size_t i = 0; i < g_geometries.size(); ++i) {
        g_scenes[0]->add(*g_geometries[i], *g_materials[i]);
    }
}

void gen_scene_cornell_box()
{
    g_scenes.push_back(std::make_unique<yart::Scene>(g_device)); // box

    g_camera = std::make_unique<yart::PerspectiveCamera>(
        Eigen::Vector3f(278, 278, 800),
        Eigen::Vector3f(278, 278, 0),
        Eigen::Vector3f(0.0f, 1.0f, 0.0f),
        0.0f,
        40.0f,
        g_width,
        g_height);
    g_camera->zoom(10.0f);
    g_scenes[0]->set_camera(*g_camera);

    g_geometries.push_back(
        std::make_unique<yart::Plane>(g_device,
                                      Eigen::Vector3f(0, 0, -555),
                                      Eigen::Vector3f(555, 0, 0),
                                      Eigen::Vector3f(0, 555, 0))); // back
    g_geometries.push_back(
        std::make_unique<yart::Plane>(g_device,
                                      Eigen::Vector3f(0, 0, 0),
                                      Eigen::Vector3f(555, 0, 0),
                                      Eigen::Vector3f(0, 0, -555))); // bottom
    g_geometries.push_back(
        std::make_unique<yart::Plane>(g_device,
                                      Eigen::Vector3f(0, 555, -555),
                                      Eigen::Vector3f(555, 0, 0),
                                      Eigen::Vector3f(0, 0, 555))); // top
    g_geometries.push_back(
        std::make_unique<yart::Plane>(g_device,
                                      Eigen::Vector3f(0, 0, 0),
                                      Eigen::Vector3f(0, 0, -555),
                                      Eigen::Vector3f(0, 555, 0))); // left
    g_geometries.push_back(
        std::make_unique<yart::Plane>(g_device,
                                      Eigen::Vector3f(555, 0, -555),
                                      Eigen::Vector3f(0, 0, 555),
                                      Eigen::Vector3f(0, 555, 0))); // right
    g_geometries.push_back(
        std::make_unique<yart::Plane>(g_device,
                                      Eigen::Vector3f(213, 554, -337),
                                      Eigen::Vector3f(130, 0, 0),
                                      Eigen::Vector3f(0, 0, 110))); // light

    g_textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.73f, 0.73f, 0.73f));
    g_textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.12f, 0.45f, 0.15f));
    g_textures.push_back(
        std::make_unique<yart::ConstantTexture>(0.65f, 0.05f, 0.05f));
    g_textures.push_back(std::make_unique<yart::ConstantTexture>(15, 15, 15));

    g_materials.push_back(
        std::make_unique<yart::Lambertian>(g_textures[0].get()));
    g_materials.push_back(
        std::make_unique<yart::Lambertian>(g_textures[1].get()));
    g_materials.push_back(
        std::make_unique<yart::Lambertian>(g_textures[2].get()));
    g_materials.push_back(
        std::make_unique<yart::DiffuseLight>(g_textures[3].get()));
    g_materials.push_back(
        std::make_unique<yart::Metal>(Eigen::Array3f::Ones(), 0.0f));

    // walls and light
    g_scenes[0]->add(*g_geometries[0], *g_materials[0]);
    g_scenes[0]->add(*g_geometries[1], *g_materials[0]);
    g_scenes[0]->add(*g_geometries[2], *g_materials[0]);
    g_scenes[0]->add(*g_geometries[3], *g_materials[1]);
    g_scenes[0]->add(*g_geometries[4], *g_materials[2]);
    g_scenes[0]->add(*g_geometries[5], *g_materials[3]);

    // boxes
    g_geometries.push_back(
        std::make_unique<yart::Box>(g_device,
                                    Eigen::Vector3f(-0.5, -0.5, -0.5),
                                    Eigen::Vector3f(0.5, 0.5, 0.5)));
    g_scenes[1]->add(*g_geometries[6], *g_materials[4]);
    g_scenes[1]->commit();
    auto binstance_0 = std::make_unique<yart::Instance>(g_device, *g_scenes[1]);
    binstance_0->scale(Eigen::Vector3f(160, 160, 160));
    binstance_0->rotate(-0.1f * M_PI, Eigen::Vector3f::UnitY());
    binstance_0->translate(Eigen::Vector3f(370, 80, -145));
    auto binstance_1 = std::make_unique<yart::Instance>(g_device, *g_scenes[1]);
    binstance_1->scale(Eigen::Vector3f(160, 320, 160));
    binstance_1->rotate(0.08f * M_PI, Eigen::Vector3f::UnitY());
    binstance_1->translate(Eigen::Vector3f(180, 160, -375));
    g_geometries.push_back(std::move(binstance_0));
    g_geometries.push_back(std::move(binstance_1));
    g_scenes[0]->add(*g_geometries[7], *g_materials[0]);
    g_scenes[0]->add(*g_geometries[8], *g_materials[0]);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "examples <n>" << std::endl
                  << "n=0, ray tracing in one weekend scene" << std::endl
                  << "n=1, cornnel box scene" << std::endl;
        return -1;
    }

    g_scenes.push_back(std::make_unique<yart::Scene>(g_device));
    std::string filename;
    if (strcmp(argv[1], "0") == 0) {
        gen_scene_rtiow();
        filename = "rtiow.png";
    }
    else if (strcmp(argv[1], "1") == 0) {
        gen_scene_cornell_box();
        filename = "cornell.png";
    }
    else {
        std::cout << "unrecognised scene number" << std::endl
                  << "examples <n>" << std::endl
                  << "n=0, ray tracing in one weekend scene" << std::endl
                  << "n=1, cornnel box scene" << std::endl;
        return -1;
    }
    g_scenes[0]->commit();

    std::vector<unsigned char> pixels(g_width * g_height * 3);
    yart::RenderData data;
    data.pixels = pixels.data();
    data.width = g_width;
    data.height = g_height;
    data.interleaved = true;

    auto tstart = std::chrono::high_resolution_clock::now();

    yart::PathTracer renderer(g_samples, g_depth);
    auto num_rays = renderer.render_tiled(*g_scenes[0], data);

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
