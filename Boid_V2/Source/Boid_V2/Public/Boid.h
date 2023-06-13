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
	
private:
	void AdjustVectorTowards(float DeltaTime, FVector targetLocation, float force, FColor debugColor);
	FVector GetCoherencePoint(TArray< AActor* > actors);
	FVector GetSeparationPoint(TArray< AActor* > actors);
	FVector GetAlignmentPoint(TArray< AActor* > actors);
	void Move(float DeltaTime);
	void RotateWithVelocity();
	//TArray< AActor* > GetOverlappingActors();
	void GetOverlappingActors(TArray<AActor*>& OutOverlappingActors);

public:	
	// Sets default values for this actor's properties
	ABoid();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	FVector GetVelocity();
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Weights")
	float coherenceForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Weights")
	float separationForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Weights")
	float alignmentForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Weights")
	float SeparationDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Weights")
	float SearchRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*UFUNCTION()
	FActorBeginOverlapSignature OnActorBeginOverlap();
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

};
