#pragma once
#include <SFML/Graphics.hpp>

class FrameBuffer
{
  public:
    static void Init(int w, int h);
    static void Free();

    static void Clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
    static void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    static void GetPixel(int x, int y, unsigned char & r, unsigned char & g, unsigned char & b);
    static int  GetWidth() { return width; }
    static int  GetHeight() { return height; }

    static void ConvertFrameBufferToSFMLImage(sf::Image & image);

  private:
    static int             width;
    static int             height;
    static unsigned char * imageData;
};
