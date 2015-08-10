#pragma once

#include "Model.h"
#include<time.h>
#include<stdarg.h>
#include <vector>
#include "colorObject.h"

namespace Rendering
{
	namespace Models
	{
		class PointCloud : public Model
		{
		public:
			PointCloud();
			~PointCloud();

			void Create(std::vector<std::vector<cv::Point3d>> threeDPointSpace);
			virtual void Draw(const glm::mat4& projection_matrix,
				const glm::mat4& view_matrix)
				override final;
			virtual void Update() override final;

		private:
			int totalData = 0;
			glm::vec3 rotation, rotation_speed;
			time_t timer;
		};
	}
}