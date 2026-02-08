#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WearableItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class AI_DEMO_API UWearableItemComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UWearableItemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void Deactivate() override;

	#pragma region Mesh Memory Management
	DECLARE_DELEGATE_TwoParams(FOnMeshLoaded, const FSoftObjectPath&, UObject*);
	
	FOnMeshLoaded OnMeshLoaded;

	UFUNCTION(BlueprintImplementableEvent)
	void OnMeshLoadedEvent(USkeletalMesh* LoadedItem);

	#pragma endregion


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> MeshToSpawn;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName SocketToAttachTo = "none";
};
