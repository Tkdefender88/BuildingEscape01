// CopyRight Justin Bak 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure plate not assigned to %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll trigger volume
	if (GetTotalMassOnPlate() > PressurePlateTriggerWeight) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}

}

float UOpenDoor::GetTotalMassOnPlate() {
	float totalMass = 0.0f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return totalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//add all of their masses - iterate through

	for (const auto* Actor : OverlappingActors) {
		totalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is in trigger volume"), *Actor->GetName())
	}
	return totalMass;
}

