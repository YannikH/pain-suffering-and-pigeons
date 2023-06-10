// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
    SetRootComponent(VisualMesh);
    SphereComponent->SetGenerateOverlapEvents(true);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    VisualMesh->SetupAttachment(SphereComponent);


    this->velocity = FVector(
        -5 + FMath::RandRange(0,10),
        -5 + FMath::RandRange(0, 10),
        0.f
    );

    //this->velocity = FVector(0.f, 0.f, 0.f);
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
    TArray<AActor*> overlappingActors = GetOverlappingActors();
    //DrawDebugSphere(GetWorld(), SphereComponent->GetComponentLocation(), 200.f, 10.f, FColor::Red, false);

    FVector OldLocation = GetActorLocation();
    AdjustVectorTowards(DeltaTime, GetCoherencePoint(overlappingActors), 10);
    AdjustVectorTowards(DeltaTime, GetSeparationPoint(overlappingActors), 20 , FColor::Red);
    AdjustVectorTowards(DeltaTime, GetAlignmentPoint(overlappingActors), 50, FColor::Blue);



    FVector velocityDelta = velocity - oldVelocity;
    velocityDelta.Normalize();
    velocityDelta *= 0.5;
    velocity = oldVelocity + velocityDelta;
    velocity.Z = 0;

    velocity.Normalize();
    velocity *= 10;

    FVector NewLocation = OldLocation + velocity;
    NewLocation.Z = 150;

    SetActorLocation(NewLocation);
}

void ABoid::RotateWithVelocity()
{
    FRotator velocityRotation = velocity.Rotation();
    FRotator rotation = GetActorRotation();
    rotation.Yaw = velocityRotation.Yaw;
    rotation.Pitch = -90.f;
    VisualMesh->SetWorldRotation(rotation);
}

TArray< AActor* > ABoid::GetOverlappingActors()
{
    TArray< AActor* > overlappingActors;
    SphereComponent->GetOverlappingActors(overlappingActors);
    FString debugText = FString::FromInt(overlappingActors.Num());
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, debugText);
    return overlappingActors;
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
        if (actor == this) continue;
        returnVec += actor->GetActorLocation();
    }

    return (returnVec / actors.Num());
}

FVector ABoid::GetSeparationPoint(TArray< AActor* > actors)
{
    FVector returnVec = FVector(0.f, 0.f, 0.f);
    for (AActor* actor : actors) {
        if (actor == this) continue;
        float distance = FVector::Dist2D(GetActorLocation(), actor->GetActorLocation());
        FVector toActor = actor->GetActorLocation() - GetActorLocation();
        toActor.Normalize();
        double dotProd = FVector::DotProduct(toActor, velocity);
        if (distance < 300) {
            DrawDebugLine(GetWorld(), GetActorLocation(), actor->GetActorLocation(), FColor::Blue);
            returnVec -= (GetActorLocation() - actor->GetActorLocation());
            //returnVec *= ((300 - distance) / 10);
        }
    }

    return returnVec * -1;
}

FVector ABoid::GetAlignmentPoint(TArray< AActor* > actors)
{
    FVector returnVec = FVector(0.f, 0.f, 0.f);
    for (AActor* actor : actors) {
        ABoid* boidActor = dynamic_cast<ABoid*>(actor);
        if (boidActor == nullptr) continue;
        if (actor == this) continue;
        returnVec += boidActor->GetVelocity();
    }

    //returnVec *= 100;

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, (returnVec / actors.Num()).ToString());
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (returnVec / actors.Num()), FColor::Purple);
    return GetActorLocation() + (returnVec / actors.Num());
}

FVector ABoid::GetVelocity() {
    return velocity;
}