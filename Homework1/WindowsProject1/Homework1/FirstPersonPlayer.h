#pragma once
#include "Player.h"

class FirstPersonPlayer : public Player {
public:
	FirstPersonPlayer();
	virtual ~FirstPersonPlayer();

public:
	virtual void Initialize() override;
	virtual void Update(float fTimeElapsed) override;

public:
	virtual void ProcessKeyboardInput() override;
	virtual void ProcessMouseInput() override;

private:
	virtual void UpdatePlayerCamera(float fTimeElapsed) override;


};

