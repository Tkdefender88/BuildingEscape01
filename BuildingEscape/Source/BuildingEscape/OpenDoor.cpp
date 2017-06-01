// CopyRight Justin Bak 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor() 
{
	Owner->SetActorRotation(FRotator(0.0f, -90.f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Poll trigger volume
	if (GetTotalMassOnPlate() > PressurePlateTriggerWeight) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	//Close the door
	if (CurrentTime - LastDoorOpenTime > DoorCloseDelay){
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOnPlate() {
	float totalMass = 0.0f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//add all of their masses - iterate through

	for (const auto* Actor : OverlappingActors) {
		totalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is in trigger volume"), *Actor->GetName())
	}
	return totalMass;
}

