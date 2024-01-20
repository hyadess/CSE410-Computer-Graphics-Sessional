#ifndef COLOR
#define COLOR

static unsigned long long int g_seed = 1;
inline int randoms()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

struct Color
{
    int r, g, b;
    Color()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    void setColor()
    {
        r = randoms();
        g = randoms();
        b = randoms();
    }
};

#endif