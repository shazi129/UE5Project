#pragma once
#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassProcessor.h"
#include "MassEntityQuery.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MassLogicTest.generated.h"

USTRUCT()
struct FFloatFragment : public FMassFragment
{
	GENERATED_BODY()
	float FloatValue;
};

USTRUCT()
struct FVectorFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector VectorValue;
};

UCLASS(BlueprintType)
class UMassLogicTestSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();

	virtual bool IsTickable() const;
	virtual void Tick(float DeltaTime);
	virtual TStatId GetStatId() const;

	UFUNCTION(BlueprintCallable)
	void TestQuery();
};

UCLASS()
class UMassLogicTestProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UMassLogicTestProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};