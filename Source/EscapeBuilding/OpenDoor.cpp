// Copyright Astrid Avalin Sørensen 2019


#include "OpenDoor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s needs a PressurePlate, but had a nullptr!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::PlayDoorOpenSound()
{
	if (!GetOwner()->FindComponentByClass<UAudioComponent>()) return;
	if (!SFXOnDoorOpen)
	{
		UE_LOG(LogTemp, Error, TEXT("%s needs a SFXDoorOpen sound, but had a nullptr!"), *GetOwner()->GetName());
	}
	//Since this is a tick method this will be called, got to make sure no sound is currently playing
	if (!GetOwner()->FindComponentByClass<UAudioComponent>()->IsPlaying())
	{
		GetOwner()->FindComponentByClass<UAudioComponent>()->SetSound(SFXOnDoorOpen);
		GetOwner()->FindComponentByClass<UAudioComponent>()->Play();
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	PlayDoorOpenSound();
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 2.f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * 2.f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetTotalMassOfActorsOnPressurePlate() > WeightNecessaryToOpenDoor)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPressurePlate()
{
	float TotalMass = 0.f;
	//Finding all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) return TotalMass;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}