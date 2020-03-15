#pragma once

#include <vector>
#include <map>
#include <mutex>
#include "Geometry.h"

#define LpImageMng (ImageMng::GetInstance().)

typedef std::vector<int> VEC_INT;

class ImageMng
{
public:
	static ImageMng & GetInstance()
	{
		return *s_Instance;
	}
	const int ImgGetID(std::string fileName);
	const VEC_INT ImgGetID(std::string fileName, Vector2 divCnt, Vector2 divSize);
private:
	struct ImageMngDeleter
	{
		void operator()(ImageMng * imageMng) const
		{
			delete imageMng;
		}
	};
	ImageMng();
	~ImageMng();
	static std::unique_ptr<ImageMng, ImageMngDeleter> s_Instance;
	std::map<std::string, VEC_INT> imgMap;		// ‰æ‘œID‚Ìƒnƒ“ƒhƒ‹‚Æ‰æ‘œ‚Ì•ªŠ„”‚ğ•Û‘¶‚·‚é•Ï”
};

