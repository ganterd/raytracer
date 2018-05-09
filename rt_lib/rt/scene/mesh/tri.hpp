#include <glm/glm.hpp>

namespace rt
{
	class Tri
	{
	public:
		glm::vec3 v[3];
		glm::vec3 n[3];
		glm::vec3 e[2];
		glm::vec2 min;
		glm::vec2 max;

		Tri(
			glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
			glm::vec3 n0, glm::vec3 n1, glm::vec3 n2
		)
		{
			v[0] = v0;
			v[1] = v1;
			v[2] = v2;
			n[0] = n0;
			n[1] = n1;
			n[2] = n2;

			// Precalculate edges
			e[0] = v1 - v0;
			e[1] = v2 - v0;

		}

		bool rayIntersection(
			const glm::vec3& origin,
			const glm::vec3& direction,
			glm::vec3& outHit
		){
			glm::vec3 h, s, q;
			float a, f, u, v;

			h = glm::cross(direction, e[1]);
			a = glm::dot(e[0], h);
			if(a > -0.0000001f && a < 0.0000001f)
				return false;

			f = 1.0f / a;
			s = origin - v[0];
			u = f * glm::dot(s, h);
			if(u < 0.0f || u > 1.0f)
				return false;

			q = glm::dot(s, e[0]);
			v = f * glm::dot(direction, q);
			if(v < 0.0f || u + v > 1.0f)
				return false;

			float t = f * glm::dot(e[1], q);
			if(t > 0.000001f)
			{
				outHit = origin + direction * t;
				return true;
			}
			return false;
		}
	}
}

