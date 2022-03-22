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
    int m_x;
    int m_y;

public:
    explicit Coord(int p_x, int p_y) : m_x(p_x), m_y(p_y) {};
    Coord(Coord const &coord)= default;

    void operator-=(Coord c) {
        m_x -= c.m_x;
        m_y -= c.m_y;
    }

    void operator+=(Coord c) {
        m_x += c.m_x;
        m_y += c.m_y;
    }

    void operator/=(int denom) {
        m_x /= denom;
        m_y /= denom;
    }

    int get_x() const {
        return m_x;
    }

    int get_y() const {
        return m_y;
    }

    float distance(Coord c) const {
        float v1 = (float) c.m_x - (float) m_x;
        float v2 = (float) c.m_y - (float) m_y;
        return sqrt(v1 * v1 + v2 * v2);
    }
};

class Pixel {
private:
    Coord *m_coord;
    Color *m_color;
    std::vector<Pixel *> m_neighbor;
    bool m_flag;
public:

    Pixel(Coord *p_coord, Color *p_color) : m_coord(p_coord), m_color(p_color), m_neighbor({}), m_flag(false) {};

    void set_flag(bool flag) {
        m_flag = flag;
    }

    bool get_flag() const {
        return m_flag;
    }

    Coord *get_coord() {
        return m_coord;
    }

    Color *get_color() {
        return m_color;
    }

    std::vector<Pixel *> get_neighbor() {
        return m_neighbor;
    }

    void add_neighbor(Pixel *px) {
        m_neighbor.push_back(px);
    }

    ~Pixel() {
        delete m_coord;
        delete m_color;
    }
};

class SuperPixel {
private:
    std::vector<Pixel *> m_cluster;
    std::vector<Pixel *> m_neighbor;
    Color *m_color;
    Coord m_coord_start;
    Coord *m_centroid;

    void add_to_cluster(Pixel *px) {
        px->set_flag(true);
        //Merge the both neighborhood
        std::vector<Pixel *> neighbor_px = px->get_neighbor();
        size_t size_neighbor_px = neighbor_px.size();
        for (int i = 0; i < size_neighbor_px; i++) {
            if(!neighbor_px.at(i)->get_flag()) {
                m_neighbor.push_back(neighbor_px.at(i));
            }
        }
        //Remove the duplicates
        auto it = std::unique(m_neighbor.begin(), m_neighbor.end());
        m_neighbor.resize(std::distance(m_neighbor.begin(), it));
        //Add the super-pixel position to the cluster
        m_cluster.push_back(px);
        //Update the mean color and the centroid of the super-pixel
        update_color();
        update_centroid();
    }

public:
    explicit SuperPixel(Pixel *px): m_coord_start(Coord(*px->get_coord())){
        px->set_flag(true);
        m_centroid = px->get_coord();
        m_cluster = {px};
        std::vector<Pixel*> neighbor = px->get_neighbor();
        int size_neighbor = (int)neighbor.size();
        for(int i = 0 ; i < size_neighbor ; i++){
            m_neighbor.push_back(neighbor.at(i));
        }
        m_color = px->get_color();
    }

    Color *get_color() {
        return m_color;
    }

    Coord get_coord_start() {
        return m_coord_start;
    }

    void update_color() {
        size_t size_cluster = m_cluster.size();
        *m_color = *m_cluster[0]->get_color();
        for (int i = 1; i < size_cluster; i++) {
            *m_color += m_cluster[i]->get_color();
        }
        *m_color /= (int) size_cluster;
    }

    void update_centroid() {
        size_t size_cluster = m_cluster.size();
        *m_centroid = *m_cluster[0]->get_coord();
        for (int i = 1; i < size_cluster; i++) {
            *m_centroid += *m_cluster[1]->get_coord();
        }
        *m_centroid /= (int) size_cluster;
    }

    bool find_neighbor_to_add(int compactness, float side_sp_px) {
        size_t size_neighbor = m_neighbor.size();
        float score = FLT_MAX;
        int ind = -1;
        for (int i = 0; i < size_neighbor; i++) {
            if(!m_neighbor[i]->get_flag()) {
                float score_crt = get_score_for(m_neighbor[i], compactness, side_sp_px);
                if (score_crt < score) {
                    ind = i;
                    score = score_crt;
                }
            }
        }
        if (ind < 0) {
            return false;
        } else {
            add_to_cluster(m_neighbor[ind]);
            return true;
        }
    }

    std::vector<Pixel *> get_cluster() {
        return m_cluster;
    }


    float get_score_for(Pixel *px, int compactness, float side_sp_px) {
        return m_color->distance(px->get_color()) +
               (float) compactness * m_centroid->distance(*px->get_coord()) / side_sp_px;
    }

};

#endif //M1S2_SUPERPIXEL_PIXEL_H
