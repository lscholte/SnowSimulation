#ifndef Asset_hpp
#define Asset_hpp

#include <string>

namespace generated
{
	class Asset
	{
		public:
			static std::string getAssetDirectory()
			{
				return "@ASSET_DIR@";
			}
	};
}


#endif
