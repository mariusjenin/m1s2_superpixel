#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include "color.h"
#include "pixel.h"

#include "image.h"

void slic(OCTET *img_in, OCTET *img_out, int w, int h, int n_sp_px, int compactness) {
    //Create all pixels
    std::cout << "Create all pixels" << std::endl;
    std::vector<Pixel *> pixels_img_in;
    pixels_img_in.resize(w * h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            auto *coord = new Coord(i, j);
            auto *color = new Color1(img_in[i * w + j]);
            pixels_img_in[i * w + j] = new Pixel(coord, color);
        }
    }
    //Add neighbors to all pixels
    std::cout << "Add neighbors to all pixels" << std::endl;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i + 1 < h) {
                if (j + 1 < w) {
                    pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[(i + 1) * w + j + 1]);
                }
                if (j - 1 >= 0) {
                    pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[(i + 1) * w + j - 1]);
                }
            }
            if (i - 1 >= 0) {
                if (j + 1 < w) {
                    pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[(i - 1) * w + j + 1]);
                }
                if (j - 1 >= 0) {
                    pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[(i - 1) * w + j - 1]);
                }
            }
        }
    }

    //Create centroid of super-pixels
    std::cout << "Create centroid of super-pixels" << std::endl;
    auto side_sp_px = (float) sqrt(w * h / n_sp_px);
    int nb_centroid_w = (int) floor((float) w / side_sp_px);
    int nb_centroid_h = (int) floor((float) h / side_sp_px);
    n_sp_px = nb_centroid_h * nb_centroid_w;
    std::vector<SuperPixel *> sp_pxs = {};
    sp_pxs.resize(n_sp_px);
    side_sp_px = std::min(w / nb_centroid_w, h / nb_centroid_h);
    int side_sp_px_2 = (int) round((float) side_sp_px / 2.f);
    for (int i = 0; i < nb_centroid_h; i++) {
        for (int j = 0; j < nb_centroid_w; j++) {
            int x = (int) round((float) side_sp_px_2 + (float) i * side_sp_px);
            int y = (int) round((float) side_sp_px_2 + (float) j * side_sp_px);
            int ind = x * h + y;
            sp_pxs[i * nb_centroid_w + j] = new SuperPixel(pixels_img_in[ind]);
        }
    }


    //Compute super-pixels
    std::cout << "Compute super-pixels" << std::endl;
    int px_remaining = w * h - nb_centroid_w * nb_centroid_h;
    while (px_remaining > 0) {
        std::cout << px_remaining << std::endl;
        size_t size_sp_pxs = sp_pxs.size();
        for (int i = 0; i < size_sp_pxs; i++) {
            bool found_and_added = sp_pxs.at(i)->find_neighbor_to_add(compactness, side_sp_px);
            if (found_and_added) px_remaining--;
        }
    }


    std::cout << "Draw the img out" << std::endl;
    size_t size_sp_pxs = sp_pxs.size();
    int count = 0;
    for (int i = 0; i < size_sp_pxs; i++) {
        std::vector<Pixel*> cluster = sp_pxs.at(i)->get_cluster();
        size_t size_cluster = cluster.size();
        for(int j = 0; j < size_cluster;j++){
            Coord coord = *cluster.at(j)->get_coord();
//            Color1 color = *((Color1 *) cluster.at(i)->get_color());
            img_out[coord.get_x() * w + coord.get_y()] = 255;
            std::cout << coord.get_x() * w + coord.get_y() << std::endl;
            count++;
        }
    }

    std::cout << count << std::endl;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            delete pixels_img_in[i * w + j];
        }
    }
    for (int i = 0; i < nb_centroid_h; i++) {
        for (int j = 0; j < nb_centroid_w; j++) {
            delete sp_pxs[i * nb_centroid_w + j];
        }
    }
}

int main(int argc, char const *argv[]) {
    char name_img_read[250], name_img_written[250];
    int n_sp_px, compactness;
    if (argc > 5 || argc < 4) {
        printf("Usage: img_in.pgm img_out.pgm nb_super_pixel compactness\n");
        return 1;
    }
    sscanf(argv[1], "%s", name_img_read);
    sscanf(argv[2], "%s", name_img_written);
    sscanf(argv[3], "%d", &n_sp_px);
    if (argc == 5) {
        sscanf(argv[4], "%d", &compactness);
    } else {
        compactness = 10;
    }

    int h, w, nb_px;

    OCTET *img_in, *img_out;

    get_line_col_pgm(name_img_read, &h, &w);
    nb_px = h * w;

    alloc_array(img_in, OCTET, nb_px);
    read_pgm(name_img_read, img_in, nb_px);
    alloc_array(img_out, OCTET, nb_px);

    slic(img_in, img_out, w, h, n_sp_px, compactness);

    write_pgm(name_img_written, img_out, h, w);

    free(img_in);
    free(img_out);
    exit(0);
}