#pragma once

class uchar4
{
public:
    union
    {
        char data[4];
        struct {
            char r, g, b, a;
        };
        struct 
        {
            char x, y, z, w;
        };
    };

    uchar4(){ };

    uchar4(char r, char g, char b, char a) : 
        r(r), g(g), b(b), a(a)
    {
        
    }
};