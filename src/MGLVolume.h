#include "MGL.h"
#include "IndexedGeometryRectangularPrism.h"

namespace Aftr
{
   class MGLVolume : public MGL
   {
   public:
      static MGLVolume* New(WO* parent);
      virtual ~MGLVolume() {};
      void onCreate();
      void render(const Camera& cam) override;
      void update(GLSLShader* compute);
   protected:
      MGLVolume(WO* parent);
      GLuint densityTexture;
      //GLuint texture2;
      int counter = 0;

      GLuint vaoHandle;
      GLuint vertexBuffer;
      GLuint indexBuffer;
   };
}