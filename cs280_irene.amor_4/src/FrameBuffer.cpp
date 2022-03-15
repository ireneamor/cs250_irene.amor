#include "FrameBuffer.h"

int             FrameBuffer::width     = 0;
int             FrameBuffer::height    = 0;
unsigned char * FrameBuffer::imageData = nullptr;

void FrameBuffer::Init(int w, int h)
{
    width     = w;
    height    = h;
    int size  = 3 * width * height;
    imageData = new unsigned char[size];
}

void FrameBuffer::Free()
{
    delete[] imageData;
}

void FrameBuffer::Clear(unsigned char r, unsigned char g, unsigned char b)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            imageData[(y * width + x) * 3 + 0] = r;
            imageData[(y * width + x) * 3 + 1] = g;
            imageData[(y * width + x) * 3 + 2] = b;
        }
    }
}

void FrameBuffer::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    // Sanity check
    if (imageData == nullptr || width <= x || x < 0 || height <= y || y < 0)
        return;

    // advance to pixel
    unsigned offset = 3 * (y * width + x);

    // set
    imageData[offset] = r;
    imageData[offset + 1] = g;
    imageData[offset + 2] = b;
}

void FrameBuffer::GetPixel(int x, int y, unsigned char & r, unsigned char & g, unsigned char & b)
{
    // Sanity check
    if (imageData == nullptr || width <= x || height <= y)
    {
        r = 250;
        g = 250;
        b = 250;

        return;
    }

    // advance to pixel
    unsigned startOffset = 3 * (y * width + x);

    // Get the color component
    r = imageData[startOffset];
    g = imageData[startOffset + 1];
    b = imageData[startOffset + 2];

}

// Convert the custom framebuffer to SFML format
void FrameBuffer::ConvertFrameBufferToSFMLImage(sf::Image & image)
{
    int w = FrameBuffer::GetWidth();
    int h = FrameBuffer::GetHeight();

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            unsigned char r, b, g;
            FrameBuffer::GetPixel(x, y, r, g, b);
            image.setPixel(x, y, sf::Color(r, g, b));
        }
    }
}