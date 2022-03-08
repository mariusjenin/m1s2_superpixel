//
// Created by mariusjenin on 08/03/2022.
//

#ifndef M1S2_SUPERPIXEL_PIXEL_H
#define M1S2_SUPERPIXEL_PIXEL_H

struct Coord {
    int x;
    int y;
    Coord(int p_x, int p_y) : x(p_x), y(p_y);

    Coord operator+(Coord c) override {
        check_type();
        Coord col = Coord(x+c.x, y+c.y);
        return col;
    }

    Coord operator-(Coord c) override {
        check_type();
        Coord col = Coord(x-c.x, y-c.y);
        return col;
    }

    void operator+=(Coord c) override {
        check_type();
        x+=c.x;
        y+=c.y;
    }

    void operator-=(Coord c) override {
        check_type();
        x-=c.x;
        y-=c.y;
    }

    void operator/=(int denom) override {
        check_type();
        x/=decom;
        y/=denom;
    }

    Coord operator/(int denom) override {
        check_type();
        return Coord(x/=decom,y/=denom);
    }

    float distance(Coord c){
        float v1 = c.x - x;
        float v2 = c.y - y;
        return sqrt(v1*v1 + v2*v2);
    }
};

struct Pixel {
    Coord coord;
    Color color;
    std::vector <Pixel> neighbor;

    Pixel(Coord p_coord, std::vector <Pixel> p_neighbor) : coord(p_coord), neighbor(p_neighbor);
};

struct SuperPixel {
    std::vector <Pixel> cluster;
    std::vector <Pixel> neighbor;
    Color color;
    Coord centroid;

    SuperPixel(Pixel px){
        cluster = {px};
        neighbor = px.neighbor;
        color = px.color;
    }

    float color_diff(Pixel px){

        return hsp_1 == hsp_2 ? 0 : (hsp_1 > hsp_2?1:-1);
    }

    void update_color(){
        size_t size_cluster = cluster.size();
        color = cluster[0].color;
        for(int i = 1 ; i < size_cluster ; i++){
            color += cluster[1].color;
        }
        color /= size_cluster;
    }

    void update_centroid(){
        size_t size_cluster = cluster.size();
        centroid = cluster[0].coord;
        for(int i = 1 ; i < size_cluster ; i++){
            centroid += cluster[1].coord;
        }
        centroid /= size_cluster;
    }

    void add_to_cluster(Pixel px) {
        //Merge the both neighborhood
        neighbor.insert(neighbor.end(), px.neighbor.begin(), px.neighbor.end());
        //Remove the duplicates
        std::vector<Coord>::iterator it = std::unique(neighbor.begin(), neighbor.end());
        neighbor.resize(std::distance(neighbor.begin(), it));
        //Add the super-pixel position to the cluster
        cluster.push_back(px);
        //Update the mean color and the centroid of the super-pixel
        update_color();
        update_centroid();
    }

    float get_score_for(Pixel px, float compactness, float side_sp_px){
        return color.distance(px.color) + compactness * coord.distance(px.coord) / side_sp_px;
    }
};

#endif //M1S2_SUPERPIXEL_PIXEL_H
