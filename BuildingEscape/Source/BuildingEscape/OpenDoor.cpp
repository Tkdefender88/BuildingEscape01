// CopyRight Justin Bak 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"


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
	
}

void UOpenDoor::OpenDoor()
{

	// Find owning actor
	AActor* Owner = GetOwner();

	//Create rotator
	FRotator rotation = FRotator(0.0f, OpenAngle, 0.0f);

	//Set the door rotation
	Owner->SetActorRotation(rotation);
}

void UOpenDoor::CloseDoor() {
	// Find owning actor
	AActor* Owner = GetOwner();

	//Create rotator
	FRotator rotation = FRotator(0.0f, -90.f, 0.0f);

	//Set the door rotation
	Owner->SetActorRotation(rotation);
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...

	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}

