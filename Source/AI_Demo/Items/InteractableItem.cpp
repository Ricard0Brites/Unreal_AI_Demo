// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractableItem.h"
#include "AI_DemoCharacter.h"

AInteractableItem::AInteractableItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractableItem::Interact_Implementation(APawn* InteractionInstigator)
{
	IsInteracting = true;

	if (AAI_DemoCharacter* OwnerRef = Cast<AAI_DemoCharacter>(InteractionInstigator))
		OwnerRef->CurrentItem = this;
}

void AInteractableItem::FinishInteract_Implementation()
{
	IsInteracting = false;
}

bool AInteractableItem::CanInteract_Implementation()
{
	return !IsInteracting;
}

EInteractableItemType AInteractableItem::GetItemType_Implementation()
{
	return ItemType;
}

void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

