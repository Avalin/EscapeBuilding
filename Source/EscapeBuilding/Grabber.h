// Copyright Astrid Avalin Sørensen 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEBUILDING_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void DebugRayCast(FVector Location, FRotator Rotation);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void GrabObject();
	void ObjectFollowPlayer();
	void ReleaseObject();
	void LoadComponents();
	void LoadPhysicsHandle();
	void LoadInputComponent();

	FVector GetPlayerViewLineStart();
	FVector GetPlayerViewLineEnd();

	const FHitResult GetFirstPhysicsBodyInReach();
};
