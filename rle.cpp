#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "image.h"

void read_size_rle(char name[], int *size) {
    FILE *f_fichier;
    int max_grey_val;

    /* cf : l'entete d'une image .pgm : P5                    */
    /*				       nb_colonnes nb_lignes */
    /*    			       max_grey_val          */


    if ((f_fichier = fopen(name, "rb")) == NULL) {
        printf("\nPas d'acces en lecture sur le fichier %s \n", name);
        exit(EXIT_FAILURE);
    } else {
        ignore_comments(f_fichier);
        fscanf(f_fichier, "%d", size);
        fclose(f_fichier);
    }
}

void write_rle(char name[], OCTET *data, int size) {
    FILE *f_fichier;

    if ((f_fichier = fopen(name, "wb")) == NULL) {
        printf("\nPas d'acces en ecriture sur le cichier %s \n", name);
        exit(EXIT_FAILURE);
    } else {
        fprintf(f_fichier, "%d\r", size);


        if ((fwrite((OCTET *) data, sizeof(OCTET), size, f_fichier))
            != (size_t) size) {
            printf("\nErreur de lecture du fichier %s \n", name);
            exit(EXIT_FAILURE);
        }
        fclose(f_fichier);
    }
}

void rle_compute_pgm(OCTET *img_in, std::vector<OCTET> &rle_vec_out, int w, int h) {
    int count = 1;
    OCTET val = (int) img_in[0];
    for (int i = 1; i < w * h; i++) {
        if (val == img_in[i] && count < 255) {
            count++;
        } else {
            rle_vec_out.push_back(count);
            rle_vec_out.push_back((int) val);
            count = 1;
            val = (int) img_in[i];
        }
    }
}

void rle_compute_ppm(OCTET *img_in, std::vector<OCTET> &rle_vec_out, int w, int h) {
    int count = 1;
    OCTET val_r = (int) img_in[0];
    OCTET val_g = (int) img_in[1];
    OCTET val_b = (int) img_in[2];
    for (int i = 3; i < w * h * 3; i += 3) {
        if (val_r == img_in[i] && val_g == img_in[i + 1] && val_b == img_in[i + 2] && count < 255) {
            count++;
        } else {
            rle_vec_out.push_back(count);
            rle_vec_out.push_back((int) val_r);
            rle_vec_out.push_back((int) val_g);
            rle_vec_out.push_back((int) val_b);
            count = 1;
            val_r = (int) img_in[i];
            val_g = (int) img_in[i + 1];
            val_b = (int) img_in[i + 2];
        }
    }
}

void rle_generate_pgm(std::vector<OCTET> rle_vec_out, OCTET *rle_out, int size_rle) {
    for (int i = 0; i < size_rle; i += 2) {
//        std::cout << (int) rle_vec_out[i] << " " << (int) rle_vec_out[i + 1] << " ";
        rle_out[i] = (int) rle_vec_out[i];
        rle_out[i + 1] = (int) rle_vec_out[i + 1];
    }
}

void rle_generate_ppm(std::vector<OCTET> rle_vec_out, OCTET *rle_out, int size_rle) {
    for (int i = 0; i < size_rle; i += 4) {
        rle_out[i] = (int) rle_vec_out[i];
        rle_out[i + 1] = (int) rle_vec_out[i + 1];
        rle_out[i + 2] = (int) rle_vec_out[i + 2];
        rle_out[i + 3] = (int) rle_vec_out[i + 3];
    }
}

void rle_inv_pgm(OCTET *rle_in, OCTET *img_out, int size_rle) {
    int count = 0;
    for (int i = 0; i < size_rle; i += 2) {
        for (int j = 0; j < rle_in[i]; j++) {
            img_out[count] = (int) rle_in[i + 1];
            count++;
        }
    }
}

void rle_inv_ppm(OCTET *rle_in, OCTET *img_out, int size_rle) {
    int count = 0;
    for (int i = 0; i < size_rle; i += 4) {
        for (int j = 0; j < rle_in[i]; j++) {
            img_out[count] = (int) rle_in[i + 1];
            img_out[count + 1] = (int) rle_in[i + 2];
            img_out[count + 2] = (int) rle_in[i + 3];
            count += 3;
        }
    }
}

int main(int argc, char const *argv[]) {
    char name_img_read[250], rle_name[250], name_img_written[250];
    int size_rle;
    if (argc < 3 || argc > 4) {
        printf("Usage: ImageIn.<pgm/ppm> rle.rle ImageOut.<pgm/ppm>\n");
        return 1;
    }
    sscanf(argv[1], "%s", name_img_read);
    sscanf(argv[2], "%s", rle_name);
    bool write_img_inv = argc > 3;
    std::string name_img_read_str = name_img_read;
    std::string type_img_read = name_img_read_str.substr(name_img_read_str.length() - 3);
    if(write_img_inv){
        sscanf(argv[3], "%s", name_img_written);
        std::string name_img_written_str = name_img_written;
        std::string type_img_written = name_img_written_str.substr(name_img_written_str.length() - 3);
        //Check if same extension
        if (write_img_inv && type_img_read != type_img_written) {
            printf("%s", ("The images are not of the same type: '" + type_img_read + "' and '" + type_img_written + "'\n").c_str());
            return 1;
        }
    }



    //Check if pgm or ppm
    if (type_img_read != "pgm" && type_img_read != "ppm") {
        printf("%s", "The images are not of a supported type : must be PGM or PPM\n");
        return 1;
    }
    bool is_rgb = type_img_read == "ppm";


    int h, w, nTaille;

    OCTET *img_in, *img_out, *rle_out;

    if (is_rgb) {
        get_line_col_ppm(name_img_read, &h, &w);
        nTaille = h * w * 3;

        alloc_array(img_in, OCTET, nTaille);
        read_ppm(name_img_read, img_in, h * w);
        alloc_array(img_out, OCTET, nTaille);
        alloc_array(rle_out, OCTET, nTaille);

        std::vector<OCTET> rle_vec_out;
        rle_compute_ppm(img_in, rle_vec_out, w, h);
        size_rle = rle_vec_out.size();
        alloc_array(rle_out, OCTET, size_rle);
        rle_generate_ppm(rle_vec_out, rle_out, size_rle);


        write_rle(rle_name, rle_out, size_rle);

        if(write_img_inv) {
            rle_inv_ppm(rle_out, img_out, size_rle);
            write_ppm(name_img_written, img_out, h, w);
        }
    } else {
        get_line_col_pgm(name_img_read, &h, &w);
        nTaille = h * w;

        alloc_array(img_in, OCTET, nTaille);
        read_pgm(name_img_read, img_in, h * w);
        alloc_array(img_out, OCTET, nTaille);
        alloc_array(rle_out, OCTET, nTaille);

        std::vector<OCTET> rle_vec_out;
        rle_compute_pgm(img_in, rle_vec_out, w, h);
        size_rle = rle_vec_out.size();
        alloc_array(rle_out, OCTET, size_rle);
        rle_generate_pgm(rle_vec_out, rle_out, size_rle);


        write_rle(rle_name, rle_out, size_rle);

        if(write_img_inv){
            rle_inv_pgm(rle_out, img_out, size_rle);
            write_pgm(name_img_written, img_out, h, w);
        }
    }


    free(img_in);
    free(img_out);
    free(rle_out);
    exit(0);
}