#pragma once
#include "../Object/Object.h"
#include <iostream>
#include <map>
#include "../Geometry.h"

enum class ANIM
{
	START,
	FRAME,
	INTERVAL,
	MAX
};

class Input;

class CharactorObject : Object
{
public:
	CharactorObject();
	CharactorObject(int groundLine);
	~CharactorObject();
	void Draw(int img);
	virtual Rect GetRect();
	
	// ����ݖ�, ���W, �摜�T�C�Y
	void Init(std::string actionName, const Vector2f & pos, const Vector2 & size);

protected:
	void ChangeAction(const char * name);		// �A�N�V�����؂�ւ�
	bool ProceedAnimFile();						// ��Ұ����ڰт�1�i�߂�
	void ReadActionFile(const char* actionPath);// ̧�ق̓ǂݍ���
	Vector2f GetPos() const;

	ObjectData actionData;

	Vector2f pos;
	Vector2f vel;
	Vector2 size;

	std::string nowActionName;

	int groundLine;
	int nowCutIdx;
	unsigned int frame;

	bool turnFlag;		// true:������, false:�E����
};