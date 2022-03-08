//
// Created by mariusjenin on 08/03/2022.
//

#ifndef M1S2_SUPERPIXEL_COLOR_H
#define M1S2_SUPERPIXEL_COLOR_H

#include <stdexcept>

class Color {
public:
    static const int COLOR_TYPE_COLOR1 = 0;
    static const int COLOR_TYPE_COLOR3 = 1;
    virtual float distance(Color* c) = 0;

    virtual Color* operator+(Color* c) = 0;

    virtual Color* operator-(Color* c) = 0;

    virtual void operator+=(Color* c) = 0;

    virtual void operator-=(Color* c) = 0;

    virtual void operator/=(int denom) = 0;
protected:
    virtual void check_type() = 0;

    virtual int get_type() = 0;

};

class Color1 : public Color {
private:
    int v;
protected:
    int get_type() override {
        return COLOR_TYPE_COLOR1;
    }

    void check_type() override {
        if (get_type() != COLOR_TYPE_COLOR1) {
            throw std::bad_typeid();
        }
    }

public:
    int get_v(){
        return v;
    }
    Color1(int p_v) : v(p_v){};


    float distance(Color* c) override{
        check_type();
        return abs(v - ((Color1*)c)->get_v());
    }

    Color* operator+(Color* c) override {
        check_type();
        auto* col = new Color1(v + ((Color1*)c)->get_v());
        return col;
    }

    Color* operator-(Color* c) override {
        check_type();
        auto* col = new Color1(v- ((Color1*)c)->get_v());
        return col;
    }

    void operator+=(Color* c) override {
        check_type();
        v += ((Color1*)c)->get_v();
    }

    void operator-=(Color* c) override {
        check_type();
        v -= ((Color1*)c)->get_v();
    }

    void operator/=(int denom) override {
        check_type();
        v /= denom;
    }
};

class Color3 : public Color {
public:
    static constexpr const float TRISTIMULUS_X = 94.8110;
    static constexpr const float TRISTIMULUS_Y = 100.;
    static constexpr const float TRISTIMULUS_Z = 107.304;
    Color3(int p_v1, int p_v2, int p_v3) : v1(p_v1), v2(p_v2), v3(p_v3){};


    int get_v1(){
        return v1;
    }
    int get_v2(){
        return v2;
    }
    int get_v3(){
        return v3;
    }

    void rgb_to_xyz(){
        v1 = 0.412453 * v1 + 0.357580 * v2 + 0.180423 * v3;
        v2 = 0.212671 * v1 + 0.715160 * v2 + 0.072169 * v3;
        v3 = 0.019334 * v1 + 0.119193 * v2 + 0.950227 * v3;
    }

    void xyz_to_rgb(){
        v1 = 3.240479 * v1 + -1.537150 * v2 + -0.498535 * v3;
        v2 = -0.969256 * v1 + 1.875992 * v2 + 0.041556 * v3;
        v3 =0.055648 * v1 + -0.204043 * v2 + 1.057311 * v3;
    }

    static float rgb_to_cielab_aux(float t){
        if(t > 0.008856){
            return pow(t,1/3);
        } else {
            return 7.787 * t + 0.13793103448;
        }
    }
    void rgb_to_cielab(){
        rgb_to_xyz();
        float x_xn= v1/TRISTIMULUS_X;
        float y_yn= v2/TRISTIMULUS_Y;
        float z_zn= v3/TRISTIMULUS_Z;
        if(y_yn > 0.008856){
            v1  = 116 * pow(y_yn,1/3) - 16;
        } else {
            v1  = 903.3 *y_yn;
        }

        v2 = 500 * ( rgb_to_cielab_aux(x_xn) - rgb_to_cielab_aux(y_yn) );
        v3 = 200 * ( rgb_to_cielab_aux(y_yn) - rgb_to_cielab_aux(z_zn) );
    }

    void cielab_to_rgb(){
        float p = (v1 +16)/116;
        v1 = TRISTIMULUS_X * pow(p + v2/500,3);
        v2 = TRISTIMULUS_Y * pow(p ,3);
        v3 = TRISTIMULUS_Z * pow(p - v3/200,3);
        xyz_to_rgb();
    }

    float distance(Color* c) override{
        float v_1 = ((Color3*)c)->get_v1() - v1;
        float v_2 = ((Color3*)c)->get_v2() - v2;
        float v_3 = ((Color3*)c)->get_v3() - v3;
        return sqrt(v_1*v_1 + v_2*v_2 + v_3*v_3);
    }

    Color* operator+(Color* c) override {
        check_type();
        auto* col = new  Color3(v1 + ((Color3*)c)->get_v1() , v2 + ((Color3*)c)->get_v2(), v3 + ((Color3*)c)->get_v3());
        return col;
    }

    Color* operator-(Color* c) override {
        check_type();
        auto* col = new Color3(v1 - ((Color3*)c)->get_v1() , v2 - ((Color3*)c)->get_v2(), v3 - ((Color3*)c)->get_v3());
        return col;
    }

    void operator+=(Color* c) override {
        check_type();
        v1 += ((Color3*)c)->get_v1() ;
        v2 += ((Color3*)c)->get_v2();
        v3 += ((Color3*)c)->get_v3();
    }

    void operator-=(Color* c) override {
        check_type();
        v1 -= ((Color3*)c)->get_v1() ;
        v2 -= ((Color3*)c)->get_v2();
        v3 -= ((Color3*)c)->get_v3();
    }

    void operator/=(int denom) override {
        check_type();
        v1 /= denom;
        v2 /= denom;
        v3 /= denom;
    }
private:
    int v1;
    int v2;
    int v3;

protected:

    int get_type() override {
        return COLOR_TYPE_COLOR3;
    }

    void check_type() override {
        if (get_type() != COLOR_TYPE_COLOR3) {
            throw std::bad_typeid();
        }
    }
};

#endif //M1S2_SUPERPIXEL_COLOR_H
