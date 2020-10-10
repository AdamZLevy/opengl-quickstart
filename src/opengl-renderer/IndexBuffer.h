#ifndef OPENGL_QUICKSTART_INDEXBUFFER_H
#define OPENGL_QUICKSTART_INDEXBUFFER_H


class IndexBuffer {

private:
    unsigned int _rendererId;
    unsigned int _count;
public:
    IndexBuffer(const unsigned int *data, unsigned int count);

    ~IndexBuffer();

    void bind() const;

    void unbind() const;

    unsigned int getCount() const { return _count; };
};


#endif //OPENGL_QUICKSTART_INDEXBUFFER_H
