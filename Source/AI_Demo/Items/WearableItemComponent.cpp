#include "Items/WearableItemComponent.h"
#include "AI_DemoCharacter.h"

UWearableItemComponent::UWearableItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWearableItemComponent::BeginPlay()
{
	Super::BeginPlay();

	//Try loading the mesh if its not already in memory
	ensureAlways(MeshToSpawn.IsValid());

	if (!MeshToSpawn.Get())
	{
		MeshToSpawn.LoadAsync(OnMeshLoaded);

		OnMeshLoaded.BindLambda([&](const FSoftObjectPath& Path, UObject* Obj)
			{
				USkeletalMesh* Mesh = Cast< USkeletalMesh>(Obj);
				ensureAlwaysMsgf(Mesh, TEXT("Item is not a valid reference in %s"), ANSI_TO_TCHAR(__FUNCTION__));
				if (Mesh)
					OnMeshLoadedEvent(Mesh);


			});
		return;
	}

	OnMeshLoadedEvent(MeshToSpawn.Get());

}

void UWearableItemComponent::Deactivate()
{
	if (AAI_DemoCharacter* OwnerRef = Cast<AAI_DemoCharacter>(GetOwner()))
		OwnerRef->CurrentItem.Reset();
}
