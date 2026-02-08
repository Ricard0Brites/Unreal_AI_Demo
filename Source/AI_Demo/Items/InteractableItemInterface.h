#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableItemInterface.generated.h"

UENUM(BlueprintType)
enum class EInteractableItemType : uint8
{
	Default UMETA(DisplayName = "Default"),
	Weapon UMETA(DisplayName = "Weapon"),

	Max UMETA(Hidden)
};

UINTERFACE(MinimalAPI)
class UInteractableItemInterface : public UInterface
{
	GENERATED_BODY()
};

class AI_DEMO_API IInteractableItemInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EInteractableItemType GetItemType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(APawn* InteractionInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract();
};
