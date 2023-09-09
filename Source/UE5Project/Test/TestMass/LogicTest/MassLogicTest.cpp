#include "MassLogicTest.h"
#include "MassEntitySubsystem.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"

void UMassLogicTestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UMassEntitySubsystem* Subsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	if (!Subsystem)
	{
		return;
	}
	FMassEntityManager& EntityManager = Subsystem->GetMutableEntityManager();

	FMassArchetypeCompositionDescriptor Descriptor;
	Descriptor.Fragments.Add<FFloatFragment>();
	Descriptor.Fragments.Add<FVectorFragment>();

	FMassArchetypeHandle ArchetypeHandle = EntityManager.CreateArchetype(Descriptor);

	//单个创建
	FMassEntityHandle EntityHandle = EntityManager.CreateEntity(ArchetypeHandle);
	FFloatFragment* Data = EntityManager.GetFragmentDataPtr<FFloatFragment>(EntityHandle);
	Data->FloatValue = -1.0f;

	//批量创建
	TArray<FMassEntityHandle> OutEntityHandles;
	EntityManager.BatchCreateEntities(ArchetypeHandle, 1000, OutEntityHandles);
	FMassExecutionContext Context = EntityManager.CreateExecutionContext(0.f);

	//初始化值
	FMassEntityQuery Query{ FFloatFragment::StaticStruct() };
	Query.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
		{
			TArrayView<FFloatFragment> FloatList = Context.GetMutableFragmentView<FFloatFragment>();
			for (int i = 0; i < FloatList.Num(); i++)
			{
				FloatList[i].FloatValue = i;
			}
		});
}

void UMassLogicTestSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UMassLogicTestSubsystem::IsTickable() const
{
	return true;
}

void UMassLogicTestSubsystem::Tick(float DeltaTime)
{

}

TStatId UMassLogicTestSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(MassLogicTestSubsystem, STATGROUP_Tickables);
}


void UMassLogicTestSubsystem::TestQuery()
{
	FMassEntityQuery FilterQuery{ FFloatFragment::StaticStruct(), FVectorFragment::StaticStruct() };
	FilterQuery.SetChunkFilter([this](const FMassExecutionContext& Context)
	{
		const TConstArrayView<FFloatFragment> FloatList = Context.GetFragmentView<FFloatFragment>();
		return true;

	});

	FMassEntityQuery LogQuery{ FFloatFragment::StaticStruct(), FVectorFragment::StaticStruct() };

	UMassEntitySubsystem* Subsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	FMassEntityManager& EntityManager = Subsystem->GetMutableEntityManager();
	FMassExecutionContext Context = EntityManager.CreateExecutionContext(0.f);

	LogQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context) 
	{
		const TArrayView<FFloatFragment> FloatList = Context.GetMutableFragmentView<FFloatFragment>();
		for (int i = 0; i < FloatList.Num(); i++)
		{
			UE_LOG(LogTemp, Error, TEXT("UMassLogicTestSubsystem::TestQuery: %f"), FloatList[i].FloatValue);
		}
	});

}

UMassLogicTestProcessor::UMassLogicTestProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);
	ExecutionOrder.ExecuteAfter.Add(FName("SimpleRandomMovementProcessor"));
	EntityQuery.RegisterWithProcessor(*this);
}

void UMassLogicTestProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FFloatFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FVectorFragment>(EMassFragmentAccess::ReadWrite);
}

void UMassLogicTestProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& ExecuteContext) 
	{
			//ExecuteContext.Defer().DestroyEntities();
	});
}

void UMassLogicTestSubsystem::AddEntity(int Count)
{
	UMassEntitySubsystem* Subsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	FMassEntityManager& EntityManager = Subsystem->GetMutableEntityManager();
}
