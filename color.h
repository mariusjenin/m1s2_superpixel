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

    virtual void operator+=(Color* c) = 0;

    virtual void operator-=(Color* c) = 0;

    virtual void operator/=(int denom) = 0;

    virtual void operator*=(int fact) = 0;

    virtual ~Color() =default;
protected:
    virtual void check_type() = 0;

    virtual int get_type() = 0;

};

class Color1 : public Color {
private:
    int m_v;
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
    int get_v() const{
        return m_v;
    }
    explicit Color1(int p_v) : m_v(p_v){};


    float distance(Color* c) override{
        check_type();
        return (float)abs(m_v - ((Color1*)c)->get_v());
    }

    void operator+=(Color* c) override {
        check_type();
        m_v += ((Color1*)c)->get_v();
    }

    void operator-=(Color* c) override {
        check_type();
        m_v -= ((Color1*)c)->get_v();
    }

    void operator/=(int denom) override {
        check_type();
        m_v /= denom;
    }

    void operator*=(int fact) override {
        check_type();
        m_v *= fact;
    }

    ~Color1() override= default;
};

class Color3 : public Color {
private:
    int m_v1;
    int m_v2;
    int m_v3;
public:
    static constexpr const float TRISTIMULUS_X = 94.8110;
    static constexpr const float TRISTIMULUS_Y = 100.;
    static constexpr const float TRISTIMULUS_Z = 107.304;
    Color3(int p_v1, int p_v2, int p_v3) : m_v1(p_v1), m_v2(p_v2), m_v3(p_v3){};


    ~Color3() override= default;

    int get_v1() const{
        return m_v1;
    }
    int get_v2() const{
        return m_v2;
    }
    int get_v3() const{
        return m_v3;
    }

    void rgb_to_xyz(){
        m_v1 = (int) round(0.412453 * m_v1 + 0.357580 * m_v2 + 0.180423 * m_v3);
        m_v2 = (int) round(0.212671 * m_v1 + 0.715160 * m_v2 + 0.072169 * m_v3);
        m_v3 = (int) round(0.019334 * m_v1 + 0.119193 * m_v2 + 0.950227 * m_v3);
    }

    void xyz_to_rgb(){
        m_v1 = (int) round(3.240479 * m_v1 + -1.537150 * m_v2 + -0.498535 * m_v3);
        m_v2 = (int) round(-0.969256 * m_v1 + 1.875992 * m_v2 + 0.041556 * m_v3);
        m_v3 = (int) round(0.055648 * m_v1 + -0.204043 * m_v2 + 1.057311 * m_v3);
    }

    static float xyz_to_cielab_aux(float t){
        if(t > 0.008856){
            return (float) pow(t,1/3);
        } else {
            return 7.787f * t + 0.13793103448f;
        }
    }
    void xyz_to_cielab(){
        float x_xn= (float) m_v1 / TRISTIMULUS_X;
        float y_yn= (float) m_v2 / TRISTIMULUS_Y;
        float z_zn= (float) m_v3 / TRISTIMULUS_Z;
        m_v1 = (int) round(116.f * round(xyz_to_cielab_aux(y_yn) - 16) );
        m_v2 = (int) (500 * round(xyz_to_cielab_aux(x_xn) - xyz_to_cielab_aux(y_yn) ));
        m_v3 = (int) (200 * round(xyz_to_cielab_aux(y_yn) - xyz_to_cielab_aux(z_zn) ));
    }
    static float cielab_to_xyz_aux(float t){
        if(t > 0.206896){
            return (float) pow(t,3);
        } else {
            return  0.128418f * (t - 0.137931f);
        }
    }
    void cielab_to_xyz(){
        float p = (float) (m_v1 + 16) / 116;
        m_v1 = (int) round(TRISTIMULUS_X * cielab_to_xyz_aux(((float)m_v1+16)/116));
        m_v2 = (int) round(TRISTIMULUS_Y * cielab_to_xyz_aux(((float)m_v1+16)/116 + (float)m_v2/500));
        m_v3 = (int) round(TRISTIMULUS_Z * cielab_to_xyz_aux(((float)m_v1+16)/116 - (float)m_v3/200));
    }

    float distance(Color* c) override{
        float v_1 = (float)((Color3*)c)->get_v1() - (float)m_v1;
        float v_2 = (float)((Color3*)c)->get_v2() - (float)m_v2;
        float v_3 = (float)((Color3*)c)->get_v3() - (float)m_v3;
        return sqrt(v_1*v_1 + v_2*v_2 + v_3*v_3);
    }

    void operator+=(Color* c) override {
        check_type();
        m_v1 += ((Color3*)c)->get_v1() ;
        m_v2 += ((Color3*)c)->get_v2();
        m_v3 += ((Color3*)c)->get_v3();
    }

    void operator-=(Color* c) override {
        check_type();
        m_v1 -= ((Color3*)c)->get_v1() ;
        m_v2 -= ((Color3*)c)->get_v2();
        m_v3 -= ((Color3*)c)->get_v3();
    }

    void operator/=(int denom) override {
        check_type();
        m_v1 /= denom;
        m_v2 /= denom;
        m_v3 /= denom;
    }

    void operator*=(int fact) override {
        check_type();
        m_v1 *= fact;
        m_v2 *= fact;
        m_v3 *= fact;
    }
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
