#version 450

const GLchar* vertexShaderSrc = R"glsl(
    in float inValue;
    out float outValue;

    void main()
    {
        outValue = sqrt(inValue);
    }
)glsl";