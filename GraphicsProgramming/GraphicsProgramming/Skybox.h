#include <string>
#include "glew.h"
#include "glut.h"
#include "SOIL.h"
#include "Vector3.h"
class Skybox
{
public:

    Skybox();
    void drawSkybox();
    void skyboxRender(Vector3 camPos);
    GLuint getSkyText();
private:

    GLuint skyTexture;
};
