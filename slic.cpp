#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include "color.h"

#include "image.h"

void slic(OCTET *img_in, OCTET *img_out, int w, int h, int n_sp_px, int compactness){

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

    slic(img_in,img_out,w,h,n_sp_px,compactness);

    write_ppm(name_img_written, img_out, h, w);

    free(img_in);
    free(img_out);
    exit(0);
}