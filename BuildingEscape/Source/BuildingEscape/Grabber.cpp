// CopyRight Justin Bak 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();
	///Look for attached physics handle
	FindPhysicsHandle();
	///Bind inputs
	SetupInputComponent();

}

void UGrabber::FindPhysicsHandle() {
	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s physics Component Missing"), *GetOwner()->GetName());
	}

}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input not found or offline"));
	}
}

void UGrabber::Grab() {
	//Ray cast to find objects in range
	FHitResult HitResult = GetFirstPhysicsBodyInRange();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	///If we hit something then attach physics handle
	if (ActorHit) {
		//Attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //allow rotation
		);
	}
}

void UGrabber::Release() {
	//TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInRange() {
	/// Get player viewpoint every frame
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
	);

	///Draw a red line for trace testing
	FVector LineTraceDirection = PlayerViewRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewLocation + LineTraceDirection;

	///Setup query Params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast out to the reaching distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player viewpoint every frame
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
	);

	///Draw a red line for trace testing
	FVector LineTraceDirection = PlayerViewRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewLocation + LineTraceDirection;

	//if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		//move object that we hold

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}
