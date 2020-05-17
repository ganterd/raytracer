#pragma once

class uchar4
{
public:
    union
    {
        unsigned char data[4];
        struct {
            unsigned char r, g, b, a;
        };
        struct 
        {
            unsigned char x, y, z, w;
        };
    };

    uchar4(){ };

    uchar4(char r, char g, char b, char a) : 
        r(r), g(g), b(b), a(a)
    {
        
    }
};