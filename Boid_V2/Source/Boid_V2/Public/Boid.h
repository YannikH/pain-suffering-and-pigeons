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
	FVector velocity;

public:	
	// Sets default values for this actor's properties
	ABoid();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};