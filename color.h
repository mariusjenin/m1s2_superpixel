//
// Created by mariusjenin on 08/03/2022.
//
//http://www.easyrgb.com/en/math.php#text2
#ifndef M1S2_SUPERPIXEL_COLOR_H
#define M1S2_SUPERPIXEL_COLOR_H

#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>

class Color {
public:
    static const int COLOR_TYPE_COLOR1 = 0;
    static const int COLOR_TYPE_COLOR3 = 1;

    virtual void clamp_values() = 0;

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

    void clamp_values() override{
        m_v = std::min(std::max(m_v,0),255);
    };

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
    static constexpr const float TRISTIMULUS_X = 95.047;
    static constexpr const float TRISTIMULUS_Y = 100.;
    static constexpr const float TRISTIMULUS_Z = 108.883;
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

    void clamp_values() override{
        m_v1 = std::min(std::max(m_v1,0),255);
        m_v2 = std::min(std::max(m_v2,0),255);
        m_v3 = std::min(std::max(m_v3,0),255);
    };

    void rgb_to_xyz(){
        float v1 = ( (float)m_v1 / 255.f );
        float v2 = ((float) m_v2 / 255.f );
        float v3 = ( (float)m_v3 / 255.f );

        if ( v1 > 0.04045 ) {
            v1 = (float)pow( ( v1 + 0.055 ) / 1.055,2.4 );
        } else  {
            v1 = v1 / 12.92f;
        }
        if ( v2 > 0.04045 ) {
            v2 = (float)pow( ( v2 + 0.055 ) / 1.055 ,2.4 );
        } else {
            v2 = v2 / 12.92f;
        }
        if ( v3 > 0.04045 ) {
            v3 = (float)pow( ( v3 + 0.055 ) / 1.055 ,2.4 );
        }
        else                   {
            v3 = v3 / 12.92f;
        }

        v1 = v1 * 100;
        v2 = v2 * 100;
        v3 = v3 * 100;

        m_v1 = (int)round(v1 * 0.4124 + v2 * 0.3576 + v3 * 0.1805);
        m_v2 = (int)round(v1 * 0.2126 + v2 * 0.7152 + v3 * 0.0722);
        m_v3 = (int)round(v1 * 0.0193 + v2 * 0.1192 + v3 * 0.9505);
        clamp_values();
    }

    void xyz_to_rgb(){
        float var_X = (float)m_v1 / 100.f;
        float var_Y = (float)m_v2 / 100.f;
        float var_Z = (float)m_v3 / 100.f;

        auto var_R = (float)(var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986);
        auto var_G = (float)(var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415);
        auto var_B = (float)(var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570);


        if ( var_R > 0.0031308 ) {
            var_R = (float)(1.055 * pow( var_R , ( 1 / 2.4 ) ) - 0.055);
        }
        else                     {
            var_R = (float)(12.92 * var_R);
        }
        if ( var_G > 0.0031308 ) {
            var_G = (float)(1.055 * pow( var_G , ( 1 / 2.4 ) ) - 0.055);
        }
        else                     {
            var_G = (float)(12.92 * var_G);
        }
        if ( var_B > 0.0031308 ) {
            var_B = (float)(1.055 * pow( var_B , ( 1 / 2.4 ) ) - 0.055);
        }
        else                     {
            var_B = (float)(12.92 * var_B);
        }

        m_v1 = (int) round(var_R * 255);
        m_v2 = (int) round(var_G * 255);
        m_v3 = (int) round(var_B * 255);

        clamp_values();
    }

    void xyz_to_cielab(){
        float var_X = (float) m_v1 / TRISTIMULUS_X;
        float var_Y =  (float) m_v2 / TRISTIMULUS_Y;
        float var_Z =  (float) m_v3 / TRISTIMULUS_Z;

        if ( var_X > 0.008856 ) {
            var_X = (float)pow(var_X ,( 1/3 ));
        }
        else                    {
            var_X = ( 7.787f * var_X ) + ( 16.f / 116.f );
        }
        if ( var_Y > 0.008856 ) {
            var_Y = (float)pow(var_Y , ( 1/3 ));
        }
        else                    {
            var_Y = ( 7.787f * var_Y ) + ( 16.f / 116.f );
        }
        if ( var_Z > 0.008856 ) {
            var_Z = (float)pow(var_Z , ( 1/3 ));
        }
        else                    {
            var_Z = ( 7.787f * var_Z ) + ( 16.f / 116.f );
        }

        m_v1 = (int) round(( 116 * var_Y ) - 16);
        m_v2 = (int) round(500 * ( var_X - var_Y ));
        m_v3 = (int) round(200 * ( var_Y - var_Z ));
        clamp_values();
    }

    void cielab_to_xyz(){
        float var_Y = ((float) m_v1 + 16.f ) / 116.f;
        float var_X = (float)m_v2 / 500.f + var_Y;
        float var_Z = var_Y - (float)m_v3 / 200.f;

        if ( pow(var_Y,3)  > 0.008856 ) {
            var_Y = (float) pow(var_Y,3);
        }
        else                       {
            var_Y = (  (float)var_Y - 16.f / 116.f ) / 7.787f;
        }
        if ( pow(var_X,3)  > 0.008856 ) {
            var_X =  (float) pow(var_X,3);
        }
        else                       {
            var_X = (  (float)var_X - 16.f / 116.f ) / 7.787f;
        }
        if ( pow(var_Z,3)  > 0.008856 ) {
            var_Z =  (float) pow(var_Z,3);
        }
        else                       {
            var_Z = ( (float)var_Z - 16.f / 116.f ) / 7.787f;
        }

        m_v1 = (int) round(var_X * TRISTIMULUS_X);
        m_v2 = (int) round(var_Y * TRISTIMULUS_Y);
        m_v3 = (int) round(var_Z * TRISTIMULUS_Z);
        clamp_values();
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
