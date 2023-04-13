#include "SimpleRandomMovementTrait.h"
#include "SimpleMovementFragment.h"
#include "MassEntityTemplateRegistry.h"

void USimpleRandomMovementTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FSimpleMovementFragment>();
}