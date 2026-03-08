#pragma once
#include <iostream>

class HpInterface
{
public:

	void SetupHp(float Hp) { HpMax = Hp; HpCurrent = HpMax; };

	float getHp() { return HpCurrent; };

	float getHpPercent() { return HpCurrent/HpMax; };
	
	//Used for taking damage and healing the player
	void HpModify(float HpModifyValue);

	//Can make it so players checks if bool dead updated or trigger an event inside die
	virtual void Die() { std::cout << "Object Died " << std::endl; };
	
protected:
	float HpMax = 100;
	float HpCurrent = 100;

};

