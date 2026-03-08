#include "HpInterface.h"


void HpInterface :: HpModify(float HpModifyValue) {
	//Prevent getting over MaxHp
	if ((HpModifyValue + HpCurrent) > HpMax) {
		HpCurrent = HpMax;
	}
	//Modify value with a heal or damage depending on the value
	else{
		HpCurrent += HpModifyValue;
	}

	if (HpCurrent <= 0)
	{
		HpCurrent = 0;
		Die();
	}
}