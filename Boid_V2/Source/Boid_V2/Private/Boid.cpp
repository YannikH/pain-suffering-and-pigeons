// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
    RootComponent = SphereComponent;
    SphereComponent->SetGenerateOverlapEvents(true);
    SphereComponent->SetSphereRadius(SearchRadius);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
    VisualMesh->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoid::AdjustVectorTowards(float DeltaTime, FVector targetLocation, float force, FColor debugColor = FColor::Green)
{
    if (targetLocation == FVector(0.f, 0.f, 0.f)) return;

    FVector OldLocation = GetActorLocation();
    FVector targetVector = targetLocation - OldLocation;
    DrawDebugLine(GetWorld(), OldLocation, OldLocation + targetVector, debugColor);

    velocity = velocity + (targetVector * force);
}

void ABoid::Move(float DeltaTime)
{
    FVector oldVelocity = velocity;
    TArray<AActor*> overlappingActors;
    GetOverlappingActors(overlappingActors);
    DrawDebugSphere(GetWorld(), SphereComponent->GetComponentLocation(), SearchRadius, 5.f, FColor::Blue, false);
    DrawDebugSphere(GetWorld(), SphereComponent->GetComponentLocation(), SeparationDistance, 5.f, FColor::Red, false);

    FVector OldLocation = GetActorLocation();
    AdjustVectorTowards(DeltaTime, GetCoherencePoint(overlappingActors), coherenceForce);
    AdjustVectorTowards(DeltaTime, GetSeparationPoint(overlappingActors), separationForce, FColor::Red);
    AdjustVectorTowards(DeltaTime, GetAlignmentPoint(overlappingActors), alignmentForce, FColor::Blue);

    FVector velocityDelta = velocity - oldVelocity;
    velocityDelta.Normalize();
    velocityDelta *= 0.5;
    velocity = oldVelocity + velocityDelta;
    velocity.Z = 0;

    // Update velocity
    velocity.Normalize();
    velocity *= 10;

    // Update position
    FVector NewLocation = OldLocation + velocity;
    NewLocation.Z = 150;
    SetActorLocation(NewLocation);
}

void ABoid::RotateWithVelocity()
{
    FRotator rotation = velocity.ToOrientationRotator();
    rotation.Pitch = -90.f;
    VisualMesh->SetWorldRotation(rotation);
}

void ABoid::GetOverlappingActors(TArray<AActor*>& OutOverlappingActors)
{
    SphereComponent->GetOverlappingActors(OutOverlappingActors);
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  
    Move(DeltaTime);
    RotateWithVelocity();
}

FVector ABoid::GetCoherencePoint(TArray< AActor* > actors)
{
    FVector returnVec = FVector(0.f, 0.f, 0.f);
    for (AActor* actor : actors) {
        float distance = FVector::Dist2D(GetActorLocation(), actor->GetActorLocation());
        if (actor == this || distance < SeparationDistance) continue;
        DrawDebugLine(GetWorld(), GetActorLocation(), actor->GetActorLocation(), FColor::Blue);
        returnVec += actor->GetActorLocation();
    }

    return (returnVec / actors.Num());
}

FVector ABoid::GetSeparationPoint(TArray< AActor* > actors)
{
    FVector returnVec = FVector::ZeroVector;
    for (AActor* actor : actors) {
        if (actor == this) continue;
        float distance = FVector::Dist2D(GetActorLocation(), actor->GetActorLocation());
        FVector toActor = GetActorLocation() - actor->GetActorLocation();
        toActor.Normalize();
        double dotProd = FVector::DotProduct(toActor, velocity);
        if (distance < SeparationDistance) {
            //DrawDebugLine(GetWorld(), GetActorLocation(), actor->GetActorLocation(), FColor::Blue);
            // returnVec -= (GetActorLocation() - actor->GetActorLocation());
            // returnVec *= ((SeparationDistance - distance) / 3);
            returnVec += toActor / distance;
        }
    }

    return returnVec;
}

FVector ABoid::GetAlignmentPoint(TArray< AActor* > actors)
{
    FVector returnVec = FVector(0.f, 0.f, 0.f);
    for (AActor* actor : actors) {
        ABoid* boidActor = dynamic_cast<ABoid*>(actor);
        float distance = FVector::Dist2D(GetActorLocation(), actor->GetActorLocation());
        if (actor == this || distance < SeparationDistance) continue;
        returnVec += boidActor->GetVelocity();
    }

    returnVec *= 100;

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (returnVec / actors.Num()), FColor::Purple);
    return GetActorLocation() + (returnVec / actors.Num());
}

FVector ABoid::GetVelocity() {
    return velocity;
}