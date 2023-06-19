// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Boid.generated.h"

UCLASS()
class BOID_V2_API ABoid : public AActor
{
	GENERATED_BODY()

	public:	
	ABoid();

	
private:
	UPROPERTY(meta = (AllowPrivateAccess = "true"), Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VisualMesh;
	UPROPERTY(meta = (AllowPrivateAccess = "true"), Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(Category = "Values", EditAnywhere)
	FVector velocity = FVector(
        -5 + FMath::RandRange(0, 10),
        -5 + FMath::RandRange(0, 10),
        0.0f
    );

	UPROPERTY(Category = "Flocking Weights", EditAnywhere)
	float coherenceForce = 1.0f;
	UPROPERTY(Category = "Flocking Weights", EditAnywhere)
	float separationForce = 1.0f;
	UPROPERTY(Category = "Flocking Weights", EditAnywhere)
	float alignmentForce = 1.0f;
	UPROPERTY(Category = "Flocking Weights", EditAnywhere)
	float SeparationDistance = 1.0f;
	UPROPERTY(Category = "Flocking Weights", EditAnywhere)
	float SearchRadius = 500.0f;

protected:
	virtual void BeginPlay() override;
	FVector GetVelocity();
	void AdjustVectorTowards(float DeltaTime, FVector targetLocation, float force, FColor debugColor);
	FVector GetCoherencePoint(TArray< AActor* > actors);
	FVector GetSeparationPoint(TArray< AActor* > actors);
	FVector GetAlignmentPoint(TArray< AActor* > actors);
	void Move(float DeltaTime);
	void RotateWithVelocity();
	void GetOverlappingActors(TArray<AActor*>& OutOverlappingActors);

public:
	virtual void Tick(float DeltaTime) override;
};
