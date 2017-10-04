#ifndef Shader_hpp
#define Shader_hpp

#include <string>

namespace generated
{
	class Shader
	{
		public:
			static std::string getShaderDirectory()
			{
				return "@SHADER_DIR@";
			}
	};
}


#endif
