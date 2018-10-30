#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Coordinatesystem : public Geometry
	{
	public:
		// Constructor
		Coordinatesystem(Shader* shader, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scale);
		// Destructor
		virtual ~Coordinatesystem();
		mat4x4 getGlobalPose();

		vec3 defaultColor;
		vec3 wonColor;

	private:

	};
};