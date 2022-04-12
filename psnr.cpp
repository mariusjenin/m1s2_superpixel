#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include "image.h"

int main(int argc, char const *argv[]) {
    char name_img_base[250], name_img_read[250];
    bool is_rgb;
    if (argc != 3) {
        printf("Usage: img_base.<pgm/ppm> img_in.<pgm/ppm>\n");
        return 1;
    }
    sscanf(argv[1], "%s", name_img_base);
    sscanf(argv[2], "%s", name_img_read);

    std::string name_img_base_str = name_img_base;
    std::string name_img_read_str = name_img_read;
    std::string type_img_base = name_img_base_str.substr(name_img_base_str.length() - 3);
    std::string type_img_read = name_img_read_str.substr(name_img_read_str.length() - 3);
    //Check if same extension
    if (type_img_base != type_img_read) {
        printf("%s", ("The images are not of the same type: '" + type_img_base + "' and '" + type_img_read + "'\n").c_str());
        return 1;
    }

    //Check if pgm or ppm
    if (type_img_base != "pgm" && type_img_base != "ppm") {
        printf("%s", "The images are not of a supported type : must be PGM or PPM\n");
        return 1;
    }
    is_rgb = type_img_read == "ppm";

    int h, w, nb_px;

    OCTET *img_base, *img_read;

    int size;
    if(is_rgb){
        get_line_col_ppm(name_img_base, &h, &w);
        nb_px = h * w;
        size = nb_px * 3;
        alloc_array(img_base, OCTET, size)
        read_ppm(name_img_base, img_base, nb_px);
        alloc_array(img_read, OCTET, size)
        read_ppm(name_img_read, img_read, nb_px);
    } else {
        get_line_col_pgm(name_img_base, &h, &w);
        nb_px = h * w;
        size = nb_px;
        alloc_array(img_base, OCTET, size)
        read_pgm(name_img_base, img_base, nb_px);
        alloc_array(img_read, OCTET, size)
        read_pgm(name_img_read, img_read, nb_px);
    }
    std::cout << psnr(img_base,img_read,size) ;

    free(img_base);
    free(img_read);
    exit(0);
}