#include <utility>

//
// Created by mariusjenin on 08/03/2022.
//

#ifndef M1S2_SUPERPIXEL_PIXEL_H
#define M1S2_SUPERPIXEL_PIXEL_H

#include <cfloat>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

class Coord {
private:
    int x;
    int y;

public:
    explicit Coord(int p_x, int p_y) : x(p_x), y(p_y) {};

    void operator-=(Coord c) {
        x -= c.x;
        y -= c.y;
    }

    void operator+=(Coord c) {
        x += c.x;
        y += c.y;
    }

    void operator/=(int denom) {
        x /= denom;
        y /= denom;
    }

    Coord operator/(int denom) {
        return Coord(x /= denom, y /= denom);
    }

    int get_x() const {
        return x;
    }

    int get_y() const {
        return y;
    }

    float distance(Coord c) const {
        float v1 = (float) c.x - (float) x;
        float v2 = (float) c.y - (float) y;
        return sqrt(v1 * v1 + v2 * v2);
    }
};

class Pixel {
private:
    Coord *coord;
    Color *color;
    std::vector<Pixel *> neighbor;
public:

    Pixel(Coord *p_coord, Color *p_color) : coord(p_coord), color(p_color), neighbor({}) {};

    Coord *get_coord() {
        return coord;
    }

    Color *get_color() {
        return color;
    }

    std::vector<Pixel *> get_neighbor() {
        return neighbor;
    }

    void add_neighbor(Pixel *px) {
        neighbor.push_back(px);
    }

    ~Pixel() {
//        std::cout << "blabla1" << std::endl;
////        delete coord;
//        std::cout << "blabla2" << std::endl;
////        delete color;
//        std::cout << "blabla3" << std::endl;
    }
};

class SuperPixel {
private:
    std::vector<Pixel *> cluster;
    std::vector<Pixel *> neighbor;
    Color *color;
    Coord *centroid;

    void add_to_cluster(Pixel *px) {
        //Merge the both neighborhood
        std::vector<Pixel*> neighbor_px = px->get_neighbor();
        size_t size_neighbor_px = neighbor_px.size();
        for(int i = 0;i < size_neighbor_px;i++){
            neighbor.push_back(neighbor_px.at(i));
        }
        //Remove the duplicates
        auto it = std::unique(neighbor.begin(), neighbor.end());
        neighbor.resize(std::distance(neighbor.begin(), it));
        //Add the super-pixel position to the cluster
        cluster.push_back(px);
        //Update the mean color and the centroid of the super-pixel
        update_color();
        update_centroid();
    }

public:
    explicit SuperPixel(Pixel *px) {
        centroid = px->get_coord();
        cluster = {px};
        neighbor = px->get_neighbor();
        color = px->get_color();
    }

    void update_color() {
        size_t size_cluster = cluster.size();
        *color = *cluster[0]->get_color();
        for (int i = 1; i < size_cluster; i++) {
            *color -= cluster[1]->get_color();
        }
        *color /= (int) size_cluster;
    }

    void update_centroid() {
        size_t size_cluster = cluster.size();
        *centroid = *cluster[0]->get_coord();
        for (int i = 1; i < size_cluster; i++) {
            *centroid += *cluster[1]->get_coord();
        }
        *centroid /= (int) size_cluster;
    }

    bool find_neighbor_to_add(int compactness, float side_sp_px) {
        size_t size_neighbor = neighbor.size();
        float score = FLT_MAX;
        int ind = -1;
        for (int i = 0; i < size_neighbor; i++) {
            float score_crt = get_score_for(neighbor[i], compactness, side_sp_px);
            if (score_crt < score) {
                ind = i;
                score = score_crt;
            }
        }
        if (ind < 0) {
            return false;
        } else {
            add_to_cluster(neighbor[ind]);
            int sizea = neighbor.size();
            neighbor.erase(neighbor.begin()+ind);
            int sizeb = neighbor.size();

            std::cout << ind << " " <<sizea << " " << sizeb << std::endl;
            return true;
        }
    }

    std::vector<Pixel *> get_cluster(){
        return cluster;
    }


    float get_score_for(Pixel *px, int compactness, float side_sp_px) {
        return color->distance(px->get_color()) + (float)compactness * centroid->distance(*px->get_coord()) / side_sp_px;
    }

};

#endif //M1S2_SUPERPIXEL_PIXEL_H
