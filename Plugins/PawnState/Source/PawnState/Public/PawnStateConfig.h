#pragma once

#include "CoreMinimal.h"
#include "PawnStateConfig.generated.h"


UENUM(BlueprintType)
enum EPawnStateRelation
{
	COEXIST = 0,			//¹²´æ
	MUTEX = 1, 				//»¥³â
	FORBIDDEN = 2		    //½ûÖ¹
};

