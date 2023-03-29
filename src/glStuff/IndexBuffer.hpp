#pragma once

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;   // number of points in the buffer (not pairs or triplets but singular datapoints)
public:
    IndexBuffer();
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();

    void AddData(const void* data, unsigned int count);

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }
};