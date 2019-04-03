#pragma once

#include <vector>
#include <map>
#include <mutex>
#include "Geometry.h"

#define LpImageMng (ImageMng::GetInstance())

typedef std::vector<int> VEC_INT;

struct ImageName
{
	/// 画像のパス名
	const std::string titleImage  = "resource/Image/title.png";
	const std::string mainImage   = "resource/Image/main.png";
	const std::string resultImage = "resource/Image/result.png";
	const std::string playerImage = "resource/Image/player.png";
	
};

class ImageMng
{
public:
	static ImageMng & GetInstance()
	{
		return *s_Instance;
	}
	const ImageName GetImage();
	const VEC_INT ImgGetID(std::string fileName);
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
	std::map<std::string, VEC_INT> imgMap;		// 画像IDのハンドルと画像の分割数を保存する変数

	ImageName image;
};

