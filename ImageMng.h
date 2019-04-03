#pragma once

#include <vector>
#include <map>
#include <mutex>
#include "Geometry.h"

#define LpImageMng (ImageMng::GetInstance())

typedef std::vector<int> VEC_INT;

struct ImageName
{
	/// �摜�̃p�X��
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
	std::map<std::string, VEC_INT> imgMap;		// �摜ID�̃n���h���Ɖ摜�̕�������ۑ�����ϐ�

	ImageName image;
};

