#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "color.h"
#include "pixel.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include "image.h"

#define SP_SQUARE_SHAPE 0
#define SP_HEXAGON_SHAPE 1

void slic(OCTET *img_in, OCTET *img_out, bool is_rgb, int w, int h, int shape, int n_sp_px, int compactness) {
    int cols_terminal;
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    cols_terminal = ts.ts_cols;
#elif defined(TIOCGWINSZ)
    struct winsize ts{};
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    cols_terminal = ts.ws_col;
#endif /* TIOCGSIZE */

    int total_pixels = w * h;

    //Create all pixels
    std::vector<Pixel *> pixels_img_in;
    pixels_img_in.resize(total_pixels);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            auto *coord = new Coord(i, j);
            Color *color;
            int indice = i * w + j;
            if (is_rgb) {
                int indice_image = 3 * (indice);
                color = new Color3(img_in[indice_image], img_in[indice_image + 1], img_in[indice_image + 2]);
//                ((Color3*)color)->rgb_to_xyz();
//                ((Color3*)color)->xyz_to_cielab();
            } else {
                color = new Color1(img_in[indice]);
            }
            pixels_img_in[indice] = new Pixel(coord, color);;
        }
    }
    //Add neighbors to all pixels
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (j - 1 >= 0)
                pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[i * w + j - 1]);
            if (i - 1 >= 0)
                pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[(i - 1) * w + j]);
            if (j + 1 < w)
                pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[i * w + j + 1]);
            if (i + 1 < h)
                pixels_img_in[i * w + j]->add_neighbor(pixels_img_in[(i + 1) * w + j]);
        }
    }

    //Create centroid of super-pixels
    auto side_sp_px = (float) sqrt(total_pixels / n_sp_px);
    int nb_centroid_w = (int) floor((float) w / side_sp_px);
    int nb_centroid_h = (int) floor((float) h / side_sp_px);
    if(shape == SP_SQUARE_SHAPE){
        n_sp_px = nb_centroid_h * nb_centroid_w;
    } else if(shape == SP_HEXAGON_SHAPE){
        n_sp_px = nb_centroid_h * nb_centroid_w + nb_centroid_h/2;
    }
    std::vector<SuperPixel *> sp_pxs = {};
    sp_pxs.resize(n_sp_px);
    side_sp_px = std::min((float)( w-1) / (float) nb_centroid_w, (float)( h-1) / (float) nb_centroid_h);
    int side_sp_px_2 = (int) floor((float) side_sp_px / 2.f);
    for (int i = 0; i < nb_centroid_h; i++) {
        int bound_w = 0;
        if(shape == SP_SQUARE_SHAPE){
            bound_w = nb_centroid_w;
        } else if(shape == SP_HEXAGON_SHAPE){
            bound_w = nb_centroid_w+(i%2==0?0:1);
        }
        for (int j = 0; j < bound_w; j++) {
            int y = 0;
            if(shape == SP_SQUARE_SHAPE){
                y = (int) round((float) side_sp_px_2 + (float) j * side_sp_px);
            } else if(shape == SP_HEXAGON_SHAPE){
                y = (int) round((i%2==0?(float) side_sp_px_2:0)+ (float) j * side_sp_px);
            }
            int x = (int) round((float) side_sp_px_2 + (float) i * side_sp_px);
            int ind = x * w + y;
            int ind_sp = 0;
            if(shape == SP_SQUARE_SHAPE){
                ind_sp =i * nb_centroid_w + j;
            } else if(shape == SP_HEXAGON_SHAPE){
                ind_sp =(int) ceil((float)i/2.f) * nb_centroid_w +(int)floor((float)i/2.f)* (nb_centroid_w+1) + j;
            }
            sp_pxs[ind_sp] = new SuperPixel(pixels_img_in[ind]);
        }
    }

    //Compute super-pixels
    int px_remaining = total_pixels - n_sp_px;
    int percent_done;
    int last_percent_done = 0;
    std::string message_loading_before = "SLIC : (";
    std::string message_loading_after = ")";
    int char_space_loading = cols_terminal - (int) message_loading_before.size() - (int) message_loading_after.size();
    while (true) {
        percent_done = (int) round((float) (((float) total_pixels - (float) px_remaining) / (float) total_pixels) *
                                   (float) char_space_loading);

        for (int i = 0; i < n_sp_px && px_remaining > 0; i++) {
            bool found_and_added = sp_pxs.at(i)->find_neighbor_to_add(compactness, side_sp_px);
            if (found_and_added) px_remaining--;
        }
        if (percent_done != last_percent_done || px_remaining == 0) {
            last_percent_done = percent_done;
            std::string loading;
            int nb_empty = char_space_loading - percent_done;
            for (int i = 0; i < percent_done; i++) {
                loading += "|";
            }
            for (int i = 0; i < nb_empty; i++) {
                loading += "-";
            }
            std::cout << std::endl;
            printf("\033c"); //clear the terminal
            printf("%s%s%s", message_loading_before.c_str(), loading.c_str(), message_loading_after.c_str());
        }
        if (px_remaining <= 0) break;
    }
    printf("\033c"); //clear the terminal


//    Draw the image
    int count = 0;
    for (int i = 0; i < n_sp_px; i++) {
        std::vector<Pixel *> cluster = sp_pxs.at(i)->get_cluster();
        size_t size_cluster = cluster.size();
        Coord coord = sp_pxs.at(i)->get_coord_start();
        Color *color;
        int indice;
        if(is_rgb){
            indice = 3*(coord.get_x() * w + coord.get_y());
            color =(Color3 *) (sp_pxs.at(i)->get_color());
//            ((Color3*)color)->cielab_to_xyz();
//            ((Color3*)color)->xyz_to_rgb();
            img_out[indice] = ((Color3 *)color)->get_v1();
            img_out[indice+1] = ((Color3 *)color)->get_v2();
            img_out[indice+2] = ((Color3 *)color)->get_v3();
        } else {
            indice = coord.get_x() * w + coord.get_y();
            color =(Color1 *) (sp_pxs.at(i)->get_color());;
            img_out[indice] = ((Color1 *)color)->get_v();
        }
        for (int j = 0; j < size_cluster; j++) {
            Coord coord_child = *(cluster.at(j))->get_coord();
            if(is_rgb){
                indice = 3*(coord_child.get_x() * w + coord_child.get_y());
                img_out[indice] = ((Color3 *)color)->get_v1();
                img_out[indice+1] = ((Color3 *)color)->get_v2();
                img_out[indice+2] = ((Color3 *)color)->get_v3();
            } else {
                indice = coord_child.get_x() * w + coord_child.get_y();
                img_out[indice] = ((Color1 *)color)->get_v();
            }
            count++;
        }
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            delete pixels_img_in[i * w + j];
        }
    }
    for (int i = 0; i < n_sp_px; i++) {
            delete sp_pxs[i];
    }
}

int main(int argc, char const *argv[]) {
    char name_img_read[250], name_img_written[250];
    int shape, n_sp_px, compactness;
    bool is_rgb;
    if (argc != 6) {
        printf("Usage: img_in.<pgm/ppm> img_out.<pgm/ppm> shape nb_super_pixel compactness\n");
        return 1;
    }
    sscanf(argv[1], "%s", name_img_read);
    sscanf(argv[2], "%s", name_img_written);
    sscanf(argv[3], "%d", &shape);
    sscanf(argv[4], "%d", &n_sp_px);
    sscanf(argv[5], "%d", &compactness);

    std::string name_img_read_str = name_img_read;
    std::string name_img_written_str = name_img_written;
    std::string type_img_read = name_img_read_str.substr(name_img_read_str.length() - 3);
    std::string type_img_written = name_img_written_str.substr(name_img_written_str.length() - 3);
    //Check if same extension
    if (type_img_read != type_img_written) {
        printf("%s", ("The images are not of the same type: '" + type_img_read + "' and '" + type_img_written + "'\n").c_str());
        return 1;
    }

    //Check if pgm or ppm
    if (type_img_read != "pgm" && type_img_read != "ppm") {
        printf("%s", "The images are not of a supported type : must be PGM or PPM\n");
        return 1;
    }
    is_rgb = type_img_read == "ppm";

    int h, w, nb_px;

    OCTET *img_in, *img_out;

    if(is_rgb){
        get_line_col_ppm(name_img_read, &h, &w);
        nb_px = h * w;
        alloc_array(img_in, OCTET, nb_px*3)
        read_ppm(name_img_read, img_in, nb_px);
        alloc_array(img_out, OCTET, nb_px*3)
    } else {
        get_line_col_pgm(name_img_read, &h, &w);
        nb_px = h * w;
        alloc_array(img_in, OCTET, nb_px)
        read_pgm(name_img_read, img_in, nb_px);
        alloc_array(img_out, OCTET, nb_px)
    }

    //Check the number of superpixel
    if (n_sp_px>nb_px && n_sp_px<1) {
        printf("%s", "Number of superpixels can't be over the number of pixels or less than 1\n");
        return 1;
    }
    slic(img_in, img_out, is_rgb, w, h, shape, n_sp_px, compactness);
    if(is_rgb){
        write_ppm(name_img_written, img_out, h, w);
    } else {
        write_pgm(name_img_written, img_out, h, w);
    }


    std::cout << "SLIC Done on "+name_img_read_str+" with \n"<<nb_px<<" pixels\n"<<n_sp_px<<" super-pixels\n"<<compactness<< " of compactness" <<std::endl;

    free(img_in);
    free(img_out);
    exit(0);
}