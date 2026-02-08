// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableItemInterface.h"
#include "WearableItemComponent.h"
#include "UObject/SoftObjectPtr.h"
#include "InteractableItem.generated.h"

UCLASS()
class AI_DEMO_API AInteractableItem : public AActor, public IInteractableItemInterface
{
	GENERATED_BODY()
	
public:	
	AInteractableItem();

protected:
	bool IsInteracting = false;

	#pragma region IInteractableItemInterface
	
	virtual EInteractableItemType GetItemType_Implementation() override;
	virtual void Interact_Implementation(APawn* InteractionInstigator) override;
	virtual void FinishInteract_Implementation() override;
	virtual bool CanInteract_Implementation() override;

#pragma endregion

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Definition")
	TSoftClassPtr<UWearableItemComponent> WearableItem;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Definition")
	EInteractableItemType ItemType;

public:	
	virtual void Tick(float DeltaTime) override;
};
