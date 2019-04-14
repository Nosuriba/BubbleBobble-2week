#pragma once

class Input
{
private:
	int inputKey;
	int lastInputKey;
	int mouseInput;
	int lastMouseInput;

public:
	Input();
	~Input();

	const bool IsPressing(int p) const;
	const bool IsTrigger(int p) const;
	const bool IsMousePressing(int p) const;
	const bool IsMouseTrigger(int p) const;

	void Update();
};

