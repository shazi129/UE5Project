#pragma once

#include "CoreMinimal.h"
#include "PawnStateConfig.generated.h"


UENUM(BlueprintType)
enum EPawnStateRelation
{
	COEXIST = 0,			//����
	MUTEX = 1, 				//����
	FORBIDDEN = 2		    //��ֹ
};

